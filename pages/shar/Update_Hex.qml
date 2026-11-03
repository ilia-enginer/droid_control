import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings


Dialog {
    id: updateHex
    modal: true
    focus: true
    x: (window.width - updateHex.width) / 2
    y: (window.height - updateHex.height) / 4
    height: parent.height / 4
    width: window.width * 0.8
    header: Label{
        text: "Доступно обновление прошивки шара.\nОб обновлении: повышена стабильность работы."
        wrapMode: Label.Wrap
        padding: 16
        font.bold: true
        font.pixelSize: 14
        background: Rectangle{ color: "#f0f0f0" }
    }

    Column {
        id: aboutColumn
        width: parent.width
        height: parent.height
        spacing: 20

        Label {
            width: parent.width
            text: "Можете установить, перейдя во вкладку 'Обновление прошивки'."
            wrapMode: Label.Wrap
            font.pixelSize: 14
            font.family: "Courier New"
            color: "#ad4d1d"
        }
    }
}
