
#include "pylt_settings.h"
#include <QSettings>


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
    param.ch1_joy1_y_min = ch1_joy1_y_min;
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
    param.ch1_joy1_y_max = ch1_joy1_y_max;
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
    param.ch2_joy1_x_min = ch2_joy1_x_min;
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
    param.ch2_joy1_x_max = ch2_joy1_x_max;
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
    param.ch3_joy2_y_min = ch3_joy2_y_min;
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
    param.ch3_joy2_y_max = ch3_joy2_y_max;
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
    param.ch4_joy2_x_min = ch4_joy2_x_min;
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
    param.ch4_joy2_x_max = ch4_joy2_x_max;
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
    param.ch5_min = ch5_min;
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
    param.ch5_max = ch5_max;
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
    param.ch6_min = ch6_min;
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
    param.ch6_max = ch6_max;
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
    param.boot_fixing = boot_fixing;
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
    param.tank_mode = tank_mode;
    emit tank_mode_chang();
}

bool
Pylt_settings::get_tank_mode  ()
{
    return tank_mode;
}

void
Pylt_settings::set_ch1_inv       (bool value)
{
    ch1_inv = value;
    param.ch1_inv = ch1_inv;
    emit ch1_inv_chang();
}

bool
Pylt_settings::get_ch1_inv           ()
{
    return ch1_inv;
}

void
Pylt_settings::set_ch2_inv       (bool value)
{
    ch2_inv = value;
    param.ch2_inv = ch2_inv;
    emit ch2_inv_chang();
}

bool
Pylt_settings::get_ch2_inv           ()
{
    return ch2_inv;
}

void
Pylt_settings::set_ch3_inv       (bool value)
{
    ch3_inv = value;
    param.ch3_inv = ch3_inv;
    emit ch3_inv_chang();
}

bool
Pylt_settings::get_ch3_inv           ()
{
    return ch3_inv;
}

void
Pylt_settings::set_ch4_inv       (bool value)
{
    ch4_inv = value;
    param.ch4_inv = ch4_inv;
    emit ch4_inv_chang();
}

bool
Pylt_settings::get_ch4_inv           ()
{
    return ch4_inv;
}

void
Pylt_settings::set_ch5_inv       (bool value)
{
    ch5_inv = value;
    param.ch5_inv = ch5_inv;
    emit ch5_inv_chang();
}

bool
Pylt_settings::get_ch5_inv           ()
{
    return ch5_inv;
}

void
Pylt_settings::set_ch6_inv       (bool value)
{
    ch6_inv = value;
    param.ch6_inv = ch6_inv;
    emit ch6_inv_chang();
}

bool
Pylt_settings::get_ch6_inv           ()
{
    return ch6_inv;
}

void
Pylt_settings::set_ch1_cheng_ch3 (bool value)
{
    ch1_cheng_ch3 = value;
    param.ch1_cheng_ch3 = ch1_cheng_ch3;
    emit ch1_cheng_ch3_chang();
}

bool
Pylt_settings::get_ch1_cheng_ch3     ()
{
    return ch1_cheng_ch3;
}

void
Pylt_settings::set_ch2_cheng_ch4 (bool value)
{
    ch2_cheng_ch4 = value;
    param.ch2_cheng_ch4 = ch2_cheng_ch4;
    emit ch2_cheng_ch4_chang();
}

bool
Pylt_settings::get_ch2_cheng_ch4     ()
{
    return ch2_cheng_ch4;
}


