import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material


Dialog {
    id: devicesDialog
    implicitWidth: parent.width * 0.8
    implicitHeight: parent.height * 0.9
    x: parent.width * 0.1
    y: parent.height * 0.05

    header: Column {
        anchors.left: devicesDialog.Left
        anchors.right: devicesDialog.Right
        spacing: 1

        Label {
            id: devicesWarning
            horizontalAlignment: Qt.AlignHCenter
            text: " Для поиска Bluetooth устройств необходимо, в настройках приложения, разрешить определять местоположение.\n\n"
            font.pixelSize: 13
            width: devicesDialog.width
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.Wrap
        }

        Label {
            id: devicesDialogStatus
            horizontalAlignment: Qt.AlignHCenter
            text: " "
            font.pixelSize: 15
            width: devicesDialog.width
      //      horizontalAlignment: Text.left
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.Wrap
            Component.onCompleted: { 
                commun_display.onUpdateChanged.connect(deviceChanged)
            }

            function deviceChanged(msg){
                devicesDialogStatus.text = msg
            }
        }
    }

    Component {
        id: dragDelegate

        Rectangle {
                   id: box
                   height:45
                   width: devicesListView.width
                   color: "#cccccc"
                   border.width: 2
                   border.color: "#666699"
                   radius: 10

                   MouseArea {
                       anchors.fill: parent
                       onClicked: {
                           device.connectToDevice(modelData.deviceAddress,modelData.deviceName,modelData.coreConfig);
                       }
                   }

                   Label {
                       id: deviceName1
                       text: modelData.deviceName
                       anchors.top: parent.top
                       anchors.topMargin: 5
                       x: 5
                   }

                   Label {
                       id: deviceAddress1
                       text: modelData.deviceAddress
                       font.pointSize: deviceName1.font.pointSize*0.7
                       anchors.bottom: box.bottom
                       anchors.bottomMargin: 5
                       x: 5
                   }

                   Label {
                       id: deficeCoreConfig
                       color: "green"
                       text: modelData.coreConfig
                       font.pointSize: deviceName1.font.pointSize*0.7
                       anchors.bottom: box.bottom
                       anchors.bottomMargin: 5
                       anchors.right: box.right
                       anchors.rightMargin: 5
                   }
               }
    }

    contentItem: ListView {
        id: devicesListView

        focus: true
        clip: true
        currentIndex: -1
        anchors.top: devicesDialog.header.bottom
        anchors.left: devicesDialog.Left
        anchors.right: devicesDialog.Right
        anchors.bottom: devicesDialog.footer.top

        model: device.devicesList
        spacing : 5

        add: Transition {
            NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 500 }
        }

//        populate: Transition {
//               NumberAnimation { properties: "x,y"; duration: 200 }
//               NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 500 }
//        }

        delegate: dragDelegate

        ScrollIndicator.vertical: ScrollIndicator { }
    }

    footer: Rectangle {
        id: footerContainer

        Component.onCompleted: {
            commun_display.onSearchInProgress.connect(onSearchInProgress)
            commun_display.onSearchFinished.connect(onSearchFinished)
        }

        function onSearchInProgress(){
            searchButton.text = ""
            searchButton.enabled = false
            busyIndicator.visible = true
        }

        function onSearchFinished(){
            searchButton.text = "Начать поиск"
            searchButton.enabled = true
            busyIndicator.visible = false
        }

        Button {
            id: get_devise
            anchors.left: footerContainer.left
            anchors.right: footerContainer.right
            anchors.bottom: searchButton.top
            anchors.margins: 5
            highlighted: true
            text: "Последнее подключенное"

            Component.onCompleted: {
                get_devise.visible = false
            }

            onClicked: {
                get_devise.visible = false
                device.get_last_device();
            }
        }

        Button {
            id: searchButton
            anchors.left: footerContainer.left
            anchors.right: footerContainer.right
            anchors.bottom: footerContainer.bottom
            anchors.margins: 5
            highlighted: true
            text: "Начать поиск"

            onClicked: {
    ////            get_devise.visible = true
                devicesWarning.visible = false
                device.startDeviceDiscovery();
            }
        }


        BusyIndicator {
            id: busyIndicator
            Material.accent: Material.Light
            anchors.bottom: footerContainer.bottom
            anchors.margins: 8
            visible: false
            height: searchButton.height - 5

            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
