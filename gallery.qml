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

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings

import "." as App


ApplicationWindow {
    id: window
    width: 360
    height: 520
    visible: true
    title: qsTr("Droid Stick v.%1").arg(mainModel.version_app)

    required property var builtInStyles

    Settings {
        id: settings
        property string style
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


    Action {
        id: navigateBackAction
//        icon.name: stackView.depth > 1 ? "back" : "drawer"
        icon.name: "drawer"
        onTriggered: {
            if (stackView.depth > 1) {
                stackView.pop()
                listView.currentIndex = -1

                drawer.open()
            } else {
                drawer.open()
            }
        }
    }

    Shortcut {
        sequence: "Menu"
        onActivated: optionsMenuAction.trigger()
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
                        text: "О программе ..."
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

            focus: true
            currentIndex: -1
            anchors.fill: parent

            delegate: ItemDelegate {
                width: listView.width
                text: model.title
                highlighted: ListView.isCurrentItem
                onClicked: {
                    listView.currentIndex = index
                    stackView.push(model.source)
                    drawer.close()
                }
             }

                model: ListModel {    
                    ListElement { title: "Джойстик"; source: "qrc:/pages/joystick/Joystick.qml" }
                    ListElement { title: "Окно настроек\nи калибровок";  source: "qrc:/pages/SettingFootPage.qml" }
                    ListElement { title: "Обновление прошивки"; source: "qrc:/pages/firmware_update.qml" }

             //       ListElement { title: "Терминал"; source: "qrc:/pages/SenderPage.qml" }
             //       ListElement { title: "Сервис"; source: "qrc:/pages/ServicePage.qml" }
             //       ListElement { title: "Настройки"; source: "qrc:/pages/SettingsPage.qml" }
                }

                //включение режима админа
                Button {
                    id: admin
                    width: Math.min(window.width, window.height) / 3 * 2
                    height: 100
                    anchors.bottom: listView.bottom
                    opacity: 0.0
                    enabled: true

                    onClicked: {
                        mainModel.adiminTapCount = 0;
                    }
                }
            }

            Component.onCompleted: {
                mainModel.onAdminTapCountChanged.connect(adminModeChanged)
            }

            function adminModeChanged(){
                if (mainModel.adiminTapCount === 4) {
                    listView.model.append({ title: "Терминал", source: "qrc:/pages/SenderPage.qml" })
                    listView.model.append({ title: "Настройки", source: "qrc:/pages/SettingsPage.qml" })
                }
                else if (mainModel.adiminTapCount === 2) {
                    listView.model.append({ title: "Сервис", source: "qrc:/pages/ServicePage.qml" })
                }
            }

//            ScrollIndicator.vertical: ScrollIndicator { }
        }

    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Pane {
            id: pane

            Image {
                id: logo
                width: pane.availableWidth / 2
                height: pane.availableHeight / 2
                anchors.centerIn: parent
                anchors.verticalCenterOffset: -50
                fillMode: Image.PreserveAspectFit
                source: "images/play_store_512.png"
            }
        }
    }

    Dialog {
        id: settingsDialog
        x: Math.round((window.width - width) / 2)
        y: Math.round(window.height / 6)
        width: Math.round(Math.min(window.width, window.height) / 3 * 2)
        modal: true
        focus: true
        title: "Settings"

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            settings.style = styleBox.displayText
            settingsDialog.close()
        }
        onRejected: {
            styleBox.currentIndex = styleBox.styleIndex
            settingsDialog.close()
        }

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            RowLayout {
                spacing: 10

                Label {
                    text: "Style:"
                }

                ComboBox {
                    id: styleBox
                    property int styleIndex: -1
                    model: window.builtInStyles
                    Component.onCompleted: {
                        styleIndex = find(settings.style, Qt.MatchFixedString)
                        if (styleIndex !== -1)
                            currentIndex = styleIndex
                    }
                    Layout.fillWidth: true
                }
            }

            Label {
                text: "Необходима перезагрузка"
                color: "#e41e25"
                opacity: styleBox.currentIndex !== styleBox.styleIndex ? 1.0 : 0.0
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    AboutPage {
        id: aboutDialog
        modal: true
        focus: true
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
    //    contentHeight: aboutColumn.height
    }

    DeviceScanerPage {
        id: devicesDialog
    }

    //кнопка теста
    Button {
        id: test_but
        width: window.width * 0.8
        height: window.height * 0.07
        visible: true
        text: "тест обновления"

        onClicked: {
            ///????
            updateAppWindow.open();///
            updateApp.checkForUpdates(1);
        }
    }

    UpdateAppPage {
        id: updateAppWindow
        modal: true
        focus: true
        x: (window.width - width) / 2
        y: (window.height - height) / 2
        width: window.width * 0.8
        height: window.height * 0.7

        Component.onCompleted: {
                 updateApp.onWindowloadOpen.connect(onWindowloadOpen)
             }

        function onWindowloadOpen(){
            updateAppWindow.open();
        }
    }

    Update_Hex {
        id: updateHex
        modal: true
        focus: true
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height * 0.3

        Component.onCompleted: {
                 updateApp.onWindowloadHexOpen.connect(onWindowloadHexOpen)
             }

        function onWindowloadHexOpen(){
            updateHex.open();
        }
    }
}
