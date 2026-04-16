#include "rx_commands.h"


Rx_commands::Rx_commands(QObject *parent) :
    QObject(parent)
{

}

Rx_commands::~Rx_commands()
{

}

void
Rx_commands::setCommun_display(Commun_display *newCommun_display)
{
    _commun_display = newCommun_display;
}

void
Rx_commands::setUpdateHex(UpdateHex *newUpdateHex)
{
    _updatehex = newUpdateHex;
}

void
Rx_commands::setSettings(Settings *newSettings)
{
    _settings = newSettings;
}

int
Rx_commands::searchCommand(QByteArray dat)
{
    const std::string empty;
    quint8 k = dat[0];
    Data = dat;
    Data.replace(0, 1, empty);
    int res = 1;

    switch (k) {
    case 0x01:
        errorCrc();
        break;
    case 0x04: //не правильная команда загрузчика
        incorrectLoaderCommand();
        break;
    case 0x05:
        incorrectCommand();
        break;
    case 0x06:
        incorrectCommandParameters();
        break;
    case 0x07: //не правильныая очередность пакетов
        incorrectPacketOrder();
        break;
    case 0x08: //ошибка загрузки прошивки
        errorFirmwareDownload();
        break;
    case 0x10:
        errorGyroscope();
        break;
    case 0x20:
        errorErasingFlash();
        break;
    case 0x21:
        errorWritingFlash();
        break;
    case 0x30:
        errorLowVoltage();
        break;
    case 0x31:
        errorHightVoltage();
        break;
    case 0x35:
        errorExcessCurrent();
        break;
    case 0x38:
        errorCurrentSensorFailure();
        break;
    case 0x39:
        errorCurrentSensorCalibration();
        break;
    case 0x40:
        checkingServiceabilityServos();
        break;
    case 0x41:
        errorServsInitialization();
        break;
    case 0x42:
        errorServoFailure(Data);
        break;
    case 0x43:
        errorServoDriverFailure(Data);
        break;
    case 0x45:
        errorAutoCalibrations();
        break;
    case 0x48:
        autocalibrationCompleted();
        break;
    case 0x50:
        idleTimeExceeded();
        break;
    case 0x51:
        idleTimeLift();
        break;
    case 0x55:
        errorMath();
        break;
    case 0x90:
        debugMess(Data);
        break;
    case 0xA0:
        leds_read(Data);
        break;
    case 0xA1:
        voltage_read(Data);
        break;
    case 0xA2:
        current_read(Data);
        break;
    case 0xA3:
        readGyroscopeAngles(Data);
        break;
    case 0xA4:
        readAcceleration(Data);
        break;
    case 0xA5:
        readAngleServo(Data);
        break;
    case 0xA6:
        on_offServo(Data);
        break;
    case 0xA7:
        readAllParams(Data);
        break;
    case 0xA8:
        writeAllParams();
        break;
    case 0xA9:
        getParamsChart(Data);
        break;
    case 0xAA:
        setBrightness();
        break;
    case 0xE0:
        fullReset();
        break;
    case 0xE1:
        writeMinAngleServo();
        break;
    case 0xE2:
        writeSettingLeds();
        break;
    case 0xE3:
        setServoAngle();
        break;
    case 0xE4:
        setCurBlock(Data);
        break;
    case 0xE5:
        getCheck();
        break;
    case 0xE6:
        getSettingServo(Data);
        break;
    case 0xE7:
        servoAutoCalibration(Data);
        break;
    case 0xE8:
        curAutoCalibration(Data);
        break;
    case 0xE9:
        setCurCalibration();
        break;
    case 0xEA:
        getCurCalibration(Data);
        break;
    case 0xEB:
        checkReset();
        break;
    case 0xEC:
        setServosStartAngle();
        break;
    case 0xED:
        setServosHomeAngle();
        break;
    case 0xEE:
        writeHomeAngleFoot();
        break;
    case 0xEF:
        resetServsFoot();
        break;
    case 0xF0:
        reboot();
        break;
    case 0xF1:
        getParamServsFoot(Data);
        break;
    case 0xF2:
        getVersion(Data);
        break;
    case 0xF3:
        writeProgram(Data);
        break;
    case 0xF4:
        writeBootloader(Data);
        break;
    case 0xF5:
        writeLabel();
        break;
    case 0xF6:
        shutdown();
        break;
    case 0xF7:
        getIntendifier(Data);
        break;
    case 0xF8:
        getInclinationAngle(Data);
        break;
    case 0xFA:
        getAzimutAngle(Data);
        break;
    case 0xFC:
        calibrServsFoot(Data);
        break;
    default:
        //если не найдено совпадение
        return res = -1;
        break;
    }

    Data.replace(0, Data.size(), empty);  //очистка массива
    return res;
}

