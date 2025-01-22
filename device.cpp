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

#include "device.h"

#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QDebug>
#include <QList>
#include <QMetaEnum>
#include <QTimer>

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

    setUpdate("Search");
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

void Device::startDeviceDiscovery()
{
    qDeleteAll(devices);
    devices.clear();
    emit devicesUpdated();

//    devices.append(new DeviceInfo("asfdf", "asdfsdf"));
//    devices.append(new DeviceInfo("asfdf1", "asdfsdf2"));
//    devices.append(new DeviceInfo("asfdf12", "asdfsdf23"));

    setUpdate("Сканирование устройств ...");
    //! [les-devicediscovery-2]
//    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::ClassicMethod);
    //! [les-devicediscovery-2]

    if (discoveryAgent->isActive()) {
        m_deviceScanState = true;
        Q_EMIT stateChanged();
        emit searchInProgress();
    }
}

//! [les-devicediscovery-3]
void Device::addDevice(const QBluetoothDeviceInfo &info)
{
//    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
//        setUpdate("Last device added: " + info.name());

        devices.append(new DeviceInfo(QBluetoothDeviceInfo(info)));

        emit devicesUpdated();
//    }
}
//! [les-devicediscovery-3]

void Device::deviceScanFinished()
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
        setUpdate("Устройства не найдены");
    } else {
        setUpdate("Сканирование завершено");
    }

    emit searchFinished();
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

QVariant Device::getCharacteristics()
{
    return QVariant::fromValue(m_characteristics);
}

QString Device::getUpdate()
{
    return m_message;
}


void
Device::connectToDevice(const QString &uuid)
{
    setUpdate("Подключение...");
    //    selectedDevice = listOfDevices.at(i);
    lastConnectedDevice_ = uuid;
    qDebug() << "User select a device: " << uuid ;
    //             QBluetoothSocket::Rf
    socket = new QBluetoothSocket(QBluetoothServiceInfo::Protocol::RfcommProtocol, this);

    socket->connectToService(QBluetoothAddress(uuid), QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));

    connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(socketError(QBluetoothSocket::SocketError)));
    connect(socket, SIGNAL(connected()), this, SLOT(deviceConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectFromDevice()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketRead()));
}

void Device::deviceConnected()
{
    setUpdate("Back\n(Discovering services...)");
    connected = true;
    //! [les-service-2]
    //    controller->discoverServices();
    //! [les-service-2]
    setUpdate("Подключено");

    mainModel_->setCurrentDeviceName(lastConnectedDevice_);
}

void Device::errorReceived(QLowEnergyController::Error /*error*/)
{
    qWarning() << "Error: " << controller->errorString();
    setUpdate(QString("Back\n(%1)").arg(controller->errorString()));
}

void Device::setUpdate(const QString &message)
{
    m_message = message;
    emit updateChanged(message);
}


QByteArray
Device::wrapData(QByteArray input)
{
    QByteArray out;

    out.append(input);

    qint16 crc = crc16(input);

    out.append(crc & 0x00FF);
    out.append((crc >> 8) & 0x00FF);

    out.replace(QByteArray(1, 0x1F), QByteArray(2, 0x1F));
    out.replace(QByteArray(1, 0x2F), QByteArray(2, 0x2F));

    out.append(0x2F);
    out.append(0x55);

    out.insert(0, 0x1F);

    return out;
}


qint16
Device::crc16(const QByteArray &arr)
{
    quint32 len = arr.size();
    uint16_t crc = 0;  //0xFFFF
    quint8 byte;
    quint8 i;
    quint32 p = 0;

    while(len-- > 0){
        byte = arr[p++];

        crc = crc ^ byte << 8;
        i = 8;
        do {
            if (crc&0x8000) {
                crc = crc<<1^0x1021;
            } else {
                crc = crc<<1;
            }
        }   while (--i);
    }

    return crc;
}

