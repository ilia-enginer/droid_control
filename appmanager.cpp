#include "appmanager.h"

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

void AppManager::setModel(MainModel *newModel)
{
    model_ = newModel;
}

void AppManager::setDevice(Device *newDevice)
{
    device_ = newDevice;
}

void AppManager::setUpdateApp(UpdateApp *newUpdateApp)
{
    updateApp_ = newUpdateApp;
}


void
AppManager::onApplicationStateChanged(Qt::ApplicationState state)
{
//    qDebug() << state;

    if(state == Qt::ApplicationState::ApplicationActive)
    {
        device_->set_rendering_flag(true);
        model_->set_rendering_flag(true);
        updateApp_->set_rendering_flag(true);

    }
    else
    {
        device_->set_rendering_flag(false);
        model_->set_rendering_flag(false);
        updateApp_->set_rendering_flag(false);
        keepScreenOn(false);
    }

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
    #if defined(Q_QDOC) || (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))

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
