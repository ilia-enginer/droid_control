import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material


ScrollView {
    id: settingsScroll

    // ---- Палитра ----
    readonly property color cBg:        "#eef3f8"
    readonly property color cCard:      "#ffffff"
    readonly property color cBorder:    "#e0e6ed"
    readonly property color cText:      "#263238"
    readonly property color cTextSoft:  "#546e7a"
    readonly property color cPrimary:   "#5b7cfa"
    readonly property color cPrimaryD:  "#3f5fd6"
    readonly property color cTrack:     "#e6ebf2"
    readonly property color cHandle:    "#ffffff"

    // Фон
    Rectangle {
        anchors.fill: parent
        color: cBg
        z: -1
    }

    Column {
        spacing: 10
        width: parent.width
        height: parent.height

        // ---------- Частота отправки ----------
        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Частота отправки команд джойстиком " + "\n" + settParam.timer1 + "мс"
            color: cText
            padding: 6
        }

        Slider {
            id: timer1Slider
            from: 100
            to: 1000
            stepSize: 10
            snapMode: Slider.SnapAlways
            value: settParam.timer1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            onMoved: { settParam.timer1 = value; chartController.xStep = value }

            background: Rectangle {
                x: timer1Slider.leftPadding
                y: timer1Slider.topPadding + timer1Slider.availableHeight / 2 - height / 2
                implicitWidth: 200
                implicitHeight: 6
                width: timer1Slider.availableWidth
                height: implicitHeight
                radius: 3
                color: cTrack
                border.width: 1
                border.color: cBorder

                Rectangle {
                    width: timer1Slider.visualPosition * parent.width
                    height: parent.height
                    radius: 3
                    color: cPrimary
                }
            }

            handle: Rectangle {
                x: timer1Slider.leftPadding + timer1Slider.visualPosition * (timer1Slider.availableWidth - width)
                y: timer1Slider.topPadding + timer1Slider.availableHeight / 2 - height / 2
                implicitWidth: 22
                implicitHeight: 22
                radius: 11
                color: timer1Slider.pressed ? "#dbe7ff" : cHandle
                border.width: 2
                border.color: cPrimaryD
            }
        }

        // ---------- Амплитуда джойстика ----------
        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Амплитуда джойстика" + "\n" + Math.round(settParam.joystickAmplitude)
            color: cText
            padding: 6
        }

        Slider {
            id: timer2Slider
            value: settParam.joystickAmplitude
            from: 1
            to: 200
            stepSize: 1
            snapMode: Slider.SnapAlways
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            onMoved: { settParam.joystickAmplitude = value }

            background: Rectangle {
                x: timer2Slider.leftPadding
                y: timer2Slider.topPadding + timer2Slider.availableHeight / 2 - height / 2
                implicitWidth: 200
                implicitHeight: 6
                width: timer2Slider.availableWidth
                height: implicitHeight
                radius: 3
                color: cTrack
                border.width: 1
                border.color: cBorder

                Rectangle {
                    width: timer2Slider.visualPosition * parent.width
                    height: parent.height
                    radius: 3
                    color: cPrimary
                }
            }

            handle: Rectangle {
                x: timer2Slider.leftPadding + timer2Slider.visualPosition * (timer2Slider.availableWidth - width)
                y: timer2Slider.topPadding + timer2Slider.availableHeight / 2 - height / 2
                implicitWidth: 22
                implicitHeight: 22
                radius: 11
                color: timer2Slider.pressed ? "#dbe7ff" : cHandle
                border.width: 2
                border.color: cPrimaryD
            }
        }

        // ---------- Амплитуда высоты ----------
        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Амплитуда высоты" + "\n" + Math.round(settParam.heightAmplitudemin) + "                  " + Math.round(settParam.heightAmplitude)
            color: cText
            padding: 6
        }

        RangeSlider {
            id: heightSlider
            from: 0
            to: 100
            stepSize: 1
            snapMode: Slider.SnapAlways
            first.value: settParam.heightAmplitudemin
            second.value: settParam.heightAmplitude
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            first.onMoved: { settParam.heightAmplitudemin = first.value }
            second.onMoved: { settParam.heightAmplitude = second.value }

            background: Rectangle {
                x: heightSlider.leftPadding
                y: heightSlider.topPadding + heightSlider.availableHeight / 2 - height / 2
                implicitWidth: 200
                implicitHeight: 6
                width: heightSlider.availableWidth
                height: implicitHeight
                radius: 3
                color: cTrack
                border.width: 1
                border.color: cBorder

                // выделенный диапазон
                Rectangle {
                    x: heightSlider.first.visualPosition * parent.width
                    width: (heightSlider.second.visualPosition - heightSlider.first.visualPosition) * parent.width
                    height: parent.height
                    radius: 3
                    color: "#a9c1ff"
                }
            }

            first.handle: Rectangle {
                x: heightSlider.leftPadding + heightSlider.first.visualPosition * (heightSlider.availableWidth - width)
                y: heightSlider.topPadding + heightSlider.availableHeight / 2 - height / 2
                implicitWidth: 22
                implicitHeight: 22
                radius: 11
                color: heightSlider.first.pressed ? "#dbe7ff" : cHandle
                border.width: 2
                border.color: cPrimaryD
            }

            second.handle: Rectangle {
                x: heightSlider.leftPadding + heightSlider.second.visualPosition * (heightSlider.availableWidth - width)
                y: heightSlider.topPadding + heightSlider.availableHeight / 2 - height / 2
                implicitWidth: 22
                implicitHeight: 22
                radius: 11
                color: heightSlider.second.pressed ? "#dbe7ff" : cHandle
                border.width: 2
                border.color: cPrimaryD
            }
        }

        // ---------- Пределы напряжения ----------
        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Пределы напряжения"
            color: cText
            padding: 6
        }

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignCenter
            text: (Number(settParam.Vmin.toFixed(1))) + "                   " + (Number(settParam.Vmax.toFixed(1)))
            color: cTextSoft
        }

        RangeSlider {
            id: voltageSlider
            from: 1.0
            to: 16.8
            first.value: settParam.Vmin
            second.value: settParam.Vmax
            stepSize: 0.1
            snapMode: Slider.SnapAlways
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            first.onMoved: { settParam.Vmin = first.value }
            second.onMoved: { settParam.Vmax = second.value }

            background: Rectangle {
                x: voltageSlider.leftPadding
                y: voltageSlider.topPadding + voltageSlider.availableHeight / 2 - height / 2
                implicitWidth: 200
                implicitHeight: 6
                width: voltageSlider.availableWidth
                height: implicitHeight
                radius: 3
                color: cTrack
                border.width: 1
                border.color: cBorder

                Rectangle {
                    x: voltageSlider.first.visualPosition * parent.width
                    width: (voltageSlider.second.visualPosition - voltageSlider.first.visualPosition) * parent.width
                    height: parent.height
                    radius: 3
                    color: "#a9c1ff"
                }
            }

            first.handle: Rectangle {
                x: voltageSlider.leftPadding + voltageSlider.first.visualPosition * (voltageSlider.availableWidth - width)
                y: voltageSlider.topPadding + voltageSlider.availableHeight / 2 - height / 2
                implicitWidth: 22
                implicitHeight: 22
                radius: 11
                color: voltageSlider.first.pressed ? "#dbe7ff" : cHandle
                border.width: 2
                border.color: cPrimaryD
            }

            second.handle: Rectangle {
                x: voltageSlider.leftPadding + voltageSlider.second.visualPosition * (voltageSlider.availableWidth - width)
                y: voltageSlider.topPadding + voltageSlider.availableHeight / 2 - height / 2
                implicitWidth: 22
                implicitHeight: 22
                radius: 11
                color: voltageSlider.second.pressed ? "#dbe7ff" : cHandle
                border.width: 2
                border.color: cPrimaryD
            }
        }
    }
}
