import QtQuick 2.1
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.settings
import QtQuick.Controls.Material


Page {
    id: page
 //   anchors.fill: parent

    Timer {
        id: setpageTimer
        interval: 150
        running: false
        repeat: true

        onTriggered: {
            mainModel.write_page();
        }
        Component.onCompleted: {
                    mainModel.onTxPageTimerOn.connect(onTxPageTimerOn)
                    mainModel.onTxPageTimerOff.connect(onTxPageTimerOff)
                }
        function onTxPageTimerOn(){
            setpageTimer.start();
        }
        function onTxPageTimerOff(){
            setpageTimer.stop();
        }
    }

    Label {
        id: senderPageLabel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        width: parent.width
        wrapMode: Label.Wrap
//        anchors.horizontalCenter: page.horizontalCenter
        horizontalAlignment: Qt.AlignHCenter
        text: mainModel.currentDeviceName
        font.pointSize: parent.height * 0.04
    }

    Label {
        id: senderPageLabel1
        anchors.top: senderPageLabel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 10
        width: parent.width
        wrapMode: Label.Wrap
//        anchors.horizontalCenter: page.horizontalCenter
        horizontalAlignment: Qt.AlignHCenter
        text: "Во время обновления не блокируйте телефон и не сворачивайте приложение"
        font.pointSize: parent.height * 0.03
        color: "red"
    }

    Label {
        id: senderPageLabel2
        anchors.top: senderPageLabel1.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 30
        width: parent.width
        wrapMode: Label.Wrap
    //    anchors.horizontalCenter: page.horizontalCenter
        horizontalAlignment: Qt.AlignHCenter
//        text: mainModel.currenUpdate
        text: "Проверьте обновление"
        font.pointSize: parent.height * 0.025
        Component.onCompleted: {
                    mainModel.onCurrenUpdateChanged.connect(onCurrenUpdateChanged)
                }
        function onCurrenUpdateChanged(){
            text = mainModel.currenUpdate
        }

    }

    Button {
        id: get_firmware_update
        width: parent.width * 0.8
        height: parent.height * 0.07
        anchors.top: senderPageLabel2.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        visible: true
        text: "Проверить обновление"

        background: Rectangle{
            property var normalColor: "#d0d413"
            property var pressedColor: "#3c965a"
            color: get_firmware_update.pressed ? pressedColor : normalColor
        }
        Component.onCompleted: {
                    mainModel.onCheckUpdate.connect(onCheckUpdate)
                }
        function onCheckUpdate(){
            busyIndicator.visible = false
            update_load.visible = false
            update_bootloader_load.visible = false
            stop_load.visible = false
            get_firmware_update.visible = true
        }
        onClicked: {
              mainModel.checkingUpdates();
        }
    }

    Button {
        id: update_load
        width: parent.width * 0.8
        height: parent.height * 0.07
        anchors.top: senderPageLabel2.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
        text: "Загрузить обновление"

        background: Rectangle{
            property var normalColor: "#c97424"
            property var pressedColor: "#88c918"
            color: update_load.pressed ? pressedColor : normalColor
        }

        Component.onCompleted: {
                    mainModel.onUpdateAvailable.connect(onUpdateAvailable)
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
            mainModel.on_pbWrite_clicked(true)
        }
    }

    Button {
        id: update_bootloader_load
        width: parent.width * 0.8
        height: parent.height * 0.07
        anchors.top: update_load.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
        text: "Загрузить bootloader"

        background: Rectangle{
            property var normalColor: "#c97424"
            property var pressedColor: "#88c918"
            color: update_bootloader_load.pressed ? pressedColor : normalColor
        }

        Component.onCompleted: {
                    mainModel.onUpdateBootloaderAvailable.connect(onUpdateBootloaderAvailable)
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
            mainModel.on_pbWrite_clicked(false)
        }
    }

    BusyIndicator {
        id: busyIndicator
        Material.accent: Material.Light
        anchors.top: senderPageLabel2.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
        height: get_firmware_update.height - 5

        Component.onCompleted: {
                    mainModel.onCheckUpdateProgress.connect(onCheckUpdateProgress)
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
                    mainModel.onStopUpdate.connect(onStopUpdate)
                }
        onClicked: {
            mainModel.on_pbStop_clicked("")
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
      checked: device.loging
      text: qsTr("Включить логирование")

      Component.onCompleted: {
          if(mainModel.adiminTapCount === -1)   loging.visible = true
          else                                  loging.visible = false
     }

      onClicked: {
          device.loging = checked
      }
    }

    Rectangle {
        id: senderBackground
        anchors.top: loging.bottom
        anchors.bottom: parent.bottom
        color: "#111111"
        width: page.width
        Component.onCompleted: {
            if(mainModel.adiminTapCount === -1)   senderBackground.visible = true
            else                                  senderBackground.visible = false
       }
    }

    ScrollView {
        id: scrolViewLogArea
        anchors.fill: senderBackground
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.interactive: true
        Component.onCompleted: {
            if(mainModel.adiminTapCount === -1)   scrolViewLogArea.visible = true
            else                                  scrolViewLogArea.visible = false
       }

        ListView {
               id: listView1
               anchors.fill: scrolViewLogArea
               snapMode:ListView.SnapToItem
               width: scrolViewLogArea.width
               clip: true

               Component.onCompleted: {
                   if(mainModel.adiminTapCount === -1)   listView1.visible = true
                   else                                  listView1.visible = false
              }
               Connections {
                   target: device
                   onLogServis: {
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
        Component.onCompleted: {
            if(mainModel.adiminTapCount === -1)   clearButton_1.visible = true
            else                                  clearButton_1.visible = false
       }
        contentItem: Text{
            text: "Clear"
            font.pointSize: 6
        }

        onClicked: {
            logListModel.clear()
        }
    }
}