void
Rx_commands::f_AdminChange(bool f)
{
    _f_Admin = f;
}

void
Rx_commands::setVReal(bool V)
{
    _setVreal = V;
}

void
Rx_commands::setCurReal(bool Cur)
{
    _setCurReal = Cur;
}

//ошибка срс    0x01
void
Rx_commands::errorCrc()
{
    if(_f_Admin)    _commun_display->log_out_J("<- ошибка CRC", " ");
    _commun_display->log_out_S("<- ошибка CRC", " ");
}

//не правильная команда загрузчика  0x04
void
Rx_commands::incorrectLoaderCommand()
{
   _updatehex->on_pbStop_clicked("Ошибка. Не правильная команда загрузчика. ");
}

//не правильная команда                 0x05
void
Rx_commands::incorrectCommand()
{
    if(_f_Admin)    _commun_display->log_out_J("<- не верная команда", " ");
    _commun_display->log_out_S("<- не верная команда", " ");
}

//не правильные параметры команды       0x06
void
Rx_commands::incorrectCommandParameters()
{
    if(_f_Admin)    _commun_display->log_out_J("<- не верные парам. команды", " ");
    _commun_display->log_out_S("<- не верные парам. команды", " ");
}

//не правильная очередность пакетов     0x07
void
Rx_commands::incorrectPacketOrder()
{
    _updatehex->on_pbStop_clicked("Ошибка. Не правильныая очередность пакетов. ");
}

//ошибка загрузки прошивки              0x08
void
Rx_commands::errorFirmwareDownload()
{
    _updatehex->on_pbStop_clicked("Ошибка загрузки прошивки. ");
}

//ошибка гироскопа                      0x10
void
Rx_commands::errorGyroscope()
{
    _commun_display->log_out_J("<- ошибка гироскопа", " ");
    _commun_display->log_out_S("<- ошибка гироскопа", " ");
}

//ошибка стирания флеш                  0x20
void
Rx_commands::errorErasingFlash()
{
    _commun_display->log_out_J("<- ошибка стирания flash", " ");
    _commun_display->log_out_S("<- ошибка стирания flash", " ");
    _updatehex->on_pbStop_clicked("Ошибка стирания flash. ");
}

//ошибка записи во флеш                 0x21
void
Rx_commands::errorWritingFlash()
{
    _commun_display->log_out_J("<- ошибка записи во flash", " ");
    _commun_display->log_out_S("<- ошибка записи во flash", " ");
    _updatehex->on_pbStop_clicked("Ошибка записи во flash. ");
}

// ошибка низкое напряжение             0x30
void
Rx_commands::errorLowVoltage()
{
    _commun_display->log_out_J("<- низкое напряжение", " ");
    _commun_display->log_out_S("<- низкое напряжение", " ");
}

// ошибка высокое напряжение            0x31
void
Rx_commands::errorHightVoltage()
{
    _commun_display->log_out_J("<- высокое напряжение", " ");
    _commun_display->log_out_S("<- высокое напряжение", " ");
}

//превышение допустимого тока           0x35
void
Rx_commands::errorExcessCurrent()
{
    _commun_display->log_out_J("<- превышение доп. тока", " ");
    _commun_display->log_out_S("<- превышение доп. тока", " ");
}

//отказ датчика тока                    0x38
void
Rx_commands::errorCurrentSensorFailure()
{
    _commun_display->log_out_J("<- отказ датчика тока", " ");
    _commun_display->log_out_S("<- отказ датчика тока", " ");
}

//ошибка калибровки датчика тока        0x39
void
Rx_commands::errorCurrentSensorCalibration()
{
    _commun_display->log_out_J("<- ошибка калибр. дат. тока", " ");
    _commun_display->log_out_S("<- ошибка калибр. дат. тока", " ");
}

