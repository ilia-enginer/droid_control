
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

    // ─── Тест уведомлений в фоне ─────────────────────────────────────────
    Rectangle {
        visible: Qt.platform.os === "android"
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 12
        }
        height: notifColumn.implicitHeight + 20
        radius: 8
        color: "#1A237E"
        opacity: 0.92

        Column {
            id: notifColumn
            spacing: 8
            width: parent.width - 24
            anchors.centerIn: parent

            Label {
                width: parent.width
                text: "🔔 Тест уведомлений в фоне"
                horizontalAlignment: Qt.AlignHCenter
                font.bold: true
                color: "white"
            }

            Label {
                id: intervalLabel
                width: parent.width
                horizontalAlignment: Qt.AlignHCenter
                color: "#90CAF9"
                text: "Интервал: " + Math.round(intervalSlider.value / 1000) + " сек"
            }

            Slider {
                id: intervalSlider
                from: 10000
                to: 300000
                value: 60000
                stepSize: 5000
                width: parent.width
                enabled: !notifColumn.testActive
                onMoved: intervalLabel.text = "Интервал: " + Math.round(value / 1000) + " сек"
            }

            property bool testActive: false

            Row {
                spacing: 12
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    text: "▶ Старт"
                    enabled: !notifColumn.testActive
                    onClicked: {
                        notifColumn.testActive = true
                        notificationClient.startTestTimer(intervalSlider.value)
                    }
                }

                Button {
                    text: "■ Стоп"
                    enabled: notifColumn.testActive
                    onClicked: {
                        notifColumn.testActive = false
                        notificationClient.stopTestTimer()
                    }
                }
            }

            Label {
                width: parent.width
                horizontalAlignment: Qt.AlignHCenter
                color: notifColumn.testActive ? "#A5D6A7" : "#EF9A9A"
                text: notifColumn.testActive
                      ? "✓ Активен — сверните приложение и ждите"
                      : "● Остановлен"
                wrapMode: Label.Wrap
            }
        }
    }
    // ─────────────────────────────────────────────────────────────────────
//    QtObject{}
}
