#ifndef RX_COMMANDSPYLT_H
#define RX_COMMANDSPYLT_H

#include <QObject>
#include "../../main/settings.h"
#include "../../communication/crc.h"
#include "../../Shar/display_working/commun_display.h"


class Rx_commands_Pylt: public QObject
{
    Q_OBJECT
public:
    explicit Rx_commands_Pylt(QObject *parent = 0);
    ~Rx_commands_Pylt();

    void setSettings(Settings *newSettings);
    void setCommun_display(Commun_display *newCommun_display);

    int searchCommand(QByteArray dat);

private:
    Settings *_settings = nullptr;
    Commun_display * _commun_display = nullptr;

    void replyJoysticActivity(QByteArray Data);     // отвечает статус байтом и реальным напряжением 0x0A
    void getIntendifier(QByteArray Num);            // ответ запроса ID устройства 0xF7
    void batteryTypeRequest(QByteArray Data);       // запрос типа аккамулятора 0xA1

};

#endif // RX_COMMANDSPYLT_H
