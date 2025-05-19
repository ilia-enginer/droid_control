
import QtQuick
import QtQuick.Controls



Page {
    id: page


    Label {
        id: senderPageLabel
        width: parent.width
        wrapMode: Label.Wrap
        anchors.top: parent.top
    //    anchors.horizontalCenter: page.horizontalCenter
        horizontalAlignment: Qt.AlignHCenter
        text: mainModel.currentDeviceName
    }

    Rectangle {
        id: senderBackground
        anchors.top: senderPageLabel.bottom
        color: "#111111"
        width: parent.width
        height: 250
    }

    ScrollView {
        id: scrolViewLogArea
        anchors.fill: senderBackground
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ListView {
               id: listView1
               anchors.fill: scrolViewLogArea
               snapMode:ListView.SnapToItem
               clip: true

               Connections {
                   target: device
                    onLog: {
                       logListModel.append({msg: type + msg})
                       listView1.positionViewAtEnd()
                   }
               }

               delegate: Column {
                   Text {
                       width: listView1.width * 0.95
                       text: msg
                       font.family: "Courier New"
                       wrapMode: Text.Wrap
                       font.pixelSize: 14
                       color: "lawngreen"
                   }
                }

               // Сама модель, в которой будут содержаться все элементы
               model: ListModel {
                   id: logListModel // задаём ей id для обращения
               }
           }
    }



    TextField {
        id: messageToSend
        placeholderText: "..."
        anchors.top: senderBackground.bottom
        validator: RegularExpressionValidator {regularExpression: /[0-9A-Fa-f]+/}

        width: parent.width
    }

    Button {
        id: sendButton
        anchors.top: messageToSend.bottom
        anchors.left: parent.left
        height: parent.height * 0.05
        width: parent.width * 0.49
        text: "Отправить"
        background: Rectangle{
            property var normalColor: "#17d47f"
            property var pressedColor: "#0fd93b"
            color: sendButton.pressed ? pressedColor : normalColor
        }
        onClicked: {
            device.sendMessage(messageToSend.text);
            if (needClear.checked) {
                messageToSend.text = ""
            }

            listView1.positionViewAtEnd()
        }
    }

    Button {
        id: clearButton
        anchors.top: messageToSend.bottom
        anchors.right: parent.right
        height: parent.height * 0.05
        width: parent.width * 0.49
        text: "Очистить поле вывода"
        background: Rectangle{
            property var normalColor: "#cb0fd9"
            property var pressedColor: "#ff084e"
            color: clearButton.pressed ? pressedColor : normalColor
        }
        onClicked: {
            logListModel.clear()
        }
    }


    CheckBox {
      id: needWrap
      anchors.top: sendButton.bottom
      checked: device.needWrap
      text: qsTr("Оборачивать протоколом")

      onClicked: {
          device.needWrap = checked
      }
    }

    CheckBox {
      id: needClear
      checked: false
      anchors.top: needWrap.bottom
      text: qsTr("Очищать поле ввода")
    }

}
