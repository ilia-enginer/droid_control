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

#include <qbluetoothuuid.h>

#include "deviceinfo.h"

DeviceInfo::DeviceInfo(const QBluetoothDeviceInfo &d)
{
    device = d;
}


DeviceInfo::DeviceInfo(QString name, QString address) :
    testName_(name),
    testAddress_(address)
{

}

QString DeviceInfo::getAddress() const
{
    if (!device.isValid()) {
        return testAddress_;
    }

    return device.address().toString();
}

QString DeviceInfo::getName() const
{
    if (!device.isValid()) {
        return testName_;
    }

    return device.name();
}


QString
DeviceInfo::getCoreConfig() const
{
    if (!device.isValid()) {
        return "XX";
    }

    QStringList out;

    if (device.coreConfigurations() &  QBluetoothDeviceInfo::BaseRateCoreConfiguration) {
        out.append("BR");
    }

    if (device.coreConfigurations() &  QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        out.append("LE");
    }


    if (device.coreConfigurations() &  QBluetoothDeviceInfo::BaseRateAndLowEnergyCoreConfiguration) {
        out.append("BRLE");
    }

    if (out.isEmpty()){
        out.append("U");
    }

    return out.join("_");
}

QBluetoothDeviceInfo DeviceInfo::getDevice()
{
    return device;
}

void DeviceInfo::setDevice(const QBluetoothDeviceInfo &dev)
{
    device = QBluetoothDeviceInfo(dev);
    Q_EMIT deviceChanged();
}
