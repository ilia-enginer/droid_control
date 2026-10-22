import QtQuick
import QtQuick.Layouts
import QtQuick.Controls 2.0
import Qt.labs.settings
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQml


Dialog {
    id: updateAppWindow
    modal: true
    focus: true
    title: "Обновление приложения"

    // Поверх всего окна
    parent: Overlay.overlay
    z: 1000
    // Не закрывать по клику вне окна; Esc — можно
    closePolicy: Popup.CloseOnEscape

    x: (window.width - updateAppWindow.width) / 2
    y: (window.height - updateAppWindow.height) / 4
    width: window.width * 0.8
    height: window.height * 0.5
    onRejected: Qt.callLater(updateAppWindow.open)

    // ---- Палитра ----
    readonly property color cCard:      "#ffffff"
    readonly property color cBorder:    "#e0e6ed"
    readonly property color cText:      "#263238"
    readonly property color cTextSoft:  "#546e7a"
    readonly property color cAccent:    "#5b7cfa"
    readonly property color cAccentD:   "#3f5fd6"
    readonly property color cMint:      "#66bb6a"
    readonly property color cMintD:     "#43a047"
    readonly property color cCoral:     "#ff7a7a"
    readonly property color cCoralD:    "#e05555"

    background: Rectangle {
        radius: 12
        color: cCard
        border.width: 1
        border.color: cBorder

        Rectangle {
            anchors.fill: parent
            anchors.margins: -2
            radius: parent.radius
            color: "#000000"
            opacity: 0.06
            z: -1
        }
    }

    Column {
        id: updateAppWindowColumn
        width: parent.width
        height: parent.height
        spacing: 10
        padding: 14

        Label {
            id: updateLabel
            width: parent.width
            text: commun_display.updateAppText
            wrapMode: Label.Wrap
            font.pixelSize: 14
            font.family: "Courier New"
            color: cText
            horizontalAlignment: Qt.AlignHCenter
            padding: 4
        }

        Label {
            id: loadLabel
            width: parent.width
            text: commun_display.loadTextApp
            wrapMode: Label.Wrap
            font.pixelSize: 14
            font.family: "Courier New"
            color: cTextSoft
            horizontalAlignment: Qt.AlignHCenter
            visible: false
            padding: 2
        }

        BusyIndicator {
            id: busyIndicator
            Material.accent: cAccent
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.2
            visible: false

            Component.onCompleted: {
                commun_display.onBusyIndicatorON.connect(onBusyIndicatorON)
                commun_display.onBusyIndicatorOFF.connect(onBusyIndicatorOFF)
            }

            function onBusyIndicatorON(){
                busyIndicator.visible = true
            }
            function onBusyIndicatorOFF(){
                busyIndicator.visible = false
            }
        }

        //процесс загрузки
        ProgressBar{
            id: load
            height: parent.height * 0.043
            anchors.left: parent.left
            anchors.right: parent.right
            visible: false
            from: 0.0
            to: {commun_display.TotalBytes}
            value: {commun_display.BytesRead}

            background: Rectangle {
                color: "#e6e6e6"
                radius: 6
                border.width: 1
                border.color: cBorder
            }

            Component.onCompleted: {
                commun_display.onStartload.connect(onStartload)
                commun_display.onStatusLoadOFF.connect(onStatusLoadOFF)
            }

            function onStartload(){
                load.visible = true
                loadLabel.visible = true
            }
            function onStatusLoadOFF(){
                load.visible = false
                loadLabel.visible = false
            }

            contentItem: Item {
                Rectangle {
                    width: load.visualPosition * parent.width
                    height: parent.height
                    radius: 6
                    color: "#17a81a"

                    gradient: Gradient {
                        GradientStop {
                            position: 0.0
                            SequentialAnimation on color {
                                loops: Animation.Infinite
                                ColorAnimation { from: "#11ed61"; to: "#0ba142"; duration: 5000 }
                                ColorAnimation { from: "#0ba142"; to: "#11ed61"; duration: 5000 }
                            }
                        }
                        GradientStop {
                            position: 1.0
                            SequentialAnimation on color {
                                loops: Animation.Infinite
                                ColorAnimation { from: "#14aaff"; to: "#437284"; duration: 5000 }
                                ColorAnimation { from: "#437284"; to: "#14aaff"; duration: 5000 }
                            }
                        }
                    }
                }
            }
        }
    }

    // ---- Кнопки ----
    RoundButton {
        id: yes
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: 10
        anchors.leftMargin: 10
        height: parent.height * 0.18
        width: parent.width * 0.42
        radius: 10
        text: "Да"

        background: Rectangle {
            radius: 10
            color: yes.pressed ? cMintD : (yes.hovered ? "#81c784" : cMint)
            border.width: 1
            border.color: cMintD
            Behavior on color { ColorAnimation { duration: 120 } }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 3
                radius: parent.radius
                color: cMintD
                opacity: yes.pressed ? 0.0 : 0.55
            }
        }

        contentItem: Text {
            text: yes.text
            color: "#ffffff"
            font.pixelSize: 14
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Component.onCompleted: { commun_display.onBut_Yes_On.connect(onBut_Yes_On) }
        function onBut_Yes_On(){ yes.visible = true }
        onClicked: {
            yes.visible = false
            busyIndicator.visible = true
            updateApp.downloadFile()
        }
    }

    RoundButton {
        id: ok
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: 10
        anchors.leftMargin: 10
        height: parent.height * 0.18
        width: parent.width * 0.42
        visible: false
        radius: 10
        text: "ОК"

        background: Rectangle {
            radius: 10
            color: ok.pressed ? cAccentD : (ok.hovered ? "#7c96ff" : cAccent)
            border.width: 1
            border.color: cAccentD
            Behavior on color { ColorAnimation { duration: 120 } }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 3
                radius: parent.radius
                color: cAccentD
                opacity: ok.pressed ? 0.0 : 0.55
            }
        }

        contentItem: Text {
            text: ok.text
            color: "#ffffff"
            font.pixelSize: 14
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Component.onCompleted: { commun_display.onBut_Ok_On.connect(onBut_Ok_On) }
        function onBut_Ok_On(){ ok.visible = true }
        onClicked: { updateApp.install() }
    }

    RoundButton {
        id: no
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: 10
        anchors.rightMargin: 10
        height: parent.height * 0.18
        width: parent.width * 0.42
        radius: 10
        text: "Отмена"

        background: Rectangle {
            radius: 10
            color: no.pressed ? cCoralD : (no.hovered ? "#ff9a9a" : cCoral)
            border.width: 1
            border.color: cCoralD
            Behavior on color { ColorAnimation { duration: 120 } }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 3
                radius: parent.radius
                color: cCoralD
                opacity: no.pressed ? 0.0 : 0.55
            }
        }

        contentItem: Text {
            text: no.text
            color: "#ffffff"
            font.pixelSize: 14
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: {
            updateApp.on_CancelDownload("")
            yes.visible = true
            updateAppWindow.close()
        }
    }
}