//проверка исправности сервоприводов    0x40
void
Rx_commands::checkingServiceabilityServos()
{
    _commun_display->log_out_J("<- проверка испр. серв", " ");
    _commun_display->log_out_S("<- проверка испр. серв", " ");
}

//ошибка инициализации серв             0x41
void
Rx_commands::errorServsInitialization()
{
    _commun_display->log_out_J("<- ошибка инициализации серв", " ");
    _commun_display->log_out_S("<- ошибка инициализации серв", " ");
}

//отказ сервопривода (номер)            0x42
void
Rx_commands::errorServoFailure(QByteArray Num)
{
    quint8  u8 = Num[0];
    _commun_display->log_out_J("<- отказ сервы № ", QString(QString::number(u8)));
    _commun_display->log_out_S("<- отказ сервы № ", QString(QString::number(u8)));
}

//отказ драйвера сервопривода (номер)  0x43
void
Rx_commands::errorServoDriverFailure(QByteArray Num)
{
    quint8  u8 = Num[0];
    _commun_display->log_out_J("<- отказ драйвера серв № ", QString(QString::number(u8)));
    _commun_display->log_out_S("<- отказ драйвера серв № ", QString(QString::number(u8)));
}

//ошибка автокалибровки                 0x45
void
Rx_commands::errorAutoCalibrations()
{
    _commun_display->log_out_J("<- ошибка автокалибр.", " ");
    _commun_display->log_out_S("<- ошибка автокалибр.", " ");
}

//автокалибровка выполнена              0x48
void
Rx_commands::autocalibrationCompleted()
{
    _commun_display->log_out_J("<- автокалибровка завершена", " ");
    _commun_display->log_out_S("<- автокалибровка завершена", " ");
}

//превышено время бездействия           0x50
void
Rx_commands::idleTimeExceeded()
{
    _commun_display->log_out_J("<- превышено время бездействия", " ");
    _commun_display->log_out_S("<- превышено время бездействия", " ");
}

//превышено время попытки подняться     0x51
void
Rx_commands::idleTimeLift()
{
    _commun_display->log_out_J("<- превышено время попытки подняться", " ");
    _commun_display->log_out_S("<- превышено время попытки подняться", " ");
}

//ошибка математики                     0x55
void
Rx_commands::errorMath()
{
    if(_f_Admin)    _commun_display->log_out_J("<- ошибка математики", " ");
    _commun_display->log_out_S("<- ошибка математики", " ");
}

//сообщение дебага                       0x90
void
Rx_commands::debugMess(QByteArray mess)
{
    QString str = QString(mess);

    if(_f_Admin)    _commun_display->log_out_J("<- ", str);
    _commun_display->log_out_S("<- ", str);
}

//чтение состояния светодиодов     0xA0
void
Rx_commands::leds_read(QByteArray leds)
{
    quint8  u8 = leds[0];
    quint8  u8_2 = leds[1];
    quint8  u8_3 = leds[2];
    quint8  u8_4 = leds[3];
    _commun_display->log_out_S("<- гамма светодиода № ", QString(QString::number(u8)) + "\nR "
                                                      + QString(QString::number(u8_2)) + "   G "
                                                      + QString(QString::number(u8_3)) + "   B "
                                                      + QString(QString::number(u8_4)));
}

//чтение напряжения                 0xA1
void
Rx_commands::voltage_read(QByteArray Volt)
{
    f_value val;
    val.data[0] = Volt[0];
    val.data[1] = Volt[1];
    val.data[2] = Volt[2];
    val.data[3] = Volt[3];
    if(val.f == val.f)
    {
        _commun_display->vrealChang(val.f);

        if(_setVreal)
        {
            _commun_display->log_out_S("<- напряжение ",  QString(QString::number(val.f)));
            _setVreal = false;
        }
    }
}

//чтение тока                       0xA2
void
Rx_commands::current_read(QByteArray Cur)
{
    f_value val;
    val.data[0] = Cur[0];
    val.data[1] = Cur[1];
    val.data[2] = Cur[2];
    val.data[3] = Cur[3];
    if(val.f == val.f)
    {
        _commun_display->curRealChang(val.f);
        if(_setCurReal)
        {
            _commun_display->log_out_S("<- ток ", QString(QString::number(val.f)));
            _setCurReal = false;
        }
    }
}

