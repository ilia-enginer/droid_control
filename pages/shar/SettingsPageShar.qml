
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material


ScrollView {

    Column {
        spacing: 3
        width: parent.width
        height: parent.height


        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Частота отправки команд джойстиком " + "\n" + settParam.timer1 + "мс"
        }

        Slider {
            id: timer1Slider
            from: 100
            to: 1000
            stepSize: 10        // шаг 10
            snapMode: Slider.SnapAlways   // привязка к шагу при перетаскивании
            value: settParam.timer1
            anchors.left: parent.left
            anchors.right: parent.right
            onMoved: {settParam.timer1 = value; chartController.xStep = value }
        }

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Амплитуда джойстика" + "\n" + Math.round(settParam.joystickAmplitude)
        }

        Slider {
            id: timer2Slider
            value: settParam.joystickAmplitude
            from: 1
            to: 200
            stepSize: 1
            snapMode: Slider.SnapAlways   // привязка к шагу при перетаскивании
            anchors.left: parent.left
            anchors.right: parent.right
            onMoved: {settParam.joystickAmplitude = value}
        }

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Амплитуда высоты" + "\n" + Math.round(settParam.heightAmplitudemin) + "                  " + Math.round(settParam.heightAmplitude)
        }
        RangeSlider {
            id: heightSlider
            from: 0
            to: 100
            stepSize: 1
            snapMode: Slider.SnapAlways   // привязка к шагу при перетаскивании
            first.value: settParam.heightAmplitudemin
            second.value: settParam.heightAmplitude
            anchors.left: parent.left
            anchors.right: parent.right
            first.onMoved: {settParam.heightAmplitudemin = first.value}
            second.onMoved: {settParam.heightAmplitude = second.value}
        }

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Пределы напряжения"
        }

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignCenter
            text: (Number(settParam.Vmin.toFixed(1))) + "                   "+ (Number(settParam.Vmax.toFixed(1)))
        }

        RangeSlider {
            id: voltageSlider
            from: 1.0
            to: 16.8
            first.value: {settParam.Vmin}
            second.value: {settParam.Vmax}
            stepSize: 0.1
            snapMode: Slider.SnapAlways   // привязка к шагу при перетаскивании
            anchors.left: parent.left
            anchors.right: parent.right
            first.onMoved: {settParam.Vmin = first.value}
            second.onMoved: {settParam.Vmax = second.value}
        }
    }
}
