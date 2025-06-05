/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
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

#ifndef DEVICE_H
#define DEVICE_H

#include <qbluetoothlocaldevice.h>
#include <QObject>
#include <QVariant>
#include <QList>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include <QBluetoothAddress>
#include <QBluetoothUuid>

#include "mainmodel.h"
#include "deviceinfo.h"
//#include "serviceinfo.h"
//#include "characteristicinfo.h"



QT_FORWARD_DECLARE_CLASS (QBluetoothDeviceInfo)
QT_FORWARD_DECLARE_CLASS (QBluetoothServiceInfo)

void delay( int millisecondsToWait );


class Device: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant devicesList READ getDevices NOTIFY devicesUpdated)
    Q_PROPERTY(QVariant servicesList READ getServices NOTIFY servicesUpdated)
    Q_PROPERTY(QVariant characteristicList READ getCharacteristics NOTIFY characteristicsUpdated)
    Q_PROPERTY(QString update READ getUpdate WRITE setUpdate NOTIFY updateChanged)
    Q_PROPERTY(bool useRandomAddress READ isRandomAddress WRITE setRandomAddress NOTIFY randomAddressChanged)
    Q_PROPERTY(bool state READ state NOTIFY stateChanged)
    Q_PROPERTY(bool controllerError READ hasControllerError)
    Q_PROPERTY(bool needWrap READ getNeedWrap WRITE setNeedWrap)
    Q_PROPERTY(bool loging READ getloging WRITE setloging)


public:
    Device(MainModel *mainModel);
    ~Device();
    QVariant getDevices();
    QVariant getServices();
    QVariant getCharacteristics();
    QString getUpdate();
    bool state();
    bool hasControllerError() const;

    bool isRandomAddress() const;
    void setRandomAddress(bool newValue);

    bool getNeedWrap();
    void setNeedWrap(bool value);

    bool getloging();
    void setloging(bool newloging);

    bool blt_on(void);

public slots:
    void startDeviceDiscovery();
    void connectToDevice(const QString &dAddress, const QString &name, const QString &config);
    void disconnectFromDevice();
    void sendUpdate(QByteArray msg);
    void sendMessage (QString msg);
    void sendMessageAndWrap (QString msg);
    void sendMessageAndWrapS (QString msg);
    void sendMessageAndWrap (quint8 code, QString msg);
    void sendparstxlog (quint8 code, quint8 cod);

    void onJoysticActivity(quint8 mode, float azimut, float amplitude, float level, bool ctrl);
    void get_check(void);

    void onGetCurReal(void);
    uint_least32_t crc32(const QByteArray &buf, qint32 format_buf);

    QString getLastConnectedDevice(void);

    void set_rendering_flag(bool fl);

    void get_last_device(void);


private slots:
    // QBluetoothDeviceDiscoveryAgent related
    void addDevice(const QBluetoothDeviceInfo&);
    void deviceScanFinished();
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error);

    // QLowEnergyController realted
    void deviceConnected();
    void errorReceived(QLowEnergyController::Error);
    void deviceDisconnected();
    void socketRead();

Q_SIGNALS:
    void devicesUpdated();
    void servicesUpdated();
    void characteristicsUpdated();
    void updateChanged(QString msg);
    void stateChanged();
    void disconnected();
    void randomAddressChanged();
    void socketDataRecieved(QString msg);
    void log(QString type, QString msg);
    void logServis(QString type, QString msg);
    void logJoy(QString type, QString msg);

    void searchInProgress();
    void searchFinished();

    void chart_data(float volt_, float cur_, float tilt_angle_, float tilt_direction_, float boost_, float angular_velocity_, float angleX_, float angleY_, float angleZ_);


private:
    void setUpdate(const QString &message);
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    DeviceInfo currentDevice;
    QList<QObject *> devices;
    QList<QObject *> m_services;
    QList<QObject *> m_characteristics;
    QString m_previousAddress;
    QString m_message;
    bool connected = false;
    QLowEnergyController *controller = nullptr;
    bool m_deviceScanState = false;
    bool randomAddress = false;

    QBluetoothSocket *socket = nullptr;
    MainModel *mainModel_ = nullptr;

    bool needWrap_ = false;
    QString lastConnectedDevice_;
    QString nameDevice_;
    QString class_;


    QByteArray wrapData(QByteArray input);

    quint16 crc16(const QByteArray &arr);


    bool setVreal = false;
    bool setCurReal = false;

    quint8 max_rx_size = 255;

    QByteArray Temp = 0;
    quint8 startByteIndex = 0;
    bool statys = false;
    bool split = false;
    bool loging = false;                //флаг логирования
    bool rendering_flag = false;        //флаг выводы сообщений на экран

};

#endif // DEVICE_H