//чтение углов гироскопа            0xA3
void
Rx_commands::readGyroscopeAngles(QByteArray Gyr)
{
    f_value val;
    float f;
    val.data[0] = Gyr[0];
    val.data[1] = Gyr[1];
    val.data[2] = Gyr[2];
    val.data[3] = Gyr[3];
    if(val.f == val.f)
    {
        f = val.f;
        val.data[0] = Gyr[4];
        val.data[1] = Gyr[5];
        val.data[2] = Gyr[6];
        val.data[3] = Gyr[7];

        if(val.f == val.f)
        {
            _commun_display->log_out_S("<- азимут, наклон\n", QString(QString::number(f)) + ",   " + QString(QString::number(val.f)));
        }
    }
}

//чтение ускорения                  0xA4
void
Rx_commands::readAcceleration(QByteArray Accel)
{
    f_value val;
    float f;
    val.data[0] = Accel[0];
    val.data[1] = Accel[1];
    val.data[2] = Accel[2];
    val.data[3] = Accel[3];
    if(val.f == val.f)
    {
        f = val.f;
        val.data[0] = Accel[4];
        val.data[1] = Accel[5];
        val.data[2] = Accel[6];
        val.data[3] = Accel[7];

        if(val.f == val.f)
        {
            _commun_display->log_out_S("<- ускорение, угловая скорость\n", QString(QString::number(f)) + ",   " + QString(QString::number(val.f)));
        }
    }
}

//чтение угла сервы                 0xA5
void
Rx_commands::readAngleServo(QByteArray Angle)
{
    f_value val;
    quint8 num;
    quint16 angl;

    num = Angle[0];

    val.u16[1] = Angle[2];
    val.u16[0] = Angle[1];
    angl = val.U16;

    _commun_display->log_out_S("<- угол сервы № ", QString(QString::number(num)) + "   " + QString(QString::number(angl)));
}

//вкл/выкл сервы                    0xA6
void
Rx_commands::on_offServo(QByteArray flag)
{
    quint8 u8 = flag[0];
    if(u8 == 0x00)
    {
        _commun_display->log_out_J("<- сервы OFF", "");
        _commun_display->log_out_S("<- сервы OFF", "");
    }
    else if(u8 == 0x01)
    {
        _commun_display->log_out_J("<- сервы ON", "");
        _commun_display->log_out_S("<- сервы ON", "");
    }
}

//запросить все параметры(точка восстановления) 0xA7
void
Rx_commands::readAllParams(QByteArray Params)
{
    if(Params.size() > 10)
    {
        if(!_settings->set_full_param(Data))
        {
            _commun_display->log_out_J("<- точка восстановления создана", "");
            _commun_display->log_out_S("<- точка восстановления создана", "");
            return;
        }
        _commun_display->log_out_J("<- ошибка записи параметров", "");
        _commun_display->log_out_S("<- ошибка записи параметров", "");
    }
    else
    {
        _commun_display->log_out_J("<- отсутствуют входящие параметры", "");
        _commun_display->log_out_S("<- отсутствуют входящие параметры", "");
    }
}

//отправить все параметры(точка восстановления)    0xA8
void
Rx_commands::writeAllParams()
{
   _commun_display->log_out_J("<- параметры восстановленны", "");
   _commun_display->log_out_S("<- параметры восстановленны", "");
}

