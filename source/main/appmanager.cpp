#include "source/main/appmanager.h"

#include <QJniObject>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QJniObject>
#include <QCoreApplication>

#if defined(Q_QDOC) || (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))
#include <QtCore/private/qandroidextras_p.h>
#include <QtCore/qjniobject.h>
#endif


AppManager::AppManager(QObject *parent)
    : QObject{parent}
{

}

void AppManager::setCommun_display(Commun_display *newCommun_display)
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
    }
    else
    {
        keepScreenOn(false);
    }

    _commun_display->set_rendering_flag(Qt::ApplicationState::ApplicationActive);

//    QString filename="Data.txt";
//    QFile file( filename );
//    if ( file.open(QIODevice::ReadWrite) )
//    {
//        QTextStream stream( &file );
//        stream << state << "\n";
//    }
}


void AppManager::keepScreenOn(bool on)
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

int AppManager::installApk(const QString& absPath)
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
