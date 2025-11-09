import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings


Dialog {

    id: updateHex
    modal: true
    focus: true
    title: "Доступно обновление прошивки шара."
    x: (window.width - updateHex.width) / 2
    y: (window.height - updateHex.height) / 4
    height: parent.height / 4
    width: Math.min(window.width, window.height) / 3 * 2

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
