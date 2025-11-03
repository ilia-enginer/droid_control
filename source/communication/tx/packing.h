#ifndef PACKING_H
#define PACKING_H

#include <QObject>
#include <QWidget>
#include "../crc.h"
//#include "../device/device.h"
#include "../../display_working/commun_display.h"
#include <QBluetoothSocket>



class Packing: public QObject
{
    Q_OBJECT

public:
    explicit Packing(QObject *parent = 0);
    ~Packing();

    void setCrc(Crc *newCrc);
//    void setDevice(Device *newDevice);
    void setCommun_display(Commun_display *newCommun_display);
    void setSocket(QBluetoothSocket *newSocket);
    void delSocket();

public slots:
    int sendMessage(QString msg, bool wrap);
    int sendMessage(QByteArray msg, bool wrap);
    QByteArray wrapData(QByteArray input);

private:
    Crc * _crc = nullptr;
//    Device * _device = nullptr;
    Commun_display * _commDisplay = nullptr;
    QBluetoothSocket *_socket = nullptr;

};

#endif // PACKING_H
