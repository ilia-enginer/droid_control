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

void
Rx_commands_Pylt::setCommun_display(Commun_display *newCommun_display)
{
    _commun_display = newCommun_display;
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
    case 0xA1:
        batteryTypeRequest(Data);
        break;
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

// ответ на запрос типа аккамулятора 0xA1
void
Rx_commands_Pylt::batteryTypeRequest(QByteArray Data)
{
    f_value val;
    // сначала реальное напряжение во флоат
    val.data[3] = Data[0];
    val.data[2] = Data[1];
    val.data[1] = Data[2];
    val.data[0] = Data[3];
    float Vreal = val.f;
    if(Vreal == Vreal)   _commun_display->vrealChang(Vreal);

    // затем номинальное
    val.data[3] = Data[4];
    val.data[2] = Data[5];
    val.data[1] = Data[6];
    val.data[0] = Data[7];
    float Vnum = val.f;
    if(Vnum == Vnum)
    {
        if(Vnum == 7.4) // 2s
        {
            _settings->setVmin(6.0);
            _settings->setVmax(8.4);
        }
        else if(Vnum == 11.1)   // 3s
        {
            _settings->setVmin(9.0);
            _settings->setVmax(12.6);
        }
        else if(Vnum == 14.8)   // 4s
        {
            _settings->setVmin(12.0);
            _settings->setVmax(16.8);
        }
        else if(Vnum == 18.5)   // 5s
        {
            _settings->setVmin(15.0);
            _settings->setVmax(21.0);
        }
        else if(Vnum == 22.2)   // 6s
        {
            _settings->setVmin(18.0);
            _settings->setVmax(25.2);
        }
    }
}
//???? вписать команду получения реального напряжения + флаги статуса
// при флаге "низкое напряжение - менять градиет ползунка напряжения на красный, при отсутствии этого флага менять обратно"
