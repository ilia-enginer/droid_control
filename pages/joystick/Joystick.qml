import QtQuick 2.1
import QtQuick.Controls


    Rectangle {
        id: joystick_root
        width: joystick.width
        height: joystick.height
        color: "transparent"
        anchors.centerIn: parent
        property bool verticalOnly : false
        property bool horizontalOnly : false

        property real offsetX : 0
        property real offsetY : 0

        property real azimuth : 0
        property real amplitude : 0
        property int mode : 0
        property real level : 0.5



        signal joystick_moved(double x, double y);

        Label {
            id: senderPageLabel
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            anchors.bottom: joystick.top
            anchors.bottomMargin: 50
            text: mainModel.currentDeviceName
        }

        Timer {
            id: joystick_timer
            interval: mainModel.timer1; running: true; repeat: true
            onTriggered: {
                device.onJoysticActivity(mode, azimuth, amplitude, level);
            }
        }

        Switch {
            id: joystick_mode_swith
            text: "Джойстик"
            checked: true
            anchors.top: offsetInfo.bottom
            anchors.topMargin: 60
            anchors.horizontalCenter: joystick.horizontalCenter

            onCheckedChanged:  {
               joystick.visible = checked
               offsetInfo.visible = checked
               joystick_buttons.visible = !checked
            }
        }

        Label {
            id: offsetInfo
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            anchors.top: joystick.bottom
            anchors.horizontalCenter: joystick.horizontalCenter
            text: "0/0"
        }

        Image {
            id: joystick

            property real angle : 0
            property real distance : 0

            source: "background.png"
            anchors.centerIn: parent

            ParallelAnimation {
                id: returnAnimation
                NumberAnimation { target: thumb.anchors; property: "horizontalCenterOffset";
                    to: 0; duration: 200; easing.type: Easing.OutSine }
                NumberAnimation { target: thumb.anchors; property: "verticalCenterOffset";
                    to: 0; duration: 200; easing.type: Easing.OutSine }
            }

            MouseArea {
                id: mouse
                property real mouseX2 : verticalOnly ? width * 0.5 : mouseX
                property real mouseY2 : horizontalOnly ? height * 0.5 : mouseY
                property real fingerAngle : Math.atan2(mouseX2, mouseY2)
                property int mcx : mouseX2 - width * 0.5
                property int mcy : mouseY2 - height * 0.5
                property bool fingerInBounds : fingerDistance2 < distanceBound2
                property real fingerDistance2 : mcx * mcx + mcy * mcy
                property real distanceBound : width * 0.5 - thumb.width * 0.5
                property real distanceBound2 : distanceBound * distanceBound

                property double signal_x : (mouseX2 - joystick.width/2) / distanceBound
                property double signal_y : -(mouseY2 - joystick.height/2) / distanceBound

                anchors.fill: parent

                onPressed: {
                    returnAnimation.stop();
                }

                onReleased: {
                    returnAnimation.restart()
                    joystick_moved(0, 0);
                    offsetX = 0
                    offsetY = 0
                    amplitude = 0
                    azimuth = 0
                    offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
                }

                onPositionChanged: {
                    if (fingerInBounds) {
                        thumb.anchors.horizontalCenterOffset = mcx
                        thumb.anchors.verticalCenterOffset = mcy
                    } else {
                        var angle = Math.atan2(mcy, mcx)
                        thumb.anchors.horizontalCenterOffset = Math.cos(angle) * distanceBound
                        thumb.anchors.verticalCenterOffset = Math.sin(angle) * distanceBound
                    }

                    // Fire the signal to indicate the joystick has moved
                    angle = Math.atan2(signal_y, signal_x)

                    var offX = 0
                    var offY = 0



                    if(fingerInBounds) {
                        offX = verticalOnly ? 0 : Math.cos(angle) * Math.sqrt(fingerDistance2) / distanceBound
                        offY = horizontalOnly ? 0 : Math.sin(angle) * Math.sqrt(fingerDistance2) / distanceBound
                        amplitude  = Math.sqrt(fingerDistance2) / distanceBound
                        azimuth = angle
                    } else {
                        offX = verticalOnly ? 0 : Math.cos(angle) * 1
                        offY = horizontalOnly ? 0 : Math.sin(angle) * 1

                        amplitude  = 1
                        azimuth = angle
                    }

                    joystick_moved(offX, offY)
                    offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
                    offsetX = offX
                    offsetY = offY
                }
            }

            Image {
                id: thumb
                source: "finger.png"
                anchors.centerIn: parent
            }
        }

        Image {
            id: joystick_buttons

            property real angle : 0
            property real distance : 0

            width: 160
            height: 160
            anchors.centerIn: parent
            visible: false

            Button {
                text: "UP"
                highlighted: true
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter

                onPressed: {
                    amplitude = 0.5
                    azimuth = 1.57
                    device.onJoysticActivity(mode, 1.57, 1, level);
                }

                onReleased: {
                    amplitude = 0
                    azimuth = 0
                }
            }
            Button {
                text: "DOWN"
                highlighted: true
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter

                onPressed: {
                    amplitude = 0.5
                    azimuth = -1.57
                    device.onJoysticActivity(mode, -1.57, 1, level);
                }

                onReleased: {
                    amplitude = 0
                    azimuth = 0
                }
            }
            Button {
                text: "LEFT"
                highlighted: true
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter

                onPressed: {
                    amplitude = 0.5
                    azimuth = -3.14
                    device.onJoysticActivity(mode, -3.14, 1, level);
                }

                onReleased: {
                    amplitude = 0
                    azimuth = 0
                }
            }
            Button {
                text: "RIGHT"
                highlighted: true
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                onPressed: {
                    amplitude = 0.5
                    azimuth = 0
                    device.onJoysticActivity(mode, 0, 1, level);
                }

                onReleased: {
                    amplitude = 0
                    azimuth = 0
                }
            }

        }

        Slider {
            id: levelSlider
            orientation: Qt.Vertical
            value: 0.5
            anchors.left: joystick.right
            anchors.leftMargin: 20
            anchors.verticalCenter: joystick.verticalCenter
            onMoved: {level = value}
        }

        Tumbler {
            model: ["1", "2", "3", "4"]
            anchors.right: joystick.left
            anchors.rightMargin: 20
            anchors.verticalCenter: joystick.verticalCenter
            onCurrentIndexChanged: {
                mode = Number(model[currentIndex])
            }
        }
    }


