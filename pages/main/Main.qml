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

import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material
import QtQuick.Window

import "." as App

import "../shar"
import "../pylt"


ApplicationWindow {
    id: window
    width: 360
    height: 520
    visible: true
    title: qsTr("Droid Stick v.%1").arg(appversion.version_app)
//    contentOrientation: Qt.LandscapeOrientation

    required property var builtInStyles
    property int active_device : 0      // для какого устройства сейчас развернута программа


    Component.onCompleted: {
        //запуск механизма проверки обновлений
        updateApp.checkForUpdates()
    }

    Shortcut {
        sequences: ["Esc", "Back"]
        enabled: stackView.depth > 1
        onActivated: navigateBackAction.trigger()
    }

    Shortcut {
        sequence: StandardKey.HelpContents
        onActivated: help()
    }

    Shortcut {
        sequence: "Menu"
        onActivated: optionsMenuAction.trigger()
    }

    //иконка меню
    Action {
        id: navigateBackAction
        icon.name: stackView.depth > 1 ? "back" : "drawer"
        enabled: false
        onTriggered: {

            if (stackView.depth > 1) {
                stackView.pop()
                listView.currentIndex = -1
                mainView.focus = true
                drawer.open()
                keyPage.visible = true
            } else { drawer.open(); keyPage.visible = true }
        }
        Component.onCompleted: {
            updateHexx.navigateBackActionOFF.connect(navigateBackActionOFF)
            updateHexx.navigateBackActionON.connect(navigateBackActionON)
        }
        function navigateBackActionOFF(){
            navigateBackAction.enabled = false;
        }
        function navigateBackActionON(){
            navigateBackAction.enabled = true;
        }
    }

    Action {
        id: optionsMenuAction
        icon.name: "menu"
        onTriggered: optionsMenu.open()
    }

    header: App.ToolBar {
       RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                action: navigateBackAction
            }

            Label {
                id: titleLabel
                text: listView.currentItem ? listView.currentItem.text : window.title
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                action: optionsMenuAction

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    Action {
                        text: "Поиск устройств"
                        onTriggered: devicesDialog.open()
                    }
                    Action {
                        text: "Настройки"
                        onTriggered: settingsDialog.open()
                    }
                    Action {
                        text: "Обновление"
                        onTriggered: updateDialog.open()
                    }
                    Action {
                        text: "О программе"
                        onTriggered: aboutDialog.open()
                    }
                }
            }
        }
    }

    Drawer {
        id: drawer
        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height
        interactive: stackView.depth === 1

        ListView {
            id: listView
            currentIndex: -1
            anchors.fill: parent

            delegate: ItemDelegate {
                width: listView.width
                text: model.title
                highlighted: ListView.isCurrentItem

                onClicked: {
                    listView.currentIndex = index
                    loadPage(index)
                    drawer.close()
                }
             }
            model: ListModel {
            }
            Component.onCompleted: {
                settParam.clearDevice.connect(clearDev)
            }
            function clearDev(){
                navigateBackAction.enabled = false;
                stackView.pop();
                listView.model.clear();
                listView.currentIndex = -1;
            }
        }       
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainView
    }

    function loadPage(page){
        keyPage.visible = false

        switch(settParam.getIdDevice()){
            case 1 :
                mainShar.loadPageShar(page);
            break;
            case 2 :
                mainPylt.loadPagePylt(page);
            break;

            default:
                break;
        }
    }

 ///////////////PAGES/////////////////

    /////////MAIN/////////
    MainView{
        id: mainView
    }
    AboutPage {
        id: aboutDialog
    }
    ApkSettings{
        id: settingsDialog
        visible: false
    }
    DeviceScanerPage {
       id: devicesDialog
    }
    Key{
        id: keyPage
        visible: true
    }
    SenderPage {
        id: terminalPage
        visible: false
    }
    SettingsPage{
        id: settingPage
        visible: false
    }
    Update_Hex {
        id: updateHex
        Component.onCompleted: { commun_display.onWindowloadHexOpen.connect(onWindowloadHexOpen) }
        function onWindowloadHexOpen(){ updateHex.open(); }
    }
    UpdateAppPage {
        id: updateAppWindow
        Component.onCompleted: { commun_display.onWindowloadOpen.connect(onWindowloadOpen) }
        function onWindowloadOpen(){ updateAppWindow.open(); }
    }
    UpdateDialogPage {
        id: updateDialog
    }
    Settings {
        id: settings
        property string style
    }
    DeviceSelection{
        id: deviceSelectionDialog
        visible: false
    }

    ////////SHAR/////////
    MainPageShar{
        id: mainShar
    }

    ////////PYLT/////////
    MainPagePylt{
        id: mainPylt
    }
}
