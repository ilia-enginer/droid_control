#include "source/main/appmanager.h"

#include <QJniObject>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QJniObject>
#include <QCoreApplication>
#include <QStringList>

#if defined(Q_QDOC) || (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))
#include <QtCore/private/qandroidextras_p.h>
#include <QtCore/qjniobject.h>
#endif


AppManager::AppManager(QObject *parent)
    : QObject{parent}
{

}

void
AppManager::setCommun_display(Commun_display *newCommun_display)
{
    _commun_display = newCommun_display;
}

void
AppManager::onApplicationStateChanged(Qt::ApplicationState state)
{
//    qDebug() << state;

    if(state == Qt::ApplicationState::ApplicationActive)
    {
        _commun_display->allUpdate();
        state_ = Qt::ApplicationState::ApplicationActive;
    }
    else
    {
        state_ = Qt::ApplicationState::ApplicationInactive;
        if(_serviceClients == 0)
        {
            keepScreenOn(false);
        }
    }

    _commun_display->set_rendering_flag(state);

//    QString filename="Data.txt";
//    QFile file( filename );
//    if ( file.open(QIODevice::ReadWrite) )
//    {
//        QTextStream stream( &file );
//        stream << state << "\n";
//    }
}


void
AppManager::keepScreenOn(bool on)
{
    #if defined(Q_OS_WINDOWS)
    {
        Q_UNUSED(on)
    }
    #elif defined(Q_QDOC) || (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))

    static bool flag_on = false;

    if(on != flag_on)
    {
        using QNativeInterface::QAndroidApplication;
          QAndroidApplication::runOnAndroidMainThread([on] {
            QJniObject activity = QtAndroidPrivate::activity();
            if (activity.isValid()) {
              QJniObject window = activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

              if (window.isValid()) {
                const int FLAG_KEEP_SCREEN_ON = 128;
                if (on) {
                  window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
                } else {
                  window.callMethod<void>("clearFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
                }
              }
            }
            QJniEnvironment env;
            if (env->ExceptionCheck()) {
              env->ExceptionClear();
            }
          });
          flag_on = on;
    }
#endif
}

void
AppManager::updateWakeLock(bool enable)
{
#if defined(Q_QDOC) || (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))
    using QNativeInterface::QAndroidApplication;
    QAndroidApplication::runOnAndroidMainThread([this, enable] {
        if (enable) {
            if (_wakeLock.isValid()) {
                return;
            }
            QJniObject activity = QtAndroidPrivate::activity();
            if (!activity.isValid()) {
                return;
            }
            QJniObject service = activity.callObjectMethod(
                        "getSystemService",
                        "(Ljava/lang/String;)Ljava/lang/Object;",
                        QJniObject::fromString("power").object<jstring>());
            if (!service.isValid()) {
                return;
            }
            const jint PARTIAL_WAKE_LOCK = 1;
            QJniObject tag = QJniObject::fromString("droid_control_wake");
            _wakeLock = service.callObjectMethod(
                        "newWakeLock",
                        "(ILjava/lang/String;)Landroid/os/PowerManager$WakeLock;",
                        PARTIAL_WAKE_LOCK,
                        tag.object<jstring>());
            if (_wakeLock.isValid()) {
                _wakeLock.callMethod<void>("acquire", "()V");
            }
        } else {
            if (_wakeLock.isValid()) {
                _wakeLock.callMethod<void>("release", "()V");
                _wakeLock = QJniObject();
            }
        }
        QJniEnvironment env;
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
        }
    });
#else
    Q_UNUSED(enable)
#endif
}

int
AppManager::getStateApp()
{
    return state_;
}

void
AppManager::ensureBluetoothPermissions()
{
#if defined(Q_QDOC) || (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))
    if (QtAndroidPrivate::androidSdkVersion() < 31)
        return;

    const QStringList permissions = {
        QStringLiteral("android.permission.BLUETOOTH_SCAN"),
        QStringLiteral("android.permission.BLUETOOTH_CONNECT")
    };

    QStringList missing;
    for (const QString &permission : permissions) {
        bool granted = false;
        using QNativeInterface::QAndroidApplication;
        QAndroidApplication::runOnAndroidMainThread([&granted, permission] {
            QJniObject activity = QtAndroidPrivate::activity();
            if (!activity.isValid())
                return;
            granted = activity.callMethod<jint>(
                          "checkSelfPermission",
                          "(Ljava/lang/String;)I",
                          QJniObject::fromString(permission).object<jstring>()) == 0;
            QJniEnvironment env;
            if (env->ExceptionCheck())
                env->ExceptionClear();
        });
        if (!granted)
            missing << permission;
    }

    if (missing.isEmpty())
        return;

    using QNativeInterface::QAndroidApplication;
    QAndroidApplication::runOnAndroidMainThread([missing] {
        QJniObject activity = QtAndroidPrivate::activity();
        if (!activity.isValid())
            return;

        QJniEnvironment env;
        jclass stringClass = env->FindClass("java/lang/String");
        if (env->ExceptionCheck()) {
            env->ExceptionClear();
            return;
        }

        jobjectArray array = env->NewObjectArray(missing.size(), stringClass, nullptr);
        for (int i = 0; i < missing.size(); ++i) {
            QJniObject str = QJniObject::fromString(missing.at(i));
            env->SetObjectArrayElement(array, i, str.object<jstring>());
        }

        const jint requestCode = 0x44;
        activity.callMethod<void>("requestPermissions", "([Ljava/lang/String;I)V", array, requestCode);

        env->DeleteLocalRef(array);
        env->DeleteLocalRef(stringClass);
        if (env->ExceptionCheck())
            env->ExceptionClear();
    });
#endif
}

int
AppManager::installApk(const QString& absPath)
{
#if defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED)
    QJniObject jPath = QJniObject::fromString(absPath);
    jint res = QJniObject::callStaticMethod<jint>(
        "org/qtproject/example/InstallAPK",
        "execute",
        "(Landroid/content/Context;Ljava/lang/String;)I",
        nullptr,
        jPath.object<jstring>()
    );
    return int(res);
#else
    Q_UNUSED(absPath);
    return -42;
#endif
}

void
AppManager::startBackgroundService()
{
#if defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED)
    ++_serviceClients;
    if(flagServiceStart)
    {
        return;
    }
    using QNativeInterface::QAndroidApplication;
    QAndroidApplication::runOnAndroidMainThread([] {
        QJniObject::callStaticMethod<void>(
            "org/qtproject/example/QtAndroidService",
            "startQtAndroidService",
            "(Landroid/content/Context;)V",
            QNativeInterface::QAndroidApplication::context()
        );
    });
    flagServiceStart = true;
    updateWakeLock(true);
#endif
}

void
AppManager::stopBackgroundService()
{
#if defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED)
    if(_serviceClients == 0)
    {
        return;
    }
    --_serviceClients;
    if(_serviceClients > 0)
    {
        return;
    }
    if(flagServiceStart)
    {
        using QNativeInterface::QAndroidApplication;
        QAndroidApplication::runOnAndroidMainThread([] {
            QJniObject::callStaticMethod<void>(
                "org/qtproject/example/QtAndroidService",
                "stopQtAndroidService",
                "(Landroid/content/Context;)V",
                QNativeInterface::QAndroidApplication::context()
            );
        });
        flagServiceStart = false;
        updateWakeLock(false);
    }
#endif
}
