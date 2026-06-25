#include "tx_commandspylt.h"
#include <QDebug>


Tx_commandsPylt::Tx_commandsPylt(QObject *parent) :
    QObject(parent)
{

}

Tx_commandsPylt::~Tx_commandsPylt()
{

}

int
Tx_commandsPylt::joysticActivity(quint8 y1, quint8 x1, quint8 y2, quint8 x2, bool boot1, bool boot2)
{
    qDebug() << "y1: " << y1;
    qDebug() << "x1: " << x1;
    qDebug() << "y2: " << y2;
    qDebug() << "x2: " << x2;

    qDebug() << "boot1: " << boot1;
    qDebug() << "boot2: " << boot2;

//            PA0     ------> ADC1_IN1	лев у
//            PA1     ------> ADC1_IN2	лев х
//            PA2     ------> ADC1_IN3	прав у
//            PA3     ------> ADC1_IN4	прав х


//            transmit.transmitBuffer[0] = 0x1F;								//старт байт
//                transmit.transmitBuffer[1] = Status.but[0];
//                transmit.transmitBuffer[2] = Status.chanel_1;
//                transmit.transmitBuffer[3] = Status.chanel_2;
//                transmit.transmitBuffer[4] = Status.chanel_3;
//                transmit.transmitBuffer[5] = Status.chanel_4;
//                transmit.transmitBuffer[6] = Status.chanel_5;
//                transmit.transmitBuffer[7] = Status.chanel_6;
//                transmit.transmitBuffer[8] = 0x00;
//                transmit.transmitBuffer[9] = 0x00;
//                transmit.transmitBuffer[10] = 0x00;
//                transmit.transmitBuffer[11] = 0x00;

//                crc = Crc8(transmit.transmitBuffer, BUFFER_SIZE_TRANS - 4);

//                transmit.transmitBuffer[12] = crc;								//запись значений контрольной суммы
//                transmit.transmitBuffer[13] = 0x2F;							//стоп бит
//                transmit.transmitBuffer[14] = 0x55;							//стоп бит


    return 0;
}
