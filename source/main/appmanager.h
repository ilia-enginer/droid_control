#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>
#include "../display_working/commun_display.h"

#if defined(Q_QDOC) || (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))
#include <QtCore/qjniobject.h>
#endif


class AppManager : public QObject
{
    Q_OBJECT
public:

    explicit AppManager(QObject *parent = nullptr);

    void setCommun_display(Commun_display *newCommun_display);
    void setNotificationclient(NotificationClient *newNotificationclient);

    void keepScreenOn(bool on);

    int getStateApp();    

    void startBackgroundService();
    void stopBackgroundService();
    
    Q_INVOKABLE int installApk(const QString& absPath);

signals:

private:
    Commun_display * _commun_display = nullptr;
    NotificationClient * _notificationClient = nullptr;

    int state_;
    bool flagServiceStart = false;
    int _serviceClients = 0;
#if defined(Q_QDOC) || (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))
    QJniObject _wakeLock;
#endif
    void updateWakeLock(bool enable);


public slots:
    void onApplicationStateChanged(Qt::ApplicationState state);
    void ensureBluetoothPermissions();

};



#endif // APPMANAGER_H
