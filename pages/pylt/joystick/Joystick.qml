import QtQuick 2.7
import QtQuick.Controls 2.0
import QtCharts 2.15
import QtQml
import QtQuick.Controls.Material
import QtQuick.Layouts

import "stick"

Item {
    id: joystick_pylt
    focus: false

    onActiveFocusChanged: {
        if (!activeFocus) joystickPyltTimer.running = false
        else joystickPyltTimer.running = true
     }

    Timer {
        id: joystickPyltTimer
        interval: settParam.timer1
        running: false
        repeat: true
      //  onTriggered: { tx_commands.joysticActivity(mode, azimuth, amplitude, level, ctrl); }
    }

    Column  {
        spacing: 5
        rotation: 90
      //  anchors.fill: parent.fill
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
     //   anchors.topMargin: width * 0.5
        //имя приемника
        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            wrapMode: Label.Wrap
            text: commun_display.curDeviceName_
        }
        //напруга
        ProgressBar{
            id: voltage
            anchors.horizontalCenter: parent.horizontalCenter
            height: joystick_pylt.width * 0.07
            width: joystick_pylt.height * 0.9
            from: {settParam.Vmin}
            to: {settParam.Vmax}
            background: Rectangle {
                color: "#e6e6e6"
                radius: 3
            }

            contentItem: Item {
                Rectangle {
                    width: voltage.visualPosition * parent.width
                    height: parent.height
                    radius: 2
                    color: "#17a81a"

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
            }
        }

        //напруга
        Label {
            id: voltageLabel
            width: voltage.width
            height: voltage.height
            wrapMode: Label.Wrap
           // horizontalAlignment: Qt.AlignHCenter
            anchors.verticalCenter: voltage.verticalCenter
            text: commun_display.Volt.toFixed(1) + "V"
            color: "black"
        }
    }




    Stick{
        id: leftPylt_stick
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 120
        rotation: 90
    }
    Stick{
        id: rightPylt_stick
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 120
        rotation: 90
    }

}
