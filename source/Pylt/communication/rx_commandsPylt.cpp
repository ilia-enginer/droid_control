#include "rx_commandsPylt.h"

Rx_commands_Pylt::Rx_commands_Pylt(QObject *parent) :
    QObject(parent)
{

}

Rx_commands_Pylt::~Rx_commands_Pylt()
{

}

void
Rx_commands_Pylt::setSettings(Settings *newSettings)
{
    _settings = newSettings;
}

int
Rx_commands_Pylt::searchCommand(QByteArray dat)
{
    const std::string empty;
    quint8 k = dat[0];
    QByteArray Data = dat;
    Data.replace(0, 1, empty);
    int res = 1;

    switch (k) {
    case 0xF7:
        getIntendifier(Data);
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
Rx_commands_Pylt::getIntendifier(QByteArray Num)
{
    f_value val;

    val.data[3] = Num[0];
    val.data[2] = Num[1];
    val.data[1] = Num[2];
    val.data[0] = Num[3];
    int id = val.int32;

    _settings->setIdDevice(id, false);
}
