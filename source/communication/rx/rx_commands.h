#ifndef RX_COMMANDS_H
#define RX_COMMANDS_H

#include <QObject>
#include <QWidget>
#include "../../display_working/commun_display.h"
#include "../../update/updatehex.h"
#include "../../main/settings.h"


class Rx_commands: public QObject
{
    Q_OBJECT

public:
    explicit Rx_commands(QObject *parent = 0);
    ~Rx_commands();

    void setCommun_display(Commun_display *newCommun_display);
    void setUpdateHex(UpdateHex *newUpdateHex);
    void setSettings(Settings *newSettings);

    int searchCommand(QByteArray dat);

    void f_AdminChange(bool f);

public slots:
    void setVReal(bool V);      //флаг запроса - не отображать сообщений после прихода напряжения
    void setCurReal(bool Cur);  //флаг запроса - не отображать сообщений после прихода тока

private:
    Commun_display * _commun_display = nullptr;
    UpdateHex *_updatehex = nullptr;
    Settings *_settings = nullptr;


    QByteArray Data;

    void errorCrc();                        //ошибка срс                            0x01
    void incorrectLoaderCommand();          //не правильная команда загрузчика      0x04
    void incorrectCommand();                //не правильная команда                 0x05
    void incorrectCommandParameters();      //не правильные параметры команды       0x06
    void incorrectPacketOrder();            //не правильная очередность пакетов     0x07
    void errorFirmwareDownload();           //ошибка загрузки прошивки              0x08
    void errorGyroscope();                  //ошибка гироскопа                      0x10
    void errorErasingFlash();               //ошибка стирания флеш                  0x20
    void errorWritingFlash();               //ошибка записи во флеш                 0x21
    void errorLowVoltage();                 // ошибка низкое напряжение             0x30
    void errorHightVoltage();               // ошибка высокое напряжение            0x31
    void errorExcessCurrent();              //превышение допустимого тока           0x35
    void errorCurrentSensorFailure();       //отказ датчика тока                    0x38
    void errorCurrentSensorCalibration();   //ошибка калибровки датчика тока        0x39
    void checkingServiceabilityServos();    //проверка исправности сервоприводов    0x40
    void errorServsInitialization();        //ошибка инициализации серв             0x41
    void errorServoFailure(QByteArray Num);         //отказ сервопривода (номер)            0x42
    void errorServoDriverFailure(QByteArray Num);  //отказ драйвера сервопривода (номер)  0x43
    void errorAutoCalibrations();           //ошибка автокалибровки                 0x45
    void autocalibrationCompleted();        //автокалибровка выполнена              0x48
    void idleTimeExceeded();                //превышено время бездействия           0x50
    void errorMath();                      //ошибка математики                     0x55
    void leds_read(QByteArray leds);       //чтение состояния светодиодов     0xA0
    void voltage_read(QByteArray Volt);       //чтение напряжения                 0xA1
    void current_read(QByteArray Cur);      //чтение тока                       0xA2
    void readGyroscopeAngles(QByteArray Gyr);//чтение углов гироскопа            0xA3
    void readAcceleration(QByteArray Accel); //чтение ускорения                  0xA4
    void readAngleServo(QByteArray Angle); //чтение угла сервы                 0xA5
    void on_offServo(QByteArray flag);     //вкл/выкл сервы                    0xA6
    void readAllParams(QByteArray Params); //запросить все параметры(точка восстановления) 0xA7
    void writeAllParams();                //отправить все параметры(точка восстановления)    0xA8
    void getParamsChart(QByteArray Params); //получение параметров для графика 0xA9
    void fullReset();                    //сброс до заводских нистроек                       0xE0
    void writeMinAngleServo();          //запись мин угла сервы во флеш, остальные углы рассчитываются     0xE1
    void writeSettingLeds();             //запись установок для светодиодов      0xE2
    void setServoAngle();               //установить серву в угол               0xE3
    void setCurBlock(QByteArray flag);  //установка флага включения/отключения блокировки сброса параметров тока       0xE4
    void getCheck();                     //запрос ошибок                    0xE5
    void getSettingServo(QByteArray Params);//запрос предустановок сервы        0xE6
    void servoAutoCalibration();        //выполнить авто калибровку серв     0xE7
    void curAutoCalibration(QByteArray Cur);//выполнить калибровку датчика тока     0xE8
    void setCurCalibration();           //записать значение калибровки тока  0xE9
    void getCurCalibration(QByteArray Cur); //запрос значений калибровки тока  0xEA
    void checkReset();                   //сбросить ошибки  0xEB
    void setServosStartAngle();          //установить сервы в стартовое положение  0xEC
    void setServosHomeAngle();          //установить сервы в домашнее положение  0xED
    void writeHomeAngleFoot();          //запись домашних углов всех серв ноги во флеш, остальные углы рассчитываются     0xEE
    void resetServsFoot();              //сброс до заводских параметры серв ноги №     0xEF
    void reboot();                       //перезагрузить контроллер     0xF0
    void getParamServsFoot(QByteArray Angle); //запрос параметров серв ноги №     0xF1
    void getVersion(QByteArray Ver);        //запрос версии  0xF2
    void writeProgram(QByteArray Num);    //команда начала передачи основной программы, передача основной программы  0xF3
    void writeBootloader(QByteArray Num); //команда начала передачи загрузчика, передача загрузчика  0xF4
    void writeLabel();                   //подпись программы 0xF5
    void shutdown();                     //выключить шар     0xF6
    void getIntendifier(QByteArray Num); //ответ запроса ID устройства 0xF7


    bool _f_Admin = false;               ///флаг режима админа
    bool _setVreal = false;              ///флаг запроса напряжения
    bool _setCurReal = false;            ///флаг запроса тока
};

#endif // RX_COMMANDS_H


