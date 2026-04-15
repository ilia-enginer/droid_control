
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material

//Pane {
Page{
    id: pane
    Image {
        id: logo
        width: pane.availableWidth / 1.9
        height: pane.availableHeight / 1.9
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -(parent.height * 0.08)
        fillMode: Image.PreserveAspectFit
        source: "../../images/play_store_512.png"
    }
    //имя приемника
    Text {
        id: devName
        width: parent.width
        wrapMode: Label.Wrap
        horizontalAlignment: Qt.AlignHCenter
        text: commun_display.curDeviceName_
        font.pixelSize: 16
    }

  //─── COM - port ─────────────────────────────────────────
    Rectangle {
        visible: Qt.platform.os === "windows"
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 12
        }
        height: notifColumn.implicitHeight + 20
        radius: 8
        color: "#6040a1"
        opacity: 0.9

        Column {
            id: notifColumn
            spacing: 8
            width: parent.width - 24
            anchors.centerIn: parent

            Label {
                width: parent.width
                text: "🔔 Настройка COM-port"
                horizontalAlignment: Qt.AlignHCenter
                font.bold: true
                color: "white"
            }

            property bool testActive: false

            Row {
                spacing: 12
                anchors.horizontalCenter: parent.horizontalCenter

                Shortcut{
                    sequence: "F3"
                    onActivated: if(!notifColumn.testActive) mainSerialPort.settingsWindowOpen();
                }

                Button {
                    text: "☣ Настройки (F3)"
                    enabled: !notifColumn.testActive
                    onClicked: {
                        mainSerialPort.settingsWindowOpen();
                    }
                }

                Shortcut{
                    sequence: "F2"
                    onActivated: {
                        if(notifColumn.testActive)
                        {
                            notifColumn.testActive = false;
                            mainSerialPort.closeSerialPort()
                        }
                        else
                        {
                            notifColumn.testActive = true;
                            mainSerialPort.openSerialPort()
                        }
                    }
                }

                Button {
                    text: "▶ Старт (F2)"
                    enabled: !notifColumn.testActive
                    onClicked: {
                        notifColumn.testActive = true
                        mainSerialPort.openSerialPort();
                    }
                }

                Button {
                    text: "■ Стоп (F2)"
                    enabled: notifColumn.testActive
                    onClicked: {
                         notifColumn.testActive = false
                         mainSerialPort.closeSerialPort();
                    }
                }
            }

            Label {
                width: parent.width
                horizontalAlignment: Qt.AlignHCenter
                color: notifColumn.testActive ? "#A5D6A7" : "#EF9A9A"
                text: notifColumn.testActive
                      ? "✓ Активен"
                      : "● Остановлен"
                wrapMode: Label.Wrap
            }
        }
    }
    // ─────────────────────────────────────────────────────────────────────
}
