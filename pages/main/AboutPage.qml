import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material


Dialog {
    id: aboutDialog
    modal: true
    focus: true
    title: "О программе..."
    width: window.width * 0.8
    height: window.height * 0.4
    x: (window.width - aboutDialog.width) / 2
    y: (window.height - aboutDialog.height) / 5

    Column {
        width: parent.width
        height: parent.height
        spacing: 5

        Label {
            width: aboutDialog.availableWidth
            text: "Droid Stick - предназначена для управления роботизированныйми устройствами по Bluetooch. Приложение активируется после подключения к устройству."
            wrapMode: Label.Wrap
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
        }
    }
}

