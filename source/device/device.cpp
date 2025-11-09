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

#include "source/device/device.h"
#include "source/main/mainmodel.h"

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
#include <string.h>
#include <QTime>
#include <QCoreApplication>
#include <QGeoCoordinate>
#include <QSettings>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QBluetoothUuid>



void
delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}


Device::Device(MainModel *mainModel) : mainModel_(mainModel)
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
Device::setPacking(Packing *newPacking)
{
    _packing = newPacking;
}

void
Device::setUnpacking(Unpacking *newUnpacking)
{
    _unpacking = newUnpacking;
}


void
Device::startDeviceDiscovery()
{
    if(!blt_on())   return;

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
     QString empty;
    if(info.name() != empty)
    {
        devices.append(new DeviceInfo(QBluetoothDeviceInfo(info)));
        emit devicesUpdated();
    }
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
    QBluetoothDeviceInfo info;

    if(!blt_on())   return;

    discoveryAgent->stop();
    deviceScanFinished();

    lastConnectedDevice_ = dAddress;
    nameDevice_ = name;

    if (config == "BR") {
        class_ = "2";
    }
    else if (config == "LE") {
        class_ = "1";
    }
    else if (config == "BRLE") {
        class_ = "3";
    }
    else{
        class_ = "0";
    }

    _commun_display->setUpdatee("Подключение к " + nameDevice_);
    _commun_display->statusDevicee(_commun_display->statusDevic::searchInProgr);

    socket = new QBluetoothSocket(QBluetoothServiceInfo::Protocol::RfcommProtocol, this);

    socket->connectToService(QBluetoothAddress(dAddress), QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));

    connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(socketError(QBluetoothSocket::SocketError)));
    connect(socket, SIGNAL(connected()), this, SLOT(deviceConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectFromDevice()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketRead()));
}

void
Device::deviceConnected()
{
    connected = true;
    //! [les-service-2]
    //    controller->discoverServices();
    //! [les-service-2]
    _commun_display->setUpdatee("Подключено");
    _commun_display->statusDevicee(_commun_display->statusDevic::searchFinish);

    //сохранение последнего подключенного устройства
    QSettings setting;
    setCurrentDeviceName(nameDevice_);
    setting.setValue("dName", nameDevice_);
    setting.setValue("dClass", class_);
    setting.setValue("dAdres", lastConnectedDevice_);

    _packing->setSocket(socket);
    _commun_display->set_connected(connected);

    mainModel_->checkingParameters();
}

void
Device::errorReceived(QLowEnergyController::Error)
{
//     qWarning() << "Error: " << controller->errorString();
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

    _packing->delSocket();
    connected = false;
    _commun_display->set_connected(connected);

    //сбросить напряжение и ток
    _commun_display->curRealChang(0.0f);
    _commun_display->vrealChang(0.0f);
}


//отправить сообщение
int
Device::sendMessage(QByteArray data)
{
    if (!socket)   {
        return -1;
    }
    int r = socket->write(data);
    return r;
}


void
Device::deviceDisconnected()
{
    qWarning() << "Устройство отключено";
    _commun_display->statusDevicee(_commun_display->statusDevic::disconnect);
    //сбросить напряжение и ток
    _commun_display->curRealChang(0.0f);
    _commun_display->vrealChang(0.0f);
}


void
Device::socketRead()
{
   QByteArray recievedData = socket->readAll();
   emit socketDataRecieved(recievedData.toHex());
   int size = recievedData.size();
   int res;

   //если терминал - вывести в сыром виде
   //в окно терминала
   _commun_display->log_out_T("<-", QString ("%1 (%2 size)").arg(QString(recievedData.toHex())).arg(size));

   //распаковка
   res = _unpacking->unpack(recievedData, size);
   //если превышена длина пакета приема
   if(res == -1)
   {
       if(mainModel_->getAdminTapCount())
       {
           _commun_display->logJoy("<- ", "Ошибка. Превышена длина входящего сообщения");
       }
   }

    res = 1;
    while(res == 1)
    {
        QByteArray mess;
        //проверка остатка в переменной темп
        res = _unpacking->checkBalance();
        if(res == 1)
        {
            _unpacking->unpack(mess, mess.size());
        }
    }
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


bool
Device::blt_on()
{
    QBluetoothLocalDevice devicee;
//    QGeoCoordinate geo;

//    if(!geo.isValid())
//    {
//         _commun_display->setUpdatee("Геолокация не поддерживается платформой.\nПоиск bt устройств невозможен.");
//        return false;
//    }

    if(!devicee.isValid())
    {
        _commun_display->setUpdatee("Bluetooth не поддерживается платформой.");
        return false;
    }


    if(devicee.hostMode() == QBluetoothLocalDevice::HostPoweredOff)// Bluetooth is not enabled
    {
        devicee.powerOn();// Call to open the local Bluetooth device
        _commun_display->setUpdatee("Включение Bluetooth");
        _commun_display->statusDevicee(_commun_display->statusDevic::searchInProgr);
        delay(1500);

        qint32 count = 0;
        while(count++ < 8)
        {
            if(devicee.hostMode() != QBluetoothLocalDevice::HostPoweredOff)      count = 8;
            delay(700);
        }
        delay(500);
    }
    return true;
}

void Device::setCurrentDeviceName(QString name)
{
    currenDeviceName_ = name;
    _commun_display->setCurDeviceName(currenDeviceName_);
}

//добавление в список последнего подключенного устройства
void
Device::get_last_device()
{
    if(!connected)
    {
        QSettings setting;
        QString dName;
        quint32 dClass;
        QString dAdres;

        dName = setting.value("dName", QString()).toString();
        dClass = setting.value("dClass").toUInt();
        dAdres = setting.value("dAdres", QString()).toString();

        if((dName.isEmpty()) || (dAdres.isEmpty()))
        {
            _commun_display->setUpdatee("Сохраненные устройства отсутствуют. Начать поиск устройств?");
            return;
        }

        stopDeviceDiscovery();
        clearDeviceDiscovery();

        addDevice(QBluetoothDeviceInfo(QBluetoothAddress(dAdres), dName, dClass));
        _commun_display->setUpdatee("Сохраненное устройство добавлено.");
    }
}