FullParam
Pylt_settings::getFullParam()
{
    // если имя не записано - отправить дефолтные параметры
    if(devName.isEmpty())   return param;

    // взять параметры из памяти, если есть
    QSettings setting;

    QString dName;
    qint32 temp_ch1_joy1_y_min;
    qint32 temp_ch1_joy1_y_max;
    qint32 temp_ch2_joy1_x_min;
    qint32 temp_ch2_joy1_x_max;
    qint32 temp_ch3_joy2_y_min;
    qint32 temp_ch3_joy2_y_max;
    qint32 temp_ch4_joy2_x_min;
    qint32 temp_ch4_joy2_x_max;
    qint32 temp_ch5_min       ;
    qint32 temp_ch5_max       ;
    qint32 temp_ch6_min       ;
    qint32 temp_ch6_max       ;
    bool   temp_boot_fixing   ;
    bool   temp_tank_mode     ;
    bool   temp_ch1_inv       ;
    bool   temp_ch2_inv       ;
    bool   temp_ch3_inv       ;
    bool   temp_ch4_inv       ;
    bool   temp_ch5_inv       ;
    bool   temp_ch6_inv       ;
    bool   temp_ch1_cheng_ch3 ;
    bool   temp_ch2_cheng_ch4 ;

    QVector<QStringList> lastParams;

    int deviceNum = 0;
    while (true)
    {
        if (setting.contains(QString("lastParams%1").arg(deviceNum))) {
            lastParams.append(setting.value(QString("lastParams%1").arg(deviceNum)).toString().split(";"));
            ++deviceNum;    // следующий номер прибора для сохранения в истории
        } else break;
    }

    //добавятся в порядке сохранения
    for(int i = deviceNum - 1; i >= 0; i--)
    {
        dName = lastParams[i].at(0);
        temp_ch1_joy1_y_min = lastParams[i].at(1).toInt();
        temp_ch1_joy1_y_max = lastParams[i].at(2).toInt();
        temp_ch2_joy1_x_min = lastParams[i].at(3).toInt();
        temp_ch2_joy1_x_max = lastParams[i].at(4).toInt();
        temp_ch3_joy2_y_min = lastParams[i].at(5).toInt();
        temp_ch3_joy2_y_max = lastParams[i].at(6).toInt();
        temp_ch4_joy2_x_min = lastParams[i].at(7).toInt();
        temp_ch4_joy2_x_max = lastParams[i].at(8).toInt();
        temp_ch5_min        = lastParams[i].at(9).toInt();
        temp_ch5_max        = lastParams[i].at(10).toInt();
        temp_ch6_min        = lastParams[i].at(11).toInt();
        temp_ch6_max        = lastParams[i].at(12).toInt();
        temp_boot_fixing      = lastParams[i].at(13).toInt();
        temp_tank_mode        = lastParams[i].at(14).toInt();
        temp_ch1_inv          = lastParams[i].at(15).toInt();
        temp_ch2_inv          = lastParams[i].at(16).toInt();
        temp_ch3_inv          = lastParams[i].at(17).toInt();
        temp_ch4_inv          = lastParams[i].at(18).toInt();
        temp_ch5_inv          = lastParams[i].at(19).toInt();
        temp_ch6_inv          = lastParams[i].at(20).toInt();
        temp_ch1_cheng_ch3    = lastParams[i].at(21).toInt();
        temp_ch2_cheng_ch4    = lastParams[i].at(22).toInt();


        if(!dName.isEmpty() && !devName.isEmpty())
        {
            // если найдено устройство с таким именем(именем подключенного устройства) - обновить параметры
            if(dName == devName)
            {
                set_ch1_joy1_y_min(temp_ch1_joy1_y_min);
                set_ch1_joy1_y_max(temp_ch1_joy1_y_max);
                set_ch2_joy1_x_min(temp_ch2_joy1_x_min);
                set_ch2_joy1_x_max(temp_ch2_joy1_x_max);
                set_ch3_joy2_y_min(temp_ch3_joy2_y_min);
                set_ch3_joy2_y_max(temp_ch3_joy2_y_max);
                set_ch4_joy2_x_min(temp_ch4_joy2_x_min);
                set_ch4_joy2_x_max(temp_ch4_joy2_x_max);
                set_ch5_min       (temp_ch5_min       );
                set_ch5_max       (temp_ch5_max       );
                set_ch6_min       (temp_ch6_min       );
                set_ch6_max       (temp_ch6_max       );
                set_boot_fixing   (temp_boot_fixing   );
                set_tank_mode     (temp_tank_mode     );
                set_ch1_inv       (temp_ch1_inv       );
                set_ch2_inv       (temp_ch2_inv       );
                set_ch3_inv       (temp_ch3_inv       );
                set_ch4_inv       (temp_ch4_inv       );
                set_ch5_inv       (temp_ch5_inv       );
                set_ch6_inv       (temp_ch6_inv       );
                set_ch1_cheng_ch3 (temp_ch1_cheng_ch3 );
                set_ch2_cheng_ch4 (temp_ch2_cheng_ch4 );
            }
        }
    }

    return param;
}

FullParam
Pylt_settings::getStructFullParam()
{
    return param;
}

void
Pylt_settings::setDevName(QString newName)
{
    if(newName.isEmpty())   return;
    devName = newName;
}

int
Pylt_settings::saveFullParam()
{
    // если имя не записано - выйти
    if(devName.isEmpty())   return -1;

    QSettings setting;

    //вычитываю все устройства
    QVector<QStringList> lastParams;
    int deviceNum = 0;
    while (true)
    {
        if (setting.contains(QString("lastParams%1").arg(deviceNum))) {
            lastParams.append(setting.value(QString("lastParams%1").arg(deviceNum)).toString().split(";"));
            ++deviceNum;    // следующий номер прибора для сохранения в истории
        } else break;
    }

    // TODO проверка есть ли устройство в списке сохраненных
    for(QStringList parametrs : lastParams) {
        if(parametrs.at(0) == devName)  return 0;
    }

    //сохраняю новое устройство
    QString device = QStringList({devName,
                                  QString("%1").arg(param.ch1_joy1_y_min),
                                  QString("%1").arg(param.ch1_joy1_y_max),
                                  QString("%1").arg(param.ch2_joy1_x_min),
                                  QString("%1").arg(param.ch2_joy1_x_max),
                                  QString("%1").arg(param.ch3_joy2_y_min),
                                  QString("%1").arg(param.ch3_joy2_y_max),
                                  QString("%1").arg(param.ch4_joy2_x_min),
                                  QString("%1").arg(param.ch4_joy2_x_max),
                                  QString("%1").arg(param.ch5_min),
                                  QString("%1").arg(param.ch5_max),
                                  QString("%1").arg(param.ch6_min),
                                  QString("%1").arg(param.ch6_max),
                                  QString("%1").arg(param.boot_fixing),
                                  QString("%1").arg(param.tank_mode),
                                  QString("%1").arg(param.ch1_inv),
                                  QString("%1").arg(param.ch2_inv),
                                  QString("%1").arg(param.ch3_inv),
                                  QString("%1").arg(param.ch4_inv),
                                  QString("%1").arg(param.ch5_inv),
                                  QString("%1").arg(param.ch6_inv),
                                  QString("%1").arg(param.ch1_cheng_ch3),
                                  QString("%1").arg(param.ch2_cheng_ch4),
                                 }).join(";");
    setting.setValue(QString("lastParams%1").arg(deviceNum), device);    // запись нового устройства

    return 0;
}
