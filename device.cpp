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
#include "mainmodel.h"

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
#include <QThread>
#include <QRegularExpression>
#include <string.h>
#include <QTime>
#include <QCoreApplication>
#include <QGeoCoordinate>
#include <QSettings>
#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QBluetoothUuid>


void delay( int millisecondsToWait )
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

     if(rendering_flag)  setUpdate("Поиск");
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
    if(!blt_on())   return;

    qDeleteAll(devices);
    devices.clear();
    if(rendering_flag)   emit devicesUpdated();

//    devices.append(new DeviceInfo("asfdf", "asdfsdf"));
//    devices.append(new DeviceInfo("asfdf1", "asdfsdf2"));
//    devices.append(new DeviceInfo("asfdf12", "asdfsdf23"));

     if(rendering_flag)  setUpdate("Сканирование устройств ...");
    //! [les-devicediscovery-2]
//    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::ClassicMethod);

    //! [les-devicediscovery-2]

    if (discoveryAgent->isActive()) {
        m_deviceScanState = true;
        Q_EMIT stateChanged();
         if(rendering_flag)  emit searchInProgress();
    }
}

//! [les-devicediscovery-3]
void Device::addDevice(const QBluetoothDeviceInfo &info)
{
     QString empty;

//    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
//        setUpdate("Last device added: " + info.name());

        if(info.name() != empty)
        {
            devices.append(new DeviceInfo(QBluetoothDeviceInfo(info)));
             if(rendering_flag)  emit devicesUpdated();
        }

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

     if(rendering_flag)  emit devicesUpdated();
    m_deviceScanState = false;
     if(rendering_flag)  emit stateChanged();
    if (devices.isEmpty()) {
         if(rendering_flag)  setUpdate("Устройства не найдены");
    } else {
         if(rendering_flag)  setUpdate("Сканирование завершено");
    }

     if(rendering_flag)  emit searchFinished();
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

     if(rendering_flag)  setUpdate("Подключение к " + nameDevice_);
    //    selectedDevice = listOfDevices.at(i);

//    qDebug() << "User select a device: " << uuid ;
    //             QBluetoothSocket::Rf
    socket = new QBluetoothSocket(QBluetoothServiceInfo::Protocol::RfcommProtocol, this);

    socket->connectToService(QBluetoothAddress(dAddress), QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));

    connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)), this, SLOT(socketError(QBluetoothSocket::SocketError)));
    connect(socket, SIGNAL(connected()), this, SLOT(deviceConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectFromDevice()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketRead()));
}

void Device::deviceConnected()
{
    QSettings setting;

    connected = true;
    //! [les-service-2]
    //    controller->discoverServices();
    //! [les-service-2]
     if(rendering_flag)  setUpdate("Подключено");

    if(rendering_flag)   mainModel_->setCurrentDeviceName(nameDevice_);

    setting.setValue("dName", nameDevice_);
    setting.setValue("dClass", class_);
    setting.setValue("dAdres", lastConnectedDevice_);

    //запрос точки восстановления
    mainModel_->full_param_check();
}

void Device::errorReceived(QLowEnergyController::Error)
{
//     qWarning() << "Error: " << controller->errorString();
     if(rendering_flag)  setUpdate(QString("Back\n(%1)").arg(controller->errorString()));
}