//получение параметров для графика 0xA9
void
Rx_commands::getParamsChart(QByteArray Params)
{
    f_value val;
    float volt = 0.0f;
    float cur = 0.0f;
    float tilt_angle = 0.0f;
    float tilt_direction = 0.0f;
    float boost = 0.0f;
    float angular_velocity = 0.0f;
    float angleX = 0.0f;
    float angleY = 0.0f;
    float angleZ = 0.0f;

    val.data[0] = Params[0];
    val.data[1] = Params[1];
    val.data[2] = Params[2];
    val.data[3] = Params[3];
    if(val.f == val.f)  volt = val.f;

    val.data[0] = Params[4];
    val.data[1] = Params[5];
    val.data[2] = Params[6];
    val.data[3] = Params[7];
    if(val.f == val.f)  cur = val.f;

    val.data[0] = Params[8];
    val.data[1] = Params[9];
    val.data[2] = Params[10];
    val.data[3] = Params[11];
    if(val.f == val.f)  tilt_angle = val.f;

    val.data[0] = Params[12];
    val.data[1] = Params[13];
    val.data[2] = Params[14];
    val.data[3] = Params[15];
    if(val.f == val.f)  tilt_direction = val.f;

    val.data[0] = Params[16];
    val.data[1] = Params[17];
    val.data[2] = Params[18];
    val.data[3] = Params[19];
    if(val.f == val.f)  boost = val.f;

    val.data[0] = Params[20];
    val.data[1] = Params[21];
    val.data[2] = Params[22];
    val.data[3] = Params[23];
    if(val.f == val.f)  angular_velocity = val.f;

    val.data[0] = Params[24];
    val.data[1] = Params[25];
    val.data[2] = Params[26];
    val.data[3] = Params[27];
    if(val.f == val.f)  angleX = val.f;

    val.data[0] = Params[28];
    val.data[1] = Params[29];
    val.data[2] = Params[30];
    val.data[3] = Params[31];
    if(val.f == val.f)  angleY = val.f;

    val.data[0] = Params[32];
    val.data[1] = Params[33];
    val.data[2] = Params[34];
    val.data[3] = Params[35];
    if(val.f == val.f)  angleZ = val.f;

    _commun_display->vrealChang(volt);
    _commun_display->curRealChang(cur);

    _commun_display->graphsOutput(volt, cur, tilt_angle, tilt_direction, boost, angular_velocity, angleX, angleY, angleZ);
}

//подтверждение установленной яркости светодиодной ленты
void
Rx_commands::setBrightness()
{
    _commun_display->log_out_S("<- яркость oled изменена", " ");
}

//сброс до заводских нистроек                       0xE0
void
Rx_commands::fullReset()
{
    _commun_display->log_out_S("<- сброс до заводских ок", " ");
    _commun_display->log_out_J("<- сброс до заводских ок", " ");
}

//запись мин угла сервы во флеш, остальные углы рассчитываются     0xE1
void
Rx_commands::writeMinAngleServo()
{
   _commun_display->log_out_S("<- установки сервы записаны", " ");
}

//запись установок для светодиодов      0xE2
void
Rx_commands::writeSettingLeds()
{
   _commun_display->log_out_S("<- установки светодиода записаны", " ");
}

//установить серву в угол               0xE3
void
Rx_commands::setServoAngle()
{
    _commun_display->log_out_S("<- угол сервы установлен", " ");
}

//установка флага включения/отключения блокировки сброса параметров тока       0xE4
void
Rx_commands::setCurBlock(QByteArray flag)
{
    quint8 u8 = flag[0];
    if(u8 == 0x00)
    {
        _commun_display->log_out_S("<- флаг блокировки тока выкл.", " ");
    }
    else if(u8 == 0x01)
    {
        _commun_display->log_out_S("<- флаг блокировки тока вкл.", " ");
    }
}

//запрос ошибок                    0xE5
void
Rx_commands::getCheck()
{
    _commun_display->log_out_S("<- запрос ошибок ок", " ");
    _commun_display->log_out_J("<- запрос ошибок ок", " ");
}

//запрос предустановок сервы        0xE6
void
Rx_commands::getSettingServo(QByteArray Params)
{
    f_value val;
    quint8 num;
    quint16 min;
    quint16 max;
    quint16 home;
    quint16 start;

    num = Params[0];

    val.data[1] = Params[2];
    val.data[0] = Params[1];
    min = val.U16;

    val.data[1] = Params[4];
    val.data[0] = Params[3];
    max = val.U16;

    val.data[1] = Params[6];
    val.data[0] = Params[5];
    home = val.U16;

    val.data[1] = Params[8];
    val.data[0] = Params[7];
    start = val.U16;

    _commun_display->log_out_S("<- установки сервы №", QString(QString::number(num)) + "\nmin " + QString(QString::number(min))
                   + "   max " + QString(QString::number(max)) + "   home " + QString(QString::number(home))
                   + "   start " + QString(QString::number(start)));
}

