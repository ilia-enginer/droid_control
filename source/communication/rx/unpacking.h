#ifndef UNPACKING_H
#define UNPACKING_H

#include <QObject>
#include <QWidget>
#include "../crc.h"
#include "../../display_working/commun_display.h"
#include "rx_commands.h"
#include "../../main/settings.h"
#include "../device/device.h"
#include "../serialComPort/mainSerialPort.h"


class Unpacking: public QObject
{
    Q_OBJECT

public:
    explicit Unpacking(QObject *parent = 0);
    ~Unpacking();
    void setCrc(Crc *newCrc);
    void setCommun_display(Commun_display *newCommun_display);
    void setRx_commands(Rx_commands *newRx_commands);
    void setSettings(Settings *newSettings);
    void setDevice(Device *newDevice);
    void setMainSerialPort(MainSerialPort *newMainSerialPort);

    int unpack(QByteArray data, int size);
    int checkBalance();

    void f_AdminChange(bool adm);

public slots:
    void messRes(QByteArray recievedData);

private:
    Crc * _crc = nullptr;
    Commun_display * _commDisplay = nullptr;
    Rx_commands *_rx_commands = nullptr;
    Settings *_settings = nullptr;
    Device *_device = nullptr;
    MainSerialPort *_mainSerialPort = nullptr;

    int gluingPackages();
    void auroraDelete();

    quint8 max_rx_size = 254;
    QByteArray dataRes;
    QByteArray Temp = 0;
    bool statys = false;
    bool split = false;

    #ifdef Q_OS_WIN
        bool _f_Admin = true;
    #elif defined(Q_OS_MACOS)
        bool _f_Admin = false;
    #elif defined(Q_OS_ANDROID)
        bool _f_Admin = false;
    #elif defined(Q_OS_LINUX)
        bool _f_Admin = true;
    #endif

};

#endif // UNPACKING_H



