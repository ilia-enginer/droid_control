import QtQuick 2.7
import QtQuick.Controls 2.0
import QtCharts 2.15
import QtQml
import QtQuick.Controls.Material
import QtQuick.Layouts

import "stick"

Item {
    id: joystick_pylt
    visible: false
    width: parent.height
    height: parent.width
    anchors.centerIn: parent
    transformOrigin: Item.Center
    rotation: 90

    onVisibleChanged: {
        if(visible === true){
    //        console.log("!!!!!!!!!! Joy Pylt Visible.")
            joystickPyltTimer.running = visible
        }
        else{
    //        console.log("!!!!!!!!!! Joy Pylt NO visible.")
            joystickPyltTimer.running = visible
        }
    }

    property int txMin : 0
    property int txMid : 124
    property int txMax : 249

    property bool but_1_Fixed : false           // состояние кнопки 1
    property bool but_2_Fixed : false           // состояние кнопки 2

    property int but_1_tx : txMin           // показания кнопки 1
    property int but_2_tx : txMin           // показания кнопки 2

    Timer {
        id: joystickPyltTimer
        interval: settParam.timer1
        running: false
        repeat: true
        onTriggered: {
            tx_commandsPylt.joysticActivity(stick1.extY, stick1.extX, stick2.extY, stick2.extX, but_1_tx, but_2_tx);
        }
    }

    //имя приемника
    Label {
        id: label
        anchors.top: parent.top
        horizontalAlignment: Qt.AlignHCenter
        width: parent.width
        wrapMode: Label.Wrap
        text: commun_display.curDeviceName_
    }


    //напруга
    property string lowPower:   "#f05454"
    property string normalPower: "#e6e6e6"

    ProgressBar{
        id: voltage
        anchors.top: label.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: joystick_pylt.height * 0.03
        width: joystick_pylt.width * 0.9
        from: {settParam.Vmin}
        to: {settParam.Vmax}
        background: Rectangle {
            id: rect1
            color: normalPower
            radius: 3
        }

        contentItem: Item {
            Rectangle {
                width: voltage.visualPosition * parent.width
                height: parent.height
                radius: 2

                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        SequentialAnimation on color {
                            loops: Animation.Infinite
                            ColorAnimation { from: "#11ed61"; to: "#0ba142"; duration: 5000 }
                            ColorAnimation { from: "#0ba142"; to: "#11ed61"; duration: 5000 }
                        }
                    }
                    GradientStop {
                        position: 1.0
                        SequentialAnimation on color {
                            loops: Animation.Infinite
                            ColorAnimation { from: "#14aaff"; to: "#437284"; duration: 5000 }
                            ColorAnimation { from: "#437284"; to: "#14aaff"; duration: 5000 }
                        }
                    }
                }
            }
        }

        Connections {
            target: commun_display

            function onVrealChanged(V) {
                voltage.value = V
            }

            function onSetChargeLevel(flag){
                rect1.color = (flag) ? lowPower : normalPower
            }
        }
    }

    //напруга
    Label {
        id: voltageLabel
        width: parent.width
        wrapMode: Label.Wrap
        horizontalAlignment: Qt.AlignHCenter
        anchors.verticalCenter: voltage.verticalCenter
        text: commun_display.Volt.toFixed(1) + "V"
        color: "black"
    }

    Stick{
        id: stick1
        anchors.left: parent.left
        anchors.leftMargin: 100
     //   anchors.top: voltage.bottom
     //   anchors.topMargin: 100
        anchors.verticalCenter: parent.verticalCenter
    }

    RoundButton {
        anchors.top: stick1.top
        anchors.topMargin: 60
        anchors.left: stick1.right
        anchors.leftMargin: 120
        hoverEnabled: false             // отключаем стилизацию наведения
        text: "1"
        background:
            Rectangle{      // фон кнопки
            property var normalColor: "#01a3a4"
            property var pressedColor: "#d94a4a"
                 anchors.fill: parent
                 color: "#c7ecee"                // цвет фона кнопки
                 border.color: but_1_Fixed === true ? pressedColor : normalColor         // цвет границы
                 radius: 5
        }
        onPressed : {
            but_1_Fixed = but_1_Fixed ? false : true
            but_1_tx = but_1_Fixed ? txMax : txMin
        }
        onReleased : {
            if(!butFixed.checked && but_1_Fixed) but_1_Fixed = false
            but_1_tx = but_1_Fixed ? txMax : txMin
        }
    }

    RoundButton {
        anchors.top: stick2.top
        anchors.topMargin: 60
        anchors.right: stick2.left
        anchors.rightMargin: 120
        hoverEnabled: false             // отключаем стилизацию наведения
        text: "2"
        background:
            Rectangle{      // фон кнопки
            property var normalColor: "#01a3a4"
            property var pressedColor: "#d94a4a"
                 anchors.fill: parent
                 color: "#c7ecee"                // цвет фона кнопки
                 border.color: but_2_Fixed === true ? pressedColor : normalColor         // цвет границы
                 radius: 5
        }
        onPressed : {
            but_2_Fixed = but_2_Fixed ? false : true
            but_2_tx = but_2_Fixed ? txMax : txMin
        }
        onReleased : {
            if(!butFixed.checked && but_2_Fixed) but_2_Fixed = false
            but_2_tx = but_2_Fixed ? txMax : txMin
        }
    }

    Stick{
        id: stick2
        anchors.right: parent.right
        anchors.rightMargin: 100
    //    anchors.top: voltage.bottom
    //    anchors.topMargin: 100
        anchors.verticalCenter: parent.verticalCenter
    }

    Switch {
        id: butFixed
        anchors.top: voltage.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: voltage.horizontalCenter
        checked: false
        Text {
            anchors.top: parent.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            text: qsTr("фиксация кнопок")
        }
    }
}
