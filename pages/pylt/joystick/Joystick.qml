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
            tx_commandsPylt.updateParameters()
        }
        else{
    //        console.log("!!!!!!!!!! Joy Pylt NO visible.")
            joystickPyltTimer.running = visible
        }
    }

    property double x1 : 0
    property double y1 : 0
    property double x2 : 0
    property double y2 : 0

    property bool but_1_Fixed : false           // состояние кнопки 1
    property bool but_2_Fixed : false           // состояние кнопки 2

    function joy_activ(){
        tx_commandsPylt.joystic_Activity(y1, x1, y2, x2, but_1_Fixed, but_2_Fixed)
    }

    Timer {
        id: joystickPyltTimer
        interval: settParam.timer1
        running: false
        repeat: true
        onTriggered: {
            tx_commandsPylt.joystic_Tx();
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
                if(flag === 0)
                    rect1.color = normalPower
                else
                    rect1.color = lowPower
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
        anchors.leftMargin: 130
        anchors.top: voltage.bottom
        anchors.topMargin: 130

        Connections {
            function onMySignal(x, y) {
            //    console.log("joy 1", "X: ", x, "Y: ", y)
                x1 = x
                y1 = y
                joy_activ();
            }
        }
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
                 border.width: 2
                 radius: 5
        }
        onPressed : {
            but_1_Fixed = but_1_Fixed ? false : true
            joy_activ();
        }
        onReleased : {
            if(!pylt_settings.boot_fixing && but_1_Fixed) but_1_Fixed = false
            joy_activ();
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
                 border.width: 2
                 radius: 5
        }
        onPressed : {
            but_2_Fixed = but_2_Fixed ? false : true
            joy_activ();
        }
        onReleased : {
            if(!pylt_settings.boot_fixing && but_2_Fixed) but_2_Fixed = false
            joy_activ();
        }
    }

    Stick{
        id: stick2
        anchors.right: parent.right
        anchors.rightMargin: 130
        anchors.top: voltage.bottom
        anchors.topMargin: 130

        Connections {
            function onMySignal(x, y) {
            //    console.log("joy 2", "X: ", x, "Y: ", y)
                x2 = x
                y2 = y
                joy_activ();
            }
        }
    }
}
