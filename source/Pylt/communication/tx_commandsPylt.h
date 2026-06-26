#ifndef TX_COMMANDSPYLT_H
#define TX_COMMANDSPYLT_H

#include <QObject>
#include "../../communication/packing.h"


class Tx_commandsPylt: public QObject
{
    Q_OBJECT

public:
    explicit Tx_commandsPylt(QObject *parent = 0);
    ~Tx_commandsPylt();
    void setPacking(Packing *newPacking);

public slots:
    int joysticActivity(quint8 y1, quint8 x1, quint8 y2, quint8 x2, quint8 boot1, quint8 boot2);    //активность джойстика 0x0A
    int batteryTypeRequest();   // запрос типа аккамулятора 0xA1

private:
    Packing * _packing = nullptr;

};

#endif // TX_COMMANDSPYLT_H