//выполнить авто калибровку серв     0xE7
void
Rx_commands::servoAutoCalibration(QByteArray flag)
{
    if(flag[0] == 0)
    {
        _commun_display->log_out_S("<- калибровка серв остановлена", " ");
        _commun_display->log_out_J("<- калибровка серв остановлена", " ");
    }
    else if(flag[0] == 1)
    {
        _commun_display->log_out_S("<- калибровка серв запущена", " ");
        _commun_display->log_out_J("<- калибровка серв запущена", " ");
    }
}

//выполнить калибровку датчика тока     0xE8
void
Rx_commands::curAutoCalibration(QByteArray Cur)
{
    f_value val;
    val.data[0] = Cur[0];
    val.data[1] = Cur[1];
    val.data[2] = Cur[2];
    val.data[3] = Cur[3];
    if(val.f == val.f)
    {
        _commun_display->log_out_S("<- калибровка тока ок ", QString(QString::number(val.f)));
        _commun_display->log_out_J("<- калибровка тока ок ", QString(QString::number(val.f)));
    }
}

//записать значение калибровки тока  0xE9
void
Rx_commands::setCurCalibration()
{
     _commun_display->log_out_S("<- знач. калибр. тока записаны", " ");
}

//запрос значений калибровки тока  0xEA
void
Rx_commands::getCurCalibration(QByteArray Cur)
{
    f_value val;
    val.data[0] = Cur[0];
    val.data[1] = Cur[1];
    val.data[2] = Cur[2];
    val.data[3] = Cur[3];
    if(val.f == val.f)
    {
        _commun_display->log_out_S("<- знач. калибр. тока ", QString(QString::number(val.f)));
    }
}

//сбросить ошибки  0xEB
void
Rx_commands::checkReset()
{
    _commun_display->log_out_J("<- ошибки стерты", " ");
    _commun_display->log_out_S("<- ошибки стерты", " ");
}

//установить сервы в стартовое положение  0xEC
void
Rx_commands::setServosStartAngle()
{
    _commun_display->log_out_S("<- сервы установлены", " ");
}

//установить сервы в домашнее положение  0xED
void
Rx_commands::setServosHomeAngle()
{
     _commun_display->log_out_S("<- сервы установлены", " ");
}

//запись домашних углов всех серв ноги во флеш, остальные углы рассчитываются     0xEE
void
Rx_commands::writeHomeAngleFoot()
{
    _commun_display->log_out_S("<- Углы записаны", " ");
    _commun_display->log_out_J("<- Углы записаны", " ");
}

//сброс до заводских параметры серв ноги №     0xEF
void
Rx_commands::resetServsFoot()
{
    _commun_display->log_out_S("<- Reset angl OK", " ");
    _commun_display->log_out_J("<- Reset angl OK", " ");
}

//перезагрузить контроллер     0xF0
void
Rx_commands::reboot()
{
    _commun_display->log_out_S("<- перезагрузка", " ");
    _commun_display->log_out_J("<- перезагрузка", " ");
}

//запрос параметров серв ноги №     0xF1
void
Rx_commands::getParamServsFoot(QByteArray Angle)
{
    f_value val;
    quint8 num;
    quint16 coxa;
    quint16 femur;
    quint16 tiba;

    num = Angle[0];

    val.data[1] = Angle[2];
    val.data[0] = Angle[1];
    coxa = val.U16;

    val.data[1] = Angle[4];
    val.data[0] = Angle[3];
    femur = val.U16;

    val.data[1] = Angle[6];
    val.data[0] = Angle[5];
    tiba = val.U16;

    _settings->setcoxaAngl(coxa);
    _settings->setfemurAngl(femur);
    _settings->settibaAngl(tiba);

    _commun_display->log_out_S("<- Углы ноги № " + QString(QString::number(num)), " прочитаны");
    _commun_display->log_out_J("<- Углы ноги № " + QString(QString::number(num)), " прочитаны");
}

