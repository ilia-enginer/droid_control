
#include "source/main/mainmodel.h"
#include "source/device/device.h"


MainModel::MainModel()
{  
}

int
MainModel::checkingParameters()
{
    //запрос точки восстановления
    //если нет сохраненной точки - запросить
    if(!_settings->full_param_check())
    {
        //на всякий. вдруг прибор еще не включен, жду
        delay(500);
       //запрашиваю параметры
        _tx_commands->readAllParams();
        delay(150);
        for(qint8 i = 0; i < 6; i++)
        {
            if(!_settings->full_param_check()) _tx_commands->readAllParams();
            else                                return 1;
            delay(120);
        }
    }
    return 0;
}

int
MainModel::checkID()
{
    // запрос ID устройства
    _tx_commands->getIntendifier();
    delay(150);
    _tx_commands->getIntendifier();
    return 1;
}

int
MainModel::checkUpdate()
{
    //проверка обновлений
    if(_updateHex->checkUpdateHex() == 1)
    {
        //открыть всплывающее окно с предложением обновиться
        _commun_display->windloadHexOpen();
    }
    return 0;
}

void
MainModel::setAdminFlag(bool value)
{
    adminFlag = value;
    _rx_commands->f_AdminChange(value);
    _updateHex->f_AdminChange(value);
    emit onAdminFlagChanged();
}

bool
MainModel::getAdminFlag()
{
    return adminFlag;
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

