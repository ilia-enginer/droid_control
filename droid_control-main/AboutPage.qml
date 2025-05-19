import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings


Dialog {
    id: aboutDialog
    modal: true
    focus: true
    title: "О программе..."
    x: (parent.width - width) / 2
    height: parent.height / 4

    Column {
        y: parent.height / 6
        width: Math.min(parent.width, parent.height) /3*2
        id: aboutColumn
        spacing: 20

        Label {
            width: aboutDialog.availableWidth
            text: "Droid Stick - предназначена для управления роботизированныйми устройствами по Bluetooch."
            wrapMode: Label.Wrap
            font.pixelSize: 12
        }
    }
}