//запрос версии  0xF2
void
Rx_commands::getVersion(QByteArray Ver)
{
    f_value val;
    //версия прошивки загрузчика
    val.data[3] = Ver[0]; val.data[2] = Ver[1]; val.data[1] = Ver[2]; val.data[0] = Ver[3];
    _updatehex->setVersBootLoaderExt(val.u32);

    if(_f_Admin)
    {
        _commun_display->log_out_S("<- Версия загрузчика:\n", _updatehex->versionToString(val.u32));
        _commun_display->log_out_J("<- Версия загрузчика:\n", _updatehex->versionToString(val.u32));
    }

    val.data[3] = Ver[4]; val.data[2] = Ver[5]; val.data[1] = Ver[6]; val.data[0] = Ver[7];
    _updatehex->setVersExt(val.u32);

    _commun_display->log_out_S("<- Версия прошивки:\n", _updatehex->versionToString(val.u32));
    _commun_display->log_out_J("<- Версия прошивки:\n", _updatehex->versionToString(val.u32));
}

//команда начала передачи основной программы, передача основной программы  0xF3
void
Rx_commands::writeProgram(QByteArray Num)
{
    f_value val;
    //номер успешно полученного пакета основной прошивки
    val.data[1] = Num[0]; val.data[0] = Num[1];
    qint32 num = val.int32;
    _commun_display->log_out_S(tr("<- подтверждение приема пакета №  %1").arg(num), "");
    _commun_display->log_out_J(tr("<- подтверждение приема пакета №  %1").arg(num), "");

    _updatehex->setPageTx(val.int32);
}

//команда начала передачи загрузчика, передача загрузчика  0xF4
void
Rx_commands::writeBootloader(QByteArray Num)
{
    f_value val;
    //номер успешно полученного пакета загрузчика
    val.data[1] = Num[0]; val.data[0] = Num[1];
    qint32 num = val.int32;
    _commun_display->log_out_S(tr("<- подтверждение приема пакета №  %1").arg(num), "");
    _commun_display->log_out_J(tr("<- подтверждение приема пакета №  %1").arg(num), "");

    _updatehex->setPageTx(val.int32);
}

//подпись программы 0xF5
void
Rx_commands::writeLabel()
{
    _commun_display->log_out_S("<- Подпись ок","");
}

//выключить      0xF6
void
Rx_commands::shutdown()
{
    _commun_display->log_out_J("<- выключение","");
    _commun_display->log_out_S("<- выключение","");
}

//ответ запроса ID устройства 0xF7
void
Rx_commands::getIntendifier(QByteArray Num)
{
    f_value val;

    val.data[3] = Num[0];
    val.data[2] = Num[1];
    val.data[1] = Num[2];
    val.data[0] = Num[3];
    int id = val.int32;

    _commun_display->log_out_S(tr("<- ID %1").arg(id), "");
    _settings->setIdDevice(id, false);
}

//ответ запроса/установки поправочного угла наклона 0xF8
void
Rx_commands::getInclinationAngle(QByteArray Angle)
{
    f_value val;
    val.data[0] = Angle[0];
    val.data[1] = Angle[1];
    val.data[2] = Angle[2];
    val.data[3] = Angle[3];
    if(val.f == val.f)
    {
        _commun_display->log_out_S("<- установка поправочного угла наклона ",  QString(QString::number(val.f)));
    }
}

//ответ запроса/установки поправочного угла азимута 0xFA
void
Rx_commands::getAzimutAngle(QByteArray Angle)
{
    f_value val;
    val.data[0] = Angle[0];
    val.data[1] = Angle[1];
    val.data[2] = Angle[2];
    val.data[3] = Angle[3];
    if(val.f == val.f)
    {
        _commun_display->log_out_S("<- установка поправочного угла азимута ",  QString(QString::number(val.f)));
    }
}

//ответ старт/стоп автокалибровки серв ноги №.. 0xFC
void
Rx_commands::calibrServsFoot(QByteArray Num)
{
    if(Num.size() >= 1)
    {
        qint8 footNum = Num[0] + 1;
        _commun_display->log_out_S("<- старт автокалибровки ноги № ",  QString(QString::number(footNum)));
        _commun_display->log_out_J("<- старт автокалибровки ноги № ",  QString(QString::number(footNum)));
    }
    else
    {
        _commun_display->log_out_S("<- стоп автокалибровки ноги", "");
        _commun_display->log_out_J("<- стоп автокалибровки ноги", "");
    }
}

