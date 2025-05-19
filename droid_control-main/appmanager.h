#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>

#include "mainmodel.h"
#include "device.h"


class AppManager : public QObject
{
    Q_OBJECT
public:
    explicit AppManager(QObject *parent = nullptr);

    void setModel(MainModel *newModel);

    void setDevice(Device *newDevice);

    void keepScreenOn(bool on);

signals:

private:
    MainModel *model_ = nullptr;
    Device *device_ = nullptr;


public slots:

    void onApplicationStateChanged(Qt::ApplicationState state);

};



#endif // APPMANAGER_H
