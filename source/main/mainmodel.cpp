
#include "source/main/mainmodel.h"
#include "source/communication/device/device.h"

#include <QDebug>

MainModel::MainModel(QObject *parent) : QObject{parent}
{  
}

MainModel::~MainModel()
{
}

void
MainModel::checkingParameters(const std::function<void()> &done)
{
    //запрос точки восстановления
    //если нет сохраненной точки - запросить
    if(_settings->full_param_check())
    {
        done();
        return;
    }

    //на всякий. вдруг прибор еще не включен, жду (асинхронно)
    QTimer::singleShot(500, this, [this, done]() {
        startRetry(
            [this]() { _tx_commands->readAllParams(); },
            [this]() { return _settings->full_param_check() != 0; },
            done,
            done,
            6, 120);
    });
}

// запрос ID устройства
void
MainModel::checkID(const std::function<void(int)> &done)
{
    startRetry(
        [this]() { _tx_commands->getIntendifier(); },
        [this]() { return _settings->getIdDevice() != _settings->NONE; },
        [this, done]() { done(_settings->getIdDevice()); },
        [this, done]() { done(_settings->NONE); },
        7, 90);
}

void
MainModel::checkUpdate()
{
    //проверка обновлений
    _updateHex->checkUpdateHex([this](int res) {
        if(res == 1)
        {
            //открыть всплывающее окно с предложением обновиться
            _commun_display->windloadHexOpen();
        }
    });
}

void
MainModel::deviceConnect(QString type, QString name)
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

    //запрос ID выполняется один раз (раньше checkID вызывался дважды с задержками)
    checkID([this, name](int id) {
        // если устройство шар
        if(id == _settings->SHAR)
        {
            // запрос точки восстановления, затем запрос версии прошивки
            checkingParameters([this]() { checkUpdate(); });
        }
        // если устройство пульт
        else if(id == _settings->PYLT)
        {
            // запрос типа аккамулятора; после ответа - пересчет параметров
            startRetry(
                [this]() { _tx_commandsPylt->batteryTypeRequest(); },
                [this]() { return _commun_display->getVolt() != 0.0; },
                [this, name]() {
                    _pylt_settings->setDevName(name);
                    _tx_commandsPylt->recalculatingParameters();
                },
                [this, name]() {
                    _pylt_settings->setDevName(name);
                    _tx_commandsPylt->recalculatingParameters();
                },
                7, 90);
        }
    });
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
                                 [this] (QString typeDevice, QString name) {
                                //     qDebug() << "test";
                                     deviceConnect(typeDevice, name);
    });
}

//асинхронное ожидание ответа: send() вызывается сразу и при каждом тике,
//пока ready() не станет true; по исчерпании попыток вызывается onFail
void
MainModel::startRetry(const std::function<void()> &send,
                      const std::function<bool()> &ready,
                      const std::function<void()> &onDone,
                      const std::function<void()> &onFail,
                      int maxAttempts, int intervalMs)
{
    if(!_retryTimer)
    {
        _retryTimer = new QTimer(this);
        _retryTimer->setInterval(intervalMs);
        connect(_retryTimer, &QTimer::timeout, this, &MainModel::retryTick);
    }
    _retryTimer->setInterval(intervalMs);

    _retrySend = send;
    _retryCheck = ready;
    _retryDone = onDone;
    _retryFail = onFail;
    _retryMaxAttempts = maxAttempts;
    _retryCount = 0;

    send();
    _retryTimer->start();
}

void
MainModel::retryTick()
{
    if(_retryCheck())
    {
        _retryTimer->stop();
        auto done = _retryDone;
        _retrySend = nullptr; _retryCheck = nullptr;
        _retryDone = nullptr; _retryFail = nullptr;
        if(done)    done();
        return;
    }

    if(++_retryCount >= _retryMaxAttempts)
    {
        _retryTimer->stop();
        auto fail = _retryFail;
        _retrySend = nullptr; _retryCheck = nullptr;
        _retryDone = nullptr; _retryFail = nullptr;
        if(fail)    fail();
        return;
    }

    _retrySend();
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
                                 [this] (QString typeDevice, QString name) {
                                //     qDebug() << "test";
                                     deviceConnect(typeDevice, name);
    });
}

void
MainModel::setPacking(Packing *newPacking)
{
    _packing = newPacking;
}

void
MainModel::setPylt_settings(Pylt_settings *newPylt_settings)
{
    _pylt_settings = newPylt_settings;
}

