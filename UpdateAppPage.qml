import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material
import QtQuick.Dialogs


Dialog {
    id: updateAppWindow
    modal: true
    focus: true
    title: "Обновление приложения"
//    modality: Qt.ApplicationModal
    onRejected: Qt.callLater(updateAppWindow.open)


    Column {
        id: updateAppWindowColumn

        width: parent.width
        height: parent.height
        spacing: 10

        Label {
            id: updateLabel
            width: parent.width
            text: updateApp.updateText
            wrapMode: Label.Wrap
            font.pixelSize: 14
            font.family: "Courier New"
            color: "#0b8fdb"
         //   anchors.horizontalCenter: parent.horizontalCenter
            horizontalAlignment: Qt.AlignHCenter
        }


        BusyIndicator {
            id: busyIndicator
            Material.accent: Material.Light
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.2

            Component.onCompleted: {
                       // mainModel.onCheckUpdateProgress.connect(onCheckUpdateProgress)
                visible = false
                    }

//            function onCheckUpdateProgress(){
//                get_firmware_update.visible = false
//                update_load.visible = false
//                busyIndicator.visible = true
//                stop_load.visible = false
//            }
        }

        //процесс загрузки
        ProgressBar{
            id: load
            height: 20
            anchors.left: parent.left
            anchors.right: parent.right
            visible: false
            from: 0
            to: {updateApp.totalBytes_}
            value: {updateApp.bytesRead_}

            background: Rectangle {
                color: "#e6e6e6"
                radius: 3
            }

            Component.onCompleted: {
                        updateApp.onStartload.connect(onStartload)
                    }

            function onStartload(){
                load.visible = true
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

