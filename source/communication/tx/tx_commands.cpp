#include <QRegularExpression>
#include "tx_commands.h"


Tx_commands::Tx_commands(QObject *parent) :
    QObject(parent)
{

}

Tx_commands::~Tx_commands()
{

}

void
Tx_commands::setPacking(Packing *newPacking)
{
    _packing = newPacking;
}

void Tx_commands::setCommun_display(Commun_display *newCommun_display)
{
    _commun_display = newCommun_display;
}

void Tx_commands::setSettings(Settings *newSettings)
{
    _settings = newSettings;
}


///активность джойстика
//команда 0x0A - ходить
//команда 0x0B - катиться
//команда 0x0C - танец
//команда 0x00 - делать ничего
int
Tx_commands::joysticActivity(quint8 mode, float azimut, float amplitude, float level, bool ctrl)
{
    QByteArray data;
    int res = -1;
    quint8 comand;
    qint16 azimut16 = 0;
    quint16 amplitude16;
    quint16 level16 = level;
    QString s;


    switch(mode)
    {
        case 1:
            comand = 0x0A;
            break;
        case 2:
            comand = 0x0B;
            break;
        case 3:
            comand = 0x0C;
            break;
        default:
            comand = 0x00;
            break;
    }

    azimut = azimut - 3.14 / 2;
    if (azimut > 0) {
        azimut16 = azimut * 57.3248;
    } else {
        azimut16 = ((3.14 - qAbs(azimut)) + 3.14) * 57.3248 + 1;
    }

    if(mode == 3)
    {
        amplitude16 = amplitude * (_settings->getJoystickAmplitude() / 2);
    }
    else
    {
        amplitude16 = amplitude * _settings->getJoystickAmplitude();
    }

    data.append(comand);
    data.append((azimut16 >> 8) & 0x00FF);
    data.append(azimut16 & 0x00FF);
    data.append((amplitude16 >> 8) & 0x00FF);
    data.append(amplitude16 & 0x00FF);
    data.append((level16 >> 8) & 0x00FF);
    data.append(level16 & 0x00FF);
    data.append(ctrl);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}


