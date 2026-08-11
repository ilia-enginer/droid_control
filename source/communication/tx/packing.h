#ifndef PACKING_H
#define PACKING_H

#include <QObject>
#include <QWidget>
#include "../crc.h"
#include "../../display_working/commun_display.h"
#include "../serialComPort/mainSerialPort.h"
#include "../device/device.h"


class Packing: public QObject
{
    Q_OBJECT

public:
    explicit Packing(QObject *parent = 0);
    ~Packing();

    void setCrc(Crc *newCrc);
    void setCommun_display(Commun_display *newCommun_display);
    void setMainSerialPort(MainSerialPort *newMainSerialPort);
    void setDevice(Device *newDevice);
    void setSocket(QBluetoothSocket *newSocket);
    void delSocket();
    void setTypeTx(QString type);

public slots:
    int sendMessage(QString msg, bool wrap);
    int sendMessage(QByteArray msg, bool wrap);
    QByteArray wrapData(QByteArray input);

private:
    Crc                 * _crc = nullptr;
    Commun_display      * _commDisplay = nullptr;
    MainSerialPort      *_mainSerialPort = nullptr;
    Device              *_device = nullptr;

    QString typeTx = "none";

};

#endif // PACKING_H
