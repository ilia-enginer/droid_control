import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings


Dialog {

    id: updateHex
    modal: true
    focus: true
    title: "Доступно обновление прошивки шара."
    x: (parent.width - width) / 2
    height: parent.height / 4


    Column {
        y: parent.height / 6
        width: Math.min(parent.width, parent.height) /3*2
        id: aboutColumn
        spacing: 20

        Label {
            width: updateHexx.availableWidth
            text: "Можете установить, перейдя во вкладку 'Обновление прошивки'."
            wrapMode: Label.Wrap
            font.pixelSize: 14
            font.family: "Courier New"
            color: "#ad4d1d"
        }
    }
}
