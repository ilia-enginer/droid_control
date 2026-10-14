import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material


Dialog {
    id: updateDialog
    modal: true
    focus: true
    x: (window.width - updateDialog.width) / 2
    y: (window.height - updateDialog.height) / 4
    width: window.width * 0.8
    height: window.height * 0.4


    Column {
        width: parent.width
        height: parent.height
        spacing: 10

        Label {
            horizontalAlignment: Qt.AlignHCenter
            width: parent.width
            text: "Обновление приложения"
            wrapMode: Label.Wrap
            font.pixelSize: 16
            font.family: "Courier New"

        }

        Label {
            horizontalAlignment: Qt.AlignHCenter
            width: parent.width
            text: " Наличие обновлений проверяется автоматически. Если по каким-либо причинам процесс обновления завершился с ошибкой, вы можете слелать это вручную.\nОткройте ссылку в браузере и нажмите 'Raw' или 'Download'. После скачивания, установочный файл будет в папке 'Загрузки'."
            wrapMode: Label.Wrap
            font.pixelSize: 14
            font.family: "Courier New"
            color: "#9a22c9"
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Courier New"
            wrapMode: Text.Wrap
            font.pixelSize: 14
            color: "#25820c"
            text: 'Ссылка <a href="https://github.com/ilia-enginer/droid_stick/blob/main/droid_stick.apk">github.com</a>'
            onLinkActivated: Qt.openUrlExternally(link)
        }

        RoundButton {
            id: butLoadUpdateApp
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height * 0.12
            width: parent.width * 0.9
            visible: mainModel.adminFlag
            radius: 8
            text: "Загрузить обновление"
            background: Rectangle{
                property var normalColor: "#9dcf0a"
                property var pressedColor: "#cfc10a"
                color: butLoadUpdateApp.pressed ? pressedColor : normalColor
                radius: 8
            }
            onClicked: { updateAppWindow.open() }
        }
    }
}
