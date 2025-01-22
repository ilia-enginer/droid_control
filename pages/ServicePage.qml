import QtQuick
import QtQuick.Controls

ScrollablePage {
    id: page

    Column {
        spacing: 20
        width: parent.width

        Button {
//            id: sendButton
//            anchors.top: messageToSend.bottom
            width: parent.width * 0.95
            text: "Сбросить настройки"

            onClicked: {
                device.sendMessageAndWrap("e0");
            }
        }
    }
}
