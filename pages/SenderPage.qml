import QtQuick
import QtQuick.Controls



ScrollablePage {
    id: page

    anchors.fill: parent

    Column {
        spacing: 20
        width: parent.width

        Label {
            id: senderPageLabel
            width: parent.width
            wrapMode: Label.Wrap
            anchors.horizontalCenter: page.horizontalCenter
            horizontalAlignment: Qt.AlignHCenter
            text: mainModel.currentDeviceName
        }

        Rectangle {
            id: senderBackground
//            anchors.fill: parent
            anchors.top: senderPageLabel.bottom
            color: "#111111"

            width: page.width * 0.95
            height: 250
        }

        ScrollView {
            id: scrolViewLogArea
            anchors.fill: senderBackground
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.interactive: true

            ListView {
                   id: listView1
                   anchors.fill: senderBackground
                   snapMode:ListView.SnapToItem
                   width: senderBackground.width
                   clip: true

                   Connections {
                       target: device
                       onLog: {
                           logListModel.append({msg: type + ":" + msg})
                       }
                   }

                   delegate: Column {
                       Text {
                           text: msg
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
            anchors.top: scrolViewLogArea.bottom
            validator: RegularExpressionValidator {regularExpression: /[0-9A-Fa-f]+/}

            width: parent.width * 0.95
        }

        Button {
            id: sendButton
            anchors.top: messageToSend.bottom
            width: parent.width * 0.95
            text: "Отправить"

            onClicked: {
                device.sendMessage(messageToSend.text);
                if (needClear.checked) {
                    messageToSend.text = ""
                }

                listView1.positionViewAtEnd()
            }
        }

        CheckBox {
          id: needWrap
          anchors.top: sendButton.bottom
          text: qsTr("Оборачивать протоколом")
          checkState: device.needWrap

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
}
