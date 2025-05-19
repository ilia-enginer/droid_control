import QtQuick 2.1
import QtQuick.Controls


    Rectangle {
        id: joystick_root
        width: parent.width
        height: parent.height
        color: "transparent"

        property bool flag_getcheck : false
        property bool verticalOnly : false
        property bool horizontalOnly : false
        property real offsetX : 0
        property real offsetY : 0
        property real azimuth : 0
        property real amplitude : 0
        property int mode : 0
        property real level : mainModel.heightAmplitude/2

        signal joystick_moved(double x, double y);


        Timer {
            id: joystick_timer
            interval: mainModel.timer1
            running: true
            repeat: true
            onTriggered: {
                if(!flag_getcheck)
                {
                    flag_getcheck = true;
                    device.get_check();
                }
                device.onJoysticActivity(mode, azimuth, amplitude, level);
            }
        }


        //окно лога
        Rectangle {
            id: senderBackground_2
            anchors.top: parent.top
            height: parent.height * 0.25
            anchors.left: tumbler.left
            anchors.right: leveltext.right
            color: "transparent"
        }

        ScrollView {
            id: scrolViewLogArea_2
            anchors.fill: senderBackground_2
            ScrollBar.vertical.policy: ScrollBar.AlwaysOff
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.interactive: true

            ListView {
                   id: listView1
                   anchors.fill: scrolViewLogArea_2
                   snapMode:ListView.SnapToItem
                   width: scrolViewLogArea_2.width
                   clip: true

                   Connections {
                       target: device
                       onLogJoy: {
                           logListModel_2.append({msg: type + msg})
                           listView1.positionViewAtEnd()
                       }

//                         function  onLogJoy() {
//                              logListModel_2.append({msg: type + msg})
//                              listView1.positionViewAtEnd()
//                          }
                   }

                   delegate: Column {
                       Text {
                           text: msg
                           font.family: "transparent"
                           font.pixelSize: 14
                           color: "orange"
                       }
                    }

                   // Сама модель, в которой будут содержаться все элементы
                   model: ListModel {
                       id: logListModel_2 // задаём ей id для обращения
                   }
               }
        }


        //напруга
//        ProgressBar{
//            id: voltage
//            height: 20
//            anchors.bottom: joystick.top
//            anchors.bottomMargin: 20
//            anchors.left: tumbler.left
//            anchors.right: leveltext.right

//            from: {mainModel.Vmin}
//            to: {mainModel.Vmax}
//            value: {mainModel.Vreal}
//        }
        ProgressBar{
            id: voltage
            height: 20
            anchors.top: senderBackground_2.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.right: parent.right

            from: {mainModel.Vmin}
            to: {mainModel.Vmax}
            value: {mainModel.Vreal}

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
        }

        //напруга
        Label {
            id: voltageLabel
            width: parent.width
            height: voltage.height
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            anchors.verticalCenter: voltage.verticalCenter
            text: mainModel.Vreal.toFixed(1) + "V"
            color: "black"
        }

        //ток
        ProgressBar{
            id: current
            height: 20
            anchors.top: voltage.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.right: parent.right

            from: 0
            to: 5
            value: {mainModel.CurReal}

//            Component.onCompleted: {
//                if(mainModel.adiminTapCount === -1)
//                {
//                    current.visible = true
//                }
//                else
//                {
//                    current.visible = false
//                }
//           }

            background: Rectangle {
                color: "#e6e6e6"
                radius: 3
            }

            contentItem: Item {
                Rectangle {
                    width: current.visualPosition * parent.width
                    height: parent.height
                    radius: 2
                    color: "#17a81a"

                    gradient: Gradient {
                        GradientStop {
                            position: 0.0
                            SequentialAnimation on color {
                                loops: Animation.Infinite
                                ColorAnimation { from: "#d6e014"; to: "#b39812"; duration: 5000 }
                                ColorAnimation { from: "#b39812"; to: "#d6e014"; duration: 5000 }
                            }
                        }
                        GradientStop {
                            position: 1.0
                            SequentialAnimation on color {
                                loops: Animation.Infinite
                                ColorAnimation { from: "#ed4811"; to: "#a1330e"; duration: 5000 }
                                ColorAnimation { from: "#a1330e"; to: "#ed4811"; duration: 5000 }
                            }
                        }
                    }
                }
            }
        }

        //ток
        Label {
            id: currentLabel
            width: parent.width
            height: current.height
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            anchors.verticalCenter: current.verticalCenter
            text: mainModel.CurReal.toFixed(2) + "А"
            color: "black"

//            Component.onCompleted: {
//                if(mainModel.adiminTapCount === -1)
//                {
//                    currentLabel.visible = true
//                }
//                else
//                {
//                    currentLabel.visible = false
//                }
//           }
        }

        //имя приемника
        Label {
            id: senderPageLabel
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            anchors.top: current.bottom
            anchors.topMargin: 10
            text: mainModel.currentDeviceName
        }

        Image {
            id: joystick
            property real angle : 0
            property real distance : 0

            source: "background.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: senderPageLabel.bottom
            anchors.topMargin: 10

            ParallelAnimation {
                id: returnAnimation
                NumberAnimation { target: thumb.anchors; property: "horizontalCenterOffset";
                    to: 0; duration: 200; easing.type: Easing.OutSine }
                NumberAnimation { target: thumb.anchors; property: "verticalCenterOffset";
                    to: 0; duration: 200; easing.type: Easing.OutSine }
            }

            Image {
                id: thumb
                source: "finger.png"
                anchors.centerIn: parent
            }
        }

        MultiPointTouchArea{
            id: mouse_touch
            property real mouseX2 : verticalOnly ? width * 0.5 : point1.x //mouseX
            property real mouseY2 : horizontalOnly ? height * 0.5 : point1.y //mouseY
            property real fingerAngle : Math.atan2(mouseX2, mouseY2)
            property int mcx : mouseX2 - width * 0.5
            property int mcy : mouseY2 - height * 0.5
            property bool fingerInBounds : fingerDistance2 < distanceBound2
            property real fingerDistance2 : mcx * mcx + mcy * mcy
            property real distanceBound : width * 0.5 - thumb.width * 0.5
            property real distanceBound2 : distanceBound * distanceBound
            property double signal_x : (mouseX2 - joystick.width/2) / distanceBound
            property double signal_y : -(mouseY2 - joystick.height/2) / distanceBound

            anchors.fill: joystick
            minimumTouchPoints: 1
            maximumTouchPoints: 5
            touchPoints: [
                TouchPoint {id: point1 },
                TouchPoint {id: point2 },
                TouchPoint {id: point3 },
                TouchPoint {id: point4 },
                TouchPoint {id: point5 }]

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
            onUpdated: {
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
            id: joystick_buttons

            property real angle : 0
            property real distance : 0

            width: 160
            height: 160
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: senderPageLabel.bottom
            anchors.topMargin: 10
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
            from: mainModel.heightAmplitudemin
            to: mainModel.heightAmplitude
            orientation: Qt.Vertical
            value: (((mainModel.heightAmplitude - mainModel.heightAmplitudemin) / 2) + mainModel.heightAmplitudemin)
            anchors.left: joystick.right
            anchors.leftMargin: 40
            anchors.verticalCenter: joystick.verticalCenter
            onMoved: {level = value}  
            Component.onCompleted: {
                            level = value
                        }
        }

        Label {
            id: leveltext
            horizontalAlignment: Qt.AlignHCenter
            anchors.top: joystick.bottom
            anchors.left: joystick.right
            anchors.leftMargin: 30
            anchors.topMargin: 20
            text: "Высота"
        }

        Label {
            id: modetext
            anchors.right: joystick.left
            anchors.rightMargin: 30
            anchors.top: joystick.bottom
            anchors.topMargin: 20
            text: "Режим\nработы"
        }

        Tumbler {
            id: tumbler
            model: ["1", "2", "3", "0"]
            anchors.right: joystick.left
            anchors.rightMargin: 20
            anchors.verticalCenter: joystick.verticalCenter
            onCurrentIndexChanged: {
                mode = Number(model[currentIndex])
            }
        }


        Label {
            id: offsetInfo
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            anchors.top: joystick.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: joystick.horizontalCenter
            text: "0/0"
            Component.onCompleted: {
                if(mainModel.adiminTapCount === -1)
                {
                    offsetInfo.visible = true
                }
                else
                {
                    offsetInfo.visible = false
                }

           }
        }

        Switch {
            id: joystick_mode_swith
            text: "Джойстик"
            checked: true
            anchors.top: offsetInfo.bottom
            anchors.topMargin: 20
            anchors.horizontalCenter: joystick.horizontalCenter
            onCheckedChanged:  {
               joystick.visible = checked
               joystick_buttons.visible = !checked

                if(mainModel.adiminTapCount === -1)
                {
                    offsetInfo.visible = checked
                }
            }
        }


    }


