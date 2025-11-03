#include <QByteArray>
#include <string.h>


#include "packing.h"




Packing::Packing(QObject *parent) :
    QObject(parent)
{

}

Packing::~Packing()
{

}

void
Packing::setCrc(Crc *newCrc)
{
    _crc = newCrc;
}

//void
//Packing::setDevice(Device *newDevice)
//{
//    _device = newDevice;
//}

void
Packing::setCommun_display(Commun_display *newCommun_display)
{
    _commDisplay = newCommun_display;
}

void
Packing::setSocket(QBluetoothSocket *newSocket)
{
    _socket = newSocket;
}

void
Packing::delSocket()
{
    _socket = nullptr;
}


//отправить сообщение
int
Packing::sendMessage(QString msg, bool wrap)
{
    QByteArray dataToSend = QByteArray::fromHex(msg.toUtf8());

    //если надо обернуть протоколом
    if(wrap) {
        dataToSend = wrapData(dataToSend);
    }

    //если нет подключения - выйти
    if (!_socket)   {
        return -1;
    }
//    int res = _device->sendMessage(dataToSend);
    int res = _socket->write(dataToSend);

    //если не отправлено - выйти
    if(res == -1)
       return res;

    //отправка лога для терминала
    _commDisplay->log_out_T("-> ", QString ("%1 (%2 size)").arg(QString(dataToSend.toHex())).arg(res));

    return res;
}

int
Packing::sendMessage(QByteArray msg, bool wrap)
{
    QByteArray dataToSend = msg;

    //если надо обернуть протоколом
    if(wrap) {
        dataToSend = wrapData(dataToSend);
    }

    //если нет подключения - выйти
    if (!_socket)   {
        return -1;
    }
//    int res = _device->sendMessage(dataToSend);
    int res = _socket->write(dataToSend);


    //если не отправлено - выйти
    if(res == -1)
       return res;

    //отправка лога для терминала
    _commDisplay->log_out_T("-> ", QString ("%1 (%2 size)").arg(QString(dataToSend.toHex())).arg(res));

    return res;
}


//запаковать сообщение
QByteArray
Packing::wrapData(QByteArray input)
{

    QByteArray out;

    out.append(input);

    qint16 crc = _crc->crc16(input);

    out.append(crc & 0x00FF);
    out.append((crc >> 8) & 0x00FF);

    out.replace(QByteArray(1, 0x1F), QByteArray(2, 0x1F));
    out.replace(QByteArray(1, 0x2F), QByteArray(2, 0x2F));

    out.append(0x2F);
    out.append(0x55);

    out.insert(0, 0x1F);

    return out;
}
