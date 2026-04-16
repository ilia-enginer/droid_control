import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material


Dialog {
    id: deviceSelectionDialog
    width: window.width * 0.7
    height: window.height * 0.4
    anchors.centerIn: parent
    modal: false
    focus: true
    title: "Выбор устройства"
    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
        settParam.setIdDevice(styleBox.currentIndex, true)
        deviceSelectionDialog.close()
    }
    onRejected: { deviceSelectionDialog.close() }

    contentItem: ColumnLayout {
        id: settingsColumn
        spacing: 20

        RowLayout {
            spacing: 7

            Label {
                text: "Type:"
            }

            ComboBox {
                id: styleBox
                Layout.fillWidth: true
                model: ListModel {
                    ListElement { name: "..."}//" title: "Джойстик"; source: "qrc:/pages/shar/joystick/Joystick.qml" }
                    ListElement { name: "Шар"}//" title: "Джойстик"; source: "qrc:/pages/shar/joystick/Joystick.qml" }
                }
            }
        }
        //включение режима админа
        CheckBox {
            id: admin
            checked: mainModel.adminFlag
            text: qsTr("Admin")
            onClicked: { mainModel.adminFlag = checked }
        }
    }
}
