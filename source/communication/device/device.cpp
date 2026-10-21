/***************************************************************************
**
** Copyright (C) 2013 BlackBerry Limited. All rights reserved.
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothservicediscoveryagent.h>

#include <QDebug>
#include <QList>
#include <QByteArray>
#include <QMetaEnum>
#include <QTimer>
#include <QRegularExpression>
#include <QGeoCoordinate>
#include <QSettings>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QBluetoothUuid>

#if defined(Q_OS_ANDROID)
#include <QJniObject>
#include <QCoreApplication>
#endif

#include "source/communication/device/device.h"
#include "source/main/mainmodel.h"

void
Device::startDeviceDiscovery()
{
    //Bluetooth включается асинхронно, чтобы не блокировать главный поток (ANR)
    blt_on([this](bool ok) {
        if (ok)     startDeviceDiscoveryImpl();
    });
}

Device::Device(QObject *parent) : QObject(parent)
{
    //! [les-devicediscovery-1]
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    discoveryAgent->setLowEnergyDiscoveryTimeout(25000);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &Device::addDevice);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred, this,
            &Device::deviceScanError);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &Device::deviceScanFinished);
    //! [les-devicediscovery-1]
}

Device::~Device()
{
    if(_btPollTimer)    _btPollTimer->stop();
    delete discoveryAgent;
    delete controller;
    qDeleteAll(devices);
    qDeleteAll(m_services);
    qDeleteAll(m_characteristics);
    devices.clear();
    m_services.clear();
    m_characteristics.clear();
}

void
Device::setCommun_display(Commun_display *newCommun_display)
{
    _commun_display = newCommun_display;
}

void
Device::startDeviceDiscoveryImpl()
{
    clearDeviceDiscovery();
    emit devicesUpdated();
    _commun_display->setUpdatee("Сканирование устройств ...");
    //! [les-devicediscovery-2]
//    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::ClassicMethod);

    //! [les-devicediscovery-2]

    if (discoveryAgent->isActive()) {
        m_deviceScanState = true;
        emit stateChanged();
        _commun_display->statusDevicee(_commun_display->statusDevic::searchInProgr);
    }
}

void
Device::stopDeviceDiscovery()
{
    discoveryAgent->stop();
   emit devicesUpdated();
   m_deviceScanState = false;
   emit stateChanged();
   _commun_display->statusDevicee(_commun_display->statusDevic::searchFinish);
}

void
Device::clearDeviceDiscovery()
{
    qDeleteAll(devices);
    devices.clear();
}

//! [les-devicediscovery-3]
void
Device::addDevice(const QBluetoothDeviceInfo &info)
{
    // не добавлять устройство без имени
    if(info.name().isEmpty())
        return;

    // проверка на дубликат
    for(QObject *obj :devices)
    {
        auto *dev = qobject_cast<DeviceInfo*>(obj);
        if(dev && dev->getAddress() == info.address().toString())
            return;
    }

    devices.append(new DeviceInfo(QBluetoothDeviceInfo(info)));
    emit devicesUpdated();
}
//! [les-devicediscovery-3]
void
Device::deviceScanFinished()
{
//    const QList<QBluetoothDeviceInfo> foundDevices = discoveryAgent->discoveredDevices();

//    for (auto &nextDevice : foundDevices) {
//        if (nextDevice.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
//            devices.append(new DeviceInfo(nextDevice));
//        }
//    }

    emit devicesUpdated();
    m_deviceScanState = false;
    emit stateChanged();
    if (devices.isEmpty()) {
         _commun_display->setUpdatee("Устройства не найдены");
    } else {
         _commun_display->setUpdatee("Сканирование завершено");
    }
    _commun_display->statusDevicee(_commun_display->statusDevic::searchFinish);
}


QVariant
Device::getDevices()
{
    return QVariant::fromValue(devices);
}


QVariant
Device::getServices()
{
    return QVariant::fromValue(m_services);
}

QVariant
Device::getCharacteristics()
{
    return QVariant::fromValue(m_characteristics);
}


void
Device::connectToDevice(const QString &dAddress, const QString &name, const QString &config)
{
    //если уже подключен к этому устройству - отключить
    if((nameDevice_ == name) && (connected_flag))
    {
        socket->disconnectFromService();
        return;
    }

    //Bluetooth включается асинхронно, чтобы не блокировать главный поток (ANR)
    blt_on([this, dAddress, name, config](bool ok) {
        if (ok)     connectToDeviceImpl(dAddress, name, config);
    });
}

void
Device::connectToDeviceImpl(const QString &dAddress, const QString &name, const QString &config)
{
    discoveryAgent->stop();
    deviceScanFinished();

    lastConnectedDevice_ = dAddress;
    nameDevice_ = name;
    class_ = config;

    _commun_display->setUpdatee("Подключение к " + nameDevice_);
    _commun_display->statusDevicee(_commun_display->statusDevic::searchInProgr);

    socket = new QBluetoothSocket(QBluetoothServiceInfo::Protocol::RfcommProtocol, this);
    socket->connectToService(QBluetoothAddress(dAddress), QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));

    connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(socketError(QBluetoothSocket::SocketError)));
    connect(socket, SIGNAL(connected()), this, SLOT(deviceConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectFromDevice()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketRead()));
}

/// вычитывание устройств из памяти
int
Device::reading_devices_from_memory(QVector<QStringList> &lastDevices, int &num)
{
    QSettings setting;
    //вычитываю все устройства
    while (true)
    {
        if (setting.contains(QString("last%1").arg(num))) {
            lastDevices.append(setting.value(QString("last%1").arg(num)).toString().split(";"));
            ++num;    // следующий номер прибора для сохранения в истории
        } else break;
    }

    return 0;
}

/// поиск устройства в списке
bool
Device::searching_device_in_memory(QVector<QStringList> lastDevices, QString id, int &num)
{
    QSettings setting;

    for(QStringList device : lastDevices) {
        if(device.at(2) == id)
        {
            return true;
        }
        num++;
    }
    return false;
}

void
Device::deviceConnected()
{
    connected_flag = true;
    //! [les-service-2]
    //    controller->discoverServices();
    //! [les-service-2]
    _commun_display->setUpdatee("Подключено к " + nameDevice_);
    _commun_display->statusDevicee(_commun_display->statusDevic::searchFinish);
    setCurrentDeviceName(nameDevice_);

    //сохранение последнего подключенного устройства
    set_last_device();
    emit connected("bluetooth", nameDevice_);
}

void
Device::errorReceived(QLowEnergyController::Error)
{
     qWarning() << "Error: " << controller->errorString();
     _commun_display->setUpdatee(QString("Back\n(%1)").arg(controller->errorString()));
}

void
Device::setUpdate(const QString &message)
{  
    _commun_display->setUpdatee(message);
}


QString
Device::getLastConnectedDevice()
{
    return  lastConnectedDevice_;
}


void
Device::disconnectFromDevice()
{
    _commun_display->setUpdatee("Отключено");
    setCurrentDeviceName("Отключено");
    // UI always expects disconnect() signal when calling this signal
    // TODO what is really needed is to extend state() to a multi value
    // and thus allowing UI to keep track of controller progress in addition to
    // device scan progress

    connected_flag = false;
    emit connected("none", "");
}

//отправить сообщение
int
Device::sendMessage(QByteArray data)
{
    if (!socket)   return -1;
    return socket->write(data);
}

void
Device::deviceDisconnected()
{
    qWarning() << "Устройство отключено";
    _commun_display->statusDevicee(_commun_display->statusDevic::disconnect);
    connected_flag = false;
    emit connected("none", "");
}

void
Device::socketRead()
{
   QByteArray recievedData = socket->readAll();
   emit socketDataRecieved(recievedData.toHex());
   emit messageReceived(recievedData);
}

// открыть настройки геолокации для включения
void
Device::openLocationSettings()
{
  #if defined(Q_OS_ANDROID)
    // создаю интент с действием для открытия настроек
    QJniObject action = QJniObject::fromString("android.settings.LOCATION_SOURCE_SETTINGS");
    QJniObject intent ("android/content/Intent", "(Ljava/lang/String;)V", action.object<jstring>());

    if(intent.isValid())
    {
        // получаю текущую активити
        QJniObject activiti = QJniObject::callStaticObjectMethod("org/qtproject/qt/android/QtNative", "activity", "()Landroid/app/Activity;");
        if(activiti.isValid())
        {
            // запускаю активити с этим интент
            activiti.callMethod<void>("startActivity", "(Landroid/content/Intent;)V", intent.object<jobject>());
        }
    }
  #endif
}

void
Device::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
    {
        _commun_display->setUpdatee("Включите Bluetooth");
        _commun_display->statusDevicee(_commun_display->statusDevic::searchFinish);
    }

    else if (error == QBluetoothDeviceDiscoveryAgent::UnsupportedPlatformError)
    {
        _commun_display->setUpdatee("Ошибка. Bluetooth не поддерживается платформой.");
        _commun_display->statusDevicee(_commun_display->statusDevic::searchFinish);
    }
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
    {
        _commun_display->setUpdatee("Запись или считывание данных\n с устройства привело к ошибке.");
        _commun_display->statusDevicee(_commun_display->statusDevic::searchFinish);
    }
    else if (error == QBluetoothDeviceDiscoveryAgent::LocationServiceTurnedOffError)
    {
        _commun_display->setUpdatee("Включите геолокацию");
        _commun_display->statusDevicee(_commun_display->statusDevic::searchFinish);
        openLocationSettings();
    }
    else {
        static QMetaEnum qme = discoveryAgent->metaObject()->enumerator(
                    discoveryAgent->metaObject()->indexOfEnumerator("Error"));
        _commun_display->setUpdatee("Error: " + QLatin1String(qme.valueToKey(error)));
       _commun_display->statusDevicee(_commun_display->statusDevic::searchFinish);
    }

    m_deviceScanState = false;
    emit devicesUpdated();
    emit stateChanged();
}

bool
Device::state()
{
    return m_deviceScanState;
}

bool
Device::hasControllerError() const
{
    return (controller && controller->error() != QLowEnergyController::NoError);
}

bool
Device::isRandomAddress() const
{
    return randomAddress;
}

void
Device::setRandomAddress(bool newValue)
{
    randomAddress = newValue;
    emit randomAddressChanged();
}


//асинхронное включение Bluetooth: done(ok) вызывается, когда BT включен,
//либо включить не удалось. Главный поток не блокируется.
void
Device::blt_on(const std::function<void(bool)> &done)
{
    QBluetoothLocalDevice devicee;

    if(!devicee.isValid())
    {
        _commun_display->setUpdatee("Bluetooth не поддерживается платформой.");
        done(false);
        return;
    }

    if(devicee.hostMode() != QBluetoothLocalDevice::HostPoweredOff)
    {
        done(true);
        return;
    }

    devicee.powerOn();// Call to open the local Bluetooth device
    _commun_display->setUpdatee("Включение Bluetooth");
    _commun_display->statusDevicee(_commun_display->statusDevic::searchInProgr);

    _btOnDone = done;
    _btPollCount = 0;
    if(!_btPollTimer)
    {
        _btPollTimer = new QTimer(this);
        _btPollTimer->setInterval(500);
        connect(_btPollTimer, &QTimer::timeout, this, &Device::btPollTick);
    }
    _btPollTimer->start();
}

void
Device::btPollTick()
{
    QBluetoothLocalDevice devicee;

    if(devicee.hostMode() != QBluetoothLocalDevice::HostPoweredOff)
    {
        _btPollTimer->stop();
        auto done = _btOnDone;
        _btOnDone = nullptr;
        if(done)    done(true);
        return;
    }

    //~6 сек на включение, затем отказ
    if(++_btPollCount >= 12)
    {
        _btPollTimer->stop();
        _commun_display->setUpdatee("Не удалось включить Bluetooth");
        _commun_display->statusDevicee(_commun_display->statusDevic::searchFinish);
        auto done = _btOnDone;
        _btOnDone = nullptr;
        if(done)    done(false);
    }
}

void Device::setCurrentDeviceName(QString name)
{
    currenDeviceName_ = name;
    _commun_display->setCurDeviceName(currenDeviceName_);
}

/// удаляет устройство из памяти и из листа
int
Device::del_device(QString id)
{
    if(id.isEmpty())
        return 1;

    /// удаляет из памяти
    QSettings setting;
    QVector<QStringList> lastDevices;
    int deviceNum = 0;

    //вычитываю все устройства
    reading_devices_from_memory(lastDevices, deviceNum);

    // TODO проверка есть ли устройство в списке сохраненных
    deviceNum = 0;
    // удаляет сохраненное
    if(searching_device_in_memory(lastDevices, id, deviceNum))
    {
        setting.remove(QString("last%1").arg(deviceNum));
        // сдвинуть остальные чтоб не было промежутка
        while(true)
        {
            if (setting.contains(QString("last%1").arg(++deviceNum))) {
                setting.setValue(QString("last%1").arg(deviceNum-1), setting.value(QString("last%1").arg(deviceNum)));
            }
            else
            {
                if (setting.contains(QString("last%1").arg(--deviceNum))) {
                    setting.remove(QString("last%1").arg(deviceNum));
                }
                break;
            }
        }
    }

    /// удаляет из листа
    deviceNum = 0;
    for (QObject *obj : devices) {
        if (obj->property("deviceAddress").toString() == id)
        {
            devices.removeAt(deviceNum);      // убираем из списка
            emit devicesUpdated();             // уведомляем QML
        }
        deviceNum++;
    }

    return 0;
}

/// поднимает устройство в начало списка в памяти и в списке
int
Device::upList_device(QString id)
{
    if(id.isEmpty())
        return 1;

    /// поднимает устройство в начало списка в списке
    for(int i = 0; i < devices.size(); ++i)
    {
        DeviceInfo *info = qobject_cast<DeviceInfo*>(devices.at(i));
        if(info && info->getAddress() == id)
        {
            if(i == 0)
                break;   // уже в начале
            devices.move(i, 0); // QList::move перемещает элемент
            emit devicesUpdated();             // уведомляем QML
            break;;
        }
    }

    /// поднимает устройство в начало списка в памяти
    QVector<QStringList> lastDevices;
    QSettings settings;
    int lastIndex;
    reading_devices_from_memory(lastDevices, lastIndex);
    int deviceNum = 0;
    // если такого устройства нет в памяти - выйти
    if(!searching_device_in_memory(lastDevices, id, deviceNum))
        return 0;
    if(deviceNum >= lastIndex-1)
        return 0;

    //читаю перемещаемое устройство до сдвига
    QVariant movedDevice = settings.value(QString("last%1").arg(deviceNum));

    // сдвигаю last(deviceNum+1)..last(lastIndex) на -1, иду с начала
    for(int i = deviceNum+1; i < lastIndex; ++i)
    {
        QVariant value = settings.value(QString("last%1").arg(i));
        settings.setValue(QString("last%1").arg(i - 1), value);
    }
    // кладу перемещаемое устройство в последний ключ
    settings.setValue(QString("last%1").arg(lastIndex), movedDevice);

    return 0;
}

//добавление сохраненных устройств в список устройств
void
Device::get_last_device()
{
    if(!connected_flag)
    {
        QString dName = "null";
        quint32 dClass = 0x020C;
        QString dAdres = "null";

        stopDeviceDiscovery();
        clearDeviceDiscovery();

        QVector<QStringList> lastDevices;
        int deviceNum = 0;
        reading_devices_from_memory(lastDevices, deviceNum);

        //добавятся в порядке сохранения
        for(int i = deviceNum - 1; i >= 0; i--)
        {
            if((!lastDevices[i].at(0).isEmpty()) &&
                (!lastDevices[i].at(1).isEmpty()) &&
                (!lastDevices[i].at(2).isEmpty()))
            {
                dName = lastDevices[i].at(0);
                dClass = lastDevices[i].at(1).toInt();
                dAdres = lastDevices[i].at(2);

                QBluetoothDeviceInfo info(QBluetoothAddress(dAdres), dName, dClass);
                info.setCoreConfigurations(QBluetoothDeviceInfo::LowEnergyCoreConfiguration);
                addDevice(info);
            }
        }

        if(deviceNum == 0)  _commun_display->setUpdatee("Сохраненные устройства отсутствуют. Начать поиск устройств?");
        else                _commun_display->setUpdatee("Сохраненные устройства добавлены.");
    }
}

/// добавляет новые имитированные устройства
void
Device::addingNewDevicesWindows()
{
    QString dName;
    // создаю смартфон
    // согласно документации: биты 8-12 - это major class, биты 2-7 - это minor class
    // PhoneDevice = 2, SmartPhone = 3
    // Закодированное значение = (2 << 8) | (3 << 2) = 0x020C
    quint32 dClass = 0x020C;
    QString dAdres;

    for(int i = 0; i < 5; i++)
    {
        nameDevice_ = dName = "name" + QString::number(i) + "*";
        class_ = QString::number(dClass);
        lastConnectedDevice_ = dAdres = "00:01:02:03:04:0" + QString::number(i);

        QBluetoothDeviceInfo info(QBluetoothAddress(dAdres), dName, dClass);
        info.setCoreConfigurations(QBluetoothDeviceInfo::LowEnergyCoreConfiguration);
//        qDebug() << "Raw coreConfigurations:" << info.coreConfigurations();

        addDevice(info);
    }

    QSettings setting;
    // очистить список
    for(int i = 0; i < 30; i++)
    {
        if (setting.contains(QString("last%1").arg(i)))
            setting.remove(QString("last%1").arg(i));
    }

    // эти устройства сохранены в памяти
    for(int i = 5; i < 10; i++)
    {
        nameDevice_ = dName = "name" + QString::number(i) + "*";
        class_ = QString::number(dClass);
        lastConnectedDevice_ = dAdres = "00:01:02:03:04:0" + QString::number(i);
        QBluetoothDeviceInfo info(QBluetoothAddress(dAdres), dName, dClass);
        info.setCoreConfigurations(QBluetoothDeviceInfo::LowEnergyCoreConfiguration);
        addDevice(info);
        set_last_device();

    }
    QVector<QStringList> lastDevices;
    int deviceNum = 0;
    reading_devices_from_memory(lastDevices, deviceNum);
}

//сохранение последнего подключенного устройства
void
Device::set_last_device()
{
    QSettings setting;

    //вычитываю все устройства
    QVector<QStringList> lastDevices;
    int deviceNum = 0;
    reading_devices_from_memory(lastDevices, deviceNum);
    deviceNum = 0;
    if(searching_device_in_memory(lastDevices, lastConnectedDevice_, deviceNum))
        return;

    //сохраняю новое устройство
    QString device = QStringList({nameDevice_, class_, lastConnectedDevice_}).join(";");
    setting.setValue(QString("last%1").arg(deviceNum), device);    // запись нового устройства
}




