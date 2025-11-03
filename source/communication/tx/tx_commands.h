#ifndef TX_COMMANDS_H
#define TX_COMMANDS_H

#include <QObject>
#include <QWidget>
#include "packing.h"
#include "../../display_working/commun_display.h"
#include "../../main/settings.h"


class Tx_commands: public QObject
{
    Q_OBJECT

public:
    explicit Tx_commands(QObject *parent = 0);
    ~Tx_commands();

    void setPacking(Packing *newPacking);
    void setCommun_display(Commun_display *newCommun_display);
    void setSettings(Settings *newSettings);


public slots:
    int joysticActivity(quint8 mode, float azimut, float amplitude, float level, bool ctrl);    //активность джойстика
    int leds_read(QString msg);          //чтение состояния светодиодов     0xA0
    int voltage_read(bool flagDisplay);  //чтение напряжения                 0xA1
    int current_read(bool flagDisplay); //чтение тока                       0xA2
    int readGyroscopeAngles();          //чтение углов гироскопа            0xA3
    int readAcceleration();             //чтение ускорения                  0xA4
    int readAngleServo(QString msg);    //чтение угла сервы                 0xA5
    int on_offServo(QString msg);       //вкл/выкл сервы                    0xA6
    int readAllParams();                //запросить все параметры(точка восстановления) 0xA7
    int writeAllParams();                //отправить все параметры(точка восстановления)    0xA8
    int fullReset();                    //сброс до заводских нистроек                       0xE0
    int writeMinAngleServo(QString msg);//запись мин угла сервы во флеш, остальные углы рассчитываются     0xE1
    int writeSettingLeds(QString msg);  //запись установок для светодиодов      0xE2
    int setServoAngle(QString msg);     //установить серву в угол               0xE3
    int setCurBlock(QString msg);        //установка флага включения/отключения блокировки сброса параметров тока       0xE4
    int getCheck();                     //запрос ошибок                    0xE5
    int getSettingServo(QString msg);   //запрос предустановок сервы        0xE6
    int servoAutoCalibration();        //выполнить авто калибровку серв     0xE7
    int curAutoCalibration();          //выполнить калибровку датчика тока     0xE8
    int setCurCalibration(QString msg); //записать значение калибровки тока  0xE9
    int getCurCalibration();            //запрос значений калибровки тока  0xEA
    int checkReset();                   //сбросить ошибки  0xEB
    int setServosStartAngle();          //установить сервы в стартовое положение  0xEC
    int setServosHomeAngle();          //установить сервы в домашнее положение  0xED
    int writeHomeAngleFoot(QString msg);//запись домашних углов всех серв ноги во флеш, остальные углы рассчитываются     0xEE
    int resetServsFoot(QString msg);    //сброс до заводских параметры серв ноги №     0xEF
    int reboot();                       //перезагрузить контроллер     0xF0
    int getParamServsFoot(QString msg); //запрос параметров серв ноги №     0xF1
    int getVersion();                   //запрос версии  0xF2
    int writeProgram(QByteArray data); //команда начала передачи основной программы, передача основной программы  0xF3
    int writeBootloader(QByteArray data); //команда начала передачи загрузчика, передача загрузчика  0xF4
    int writeLabel();                   //подпись программы 0xF5
    int shutdown();                     //выключить шар     0xF6  
    int getIntendifier();               //запрос ID устройства 0xF7


private:
    void StringToIntToByte(QString msg, QByteArray *data);
    void StringToFloatToByte(QString msg, QByteArray *data);
    int Sending(QByteArray, QString);


    Packing * _packing = nullptr;
    Commun_display * _commun_display = nullptr;
    Settings * _settings = nullptr;

};

#endif // TX_COMMANDS_H
