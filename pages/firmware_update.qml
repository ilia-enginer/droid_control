import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.settings
import QtQuick.Controls.Material



Page {
    id: page

    Label {
        id: senderPageLabel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        width: parent.width
        wrapMode: Label.Wrap
        horizontalAlignment: Qt.AlignHCenter
        text: commun_display.curDeviceName_
        font.pointSize: parent.height * 0.022
    }

    Label {
        id: senderPageLabel1
        anchors.top: senderPageLabel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        width: parent.width
        wrapMode: Label.Wrap
        horizontalAlignment: Qt.AlignHCenter
        text: "Во время обновления не блокируйте телефон и не сворачивайте приложение"
        font.pointSize: parent.height * 0.027
        color: "red" 
    }

    Label {
        id: senderPageLabel2
        anchors.top: senderPageLabel1.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        width: parent.width
        wrapMode: Label.Wrap
        horizontalAlignment: Qt.AlignHCenter
        text: "Проверьте обновление"
        font.pointSize: parent.height * 0.025
        Component.onCompleted: {
                    commun_display.onCurrenUpdateChanged.connect(onCurrenUpdateChanged)
                }
        function onCurrenUpdateChanged(msg){
            text = msg
        }

    }

    Button {
        id: get_firmware_update
        width: parent.width * 0.8
        height: parent.height * 0.07
        anchors.top: senderPageLabel2.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        visible: true
        text: "Проверить обновление"

        background: Rectangle{
            property var normalColor: "#d0d413"
            property var pressedColor: "#3c965a"
            color: get_firmware_update.pressed ? pressedColor : normalColor
        }
        Component.onCompleted: {
                    commun_display.onCheckUpdate.connect(onCheckUpdate)
                }
        function onCheckUpdate(){
            busyIndicator.visible = false
            update_load.visible = false
            update_bootloader_load.visible = false
            stop_load.visible = false
            get_firmware_update.visible = true
        }
        onClicked: {
              updateHexx.checkingUpdates();
        }
    }

    Button {
        id: update_load
        width: parent.width * 0.8
        height: parent.height * 0.07
        anchors.top: senderPageLabel2.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
        text: "Загрузить обновление"

        background: Rectangle{
            property var normalColor: "#c97424"
            property var pressedColor: "#88c918"
            color: update_load.pressed ? pressedColor : normalColor
        }

        Component.onCompleted: {
                    commun_display.onUpdateAvailable.connect(onUpdateAvailable)
                }
        function onUpdateAvailable(){
            get_firmware_update.visible = false
            busyIndicator.visible = false
            update_load.visible = true
            stop_load.visible = false
        }
        onClicked: {
            update_load.visible = false
            update_bootloader_load.visible = false
            busyIndicator.visible = true
            updateHexx.on_pbWrite_clicked(true)
        }
    }

    Button {
        id: update_bootloader_load
        width: parent.width * 0.8
        height: parent.height * 0.07
        anchors.top: update_load.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
        text: "Загрузить bootloader"

        background: Rectangle{
            property var normalColor: "#c97424"
            property var pressedColor: "#88c918"
            color: update_bootloader_load.pressed ? pressedColor : normalColor
        }

        Component.onCompleted: {
                    commun_display.onUpdateBootloaderAvailable.connect(onUpdateBootloaderAvailable)
                }
        function onUpdateBootloaderAvailable(){
            get_firmware_update.visible = false
            busyIndicator.visible = false
            update_load.visible = true
            stop_load.visible = false
            update_bootloader_load.visible = true
        }
        onClicked: {
            update_load.visible = false
            update_bootloader_load.visible = false
            busyIndicator.visible = true
            updateHexx.on_pbWrite_clicked(false)
        }
    }

    BusyIndicator {
        id: busyIndicator
        Material.accent: Material.Light
        anchors.top: senderPageLabel2.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
        height: get_firmware_update.height * 0.95

        Component.onCompleted: {
                    commun_display.onCheckUpdateProgress.connect(onCheckUpdateProgress)
                }
        function onCheckUpdateProgress(){
            get_firmware_update.visible = false
            update_load.visible = false
            busyIndicator.visible = true
            stop_load.visible = false
        }
    }

    Button {
        id: stop_load
        width: parent.width * 0.8
        height: parent.height * 0.07
        anchors.top: busyIndicator.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
        text: "Остановить обновление"

        background: Rectangle{
            property var normalColor: "#db0b3f"
            property var pressedColor: "#821f38"
            color: stop_load.pressed ? pressedColor : normalColor
        }
        Component.onCompleted: {
                    commun_display.onStopUpdate.connect(onStopUpdate)
                }
        onClicked: {
            updateHexx.on_pbStop_clicked("")
        }
        function onStopUpdate(){
            get_firmware_update.visible = false
            update_load.visible = false
            busyIndicator.visible = true
            stop_load.visible = true
        }
    }

    CheckBox {
      id: loging
      anchors.top: update_bootloader_load.bottom
      anchors.left: parent.left
      checked: settParam.loging
      text: qsTr("Включить логирование")
      visible: false

      onClicked: {
          settParam.loging = checked
      }
    }

    CheckBox {
      id: loging_visible
      anchors.top: loging.top
      anchors.right: parent.right
      checked: false
      text: qsTr("Вкл. экран")
      visible: false

      Component.onCompleted: {
                if(mainModel.adminTapCount)   loging_visible.visible = true
            }

      onClicked: {
          loging.visible = loging_visible.checked
           senderBackground.visible = loging_visible.checked
           scrolViewLogArea.visible = loging_visible.checked
           listView1.visible = loging_visible.checked
           clearButton_1.visible = loging_visible.checked
      }
    }

    Rectangle {
        id: senderBackground
        anchors.top: loging.bottom
        anchors.bottom: parent.bottom
        color: "#111111"
        width: page.width
        visible: false
    }

    ScrollView {
        id: scrolViewLogArea
        anchors.fill: senderBackground
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.interactive: true
        visible: false

        ListView {
               id: listView1
               anchors.fill: scrolViewLogArea
               snapMode:ListView.SnapToItem
               width: scrolViewLogArea.width
               clip: true
               visible: false

               Connections {
                   target: commun_display
                   function onLogServis(type, msg) {
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

    Button {
        id: clearButton_1
        width: senderBackground.width * 0.08
        height: senderBackground.height
        anchors.top: senderBackground.top
        anchors.right: senderBackground.right
        opacity: 0.2
        visible: false

        contentItem: Text{
            text: "Clear"
            font.pointSize: 6
        }

        onClicked: {
            logListModel.clear()
        }
    }
}
