#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>

#include "mainmodel.h"
#include "device.h"
#include "updateapp.h"


class AppManager : public QObject
{
    Q_OBJECT
public:
    explicit AppManager(QObject *parent = nullptr);

    void setModel(MainModel *newModel);

    void setDevice(Device *newDevice);
    void setUpdateApp(UpdateApp *newUpdateApp);

    void keepScreenOn(bool on);
    
    Q_INVOKABLE int installApk(const QString& absPath);
signals:

private:
    MainModel *model_ = nullptr;
    Device *device_ = nullptr;
    UpdateApp *updateApp_ = nullptr;

public slots:

    void onApplicationStateChanged(Qt::ApplicationState state);

};



#endif // APPMANAGER_H
