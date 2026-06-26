
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
     _tx_commands->getIntendifier();
     delay(150);
     for(qint8 i = 0; i < 7; i++)
     {
         if(_settings->getIdDevice() == _settings->NONE) _tx_commands->getIntendifier();
         else                                            return _settings->getIdDevice();
         delay(90);
     }
     return 0;
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
    qDebug() << "connect to device = " << type;

    if(type.isEmpty())  return;
    if(type == "none")
    {
        _packing->setTypeTx(type);
        _commun_display->set_connected(false);
        return;
    }

    // установка типа передатчика
    _packing->setTypeTx(type);
    _commun_display->set_connected(true);

    // если устройство шар
    if(checkID() == _settings->SHAR)
    {
        // запрос точки восстановления
        checkingParameters();
        // запрос версии прошивки
        checkUpdate();
    }
    // если устройство пульт
    else if(checkID() == _settings->PYLT)
    {
        // запрос типа аккамулятора
        _tx_commandsPylt->batteryTypeRequest();
        delay(150);
        for(qint8 i = 0; i < 7; i++)
        {
            if(_commun_display->getVolt() == 0.0) _tx_commandsPylt->batteryTypeRequest();
            else                                  return;
            delay(90);
        }
    }
}

void
MainModel::setAdminFlag(bool value)
{
    adminFlag = value;
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
MainModel::setTx_commandsPylt(Tx_commandsPylt *newTx_commandsPylt)
{
    _tx_commandsPylt = newTx_commandsPylt;
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

