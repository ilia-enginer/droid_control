
#include "source/main/mainmodel.h"
#include "source/device/device.h"


MainModel::MainModel()
{  
}

int
MainModel::getservisIndexMenu() const
{
    return servisIndexMenu;
}

void
MainModel::setservisIndexMenu(int newservisIndexMenu)
{
    servisIndexMenu = newservisIndexMenu;
}

int
MainModel::checkingParameters()
{
    //запрос точки восстановления
    //если нет сохраненной точки - запросить
    if(!_settings->full_param_check())
    {
        //на всякий. вдруг шар еще не включен, жду
        delay(500);
       //запрашиваю параметры с шара если их нет
        if(!_settings->full_param_check())
        {
            _tx_commands->readAllParams();
            delay(150);
            for(qint8 i = 0; i < 6; i++)
            {
                if(!_settings->full_param_check()) _tx_commands->readAllParams();
                else                        i = 6;
                delay(90);
            }
         }
    }

    //проверка обновлений
    if(_updateHex->checkUpdateHex() == 1)
    {
        //открыть всплывающее окно с предложением обновиться
        _commun_display->windloadHexOpen();
    }

    return 0;
}

void
MainModel::incAdminTapCount(int value)
{
    Q_UNUSED(value)

    if(adminTapCount > 4)
    {
        adminTapCount = -1;
        _updateHex->f_AdminChange(true);
        _rx_commands->f_AdminChange(true);
    }
    else if(adminTapCount >= 0)
    {
     adminTapCount ++;
    }
    emit onAdminTapCountChanged();
}

int
MainModel::getAdminTapCount()
{
    if(adminTapCount == -1)
        return 1;
    return 0;
}

void
MainModel::setDevice(Device *device)
{
    device_ = device;
}

void
MainModel::setSettings(Settings *newSettings)
{
    _settings = newSettings;
}

void
MainModel::setTx_commands(Tx_commands *newTx_commands)
{
    _tx_commands = newTx_commands;
}

void
MainModel::setRx_commands(Rx_commands *newRx_commands)
{
    _rx_commands = newRx_commands;
}

void
MainModel::setUpdateHex(UpdateHex *newUpdateHex)
{
    _updateHex = newUpdateHex;
}

void
MainModel::setCommun_display(Commun_display *newCommun_display)
{
    _commun_display = newCommun_display;
}