void Device::disconnectFromDevice()
{
    setUpdate("отключено");
    // UI always expects disconnect() signal when calling this signal
    // TODO what is really needed is to extend state() to a multi value
    // and thus allowing UI to keep track of controller progress in addition to
    // device scan progress

    //    if (controller->state() != QLowEnergyController::UnconnectedState)
    //        controller->disconnectFromDevice();
    //    else
    //        deviceDisconnected();

    mainModel_->setCurrentDeviceName("<none>");
}


void
Device::sendMessage(QString msg)
{
    QByteArray dataToSEnd = QByteArray::fromHex(msg.toUtf8());

    if (needWrap_) {
        dataToSEnd = wrapData(dataToSEnd);
    }

    emit log("try_send", QString(dataToSEnd.toHex()));

    if (!socket)   {
        return;
    }

    int r = socket->write(dataToSEnd);

    emit log("send", QString ("%1 (%2 size)").arg(QString(dataToSEnd.toHex())).arg(r));
}


void
Device::sendMessageAndWrap(QString msg)
{
    QByteArray dataToSEnd = QByteArray::fromHex(msg.toUtf8());

    dataToSEnd = wrapData(dataToSEnd);
    emit log("try_send", QString(dataToSEnd.toHex()));

    if (!socket)   {
        return;
    }

    int r = socket->write(dataToSEnd);

    emit log("send", QString ("%1 (%2 size)").arg(QString(dataToSEnd.toHex())).arg(r));
}


void
Device::onJoysticActivity(quint8 mode, float azimut, float amplitude, float level)
{
    qDebug() << mode << azimut << amplitude << level;

    QByteArray msg;

    msg.append(static_cast<quint8>(mode));

    azimut = azimut - 3.14 / 2;

    qint16 azimut16 = 0;

    if (azimut > 0) {
        azimut16 = azimut * 57.3248;
    } else {
        azimut16 = ((3.14 - qAbs(azimut)) + 3.14) * 57.3248 + 1;
    }

    quint16 amplitude16 = amplitude * mainModel_->getJoystickAmplitude();
    quint16 level16 = level * 4095;

    qDebug() << mode << azimut16 << amplitude16 << level16;

    msg.append((azimut16 >> 8) & 0x00FF);
    msg.append(azimut16 & 0x00FF);

    msg.append((amplitude16 >> 8) & 0x00FF);
    msg.append(amplitude16 & 0x00FF);
    msg.append((level16 >> 8) & 0x00FF);
    msg.append(level16 & 0x00FF);

    msg = wrapData(msg);
    emit log("try_send", QString(msg.toHex()));

    if (!socket)   {
        return;
    }

    int r = socket->write(msg);

    emit log("send", QString ("%1 (%2 size)").arg(QString(msg.toHex())).arg(r));
}

void Device::deviceDisconnected()
{
    qWarning() << "Устройство отключено";
    emit disconnected();
}


void
Device::socketRead()
{
    QByteArray recievedData = socket->readAll();
    emit socketDataRecieved(recievedData.toHex());

    emit log("incoming", recievedData.toHex());
}


void Device::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        setUpdate("Включите Bluetooth");
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
        setUpdate("Writing or reading from the device resulted in an error.");
    else {
        static QMetaEnum qme = discoveryAgent->metaObject()->enumerator(
                    discoveryAgent->metaObject()->indexOfEnumerator("Error"));
        setUpdate("Error: " + QLatin1String(qme.valueToKey(error)));
    }

    m_deviceScanState = false;
    emit devicesUpdated();
    emit stateChanged();
}

bool Device::state()
{
    return m_deviceScanState;
}

bool Device::hasControllerError() const
{
    return (controller && controller->error() != QLowEnergyController::NoError);
}

bool Device::isRandomAddress() const
{
    return randomAddress;
}

void Device::setRandomAddress(bool newValue)
{
    randomAddress = newValue;
    emit randomAddressChanged();
}


bool
Device::getNeedWrap()
{
    return needWrap_;
}


void
Device::setNeedWrap(bool value)
{
    needWrap_ = value;
}
