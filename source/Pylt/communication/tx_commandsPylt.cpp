#include "tx_commandspylt.h"
#include <QDebug>


Tx_commandsPylt::Tx_commandsPylt(QObject *parent) :
    QObject(parent)
{

}

Tx_commandsPylt::~Tx_commandsPylt()
{

}

void
Tx_commandsPylt::setPacking(Packing *newPacking)
{
    _packing = newPacking;
}

//активность джойстика 0x0A
int
Tx_commandsPylt::joysticActivity(quint8 y1, quint8 x1, quint8 y2, quint8 x2, quint8 boot1, quint8 boot2)
{
    QByteArray data;
    int res = -1;
    QString s;
    quint8 comand = 0x0A;

    data.append(comand);
    data.append(y1);
    data.append(x1);
    data.append(y2);
    data.append(x2);
    data.append(boot1);
    data.append(boot2);

    //отправка команды, вывод лога
    res = _packing->Sending(data, s);
    return res;
}

// запрос типа аккамулятора 0xA1
int
Tx_commandsPylt::batteryTypeRequest()
{
    QByteArray data;
    int res = -1;
    QString s = "запрос типа аккамулятора";
    quint8 comand = 0xA1;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = _packing->Sending(data, s);
    return res;
}