void Device::setUpdate(const QString &message)
{
    m_message = message;
    if(rendering_flag)     emit updateChanged(message);
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


quint16
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


/*
  Name  : CRC-32
  Poly  : 0x04C11DB7    x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11
                       + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1
  Init  : 0xFFFFFFFF
  Revert: true
  XorOut: 0xFFFFFFFF
  Check : 0xCBF43926 ("123456789")
  MaxLen: 268 435 455 байт (2 147 483 647 бит) - обнаружение
   одинарных, двойных, пакетных и всех нечетных ошибок
*/

const uint_least32_t Crc32Table[256] = {
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
    0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

///указатель на буфер
/// формат буфура
/// 8 бит
/// 16 бит
/// 32 бита
uint_least32_t Device::crc32(const QByteArray &buf, qint32 format_buf)
{
    size_t len = buf.size();
    quint32 p = 0;
    uint_least32_t crc = 0xFFFFFFFF;
    f_value format_convers;

    if(format_buf == 8)
    {
        while (len)
        {
            format_convers.data[0] = buf[p++];
            format_convers.data[1] = buf[p++];
            format_convers.data[2] = buf[p++];
            format_convers.data[3] = buf[p++];

            crc = (crc >> 8) ^ Crc32Table[(crc ^ format_convers.u32) & 0xFF];
            len -= 4;

        }
    }
    else
    {
        crc = 0;
    }

    return crc ^ 0xFFFFFFFF;
}

QString Device::getLastConnectedDevice()
{
    return  lastConnectedDevice_;
}

void Device::set_rendering_flag(bool fl)
{
    rendering_flag = fl;
}



void Device::disconnectFromDevice()
{
    if(rendering_flag)  setUpdate("Отключено");
    // UI always expects disconnect() signal when calling this signal
    // TODO what is really needed is to extend state() to a multi value
    // and thus allowing UI to keep track of controller progress in addition to
    // device scan progress

    //    if (controller->state() != QLowEnergyController::UnconnectedState)
    //        controller->disconnectFromDevice();
    //    else
    //        deviceDisconnected();

    mainModel_->setCurrentDeviceName("Отсутствует подключение");

    //сбросить напряжение и ток
    mainModel_->setCurReal(0.0f);
    mainModel_->setVreal(0.0f);
}

void Device::sendUpdate(QByteArray msg)
{
    QByteArray dataToSEnd = msg;
    int r = -1;
    dataToSEnd = wrapData(dataToSEnd);
//    qDebug() << dataToSEnd.toHex();
    //вывод лога
//    if(loging)
//    {
//        if(rendering_flag)
//        {
//            emit logServis("-> ", QString ("%1 (%2 size)").arg(QString(dataToSEnd.toHex())).arg(r));
//        }
//    }

    if (!socket)   {
        return;
    }

    r = socket->write(dataToSEnd);
    //вывод лога
    if(loging)
    {
        if(rendering_flag)
        {
            emit logServis("-> ", QString ("%1 (%2 size)").arg(QString(dataToSEnd.toHex())).arg(r));
        }
    }
}


//для терминала
void
Device::sendMessage(QString msg)
{
    QByteArray dataToSEnd = QByteArray::fromHex(msg.toUtf8());

    if (needWrap_) {
        dataToSEnd = wrapData(dataToSEnd);
    }

//    if(rendering_flag)    emit log("try_send", QString(dataToSEnd.toHex()));

    if (!socket)   {
        return;
    }

    int r = socket->write(dataToSEnd);

    if(rendering_flag)    emit log("->", QString ("%1 (%2 size)").arg(QString(dataToSEnd.toHex())).arg(r));
}

//для терминала
void
Device::sendMessageAndWrap(QString msg)
{
    QByteArray dataToSEnd = QByteArray::fromHex(msg.toUtf8());
    dataToSEnd = wrapData(dataToSEnd);

//    if(rendering_flag)    emit log("try_send", QString(dataToSEnd.toHex()));
//    qDebug() << dataToSEnd.toHex();

    if (!socket)   {
        return;
    }

    int r = socket->write(dataToSEnd);

    if(rendering_flag)    emit log("->", QString ("%1 (%2 size)").arg(QString(dataToSEnd.toHex())).arg(r));
}

void Device::sendMessageAndWrapS (QString msg)
{
    unsigned char k;
    quint8 cod;

    QByteArray dataToSEnd = QByteArray::fromHex(msg.toUtf8());

//    qDebug() << dataToSEnd.toHex();

    k = dataToSEnd[0];

    //если передача параметров
    if(k == 0xA8)
    {
        QByteArray param = mainModel_->get_full_param();
        if(param.size() > 10)
        {
            for(int i = 0; i < param.size(); i++)
            {
                dataToSEnd.append(param[i]);
            }
        }
        else
        {
            k = 0x00;
        }
    }
    dataToSEnd = wrapData(dataToSEnd);

    if(k == 0xE4 || k == 0xA6)
    {
        cod = dataToSEnd[3];
    }

//    qDebug() << dataToSEnd.toHex();

    //перед отправкой
//    if(rendering_flag)    sendparstxlog(k);
//    if(rendering_flag)    emit logServis("try_send", QString(dataToSEnd.toHex()));
//    if(rendering_flag)    emit logJoy("<-", QString(dataToSEnd.toHex()));


    if (!socket)   {
        return;
    }

 //   int r = socket->write(dataToSEnd);
 //   if(rendering_flag)    emit logServis("->", QString ("%1 (%2 size)").arg(QString(dataToSEnd.toHex())).arg(r));

    int r = -1;
    if(k != 0x00)
    {
        r = socket->write(dataToSEnd);
    }
    if(rendering_flag)    sendparstxlog(k, cod);

    //вывод лога
    if(loging)
    {
        if(rendering_flag)
        {
            emit logJoy("->", QString ("%1 (%2 size)").arg(QString(dataToSEnd.toHex())).arg(r));
            emit logServis("->", QString ("%1 (%2 size)").arg(QString(dataToSEnd.toHex())).arg(r));
        }
    }
}


void Device::sendMessageAndWrap(quint8 code, QString msg)
{
    unsigned char k = code;
    quint8 cod;

    QByteArray dataToSEnd;


    if(k == 0xF5)
    {
        f_value val;
        //0xF3A58E1D
        val.data[0] = 0xF3;
        val.data[1] = 0xA5;
        val.data[2] = 0x8E;
        val.data[3] = 0x1D;

        dataToSEnd.append(val.data[0]);
        dataToSEnd.append(val.data[1]);
        dataToSEnd.append(val.data[2]);
        dataToSEnd.append(val.data[3]);
    }
    else if(k != 0xe9)
    {
        QStringList sl = msg.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        for(QString s: sl) {
            int i = s.toInt();
            if ((i > 0xFF) && (i <= 0xFFFF))
            {
                dataToSEnd.append(i >> 8);
                i = i % 0x100;
            }
            dataToSEnd.append(i);   // 1 byte (unsigned)
        }
    }
    else if(k == 0xe9)
    {
        f_value val;
        val.f = msg.toFloat();
        if (val.f == val.f) {
            dataToSEnd.append(val.data[0]);
            dataToSEnd.append(val.data[1]);
            dataToSEnd.append(val.data[2]);
            dataToSEnd.append(val.data[3]);
        }
//         qDebug() << val.f << dataToSEnd.toHex();
    }
    dataToSEnd.prepend(code);
    dataToSEnd = wrapData(dataToSEnd);
    cod = dataToSEnd[2];
 //   qDebug() << dataToSEnd.toHex();


//    qDebug() << dataToSEnd.toHex();
//     if(rendering_flag)    sendparstxlog(k, cod);
//    if(rendering_flag)    emit logServis("try_send", QString(dataToSEnd.toHex()));
//    if(rendering_flag)    emit logJoy("<-", QString(dataToSEnd.toHex()));

    if (!socket)   {
        return;
    }

 //   int r = socket->write(dataToSEnd);
 //   if(rendering_flag)    emit logServis("->", QString ("%1 (%2 size)").arg(QString(dataToSEnd.toHex())).arg(r));

    int r = -1;
    if(k != 0x00)
    {
        r = socket->write(dataToSEnd);
    }

   if(rendering_flag)    sendparstxlog(k, cod);
   //вывод лога
   if(loging)
   {
       if(rendering_flag)
       {
           emit logJoy("->", QString ("%1 (%2 size)").arg(QString(dataToSEnd.toHex())).arg(r));
           emit logServis("->", QString ("%1 (%2 size)").arg(QString(dataToSEnd.toHex())).arg(r));
       }
   }
}

void Device::sendparstxlog(quint8 code, quint8 cod)
{
    QString s;

    switch (code) {
    case 0x00:
        s = "ошибка";
        break;
    case 0xE0:
        s = "сброс до заводских";
        break;
    case 0xE1:
        s = "запись мин угла сервы";
        break;
    case 0xE2:
        s = "запись уст. светодиодов";
        break;
    case 0xE3:
        s = "установить серву в угол";
        break;
    case 0xE4:
        if(cod == 0x00)         s = "флаг блокировки тока OFF";
        else if (cod == 0x01)   s = "флаг блокировки тока ON";
        break;
    case 0xE5:
        s = "запрос ошибок";
        break;
    case 0xE6:
        s = "запрос уст. сервы";
        break;
    case 0xE7:
        s = "автокалибровка серв";
        break;
    case 0xE8:
        s = "автокалибр. датчика тока";
        break;
    case 0xE9:
        s = "записать калибровку тока";
        break;
    case 0xEA:
        s = "запрос калибровки тока";
        break;
    case 0xEB:
        s = "стереть ошибки";
        break;
    case 0xEC:
        s = "сервы в стартовое положение";
        break;
    case 0xED:
        s = "сервы в домашнее положение";
        break;
    case 0xEE:
        s = "сохранение углов";
        break;
    case 0xEF:
        s = "Reset foot angl";
        break;
    case 0xF0:
        s = "команда перезагрузки";
        break;
    case 0xF1:
        s = "обновление углов серв...";
        break;
    case 0xF2:
        s = "запрос версии";
        break;
    case 0xF5:
        s = "подпись приложения";
        break;
    case 0xA0:
        s = "опрос светодиода";
        break;
    case 0xA1:
        s = "запрос напряжения";
        setVreal = true;
        break;
    case 0xA2:
        s = "запрос тока";
        setCurReal = true;
        break;
    case 0xA3:
        s = "запрос углов гироскопа";
        break;
    case 0xA4:
        s = "запрос ускорения";
        break;
    case 0xA5:
        s = "запрос угла сервы";
        break;
    case 0xA6:
        if(cod == 0x00)         s = "питание серв OFF";
        else if (cod == 0x01)   s = "питание серв ON";
        break;
    case 0xA7:
        s = "запрос точки восстановления";
        break;
    case 0xA8:
        s = "отправка точки восстановления";
        break;

    default:
       return;
        break;
    }
   if(rendering_flag)    emit logServis("-> ", s);
}



void
Device::onJoysticActivity(quint8 mode, float azimut, float amplitude, float level, bool ctrl)
{
//    qDebug() << mode << azimut << amplitude << level;

    QByteArray msg;

    if(mode == 1)           msg.append(static_cast<quint8>(0x0A));
    else if(mode == 2)           msg.append(static_cast<quint8>(0x0B));
    else if(mode == 3)           msg.append(static_cast<quint8>(0x0C));
    else                   msg.append(static_cast<quint8>(0x00));

    azimut = azimut - 3.14 / 2;

    qint16 azimut16 = 0;

    if (azimut > 0) {
        azimut16 = azimut * 57.3248;
    } else {
        azimut16 = ((3.14 - qAbs(azimut)) + 3.14) * 57.3248 + 1;
    }
 //   qDebug() << azimut16;

    quint16 amplitude16;
    if(mode == 3)
    {
        amplitude16 = amplitude * (mainModel_->getJoystickAmplitude() / 2);
    }
    else
    {
        amplitude16 = amplitude * mainModel_->getJoystickAmplitude();
    }

    quint16 level16 = level;

//    qDebug() << mode << azimut16 << amplitude16 << level16;

    msg.append((azimut16 >> 8) & 0x00FF);
    msg.append(azimut16 & 0x00FF);

    msg.append((amplitude16 >> 8) & 0x00FF);
    msg.append(amplitude16 & 0x00FF);
    msg.append((level16 >> 8) & 0x00FF);
    msg.append(level16 & 0x00FF);
    msg.append(ctrl);

    msg = wrapData(msg);
//  if(rendering_flag)      emit logJoy("try_send", QString(msg.toHex()));
//  qDebug() << QString(msg.toHex());

    if (!socket)   {
        return;
    }

    socket->write(msg);
    int r = socket->write(msg);

    //  if(rendering_flag)    emit logJoy("->", QString ("%1 (%2 size)").arg(QString(msg.toHex())).arg(r));

    //вывод лога
    if(loging)
    {
        if(rendering_flag)
        {
            emit logJoy("->", QString ("%1 (%2 size)").arg(QString(msg.toHex())).arg(r));
            emit logServis("->", QString ("%1 (%2 size)").arg(QString(msg.toHex())).arg(r));
        }
    }
}

void Device::get_check()
{
    //при первом входе в режим джойстика - запросить ошибки
    QByteArray get_check, s;
    get_check.insert(0, 0xE5);
    get_check = wrapData(get_check);

//    if(rendering_flag)    emit logJoy("try_send", QString(get_check.toHex()));

     if (!socket)   {
         return;
     }
    int r = socket->write(get_check);

    s = "запрос ошибок";
    if(rendering_flag)    emit logJoy("->", s);

    //вывод лога
    if(loging)
    {
        if(rendering_flag)
        {
            emit logJoy("->", QString ("%1 (%2 size)").arg(QString(get_check.toHex())).arg(r));
            emit logServis("->", QString ("%1 (%2 size)").arg(QString(get_check.toHex())).arg(r));
        }
    }
}

void Device::onGetCurReal()
{
    //запрос тока
    QByteArray get_cur;
    get_cur.insert(0, 0xA2);
    get_cur = wrapData(get_cur);

//   if(rendering_flag)     emit logJoy("-> ",  QString(get_cur.toHex()));

     if (!socket)   {
         return;
     }
    int r = socket->write(get_cur);
    //вывод лога
    if(loging)
    {
        if(rendering_flag)
        {
            emit logJoy("->", QString ("%1 (%2 size)").arg(QString(get_cur.toHex())).arg(r));
            emit logServis("->", QString ("%1 (%2 size)").arg(QString(get_cur.toHex())).arg(r));
        }
    }
}

void Device::deviceDisconnected()
{
    qWarning() << "Устройство отключено";
    emit disconnected();
    //сбросить напряжение и ток
    mainModel_->setCurReal(0.0f);
    mainModel_->setVreal(0.0f);
}


void
Device::socketRead()
{
   QByteArray recievedData = socket->readAll();
   emit socketDataRecieved(recievedData.toHex());
   int r = recievedData.size();

   //если приложение свернуто - не парсить
  if(!rendering_flag) return;

   const std::string empty;
//   QByteArray Dat = recievedData;

   //если терминал - вывести в сыром виде
   //в окно терминала
   if(rendering_flag)  emit log("<-", QString ("%1 (%2 size)").arg(QString(recievedData.toHex())).arg(r));

   //вывод лога
   if(loging)
   {
       if(rendering_flag)
       {
           emit logJoy("<-", QString ("%1 (%2 size)").arg(QString(recievedData.toHex())).arg(r));
           emit logServis("<-", QString ("%1 (%2 size)").arg(QString(recievedData.toHex())).arg(r));
       }
   }

   //склейка разорванных пакетов
   for (quint8 i = 0; i < recievedData.size(); i++)
   {
     //если пакет не начат
     if (statys == false)
     {
       if (recievedData[i] == 0x1F)
       {
         startByteIndex = i;
         statys = true;
       }
     }
     //если пакет начат
     else if (statys == true)
     {
       //если найден конец первой посылки
       if (i + 1 < recievedData.size() &&
           recievedData[i] == 0x2F &&
           recievedData[i + 1] == 0x55 &&
           i > (startByteIndex + 1) && split == false &&
           recievedData.size() < max_rx_size)
       {
        //удалить все до 1F в recievedData
        if(startByteIndex > 0)
        {
            recievedData.replace(0, startByteIndex, empty);
        }
         statys = false;
         startByteIndex = 0;
         i = recievedData.size();
       }
       //если найден конец и пакет был разделен
       else if(i + 1 < recievedData.size() &&
               recievedData[i] == 0x2F &&
               recievedData[i + 1] == 0x55 &&
               split == true &&
               (recievedData.size() + Temp.size()) < max_rx_size)
       {
            //поместить Temp в начало recievedData
      //     for(qint32 k = Temp.size()-1; k >= 0; k--) recievedData.insert(0, Temp[k]);
           for(qint32 k = 0; k < recievedData.size(); k++) Temp.append(recievedData[k]);
           recievedData.replace(0, recievedData.size(), empty);
           for(qint32 k = 0; k < Temp.size(); k++) recievedData.append(Temp[k]);

            split = false;
            statys = false;
            startByteIndex = 0;
            Temp.replace(0, Temp.size(), empty);
            i = recievedData.size();
       }
       else if(recievedData.size() + Temp.size() >= max_rx_size)
       {

           Temp.replace(0, Temp.size(), empty);
           startByteIndex = 0;
           split = false;
           statys = false;
           i = recievedData.size();
           recievedData.replace(0, recievedData.size(), empty);
           return;
       }
       //если конец пакета - надо склеить
       else if (i + 2 >= recievedData.size())
       {
           //удалить все до 1F в recievedData
           if(startByteIndex > 0)
           {
               recievedData.replace(0, startByteIndex, empty);
           }
           //поместить recievedData в Temp
     //      for(qint32 k = recievedData.size()-1; k >= 0; k--) Temp.insert(0, recievedData[k]);

           for(qint32 k = 0; k < recievedData.size(); k++) Temp.append(recievedData[k]);

           i = recievedData.size();
            recievedData.replace(0, recievedData.size(), empty);
            split = true;
            startByteIndex = 0;
            return;
       }
     }
   }

   //если сервисное окно
   //проверить crc
       //проверить совпадение с известным протоколом
           //если ответ известный - вывести в распарсеном виде
        /****   //если нет - в сыром, но без crc и признаком начала/конца  ****/

   //удаление старт байта, стоп байтов
   if(recievedData[0] == recievedData[1] &&
           recievedData[0] == 0x1F && recievedData.size() > 1)
    {
        recievedData.replace(0, 2, empty);
    }
    else if(recievedData[0] == 0x1F)
    {
        recievedData.replace(0, 1, empty);
    }

    if(recievedData[recievedData.size() - 1] == 0x55)
    {
        recievedData.replace(recievedData.size()-1, 1, empty);
    }
    if(recievedData[recievedData.size() - 1] == 0x2F)
    {
        recievedData.replace(recievedData.size()-1, 1, empty);
    }

    //удаление задваивания 1f, 2f
    qint32 size = recievedData.size();
    for(qint32 i = 0; i < size; i++)
    {
        if(recievedData[i] == recievedData[i+1] && recievedData[i] == 0x1f)
        {
            recievedData.replace(i, 1, empty);
            size--;
            i++;
        }
        else if(recievedData[i] == recievedData[i+1] && recievedData[i] == 0x2f)
        {
            recievedData.replace(i, 1, empty);
            size--;
            i++;
        }
    }


    size = recievedData.size();
    quint16 crc_recieved = (recievedData[size-1] << 8) | recievedData[size-2];
    recievedData.replace(size-2, 2, empty);
    quint16 crc = crc16(recievedData);

    if (crc == crc_recieved)
    {
        f_value val;
        float f;
        quint16 u16;
        quint16 u16_2;
        quint16 u16_3;
        quint8  u8;
        quint8  u8_2;
        quint8  u8_3;
        quint8  u8_4;

        float volt = 0.0f;
        float cur = 0.0f;
        float tilt_angle = 0.0f;
        float tilt_direction = 0.0f;
        float boost = 0.0f;
        float angular_velocity = 0.0f;
        float angleX = 0.0f;
        float angleY = 0.0f;
        float angleZ = 0.0f;

        quint16 k = recievedData[0];
        QByteArray Data = recievedData;
        recievedData.replace(0, recievedData.size(), empty);
        Data.replace(0, 1, empty);

        switch (k) {
        case 0x01:
            emit logServis("<- ошибка CRC", " ");
            if(mainModel_->getAdminTapCount() == -1)    emit logJoy("<- ошибка CRC", " ");
            break;
        case 0x04: //не правильная команда загрузчика
            mainModel_->on_pbStop_clicked("Ошибка. Не правильная команда загрузчика. ");
            break;
        case 0x05:
            emit logServis("<- не верная команда", " ");
            if(mainModel_->getAdminTapCount() == -1)   emit logJoy("<- не верная команда", " ");
            break;
        case 0x06:
            emit logServis("<- не верные парам. команды", " ");
            if(mainModel_->getAdminTapCount() == -1)    emit logJoy("<- не верные парам. команды", " ");
            break;
        case 0x07: //не правильныая очередность пакетов
            mainModel_->on_pbStop_clicked("Ошибка. Не правильныая очередность пакетов. ");
            break;
        case 0x08: //ошибка загрузки прошивки
            mainModel_->on_pbStop_clicked("Ошибка загрузки прошивки. ");
            break;
        case 0x10:
            emit logServis("<- ошибка гироскопа", " ");
            emit logJoy("<- ошибка гироскопа", " ");
            break;
        case 0x20:
            mainModel_->on_pbStop_clicked("Ошибка стирания flash. ");
            emit logServis("<- ошибка стирания flash", " ");
            emit logJoy("<- ошибка стирания flash", " ");
            break;
        case 0x21:
            mainModel_->on_pbStop_clicked("Ошибка записи во flash. ");
            emit logServis("<- ошибка записи во flash", " ");
            emit logJoy("<- ошибка записи во flash", " ");
            break;
        case 0x30:
            emit logServis("<- низкое напряжение", " ");
            emit logJoy("<- низкое напряжение", " ");
            break;
        case 0x31:
            emit logServis("<- высокое напряжение", " ");
            emit logJoy("<- высокое напряжение", " ");
            break;
        case 0x35:
            emit logServis("<- превышение доп. тока", " ");
            emit logJoy("<- превышение доп. тока", " ");
            break;
        case 0x38:
            emit logServis("<- отказ датчика тока", " ");
            emit logJoy("<- отказ датчика тока", " ");
            break;
        case 0x39:
            emit logServis("<- ошибка калибр. дат. тока", " ");
            emit logJoy("<- ошибка калибр. дат. тока", " ");
            break;
        case 0x40:
            emit logServis("<- проверка испр. серв", " ");
            emit logJoy("<- проверка испр. серв", " ");
            break;
        case 0x41:
            emit logServis("<- ошибка инициализации серв", " ");
            emit logJoy("<- ошибка инициализации серв", " ");
            break;
        case 0x42:
            u8 = Data[0];
            emit logServis("<- отказ сервы № ", QString(QString::number(u8)));
            emit logJoy("<- отказ сервы № ", QString(QString::number(u8)));
            break;
        case 0x43:
            u8 = Data[0];
            emit logServis("<- отказ драйвера серв № ", QString(QString::number(u8)));
            emit logJoy("<- отказ драйвера серв № ", QString(QString::number(u8)));
            break;
        case 0x45:
            emit logServis("<- ошибка автокалибр.", " ");
            emit logJoy("<- ошибка автокалибр.", "");
            break;
        case 0x48:
            emit logServis("<- автокалибровка завершена", " ");
            emit logJoy("<- автокалибровка завершена", " ");
            break;
        case 0x50:
            emit logServis("<- превышено время бездействия", " ");
            emit logJoy("<- превышено время бездействия", " ");
            break;
        case 0x55:
            emit logServis("<- ошибка математики", " ");
            if(mainModel_->getAdminTapCount() == -1)    emit logJoy("<- ошибка математики", " ");
            break;
        case 0xE0:
            emit logServis("<- сброс до заводских ок", " ");
            emit logJoy("<- сброс до заводских ок", " ");
            break;
        case 0xE1:
            emit logServis("<- установки сервы записаны", " ");
            break;
        case 0xE2:
            emit logServis("<- установки светодиода записаны", " ");
            break;
        case 0xE3:
            emit logServis("<- угол сервы установлен", " ");
            break;
        case 0xE4:
            u8 = Data[0];
            if(u8 == 0x00)
            {
                emit logServis("<- флаг блокировки тока выкл.", " ");
            }
            else if(u8 == 0x01)
            {
                emit logServis("<- флаг блокировки тока вкл.", " ");
            }

            break;
        case 0xE5:
            emit logServis("<- запрос ошибок ок", " ");
            emit logJoy("<- запрос ошибок ок", " ");
            break;
        case 0xE6:
            val.data[1] = Data[8];
            val.data[0] = Data[7];

            u8 = Data[0];
            u16 = Data[2] << 8 | Data[1];
            u16_2 = Data[4] << 8 | Data[3];
            u16_3 = Data[6] << 8 | Data[5];

            emit logServis("<- установки сервы №", QString(QString::number(u8)) + "\nmin " + QString(QString::number(u16))
                           + "   max " + QString(QString::number(u16_2)) + "   home " + QString(QString::number(u16_3))
                           + "   start " + QString(QString::number(val.int32)));

            break;
        case 0xE7:
            emit logServis("<- калибровка серв ок", " ");
            emit logJoy("<- калибровка серв ок", " ");
            break;
        case 0xE8:
            val.data[0] = Data[0];
            val.data[1] = Data[1];
            val.data[2] = Data[2];
            val.data[3] = Data[3];
            if(val.f == val.f)
            {
                emit logServis("<- калибровка тока ок ", QString(QString::number(val.f)));
                emit logJoy("<- калибровка тока ок ", QString(QString::number(val.f)));
            }
            break;
        case 0xE9:
             emit logServis("<- знач. калибр. тока записаны", " ");
            break;
        case 0xEA:
            val.data[0] = Data[0];
            val.data[1] = Data[1];
            val.data[2] = Data[2];
            val.data[3] = Data[3];
            if(val.f == val.f)
            {
                emit logServis("<- знач. калибр. тока ", QString(QString::number(val.f)));
            }
            break;
        case 0xEB:
            emit logServis("<- ошибки стерты", " ");
            break;
        case 0xEC:
        case 0xED:
            emit logServis("<- сервы установлены", " ");
            break;
        case 0xEE:
            emit logServis("<- Углы записаны", " ");
            emit logJoy("<- Углы записаны", " ");
            break;
        case 0xEF:
            emit logServis("<- Reset angl OK", " ");
            emit logJoy("<- Reset angl OK", " ");
            break;
        case 0xF0:
            emit logServis("<- перезагрузка", " ");
            emit logJoy("<- перезагрузка", " ");
            break;
        case 0xF1:
            mainModel_->setcoxaAngl(Data[2] << 8 | Data[1]);
            mainModel_->setfemurAngl(Data[4] << 8 | Data[3]);
            mainModel_->settibaAngl(Data[6] << 8 | Data[5]);
            emit logServis("<- Углы ноги № " + QString(QString::number(Data[0])), " прочитаны");
            emit logJoy("<- Углы ноги № " + QString(QString::number(Data[0])), " прочитаны");
            break;
        case 0xF2:
            //версия прошивки загрузчика
            val.data[3] = Data[0]; val.data[2] = Data[1]; val.data[1] = Data[2]; val.data[0] = Data[3];
            mainModel_->setVersBootLoaderExt(val.u32);

            if(mainModel_->getAdminTapCount() == -1)    emit logServis("<- Версия загрузчика:\n", mainModel_->versionToString(val.u32));
            if(mainModel_->getAdminTapCount() == -1)    emit logJoy("<- Версия загрузчика:\n", mainModel_->versionToString(val.u32));

            val.data[3] = Data[4]; val.data[2] = Data[5]; val.data[1] = Data[6]; val.data[0] = Data[7];
            mainModel_->setVersExt(val.u32);
            emit logServis("<- Версия прошивки:\n", mainModel_->versionToString(val.u32));
            emit logJoy("<- Версия прошивки:\n", mainModel_->versionToString(val.u32));

            break;
        case 0xF3:
            //номер успешно полученного пакета основной прошивки
            val.data[1] = Data[0]; val.data[0] = Data[1];
            mainModel_->setPageTx(val.int32);
            break;
        case 0xF4:
            //номер успешно полученного пакета загрузчика
            val.data[1] = Data[0]; val.data[0] = Data[1];
            mainModel_->setPageTx(val.int32);
            break;
        case 0xF5:
            emit logServis("<- Подпись ок","");
            break;
        case 0xA0:
            u8 = Data[0];
            u8_2 = Data[1];
            u8_3 = Data[2];
            u8_4 = Data[3];

            emit logServis("<- гамма светодиода № ", QString(QString::number(u8)) + "\nR "
                           + QString(QString::number(u8_2)) + "   G "
                           + QString(QString::number(u8_3)) + "   B "
                           + QString(QString::number(u8_4)));
            break;
        case 0xA1:
            val.data[0] = Data[0];
            val.data[1] = Data[1];
            val.data[2] = Data[2];
            val.data[3] = Data[3];
            if(val.f == val.f)  mainModel_->setVreal(val.f);
            if(setVreal)
            {
                emit logServis("<- напряжение ",  QString(QString::number(val.f)));
                setVreal = false;
            }
            break;
        case 0xA2:
            val.data[0] = Data[0];
            val.data[1] = Data[1];
            val.data[2] = Data[2];
            val.data[3] = Data[3];
            if(val.f == val.f)
            {
                mainModel_->setCurReal(val.f);
                if(setCurReal)
                {
                    emit logServis("<- ток ", QString(QString::number(val.f)));
                    setCurReal = false;
                }
            }
            break;
        case 0xA3:
            val.data[0] = Data[0];
            val.data[1] = Data[1];
            val.data[2] = Data[2];
            val.data[3] = Data[3];
            if(val.f == val.f)
            {
                f = val.f;
                val.data[0] = Data[4];
                val.data[1] = Data[5];
                val.data[2] = Data[6];
                val.data[3] = Data[7];

                if(val.f == val.f)
                {
                    emit logServis("<- азимут, наклон\n", QString(QString::number(f)) + ",   " + QString(QString::number(val.f)));
                }
            }
            break;
        case 0xA4:
            val.data[0] = Data[0];
            val.data[1] = Data[1];
            val.data[2] = Data[2];
            val.data[3] = Data[3];
            if(val.f == val.f)
            {
                f = val.f;
                val.data[0] = Data[4];
                val.data[1] = Data[5];
                val.data[2] = Data[6];
                val.data[3] = Data[7];

                if(val.f == val.f)
                {
                    emit logServis("<- ускорение, угловая скорость\n", QString(QString::number(f)) + ",   " + QString(QString::number(val.f)));
                }
            }
            break;
        case 0xA5:
            u8 = Data[0];
            u16 = Data[2] << 8 | Data[1];

            emit logServis("<- угол сервы № ", QString(QString::number(u8)) + "   " + QString(QString::number(u16)));
            break;

        case 0xA6:
            u8 = Data[0];
            if(u8 == 0x00)
            {
                emit logServis("<- сервы OFF", "");
            }
            else if(u8 == 0x01)
            {
                emit logServis("<- сервы ON", "");
            }

            break;
        case 0xA7:
            if(Data.size() > 10)
            {
                mainModel_->set_full_param(Data);

                QSettings setting;
                setting.setValue("fullParam", Data);

               emit logServis("<- точка восстановления создана", "");
            }
            else
            {
                emit logServis("<- отсутствуют входящие параметры", "");
            }
            break;
        case 0xA8:
           emit logServis("<- параметры восстановленны", "");
            break;
        case 0xA9:
            val.data[0] = Data[0];
            val.data[1] = Data[1];
            val.data[2] = Data[2];
            val.data[3] = Data[3];
            volt = val.f;

            val.data[0] = Data[4];
            val.data[1] = Data[5];
            val.data[2] = Data[6];
            val.data[3] = Data[7];
            cur = val.f;

            val.data[0] = Data[8];
            val.data[1] = Data[9];
            val.data[2] = Data[10];
            val.data[3] = Data[11];
            tilt_angle = val.f;

            val.data[0] = Data[12];
            val.data[1] = Data[13];
            val.data[2] = Data[14];
            val.data[3] = Data[15];
            tilt_direction = val.f;

            val.data[0] = Data[16];
            val.data[1] = Data[17];
            val.data[2] = Data[18];
            val.data[3] = Data[19];
            boost = val.f;

            val.data[0] = Data[20];
            val.data[1] = Data[21];
            val.data[2] = Data[22];
            val.data[3] = Data[23];
            angular_velocity = val.f;

            val.data[0] = Data[24];
            val.data[1] = Data[25];
            val.data[2] = Data[26];
            val.data[3] = Data[27];
            angleX = val.f;

            val.data[0] = Data[28];
            val.data[1] = Data[29];
            val.data[2] = Data[30];
            val.data[3] = Data[31];
            angleY = val.f;

            val.data[0] = Data[32];
            val.data[1] = Data[33];
            val.data[2] = Data[34];
            val.data[3] = Data[35];
            angleZ = val.f;

            emit chart_data(volt, cur, tilt_angle, tilt_direction, boost, angular_velocity, angleX, angleY, angleZ);

            break;

        default:
            //если не найдено совпадение - вывести как есть
       //     emit logServis("<-", recievedData.toHex());
            break;
        }
    }
    else
    {
    //     emit logServis("no crc<-", Dat.toHex());
    }
}


void Device::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
//    QBluetoothLocalDevice device;

    if (error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
    {
        if(rendering_flag)  setUpdate("Включите Bluetooth");
        if(rendering_flag)  searchFinished();
    }

    else if (error == QBluetoothDeviceDiscoveryAgent::UnsupportedPlatformError)
    {
        if(rendering_flag)  setUpdate("Bluetooth не поддерживается платформой.");
        if(rendering_flag)  searchFinished();
    }
    else if (error == QBluetoothDeviceDiscoveryAgent::InputOutputError)
    {
        if(rendering_flag)  setUpdate("Запись или считывание данных\n с устройства привело к ошибке.");
        if(rendering_flag)  searchFinished();
    }
    else if (error == QBluetoothDeviceDiscoveryAgent::LocationServiceTurnedOffError)
    {
        if(rendering_flag)  setUpdate("Включите геолокацию.");
        if(rendering_flag)  searchFinished();
    }
    else {
        static QMetaEnum qme = discoveryAgent->metaObject()->enumerator(
                    discoveryAgent->metaObject()->indexOfEnumerator("Error"));
        if(rendering_flag)  setUpdate("Error: " + QLatin1String(qme.valueToKey(error)));
        if(rendering_flag)  searchFinished();
    }

    m_deviceScanState = false;
    if(rendering_flag)  emit devicesUpdated();
    if(rendering_flag)  emit stateChanged();
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

bool Device::getloging()
{
    return loging;
}

void Device::setloging(bool newloging)
{
   loging = newloging;
}

bool Device::blt_on()
{
    QBluetoothLocalDevice device;
    QGeoCoordinate geo;

    if(!geo.isValid())
    {
         if(rendering_flag)  setUpdate("Геолокация не поддерживается платформой.\nПоиск bt устройств невозможен.");
 //       return false;
    }

    if(!device.isValid())
    {
        if(rendering_flag)  setUpdate("Bluetooth не поддерживается платформой.");
        return false;
    }


    if(device.hostMode() == QBluetoothLocalDevice::HostPoweredOff)// Bluetooth is not enabled
    {
        device.powerOn();// Call to open the local Bluetooth device
         if(rendering_flag)  setUpdate("Включение Bluetooth");
         if(rendering_flag)  emit searchInProgress();
        delay(1500);

        qint32 count = 0;
        while(count++ < 8)
        {
            if(device.hostMode() != QBluetoothLocalDevice::HostPoweredOff)      count = 8;
            delay(700);
        }
        delay(500);
    }
    return true;
}

void Device::get_last_device()
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
        return;
    }


    addDevice(QBluetoothDeviceInfo(QBluetoothAddress(dAdres), dName, dClass));
}


