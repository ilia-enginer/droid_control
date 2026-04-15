
#include "source/main/mainmodel.h"
#include "source/communication/device/device.h"

#include <QDebug>

MainModel::MainModel(QObject *parent) : QObject{parent}
{  
}

MainModel::~MainModel()
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

// запрос ID устройства
int
MainModel::checkID()
{ 
    //если не админский режим - сначала сбросить приложение
    if(!adminFlag)
    {
        _settings->setIdDevice(0);

         _tx_commands->getIntendifier();
         delay(150);
         for(qint8 i = 0; i < 7; i++)
         {
             if(_settings->getIdDevice() == _settings->NONE) _tx_commands->getIntendifier();
             else                                            return 1;
             delay(90);
         }
         return 0;
    }
    else{
        _tx_commands->getIntendifier();
        delay(150);
        _tx_commands->getIntendifier();
        return 1;
    }
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
MainModel::deviceConnect(QString type)
{
    qDebug() << "connect to device = " <<type;

    if(type.isEmpty())  return;
    if(type == "none")
    {
        _packing->setTypeTx(type);
        _commun_display->set_connected(false);
        return;
    }

    _packing->setTypeTx(type);
    _commun_display->set_connected(true);

    if(checkID())
    {
        checkingParameters();
        checkUpdate();
    }
}

void
MainModel::setAdminFlag(bool value)
{
    adminFlag = value;
    _rx_commands->f_AdminChange(value);
    _updateHex->f_AdminChange(value);
    _unpacking->f_AdminChange(value);

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
    connect(device_, &Device::connected, this,
                                 [this] (QString typeDevice) {
                                //     qDebug() << "test";
                                     deviceConnect(typeDevice);
    });
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

void
MainModel::setMainSerialComPort(MainSerialPort *newMainSerialPort)
{
    _mainserialport = newMainSerialPort;
    connect(_mainserialport, &MainSerialPort::connected, this,
                                 [this] (QString typeDevice) {
                                //     qDebug() << "test";
                                     deviceConnect(typeDevice);
    });
}

void
MainModel::setPacking(Packing *newPacking)
{
    _packing = newPacking;
}

void
MainModel::setUnpacking(Unpacking *newUnpacking)
{
    _unpacking = newUnpacking;
}
