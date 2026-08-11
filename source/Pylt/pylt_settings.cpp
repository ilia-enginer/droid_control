
#include "pylt_settings.h"


Pylt_settings::Pylt_settings(QObject *parent) :
    QObject(parent)
{

}

Pylt_settings::~Pylt_settings()
{

}


void
Pylt_settings::set_ch1_joy1_y_min(qint32 value)
{
    ch1_joy1_y_min = value;
    emit ch1_joy1_y_min_chang();
}

qint32
Pylt_settings::get_ch1_joy1_y_min()
{
    return ch1_joy1_y_min;
}

void
Pylt_settings::set_ch1_joy1_y_max(qint32 value)
{
    ch1_joy1_y_max = value;
    emit ch1_joy1_y_max_chang();
}

qint32
Pylt_settings::get_ch1_joy1_y_max()
{
    return ch1_joy1_y_max;
}

void
Pylt_settings::set_ch2_joy1_x_min(qint32 value)
{
    ch2_joy1_x_min = value;
    emit ch2_joy1_x_min_chang();
}

qint32
Pylt_settings::get_ch2_joy1_x_min()
{
    return ch2_joy1_x_min;
}

void
Pylt_settings::set_ch2_joy1_x_max(qint32 value)
{
    ch2_joy1_x_max = value;
    emit ch2_joy1_x_max_chang();
}
qint32
Pylt_settings::get_ch2_joy1_x_max()
{
    return ch2_joy1_x_max;
}

void
Pylt_settings::set_ch3_joy2_y_min(qint32 value)
{
    ch3_joy2_y_min = value;
    emit ch3_joy2_y_min_chang();
}

qint32
Pylt_settings::get_ch3_joy2_y_min()
{
    return ch3_joy2_y_min;
}

void
Pylt_settings::set_ch3_joy2_y_max(qint32 value)
{
    ch3_joy2_y_max = value;
    emit ch3_joy2_y_max_chang();
}

qint32
Pylt_settings::get_ch3_joy2_y_max()
{
    return ch3_joy2_y_max;
}

void
Pylt_settings::set_ch4_joy2_x_min(qint32 value)
{
    ch4_joy2_x_min = value;
    emit ch4_joy2_x_min_chang();
}

qint32
Pylt_settings::get_ch4_joy2_x_min()
{
    return ch4_joy2_x_min;
}

void
Pylt_settings::set_ch4_joy2_x_max(qint32 value)
{
    ch4_joy2_x_max = value;
    emit ch4_joy2_x_max_chang();
}

qint32
Pylt_settings::get_ch4_joy2_x_max()
{
    return ch4_joy2_x_max;
}

void
Pylt_settings::set_ch5_min(qint32 value)
{
    ch5_min = value;
    emit ch5_min_chang();
}

qint32
Pylt_settings::get_ch5_min()
{
    return ch5_min;
}

void
Pylt_settings::set_ch5_max(qint32 value)
{
    ch5_max = value;
    emit ch5_max_chang();
}

qint32
Pylt_settings::get_ch5_max()
{
    return ch5_max;
}

void
Pylt_settings::set_ch6_min(qint32 value)
{
    ch6_min = value;
    emit ch6_min_chang();
}

qint32
Pylt_settings::get_ch6_min()
{
    return ch6_min;
}

void
Pylt_settings::set_ch6_max(qint32 value)
{
    ch6_max = value;
    emit ch6_max_chang();
}

qint32
Pylt_settings::get_ch6_max()
{
    return ch6_max;
}

void
Pylt_settings::set_boot_fixing(bool value)
{
    boot_fixing = value;
    emit boot_fixing_chang();
}

bool
Pylt_settings::get_boot_fixing()
{
    return boot_fixing;
}

void
Pylt_settings::set_tank_mode  (bool value)
{
    tank_mode = value;
    emit tank_mode_chang();
}

bool
Pylt_settings::get_tank_mode  ()
{
    return tank_mode;
}

FullParam
Pylt_settings::getFullParam()
{
    FullParam p;

    // взять параметры из памяти, если есть
    //???

    // если нет - вернуть дефолтные
    p.ch1_joy1_y_min = ch1_joy1_y_min;
    p.ch1_joy1_y_max = ch1_joy1_y_max;
    p.ch2_joy1_x_min = ch2_joy1_x_min;
    p.ch2_joy1_x_max = ch2_joy1_x_max;
    p.ch3_joy2_y_min = ch3_joy2_y_min;
    p.ch3_joy2_y_max = ch3_joy2_y_max;
    p.ch4_joy2_x_min = ch4_joy2_x_min;
    p.ch4_joy2_x_max = ch4_joy2_x_max;
    p.ch5_min        = ch5_min;
    p.ch5_max        = ch5_max;
    p.ch6_min        = ch6_min;
    p.ch6_max        = ch6_max;
    p.boot_fixing    = boot_fixing;
    p.tank_mode      = tank_mode;

    return p;
}

int
Pylt_settings::saveFullParam()
{
    // сохранить параметры в память
    FullParam p;
    p.ch1_joy1_y_min = ch1_joy1_y_min;
    p.ch1_joy1_y_max = ch1_joy1_y_max;
    p.ch2_joy1_x_min = ch2_joy1_x_min;
    p.ch2_joy1_x_max = ch2_joy1_x_max;
    p.ch3_joy2_y_min = ch3_joy2_y_min;
    p.ch3_joy2_y_max = ch3_joy2_y_max;
    p.ch4_joy2_x_min = ch4_joy2_x_min;
    p.ch4_joy2_x_max = ch4_joy2_x_max;
    p.ch5_min        = ch5_min;
    p.ch5_max        = ch5_max;
    p.ch6_min        = ch6_min;
    p.ch6_max        = ch6_max;
    p.boot_fixing    = boot_fixing;
    p.tank_mode      = tank_mode;

    //??? сохранить параметры в память


    return 0;
}