//чтение состояния светодиодов 0xA0
int
Tx_commands::leds_read(QString msg)
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "опрос светодиода # " + msg;
    quint8 comand = 0xA0;

    //раскладывание строки по интовым числам
    StringToIntToByte(msg, &data);

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//чтение напряжения 0xA1
int
Tx_commands::voltage_read(bool flagDisplay)
{
    QByteArray data;
    int res = -1;
    QString s;
    if(flagDisplay) s = "запрос напряжения";
    quint8 comand = 0xA1;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//чтение тока 0xA2
int
Tx_commands::current_read(bool flagDisplay)
{
    QByteArray data;
    int res = -1;
    QString s;
    if(flagDisplay) s = "запрос тока";
    quint8 comand = 0xA2;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//чтение углов гироскопа 0xA3
int
Tx_commands::readGyroscopeAngles()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "запрос углов гироскопа";
    quint8 comand = 0xA3;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//чтение ускорения 0xA4
int
Tx_commands::readAcceleration()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "запрос ускорения";
    quint8 comand = 0xA4;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//чтение угла сервы 0xA5
int
Tx_commands::readAngleServo(QString msg)
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "запрос угла сервы # " + msg;
    quint8 comand = 0xA5;

    //раскладывание строки по интовым числам
    StringToIntToByte(msg, &data);

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//вкл/откл сервы 0xA6
int
Tx_commands::on_offServo(QString msg)
{
    QByteArray data;
    int res = -1;
    QString s;
    quint8 comand = 0xA6;

    //раскладывание строки по интовым числам
    StringToIntToByte(msg, &data);

    if(data[0] == 0x00)    s = "питание серв OFF";
    else                   s = "питание серв ON";

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//запросить все параметры(точка восстановления) 0xA7
int
Tx_commands::readAllParams()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "запрос точки восстановления";
    quint8 comand = 0xA7;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//отправить все параметры(точка восстановления) 0xA8
int
Tx_commands::writeAllParams()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "отправка точки восстановления";
    quint8 comand = 0xA8;

    //вписывание команды
    data.prepend(comand);

    //вписывание параметров
    data.prepend(_settings->get_full_param());

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//сброс до заводских нистроек 0xE0
int
Tx_commands::fullReset()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "сброс до заводских";
    quint8 comand = 0xE0;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//запись мин угла сервы во флеш, остальные углы рассчитываются 0xE1
int
Tx_commands::writeMinAngleServo(QString msg)
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "запись мин угла сервы " + msg;
    quint8 comand = 0xE1;

    //раскладывание строки по интовым числам
    StringToIntToByte(msg, &data);

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//запись установок для светодиодов 0xE2
int
Tx_commands::writeSettingLeds(QString msg)
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "запись уст. светодиодов № R G B " + msg;
    quint8 comand = 0xE2;

    //раскладывание строки по интовым числам
    StringToIntToByte(msg, &data);

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//установить серву в определенное положение 0xE3
int
Tx_commands::setServoAngle(QString msg)
{
    QByteArray data;
    int res = -1;
    QString s;
    quint8 comand = 0xE3;

    //раскладывание строки по интовым числам
    StringToIntToByte(msg, &data);

    s = "задать угол сервы № " + data[0];

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//установка флага включения/отключения блокировки сброса параметров тока       0xE4
int
Tx_commands::setCurBlock(QString msg)
{
    QByteArray data;
    int res = -1;
    QString s;
    quint8 comand = 0xE4;

    //раскладывание строки по интовым числам
    StringToIntToByte(msg, &data);

    if(data[0] == 0x00)    s = "флаг блокировки тока OFF";
    else                   s = "флаг блокировки тока ON";

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//запрос ошибок         0xE5
int
Tx_commands::getCheck()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "запрос ошибок";
    quint8 comand = 0xE5;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}


//запрос предустановок сервы        0xE6
int
Tx_commands::getSettingServo(QString msg)
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "запрос уст. сервы № " + msg;
    quint8 comand = 0xE6;

    //раскладывание строки по интовым числам
    StringToIntToByte(msg, &data);

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//выполнить авто калибровку серв     0xE7
int
Tx_commands::servoAutoCalibration()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "автокалибровка серв";
    quint8 comand = 0xE7;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//выполнить калибровку датчика тока     0xE8
int
Tx_commands::curAutoCalibration()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "автокалибр. датчика тока";
    quint8 comand = 0xE8;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//записать значение калибровки тока  0xE9
int
Tx_commands::setCurCalibration(QString msg)
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "записать калибровку тока " + msg;
    quint8 comand = 0xE6;

    //раскладывание строки по float числам
    StringToFloatToByte(msg, &data);

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//запрос значений калибровки тока  0xEA
int
Tx_commands::getCurCalibration()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "запрос калибровки тока";
    quint8 comand = 0xEA;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//сбросить ошибки  0xEB
int
Tx_commands::checkReset()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "стереть ошибки";
    quint8 comand = 0xEB;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//установить сервы в стартовое положение  0xEC
int
Tx_commands::setServosStartAngle()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "сервы в стартовое положение";
    quint8 comand = 0xEC;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//установить сервы в домашнее положение  0xED
int
Tx_commands::setServosHomeAngle()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "сервы в домашнее положение";
    quint8 comand = 0xED;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//запись домашних углов всех серв ноги во флеш, остальные углы рассчитываются     0xEE
int
Tx_commands::writeHomeAngleFoot(QString msg)
{
    QByteArray data;
    int res = -1;
    QString s;
    quint8 comand = 0xEE;

    //раскладывание строки по интовым числам
    StringToIntToByte(msg, &data);

    int num = static_cast<unsigned char>(data[0])+1;
    s = "сохранение углов ноги № " + QString::number(num);

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//сброс до заводских параметры серв ноги №     0xEF
int
Tx_commands::resetServsFoot(QString msg)
{
    QByteArray data;
    int res = -1;
    QString s;
    quint8 comand = 0xEF;

    //раскладывание строки по интовым числам
    StringToIntToByte(msg, &data);

    int num = static_cast<unsigned char>(data[0])+1;
    s = "Reset angl foot № " + QString::number(num);

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//перезагрузить контроллер     0xF0
int
Tx_commands::reboot()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "команда перезагрузки";
    quint8 comand = 0xF0;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//запрос параметров серв ноги №     0xF1
int
Tx_commands::getParamServsFoot(QString msg)
{
    QByteArray data;
    int res = -1;
    QString s;
    int num = msg.toInt() + 1;
    s = "обновление углов серв ноги № " + QString::number(num);
    quint8 comand = 0xF1;

    //раскладывание строки по int числам
    StringToIntToByte(msg, &data);

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//запрос версии  0xF2
int
Tx_commands::getVersion()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "запрос версии";
    quint8 comand = 0xF2;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}


//команда начала передачи основной программы, передача основной программы 0xF3
int
Tx_commands::writeProgram(QByteArray data)
{
    int res = -1;
    data.prepend(0xF3);


    return res;
}

//команда начала передачи загрузчика, передача загрузчика  0xF4
int
Tx_commands::writeBootloader(QByteArray data)
{
    int res = -1;
    data.prepend(0xF4);
    _packing->sendMessage(data, true);

    return res;
}

//подпись программы 0xF5
int
Tx_commands::writeLabel()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "подпись приложения";
    quint8 comand = 0xF5;
    //0xF3A58E1D
    f_value val;
    val.int32 = 0xF3A58E1D;

    //вписывание команды
    data.prepend(comand);

    //подпись
    data.append(val.data[3]);
    data.append(val.data[2]);
    data.append(val.data[1]);
    data.append(val.data[0]);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//выключить шар     0xF6
int
Tx_commands::shutdown()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "команда выключения";
    quint8 comand = 0xF6;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

//запрос ID устройства 0xF7
int
Tx_commands::getIntendifier()
{
    QByteArray data;
    int res = -1;
    QString s;
    s = "Запрос ID";
    quint8 comand = 0xF7;

    //вписывание команды
    data.prepend(comand);

    //отправка команды, вывод лога
    res = Sending(data, s);

    return res;
}

void
Tx_commands::StringToIntToByte(QString msg, QByteArray *data)
{
    QStringList sl = msg.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    for(QString s: sl) {
        int i = s.toInt();
        if ((i > 0xFF) && (i <= 0xFFFF))
        {
            data->append(i >> 8);
            i = i % 0x100;
        }
        data->append(i);   // 1 byte (unsigned)
    }
}

void
Tx_commands::StringToFloatToByte(QString msg, QByteArray *data)
{
    f_value val;
    val.f = msg.toFloat();
    if (val.f == val.f) {
        data->append(val.data[0]);
        data->append(val.data[1]);
        data->append(val.data[2]);
        data->append(val.data[3]);
    }
}

int
Tx_commands::Sending(QByteArray data, QString s)
{
    int res = -1;
    QByteArray dataToSend;

    //обертывание в протокол
    dataToSend = _packing->wrapData(data);

    //отправка сообщения
    res = _packing->sendMessage(dataToSend, false);

    if(res == -1)
        return res;

    //вывод лога
    if(_settings->getLoging())
    {
        _commun_display->log_out_J("-> ", QString ("%1 (%2 size)").arg(QString(dataToSend.toHex())).arg(res));
        _commun_display->log_out_S("-> ", QString ("%1 (%2 size)").arg(QString(dataToSend.toHex())).arg(res));
    }

    //вывод расшифровки лога
    if(!s.isEmpty())
    {
        res = _commun_display->log_out_S("-> ", s);
        res = _commun_display->log_out_J("-> ", s);
    }
    return res;
}


