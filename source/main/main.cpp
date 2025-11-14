/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
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

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QQuickStyle>
#include <QIcon>
#include "source/device/device.h"
#include "source/main/mainmodel.h"
#include "source/main/appmanager.h"
#include "source/update/updateapp.h"
#include "source/update/appversion.h"
#include "source/communication/crc.h"
#include "source/communication/tx/packing.h"
#include "source/communication/rx/unpacking.h"
#include "source/communication/rx/rx_commands.h"
#include "source/display_working/commun_display.h"
#include "source/communication/tx/tx_commands.h"
#include "source/display_working/commun_display.h"
#include "source/main/settings.h"
#include "source/update/updatehex.h"
#include "source/update/appversion.h"
#include "source/main/feedback.h"
#include "source/info/info.h"



int main(int argc, char *argv[])
{
    QApplication::setApplicationName("Gallery");
    QApplication::setOrganizationName("QtProject");
    QApplication app(argc, argv);

    QIcon::setThemeName("gallery");

    QSettings qsettings;

    // If this is the first time we're running the application,
    // we need to set a style in the settings so that the QML
    // can find it in the list of built-in styles.
    ///////////////////qsettings.value("style").toString();
    /// //////////////
    const QString styleInSettings = qsettings.value("style").toString();
    if (styleInSettings.isEmpty()){
        qsettings.setValue(QLatin1String("style"), "Imagine");
    }

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")){
        QQuickStyle::setStyle(qsettings.value("style").toString());
    }

    QQmlApplicationEngine engine;

    QStringList builtInStyles = { QLatin1String("Fusion"), QLatin1String("Imagine"),
                                QLatin1String("Material"), QLatin1String("Universal")};
#if defined(Q_OS_MACOS)
    builtInStyles << QLatin1String("macOS");
//    builtInStyles << QLatin1String("Imagine");
#elif defined(Q_OS_WINDOWS)
    builtInStyles << QLatin1String("Windows");
#endif

    AppManager appManager;
    Commun_display commun_display;
    MainModel model;
    Device d(&model);
    UpdateApp updateApp;
    Crc crc;
    Packing packing;
    Unpacking unpacking;
    Tx_commands tx_commands;
    Rx_commands rx_commands;
    Settings settings;
    UpdateHex updateHex;
    AppVersion appversion;
    Feedback feedback;
    Info info;



    engine.rootContext()->setContextProperty("device", &d);
    engine.rootContext()->setContextProperty("mainModel", &model);
    engine.rootContext()->setContextProperty("updateApp", &updateApp);
    engine.rootContext()->setContextProperty("packing", &packing);
    engine.rootContext()->setContextProperty("commun_display", &commun_display);
    engine.rootContext()->setContextProperty("tx_commands", &tx_commands);
    engine.rootContext()->setContextProperty("settParam", &settings);
    engine.rootContext()->setContextProperty("updateHexx", &updateHex);
    engine.rootContext()->setContextProperty("appversion", &appversion);
    engine.rootContext()->setContextProperty("rx_commands", &rx_commands);
    engine.rootContext()->setContextProperty("feedback", &feedback);
    engine.rootContext()->setContextProperty("info", &info);

    packing.setCrc(&crc);
    packing.setCommun_display(&commun_display);

    unpacking.setCrc(&crc);
    unpacking.setCommun_display(&commun_display);
    unpacking.setRx_commands(&rx_commands);
    unpacking.setSettings(&settings);

    tx_commands.setPacking(&packing);
    tx_commands.setCommun_display(&commun_display);
    tx_commands.setSettings(&settings);

    rx_commands.setCommun_display(&commun_display);
    rx_commands.setUpdateHex(&updateHex);
    rx_commands.setSettings(&settings);

    settings.setCommun_display(&commun_display);

    d.setCommun_display(&commun_display);
    d.setPacking(&packing);
    d.setUnpacking(&unpacking);

    updateHex.setTx_commands(&tx_commands);
    updateHex.setCrc(&crc);
    updateHex.setCommun_display(&commun_display);
    updateHex.setSettings(&settings);

    model.setDevice(&d);
    model.setUpdateHex(&updateHex);
    model.setCommun_display(&commun_display);
    model.setRx_commands(&rx_commands);
    model.setSettings(&settings);
    model.setTx_commands(&tx_commands);

    appManager.setCommun_display(&commun_display);

    updateApp.setAppManager(&appManager);
    updateApp.setCommun_display(&commun_display);

    QObject::connect(&app, &QApplication::applicationStateChanged, &appManager, &AppManager::onApplicationStateChanged);

    //вывод версии в сообщении для изменения txt файла
    #if defined(Q_OS_WINDOWS)    
        AppVersion *AppVer = new AppVersion();
        qint32 ver = AppVer->getAppVersion();

        QMessageBox msgBox;
        msgBox.setWindowModality(Qt::ApplicationModal);
        msgBox.setWindowTitle("Версия приложения.");
        msgBox.setIcon(QMessageBox::Warning);
        ver = AppVer->getAppVersion();
        msgBox.setText(QString("%1\n\n%2.%3.%4.%5.%6.%7")
                       .arg(ver)                                         //версия полным числом
                       .arg(ver >> 28, 1, 10)                            // версия
                       .arg((ver >> 20) & 0x0FF, 2, 10, QChar('0'))      // год
                       .arg((ver >> 16) & 0x0F, 2, 10, QChar('0'))       // месяц
                       .arg((ver >> 11) & 0x1F, 2, 10, QChar('0'))       // день
                       .arg((ver >> 6) & 0x01F, 2, 10, QChar('0'))       // час
                       .arg(ver & 0x3F, 2, 10, QChar('0')));             // минуты

        msgBox.setDetailedText(QString("%1").arg(ver));                   //версия полным числом для копирования
        msgBox.exec();
    #endif

    engine.setInitialProperties({{ "builtInStyles", builtInStyles }});
    engine.load(QUrl("qrc:/pages/main.qml"));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    return app.exec();
}
