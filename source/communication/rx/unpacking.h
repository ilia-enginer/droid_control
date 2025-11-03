#ifndef UNPACKING_H
#define UNPACKING_H

#include <QObject>
#include <QWidget>
#include "../crc.h"
#include "../../display_working/commun_display.h"
#include "rx_commands.h"
#include "../../main/settings.h"


class Unpacking: public QObject
{
    Q_OBJECT

public:

    explicit Unpacking(QObject *parent = 0);
    ~Unpacking();
    void setCrc(Crc *newCrc);
    void setCommun_display(Commun_display *newCommun_display);
    void setRx_commands(Rx_commands *newRx_commands);
    void setSettings(Settings *newSettings);

    int unpack(QByteArray data, int size);

private:
    Crc * _crc = nullptr;
    Commun_display * _commDisplay = nullptr;
    Rx_commands *_rx_commands = nullptr;
    Settings *_settings = nullptr;


    int gluingPackages();
    void auroraDelete();

    quint8 max_rx_size = 254;
    QByteArray dataRes;
    QByteArray Temp = 0;
    quint8 startByteIndex = 0;
    bool statys = false;
    bool split = false;

};

#endif // UNPACKING_H



