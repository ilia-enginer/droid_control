#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QObject>
#include "settings.h"
#include "../communication/tx/tx_commands.h"
#include "../communication/rx/rx_commands.h"
#include "../update/updatehex.h"
#include "../display_working/commun_display.h"
#include "../communication/serialComPort/mainSerialPort.h"
#include "../communication/tx/packing.h"
#include "../communication/rx/unpacking.h"

class Device;
class MainSerialPort;

class MainModel: public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(bool adminFlag READ getAdminFlag WRITE setAdminFlag NOTIFY onAdminFlagChanged)

    explicit MainModel(QObject *parent = nullptr);
    ~MainModel();

    void setDevice(Device *device);
    void setSettings(Settings *newSettings);
    void setTx_commands(Tx_commands *newTx_commands);
    void setRx_commands(Rx_commands *newRx_commands);
    void setUpdateHex(UpdateHex *newUpdateHex);
    void setCommun_display(Commun_display *newCommun_display);
    void setMainSerialComPort(MainSerialPort *newMainSerialPort);
    void setPacking(Packing *newPacking);
    void setUnpacking(Unpacking *newUnpacking);

    int checkingParameters();
    int checkID();
    int checkUpdate();  

public slots:
    void setAdminFlag(bool value);
    bool getAdminFlag();
    void deviceConnect(QString type);

Q_SIGNALS:
    void onAdminFlagChanged();

private:
    Device              *device_ = nullptr;
    Settings            *_settings = nullptr;
    Tx_commands         *_tx_commands = nullptr;
    UpdateHex           *_updateHex = nullptr;
    Commun_display      *_commun_display = nullptr;
    Rx_commands         *_rx_commands = nullptr;
    MainSerialPort      *_mainserialport = nullptr;
    Packing             *_packing;
    Unpacking           *_unpacking;

    ///флаг админа
    /// если -1, то полный доступ ко всем функциям
    #ifdef Q_OS_WIN
        bool adminFlag = true;
    #elif defined(Q_OS_MACOS)
        bool adminFlag = false;
    #elif defined(Q_OS_ANDROID)
        bool adminFlag = false;
    #elif defined(Q_OS_LINUX)
        bool adminFlag = true;
    #endif

};
#endif // MAINMODEL_H
