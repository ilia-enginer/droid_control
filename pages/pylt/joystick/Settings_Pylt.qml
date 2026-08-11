
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtCharts 2.15
import QtQml
import QtQuick.Controls.Material
import QtQuick.Layouts

ScrollView {
    id: scrollView
    width: parent.width
    height: parent.height

    Column {
        spacing: 3
        width: parent.width
        height: parent.height

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Частота отправки команд джойстиком " + "\n" + settParam.timer1 + "мс"
            Component.onCompleted: { settParam.onTimer1Changed.connect(modelChanged) }
            function modelChanged(){ this.text = "Частота отправки команд джойстиком" + "\n" + value + "мс" }
        }

        Slider {
            from: 100
            to: 1000
            value: settParam.timer1
            stepSize: 10
            snapMode: RangeSlider.SnapAlways // Включаем привязку
            anchors.left: parent.left
            anchors.right: parent.right
            onMoved: {settParam.timer1 = value}
            Component.onCompleted: { settParam.onTimer1Changed.connect(modelChanged) }
            function modelChanged(){ this.value = value }
        }

    ///////////// Y лев /////////////
        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Амплитуда канала 1 (Y лев)"
        }

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignCenter
            text: (Number(pylt_settings.ch1_joy1_y_min)) + "                   "+ (Number(pylt_settings.ch1_joy1_y_max))
        }

        RangeSlider {
            from: 500
            to: 2500
            first.value: {pylt_settings.ch1_joy1_y_min}
            second.value: {pylt_settings.ch1_joy1_y_max}
            stepSize: 10
            snapMode: RangeSlider.SnapAlways // Включаем привязку
            anchors.left: parent.left
            anchors.right: parent.right
            first.onMoved: {pylt_settings.ch1_joy1_y_min = first.value}
            second.onMoved: {pylt_settings.ch1_joy1_y_max = second.value}
        }
    ///////////// Y лев /////////////

    ///////////// X лев /////////////
        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Амплитуда канала 2 (X лев)"
        }

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignCenter
            text: (Number(pylt_settings.ch2_joy1_x_min)) + "                   "+ (Number(pylt_settings.ch2_joy1_x_max))
        }

        RangeSlider {
            id: voltageSlider
            from: 500
            to: 2500
            first.value: {pylt_settings.ch2_joy1_x_min}
            second.value: {pylt_settings.ch2_joy1_x_max}
            stepSize: 10
            snapMode: RangeSlider.SnapAlways // Включаем привязку
            anchors.left: parent.left
            anchors.right: parent.right
            first.onMoved: {pylt_settings.ch2_joy1_x_min = first.value}
            second.onMoved: {pylt_settings.ch2_joy1_x_max = second.value}
        }
    ///////////// X лев /////////////


    ///////////// Y прав /////////////
        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Амплитуда канала 3 (Y прав)"
        }

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignCenter
            text: (Number(pylt_settings.ch3_joy2_y_min)) + "                   "+ (Number(pylt_settings.ch3_joy2_y_max))
        }

        RangeSlider {
            from: 500
            to: 2500
            first.value: {pylt_settings.ch3_joy2_y_min}
            second.value: {pylt_settings.ch3_joy2_y_max}
            stepSize: 10
            snapMode: RangeSlider.SnapAlways // Включаем привязку
            anchors.left: parent.left
            anchors.right: parent.right
            first.onMoved: {pylt_settings.ch3_joy2_y_min = first.value}
            second.onMoved: {pylt_settings.ch3_joy2_y_max = second.value}
        }
    ///////////// Y прав /////////////


    ///////////// X прав /////////////
        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Амплитуда канала 4 (X прав)"
        }

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignCenter
            text: (Number(pylt_settings.ch4_joy2_x_min)) + "                   "+ (Number(pylt_settings.ch4_joy2_x_max))
        }

        RangeSlider {
            from: 500
            to: 2500
            first.value: {pylt_settings.ch4_joy2_x_min}
            second.value: {pylt_settings.ch4_joy2_x_max}
            stepSize: 10
            snapMode: RangeSlider.SnapAlways // Включаем привязку
            anchors.left: parent.left
            anchors.right: parent.right
            first.onMoved: {pylt_settings.ch4_joy2_x_min = first.value}
            second.onMoved: {pylt_settings.ch4_joy2_x_max = second.value}
        }
    ///////////// X прав /////////////

    ///////////// кнопка лев /////////////
        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Амплитуда кнопки 1 (лев.)"
        }

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignCenter
            text: (Number(pylt_settings.ch5_min)) + "                   "+ (Number(pylt_settings.ch5_max))
        }

        RangeSlider {
            from: 500
            to: 2500
            first.value: {pylt_settings.ch5_min}
            second.value: {pylt_settings.ch5_max}
            stepSize: 10
            snapMode: RangeSlider.SnapAlways // Включаем привязку
            anchors.left: parent.left
            anchors.right: parent.right
            first.onMoved: {pylt_settings.ch5_min = first.value}
            second.onMoved: {pylt_settings.ch5_max = second.value}
        }
    ///////////// кнопка лев /////////////

    ///////////// кнопка прав /////////////
        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Амплитуда кнопки 2 (прав.)"
        }

        Label {
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignCenter
            text: (Number(pylt_settings.ch6_min)) + "                   "+ (Number(pylt_settings.ch6_max))
        }

        RangeSlider {
            from: 500
            to: 2500
            first.value: {pylt_settings.ch6_min}
            second.value: {pylt_settings.ch6_max}
            stepSize: 10
            snapMode: RangeSlider.SnapAlways // Включаем привязку
            anchors.left: parent.left
            anchors.right: parent.right
            first.onMoved: {pylt_settings.ch6_min = first.value}
            second.onMoved: {pylt_settings.ch6_max = second.value}
        }
    ///////////// кнопка прав /////////////

        Switch {
            width: parent.width
            anchors.left: parent.left
            checked: pylt_settings.boot_fixing
            text: qsTr("фиксация кнопок")
            onClicked: { pylt_settings.boot_fixing = checked }
        }

        Switch {
            width: parent.width
            anchors.left: parent.left
            checked: pylt_settings.tank_mode
            text: qsTr("управление танком")
            onClicked: { pylt_settings.tank_mode = checked }
        }

        RoundButton {
            id: but_save
            width: parent.width * 0.95
            anchors.horizontalCenter: parent.horizontalCenter
            hoverEnabled: false             // отключаем стилизацию наведения
            text: "Сохранить параметры"
            background:
                Rectangle{      // фон кнопки
                property var normalColor: "#01a3a4"
                property var pressedColor: "#d94a4a"
                     anchors.fill: parent
                     color: "#c7ecee"                // цвет фона кнопки
                     border.color: but_save.pressed === true ? pressedColor : normalColor         // цвет границы
                     border.width: 4
                     radius: 5
            }
            onReleased: {
                pylt_settings.saveFullParam()
            }
        }
    }
}

