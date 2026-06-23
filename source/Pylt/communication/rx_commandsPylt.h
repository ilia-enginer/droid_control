#ifndef RX_COMMANDSPYLT_H
#define RX_COMMANDSPYLT_H

#include <QObject>
#include "../../main/settings.h"
#include "../../communication/crc.h"


class Rx_commands_Pylt: public QObject
{
    Q_OBJECT
public:
    explicit Rx_commands_Pylt(QObject *parent = 0);
    ~Rx_commands_Pylt();

    void setSettings(Settings *newSettings);

    int searchCommand(QByteArray dat);

private:
    Settings *_settings = nullptr;

    void getIntendifier(QByteArray Num); //ответ запроса ID устройства 0xF7

};

#endif // RX_COMMANDSPYLT_H
