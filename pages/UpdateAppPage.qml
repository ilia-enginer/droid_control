import QtQuick
import QtQuick.Layouts
import QtQuick.Controls  2.0
import Qt.labs.settings
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQml




Dialog {
    id: updateAppWindow
    modal: true
    focus: true
    title: "Обновление приложения"
    x: (window.width - updateAppWindow.width) / 2
    y: (window.height - updateAppWindow.height) / 4
    width: window.width * 0.8
    height: window.height * 0.5
    onRejected: Qt.callLater(updateAppWindow.open)


    Column {
        id: updateAppWindowColumn
        width: parent.width
        height: parent.height
        spacing: 10

        Label {
            id: updateLabel
            width: parent.width
            text: commun_display.updateAppText
            wrapMode: Label.Wrap
            font.pixelSize: 14
            font.family: "Courier New"
            color: "#0b8fdb"
            horizontalAlignment: Qt.AlignHCenter
        }

        Label {
            id: loadLabel
            width: parent.width
            text: commun_display.loadTextApp
            wrapMode: Label.Wrap
            font.pixelSize: 14
            font.family: "Courier New"
            color: "#0b8fdb"
            horizontalAlignment: Qt.AlignHCenter
            visible: false
        }


        BusyIndicator {
            id: busyIndicator
            Material.accent: Material.Light
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.2

            Component.onCompleted: {
                        commun_display.onBusyIndicatorON.connect(onBusyIndicatorON)
                        commun_display.onBusyIndicatorOFF.connect(onBusyIndicatorOFF)

                        visible = false
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
                radius: 3
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
                    radius: 2
                    color: "#17a81a"

                    gradient: Gradient {
                        GradientStop {
                            position: 0.0
                            SequentialAnimation on color {
                                loops: Animation.Infinite
                                ColorAnimation { from: "#d6e014"; to: "#b39812"; duration: 5000 }
                                ColorAnimation { from: "#b39812"; to: "#d6e014"; duration: 5000 }
                            }
                        }
                        GradientStop {
                            position: 1.0
                            SequentialAnimation on color {
                                loops: Animation.Infinite
                                ColorAnimation { from: "#ed4811"; to: "#a1330e"; duration: 5000 }
                                ColorAnimation { from: "#a1330e"; to: "#ed4811"; duration: 5000 }
                            }
                        }
                    }
                }
            }
        }
    }


    Button {
        id: yes
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        height: parent.height * 0.25
        width: parent.width * 0.4
        text: "Да"
        background: Rectangle{
            property var normalColor: "#07e67a"
            property var pressedColor: "#0b6b3d"
            color: yes.pressed ? pressedColor : normalColor
        }

        onClicked: {
            yes.visible = false
            busyIndicator.visible = true;
            updateApp.downloadFile();
        }
    }

    Button {
        id: ok
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        height: parent.height * 0.25
        width: parent.width * 0.4
        text: "Да"
        background: Rectangle{
            property var normalColor: "#07e67a"
            property var pressedColor: "#0b6b3d"
            color: ok.pressed ? pressedColor : normalColor
        }
        Component.onCompleted: {
                    commun_display.onBut_Ok_On.connect(onBut_Ok_On)
                    visible = false
                }

        function onBut_Ok_On(){
                ok.visible = true
        }
        onClicked: {
            updateApp.install();
        }
    }


    Button {
        id: no
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        height: parent.height * 0.25
        width: parent.width * 0.4
        text: "Отмена"
        background: Rectangle{
            property var normalColor: "#ad4d1d"
            property var pressedColor: "#ff084e"
            color: no.pressed ? pressedColor : normalColor
        }

        onClicked: {
            updateApp.on_CancelDownload();
            updateAppWindow.close()
        }
    }
}

