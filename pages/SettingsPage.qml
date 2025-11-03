
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material


ScrollablePage {
    id: page

    Column {
        spacing: 10
        width: parent.width
        height: parent.height


        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Частота отправки команд джойстиком " + "\n" + settParam.timer1 + "мс"

            Component.onCompleted: {
                settParam.onTimer1Changed.connect(modelChanged)
            }

            function modelChanged(){
                this.text = "Частота отправки команд джойстиком" + "\n" + value + "мс"
            }
        }

        Slider {
            id: timer1Slider
            from: 100
            to: 1000
            value: settParam.timer1
            anchors.left: parent.left
            anchors.right: parent.right
            onMoved: {settParam.timer1 = value}

            Component.onCompleted: {
                settParam.onTimer1Changed.connect(modelChanged)
            }

            function modelChanged(){
                this.value = value
            }
        }

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Амплитуда джойстика" + "\n" + Math.round(settParam.joystickAmplitude)

            Component.onCompleted: {
                settParam.joystickAmplitudeChanged.connect(modelChanged)
            }

            function modelChanged(){
                this.text = "Амплитуда джойстика" + "\n" + Math.round(value)
            }
        }

        Slider {
            id: timer2Slider
            value: settParam.joystickAmplitude
            from: 1
            to: 200
            anchors.left: parent.left
            anchors.right: parent.right
            onMoved: {settParam.joystickAmplitude = value}

            Component.onCompleted: {
                settParam.joystickAmplitudeChanged.connect(modelChanged)
            }

            function modelChanged(){
                this.value = value
            }
        }

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Амплитуда высоты" + "\n" + Math.round(settParam.heightAmplitudemin) + "                  " + Math.round(settParam.heightAmplitude)

//            Component.onCompleted: {
//                settParam.heightAmplitudeChangedmin.connect(modelChanged)
//                settParam.heightAmplitudeChanged.connect(modelChanged)
//            }
        }
        RangeSlider {
            id: heightSlider
            from: 0
            to: 60
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
//            Component.onCompleted: {
//                settParam.VminChanged.connect(modelChanged)
//                settParam.VmaxChanged.connect(modelChanged)
//            }
        }

        RangeSlider {
            id: voltageSlider
            from: 1.0
            to: 16.8
            first.value: {settParam.Vmin}
            second.value: {settParam.Vmax}
            anchors.left: parent.left
            anchors.right: parent.right
            first.onMoved: {settParam.Vmin = first.value}
            second.onMoved: {settParam.Vmax = second.value}
        }
    }
}
