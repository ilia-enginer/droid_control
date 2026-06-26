
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material


ScrollView {
    width: parent.width
    height: parent.height

    property int joy1_y_min : 1000
    property int joy1_y_max : 2000
    property int joy1_x_min : 1000
    property int joy1_x_max : 2000

    property int joy2_y_min : 1000
    property int joy2_y_max : 2000
    property int joy2_x_min : 1000
    property int joy2_x_max : 2000

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
            text: (Number(joy1_y_min)) + "                   "+ (Number(joy1_y_max))
        }

        RangeSlider {
            from: 500
            to: 2500
            first.value: {joy1_y_min}
            second.value: {joy1_y_max}
            stepSize: 10
            snapMode: RangeSlider.SnapAlways // Включаем привязку
            anchors.left: parent.left
            anchors.right: parent.right
            first.onMoved: {joy1_y_min = first.value}
            second.onMoved: {joy1_y_max = second.value}
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
            text: (Number(joy1_x_min)) + "                   "+ (Number(joy1_x_max))
        }

        RangeSlider {
            id: voltageSlider
            from: 500
            to: 2500
            first.value: {joy1_x_min}
            second.value: {joy1_x_max}
            stepSize: 10
            snapMode: RangeSlider.SnapAlways // Включаем привязку
            anchors.left: parent.left
            anchors.right: parent.right
            first.onMoved: {joy1_x_min = first.value}
            second.onMoved: {joy1_x_max = second.value}
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
            text: (Number(joy2_y_min)) + "                   "+ (Number(joy2_y_max))
        }

        RangeSlider {
            from: 500
            to: 2500
            first.value: {joy2_y_min}
            second.value: {joy2_y_max}
            stepSize: 10
            snapMode: RangeSlider.SnapAlways // Включаем привязку
            anchors.left: parent.left
            anchors.right: parent.right
            first.onMoved: {joy2_y_min = first.value}
            second.onMoved: {joy2_y_max = second.value}
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
            text: (Number(joy2_x_min)) + "                   "+ (Number(joy2_x_max))
        }

        RangeSlider {
            from: 500
            to: 2500
            first.value: {joy2_x_min}
            second.value: {joy2_x_max}
            stepSize: 10
            snapMode: RangeSlider.SnapAlways // Включаем привязку
            anchors.left: parent.left
            anchors.right: parent.right
            first.onMoved: {joy2_x_min = first.value}
            second.onMoved: {joy2_x_max = second.value}
        }
    ///////////// X прав /////////////
    }
}

