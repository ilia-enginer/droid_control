
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

void
Tx_commandsPylt::setPylt_settings(Pylt_settings *newPylt_settings)
{
    _pylt_settings = newPylt_settings;
}

int
Tx_commandsPylt::recalculatingParameters()
{
    qint32 old_min = 500;
    qint32 old_max = 2500;

    qint32 new_old_min = 0;
    qint32 new_old_max = 249;

    // получить параметры пользовательского вида из класса настроек
    FullParam param = _pylt_settings->getFullParam();
    valuesParam = param;

    // пересчитать параметры в значения, которые принимает пульт
    valuesParam.ch1_joy1_y_min = uint_32_changing_range(param.ch1_joy1_y_min, old_min, old_max, new_old_min, new_old_max);
    valuesParam.ch1_joy1_y_max = uint_32_changing_range(param.ch1_joy1_y_max, old_min, old_max, new_old_min, new_old_max);
    valuesParam.ch2_joy1_x_min = uint_32_changing_range(param.ch2_joy1_x_min, old_min, old_max, new_old_min, new_old_max);
    valuesParam.ch2_joy1_x_max = uint_32_changing_range(param.ch2_joy1_x_max, old_min, old_max, new_old_min, new_old_max);

    valuesParam.ch3_joy2_y_min = uint_32_changing_range(param.ch3_joy2_y_min, old_min, old_max, new_old_min, new_old_max);
    valuesParam.ch3_joy2_y_max = uint_32_changing_range(param.ch3_joy2_y_max, old_min, old_max, new_old_min, new_old_max);
    valuesParam.ch4_joy2_x_min = uint_32_changing_range(param.ch4_joy2_x_min, old_min, old_max, new_old_min, new_old_max);
    valuesParam.ch4_joy2_x_max = uint_32_changing_range(param.ch4_joy2_x_max, old_min, old_max, new_old_min, new_old_max);

    valuesParam.ch5_min = uint_32_changing_range(param.ch5_min, old_min, old_max, new_old_min, new_old_max);
    valuesParam.ch5_max = uint_32_changing_range(param.ch5_max, old_min, old_max, new_old_min, new_old_max);

    valuesParam.ch6_min = uint_32_changing_range(param.ch6_min, old_min, old_max, new_old_min, new_old_max);
    valuesParam.ch6_max = uint_32_changing_range(param.ch6_max, old_min, old_max, new_old_min, new_old_max);

    return 0;
}

int
Tx_commandsPylt::joystic_Activity(float y_1, float x_1, float y_2, float x_2, bool but_1, bool but_2)
{
    float old_min = -1.0;
    float old_max = 1.0;

    float ch1;
    float ch2;
    float ch3;
    float ch4;
    float ch5;
    float ch6;

    flag_calculation = true;    // расчет начат

    // при необходимости здесь можно поменять местами
    if(valuesParam.ch1_cheng_ch3)
    {
        ch1 = y_2;
        ch3 = y_1;
    }
    else
    {
        ch1 = y_1;
        ch3 = y_2;
    }
    if(valuesParam.ch2_cheng_ch4)
    {
        ch2 = x_2;
        ch4 = x_1;
    }
    else
    {
        ch2 = x_1;
        ch4 = x_2;
    }

    //???  расчет если танк

    // расчет
    //переворачивание если инверсия
    if(valuesParam.ch1_inv) ch1 = float_changing_range(ch1, old_min, old_max, (float)valuesParam.ch1_joy1_y_max, (float)valuesParam.ch1_joy1_y_min);
    else                    ch1 = float_changing_range(ch1, old_min, old_max, (float)valuesParam.ch1_joy1_y_min, (float)valuesParam.ch1_joy1_y_max);
    if(valuesParam.ch2_inv) ch2 = float_changing_range(ch2, old_min, old_max, (float)valuesParam.ch2_joy1_x_max, (float)valuesParam.ch2_joy1_x_min);
    else                    ch2 = float_changing_range(ch2, old_min, old_max, (float)valuesParam.ch2_joy1_x_min, (float)valuesParam.ch2_joy1_x_max);
    if(valuesParam.ch3_inv) ch3 = float_changing_range(ch3, old_min, old_max, (float)valuesParam.ch3_joy2_y_max, (float)valuesParam.ch3_joy2_y_min);
    else                    ch3 = float_changing_range(ch3, old_min, old_max, (float)valuesParam.ch3_joy2_y_min, (float)valuesParam.ch3_joy2_y_max);
    if(valuesParam.ch4_inv) ch4 = float_changing_range(ch4, old_min, old_max, (float)valuesParam.ch4_joy2_x_max, (float)valuesParam.ch4_joy2_x_min);
    else                    ch4 = float_changing_range(ch4, old_min, old_max, (float)valuesParam.ch4_joy2_x_min, (float)valuesParam.ch4_joy2_x_max);
    if(valuesParam.ch5_inv)
    {
        if(but_1)   ch5 = valuesParam.ch5_max;
        else        ch5 = valuesParam.ch5_min;
    }
    else
    {
        if(but_1)   ch5 = valuesParam.ch5_min;
        else        ch5 = valuesParam.ch5_max;
    }
    if(valuesParam.ch6_inv)
    {
        if(but_2)   ch6 = valuesParam.ch6_max;
        else        ch6 = valuesParam.ch6_min;
    }
    else
    {
        if(but_2)   ch6 = valuesParam.ch6_min;
        else        ch6 = valuesParam.ch6_max;
    }

    y1      = (qint8)ch1;
    x1      = (qint8)ch2;
    y2      = (qint8)ch3;
    x2      = (qint8)ch4;
    but1    = (qint8)ch5;
    but2    = (qint8)ch6;

    flag_calculation = false;   // расчет закончен
    if(flag_deferred_transfer)  // если нужна отложенная передача
    {
        joystic_Tx();
        flag_deferred_transfer = false;
    }
    return 0;
}

// передача команды управления 0x0A
int
Tx_commandsPylt::joystic_Tx()
{
    QByteArray data;
    int res = -4;
    QString s;
    quint8 comand = 0x0A;
    quint8 but_bit = 0;     // байтовое состояние кнопок

    // если идет расчет - сделать отложенную передачу
    if(flag_calculation)
    {
        flag_deferred_transfer = true;
        return res;
    }

    data.append(comand);
    data.append(but_bit);
    data.append(y1);
    data.append(x1);
    data.append(y2);
    data.append(x2);
    data.append(but1);
    data.append(but2);

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

qint32
Tx_commandsPylt::uint_32_changing_range(qint32 old, qint32 old_min, qint32 old_max, qint32 new_min, qint32 new_max)
{
    qint32 converted;
    if(old >= old_max)				converted = new_max;
    else if (old <= old_min)        converted = new_min;
    else                            converted = ((old - old_min) * (new_max - new_min)) / (old_max - old_min) + new_min;
    return converted;
}

qint8
Tx_commandsPylt::uint_8_changing_range(qint8 old, qint8 old_min, qint8 old_max, qint8 new_min, qint8 new_max)
{
    qint8 converted;
    if(old >= old_max)				converted = new_max;
    else if (old <= old_min)        converted = new_min;
    else                            converted = ((old - old_min) * (new_max - new_min)) / (old_max - old_min) + new_min;
    return converted;
}

float
Tx_commandsPylt::float_changing_range(float old, float old_min, float old_max, float new_min, float new_max)
{
    float converted;
    if(old >= old_max)				converted = new_max;
    else if (old <= old_min)        converted = new_min;
    else                            converted = ((old - old_min) * (new_max - new_min)) / (old_max - old_min) + new_min;
    return converted;
}
