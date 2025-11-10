
import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Window 2.0
import QtQuick.Controls.Material

Page {
    id: page

    property int butClick : 0
    property int coxaL : 0
    property int coxaH : 0
    property int femurL : 0
    property int femurH : 0
    property int tibaL : 0
    property int tibaH : 0

    Label {
        id: senderPageLabel
        anchors.top: parent.top
        width: parent.width
        wrapMode: Label.Wrap
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Qt.AlignHCenter
        text: commun_display.curDeviceName_
    }

    Label {
        id: warring
        anchors.top: senderPageLabel.bottom
        width: parent.width
        wrapMode: Label.Wrap
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Qt.AlignHCenter
        text: "Не меняйте настройки, если не уверены!"
        color: "red"
        font.pointSize: 16
    }

    Rectangle {
        id: senderBackground
        anchors.top: warring.bottom
        color: "#f7ebee"
        width: parent.width
        height: parent.height * 0.2
    }

    ScrollView {
        id: scrolViewLogArea
        anchors.fill: senderBackground
        ScrollBar.vertical.policy: ScrollBar.AlwaysOff
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.interactive: true

        ListView {
               id: listView1
               anchors.fill: scrolViewLogArea
               snapMode:ListView.SnapToItem
               width: parent.width
               clip: true

               Connections {
                   target: commun_display
                   function onLogJoy(type, msg) {
                       logListModel.append({msg: type + msg})
                       listView1.positionViewAtEnd()
                   }
               }
               delegate: Column {
                   Text {
                       text: msg
                       font.family: "Courier New"
                       font.pixelSize: 14
                       color: "Orange"
                   }
                }
               // Сама модель, в которой будут содержаться все элементы
               model: ListModel {
                   id: logListModel // задаём ей id для обращения
               }
           }
    }

    Button {
        id: clearButton_1
        width: senderBackground.width * 0.08
        height: senderBackground.height
        anchors.top: senderBackground.top
        anchors.right: senderBackground.right

        contentItem: Text{
            text: "Clear"
            font.pointSize: 6
        }

        onClicked: {
            logListModel.clear()
        }
    }



    SwipeView {
        id: swipeView

        anchors.top: senderBackground.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        currentIndex: pageIndicator.currentIndex

        //страница основных настроек
        Item {
            id: page_settings

            Label {
                id: label_1
                anchors.top: page_settings.top
                width: page_settings.width
                wrapMode: Label.Wrap
                anchors.horizontalCenter: page_settings.horizontalCenter
                horizontalAlignment: Qt.AlignHCenter
                text: "окно основных настроек"
            }

            //////////система/////////////
           Button {
                id: get_err
                anchors.top: label_1.bottom
                anchors.topMargin: 5
                width: page_settings.width
                height: parent.height * 0.08
                text: "запрос ошибок"
                background: Rectangle{
                    property var normalColor: "#c759d4"
                    property var pressedColor: "#5813a1"
                    color: get_err.pressed ? pressedColor : normalColor
                }
                onClicked: {
                    getcurtimer.running = false
                    tx_commands.getCheck();
                    listView1.positionViewAtEnd()
                    getcurtimer.running = true
                }
            }

             Button {
              id: clear_err
              anchors.top: get_err.bottom
              width: page_settings.width
              height: page_settings.height * 0.08
              text: "стереть ошибки"
              background: Rectangle{
                  property var normalColor: "#c759d4"
                  property var pressedColor: "#5813a1"
                  color: clear_err.pressed ? pressedColor : normalColor
              }
               onClicked: {
                   getcurtimer.running = false
                  tx_commands.checkReset();
                  listView1.positionViewAtEnd()
                   getcurtimer.running = true
               }
            }

            Button {
              id: reboot
              anchors.top: clear_err.bottom
              width: page_settings.width
              height: page_settings.height * 0.08
              text: "перезагрузка шара"
              background: Rectangle{
                  property var normalColor: "#c759d4"
                  property var pressedColor: "#5813a1"
                  color: reboot.pressed ? pressedColor : normalColor
              }
               onClicked: {
                   getcurtimer.running = false
                  tx_commands.reboot();
                  listView1.positionViewAtEnd()
                   getcurtimer.running = true
               }
            }

           Button {
                id: get_full_param
                anchors.top: reboot.bottom
                width: page_settings.width
                height: page_settings.height * 0.08
                text: "запрос точки восстановления"
                background: Rectangle{
                    property var normalColor: "#c759d4"
                    property var pressedColor: "#5813a1"
                    color: get_full_param.pressed ? pressedColor : normalColor
                }
                onClicked: {
                    getcurtimer.running = false
                    tx_commands.readAllParams();
                    listView1.positionViewAtEnd()
                    getcurtimer.running = true
                }
            }

          Button {
                id: set_full_param
                anchors.top: get_full_param.bottom
                width: page_settings.width
                height: page_settings.height * 0.08
                text: "восстановить параметры"
                background: Rectangle{
                    property var normalColor: "#c759d4"
                    property var pressedColor: "#5813a1"
                    color: set_full_param.pressed ? pressedColor : normalColor
                }
                onClicked: {
                    getcurtimer.running = false
                    tx_commands.writeAllParams();
                    listView1.positionViewAtEnd()
                    getcurtimer.running = true
                }
            }


            //////////ток/////////////
              Button {
              id: cur_calibr_auto
              anchors.top: set_full_param.bottom
              width: page_settings.width
              height: page_settings.height * 0.08
              text: "автокалибровка датчика тока"
              background: Rectangle{
                  property var normalColor: "#d66364"
                  property var pressedColor: "#800909"
                  color: cur_calibr_auto.pressed ? pressedColor : normalColor
              }
               onClicked: {
                   getcurtimer.running = false
                  tx_commands.curAutoCalibration();
                  listView1.positionViewAtEnd()
                   getcurtimer.running = true
               }
            }

            //////////сервы/////////////
            Button {
              id: servscalibr
              anchors.top: cur_calibr_auto.bottom
              width: page_settings.width
              height: page_settings.height * 0.08
              text: "автокалибровка серв"
              background: Rectangle{
                  property var normalColor: "#1fc2b2"
                  property var pressedColor: "#17d47f"
                  color: servscalibr.pressed ? pressedColor : normalColor
              }
               onClicked: {
                   getcurtimer.running = false
                  tx_commands.servoAutoCalibration();
                  listView1.positionViewAtEnd()
                   getcurtimer.running = true
               }
             }

            Button {
                id: serv_off_but
                anchors.top: servscalibr.bottom
                width: page_settings.width * 0.47
                height: page_settings.height * 0.08
                anchors.left: page_settings.left
                text: "откл. сервы"
                background: Rectangle{
                    property var normalColor: "#7387d1"
                    property var pressedColor: "#052bb3"
                    color: serv_off_but.pressed ? pressedColor : normalColor
                }
                onClicked: {
                    getcurtimer.running = false
                    tx_commands.on_offServo(0x00);
                    listView1.positionViewAtEnd()
                    getcurtimer.running = true
                }
            }

            Button {
                id: serv_on_but
                anchors.top: servscalibr.bottom
                width: page_settings.width * 0.47
                height: page_settings.height * 0.08
                anchors.right: parent.right
                text: "вкл. сервы"
                background: Rectangle{
                    property var normalColor: "#7387d1"
                    property var pressedColor: "#052bb3"
                    color: serv_on_but.pressed ? pressedColor : normalColor
                }
                onClicked: {
                    getcurtimer.running = false
                    tx_commands.on_offServo(0x01);
                    listView1.positionViewAtEnd()
                    getcurtimer.running = true
                }
            }
        }

        //страница калибровок углов конечностей
        Item {
            id: page_setting_foot

            Timer {
                id: getcurtimer
                interval: 250; running: true; repeat: true
                onTriggered: {
                    tx_commands.current_read(false);
                }
            }

            Label {
                id: label_2
                anchors.top: page_setting_foot.top
                width: page_setting_foot.width
                wrapMode: Label.Wrap
                anchors.horizontalCenter: page_setting_foot.horizontalCenter
                horizontalAlignment: Qt.AlignHCenter
                text: "окно корректировки 'домашних' углов конечностей"
            }

            Button {
                id: get_foot_param
                anchors.top: label_2.bottom
                anchors.left: page_setting_foot.left
                width: page_setting_foot.width * 0.5
                height: page_setting_foot.height * 0.075
                text: "Прочитать парам."
                visible: false

                onClicked: {
                    getcurtimer.running = false
                    tx_commands.getParamServsFoot(butClick);
                    listView1.positionViewAtEnd()
                    getcurtimer.running = true
                }
            }

            Button {
                id: set_foot_param
                anchors.top: label_2.bottom
                width: page_setting_foot.width * 0.5
                anchors.right: parent.right
                height: page_setting_foot.height * 0.08
                text: "Сохранить парам."
                visible: false
                background: Rectangle{
                    property var normalColor: "orange"
                    property var pressedColor: "green"
                    color: set_foot_param.pressed ? pressedColor : normalColor
                }

                onClicked: {
                    getcurtimer.running = false
                    coxaH = settParam.coxaAngl >> 8 & 0x00ff
                    coxaL = settParam.coxaAngl & 0x00ff
                    femurH = settParam.femurAngl >> 8 & 0x00ff
                    femurL = settParam.femurAngl & 0x00ff
                    tibaH = settParam.tibaAngl >> 8 & 0x00ff
                    tibaL = settParam.tibaAngl & 0x00ff

                    tx_commands.writeHomeAngleFoot(butClick + " " + coxaL + " " + coxaH + " " + femurL + " " + femurH + " " + tibaL + " " + tibaH);

                    listView1.positionViewAtEnd()
                    getcurtimer.running = true
                }
            }

            //ток
            ProgressBar{
                id: current
                height: 20
                anchors.left: page_setting_foot.left
                anchors.right: page_setting_foot.right
                anchors.top: set_foot_param.bottom
                from: 0
                to: 5

                background: Rectangle {
                    color: "#e6e6e6"
                    radius: 3
                }

                contentItem: Item {
                    Rectangle {
                        width: current.visualPosition * page_setting_foot.width
                        height: current.height
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
                Connections {
                    target: commun_display
                   function onCurRealChanged(Cur) {
                        current.value = Cur
                    }
                }
            }

            Label {
                id: labelcur
                width: page_setting_foot.width
                wrapMode: Label.Wrap
                horizontalAlignment: Qt.AlignHCenter
                anchors.verticalCenter: current.verticalCenter
                text: "Ток потребления" + "   " + commun_display.Cur.toFixed(2) + "А"
            }


            Label {
                id: label1
                anchors.top: labelcur.bottom
                anchors.topMargin: 5
                width: page_setting_foot.width
                wrapMode: Label.Wrap
                anchors.horizontalCenter: page_setting_foot.horizontalCenter
                horizontalAlignment: Qt.AlignHCenter
                text: "Какую ногу калибруем?"
            }

           RadioButton{
               id: box1
               anchors.left: page_setting_foot.left
               anchors.top: label1.bottom
               text: "1"
               onClicked: {
                   getcurtimer.running = false
                   get_foot_param.visible = true
                   set_foot_param.visible = true
                   butClick = 0
                   tx_commands.getParamServsFoot(butClick);
                   getcurtimer.running = true
               }
           }
           RadioButton{
               id: box2
               anchors.left: box1.right
               anchors.top: label1.bottom
               anchors.leftMargin: 10
               text: "2"
               onClicked: {
                   getcurtimer.running = false
                   get_foot_param.visible = true
                   set_foot_param.visible = true
                   butClick = 1
                   tx_commands.getParamServsFoot(butClick);
                   getcurtimer.running = true
               }
           }
           RadioButton{
               id: box3
               anchors.left: box2.right
               anchors.top: label1.bottom
               anchors.leftMargin: 10
               text: "3"
               onClicked: {
                   getcurtimer.running = false
                   get_foot_param.visible = true
                   set_foot_param.visible = true
                   butClick = 2
                   tx_commands.getParamServsFoot(butClick);
                   getcurtimer.running = true
               }
           }
           RadioButton{
               id: box4
               anchors.right: box5.left
               anchors.top: label1.bottom
               anchors.rightMargin: 10
               text: "4"
               onClicked: {
                   getcurtimer.running = false
                   get_foot_param.visible = true
                   set_foot_param.visible = true
                   butClick = 3
                   tx_commands.getParamServsFoot(butClick);
                   getcurtimer.running = true
               }
           }
           RadioButton{
               id: box5
               anchors.right: box6.left
               anchors.top: label1.bottom
               anchors.rightMargin: 10
               text: "5"
               onClicked: {
                   getcurtimer.running = false
                   get_foot_param.visible = true
                   set_foot_param.visible = true
                   butClick = 4
                   tx_commands.getParamServsFoot(butClick);
                   getcurtimer.running = true
               }
           }
           RadioButton{
               id: box6
               anchors.right: parent.right
               anchors.top: label1.bottom
               text: "6"
               onClicked: {
                   getcurtimer.running = false
                   get_foot_param.visible = true
                   set_foot_param.visible = true
                   butClick = 5
                   tx_commands.getParamServsFoot(butClick);
                   getcurtimer.running = true
               }
           }


           //coxa
           Button {
               id: left_left_coxa
               anchors.top: box1.bottom
               anchors.left: page_setting_foot.left
               width: page_setting_foot.width * 0.12
               height: page_setting_foot.height * 0.08
               background: Rectangle{
                   property var normalColor: "#12c98c"
                   property var pressedColor: "#0c7854"
                   color: left_left_coxa.pressed ? pressedColor : normalColor
               }
               text: "<<"
               onClicked: {
                   getcurtimer.running = false
                   if(settParam.coxaAngl >= 10)
                   {
                       settParam.coxaAngl = settParam.coxaAngl - 10
                       coxaH = settParam.coxaAngl >> 8 & 0x00ff
                       coxaL = settParam.coxaAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 0) + " " + coxaL + " " + coxaH);
                   }
                    getcurtimer.running = true
               }
           }

           Button {
               id: left_coxa
               anchors.top: left_left_coxa.top
               anchors.left: left_left_coxa.right
               anchors.leftMargin: 5
               width: page_setting_foot.width * 0.12
               height: page_setting_foot.height * 0.08
               background: Rectangle{
                   property var normalColor: "#12c98c"
                   property var pressedColor: "#0c7854"
                   color: left_coxa.pressed ? pressedColor : normalColor
               }
               text: "<"
               onClicked: {
                   getcurtimer.running = false
                   if(settParam.coxaAngl >= 1)
                   {
                       settParam.coxaAngl = settParam.coxaAngl - 1
                       coxaH = settParam.coxaAngl >> 8 & 0x00ff
                       coxaL = settParam.coxaAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 0) + " " + coxaL + " " + coxaH);
                   }
                    getcurtimer.running = true
               }
           }

           Button {
               id: right_right_coxa
               anchors.top: left_left_coxa.top
               anchors.right: parent.right
               width: page_setting_foot.width * 0.12
               height: page_setting_foot.height * 0.08
               background: Rectangle{
                   property var normalColor: "#12c98c"
                   property var pressedColor: "#0c7854"
                   color: right_right_coxa.pressed ? pressedColor : normalColor
               }
               text: ">>"
               onClicked: {
                   getcurtimer.running = false
                   if(settParam.coxaAngl <= 260)
                   {
                       settParam.coxaAngl = settParam.coxaAngl + 10
                       coxaH = settParam.coxaAngl >> 8 & 0x00ff
                       coxaL = settParam.coxaAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 0) + " " + coxaL + " " + coxaH);
                   }
                    getcurtimer.running = true
               }
           }

           Button {
               id: right_coxa
               anchors.top: right_right_coxa.top
               anchors.right: right_right_coxa.left
               anchors.rightMargin: 5
               width: page_setting_foot.width * 0.12
               height: page_setting_foot.height * 0.08
               background: Rectangle{
                   property var normalColor: "#12c98c"
                   property var pressedColor: "#0c7854"
                   color: right_coxa.pressed ? pressedColor : normalColor
               }
               text: ">"
               onClicked: {
                   getcurtimer.running = false
                   if(settParam.coxaAngl <= 269)
                   {
                       settParam.coxaAngl = settParam.coxaAngl + 1
                       coxaH = settParam.coxaAngl >> 8 & 0x00ff
                       coxaL = settParam.coxaAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 0) + " " + coxaL + " " + coxaH);
                   }
                    getcurtimer.running = true
               }
           }


           Label {
               id: label2
               width: page_setting_foot.width
               anchors.verticalCenter: left_left_coxa.verticalCenter
               wrapMode: Label.Wrap
               horizontalAlignment: Qt.AlignHCenter
               text: "угол сервы coxa" + "      " + Math.round(settParam.coxaAngl)
           }

           Slider {
               id: slider1
               anchors.top: left_left_coxa.top                  ///??
               value: settParam.coxaAngl
               from: 0
               to: 270
               anchors.left: parent.left
               anchors.right: parent.right
               anchors.topMargin: left_left_coxa.height * 0.5       ///??
               onMoved: {settParam.coxaAngl = value}


               Component.onCompleted: {
                   settParam.coxaAnglChanged.connect(modelChanged)
               }

                onPressedChanged: {
                    if(!pressed)
                    {
                        getcurtimer.running = false
                       coxaH = settParam.coxaAngl >> 8 & 0x00ff
                       coxaL = settParam.coxaAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 0) + " " + coxaL + " " + coxaH);
                        getcurtimer.running = true
                    }
               }

               function modelChanged(){
                   this.value = value
               }
           }


           //femur
           Button {
               id: left_left_femur
               anchors.top: left_left_coxa.bottom
               anchors.left: page_setting_foot.left
               anchors.topMargin: left_left_coxa.height * 0.9
               width: page_setting_foot.width * 0.12
               height: parent.height * 0.08
               background: Rectangle{
                   property var normalColor: "#a0cf13"
                   property var pressedColor: "#70910a"
                   color: left_left_femur.pressed ? pressedColor : normalColor
               }
               text: "<<"
               onClicked: {
                   getcurtimer.running = false
                   if(settParam.femurAngl >= 10)
                   {
                       settParam.femurAngl = settParam.femurAngl - 10
                       femurH = settParam.femurAngl >> 8 & 0x00ff
                       femurL = settParam.femurAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 1) + " " + femurL + " " + femurH);
                   }
                    getcurtimer.running = true
               }
           }

           Button {
               id: left_femur
               anchors.top: left_left_femur.top
               anchors.left: left_left_femur.right
               anchors.leftMargin: 5
               width: page_setting_foot.width * 0.12
               height: parent.height * 0.08
               background: Rectangle{
                   property var normalColor: "#a0cf13"
                   property var pressedColor: "#70910a"
                   color: left_femur.pressed ? pressedColor : normalColor
               }
               text: "<"
               onClicked: {
                   getcurtimer.running = false
                   if(settParam.femurAngl >= 1)
                   {
                       settParam.femurAngl = settParam.femurAngl - 1
                       femurH = settParam.femurAngl >> 8 & 0x00ff
                       femurL = settParam.femurAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 1) + " " + femurL + " " + femurH);
                   }
                    getcurtimer.running = true
               }
           }

           Button {
               id: right_right_femur
               anchors.top: left_left_femur.top
               anchors.right: parent.right
               width: page_setting_foot.width * 0.12
               height: parent.height * 0.08
               background: Rectangle{
                   property var normalColor: "#a0cf13"
                   property var pressedColor: "#70910a"
                   color: right_right_femur.pressed ? pressedColor : normalColor
               }
               text: ">>"
               onClicked: {
                   getcurtimer.running = false
                   if(settParam.femurAngl <= 260)
                   {
                       settParam.femurAngl = settParam.femurAngl + 10
                       femurH = settParam.femurAngl >> 8 & 0x00ff
                       femurL = settParam.femurAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 1) + " " + femurL + " " + femurH);
                   }
                    getcurtimer.running = true
               }
           }

           Button {
               id: right_femur
               anchors.top: right_right_femur.top
               anchors.right: right_right_femur.left
               anchors.rightMargin: 5
               width: page_setting_foot.width * 0.12
               height: parent.height * 0.08
               background: Rectangle{
                   property var normalColor: "#a0cf13"
                   property var pressedColor: "#70910a"
                   color: right_femur.pressed ? pressedColor : normalColor
               }
               text: ">"
               onClicked: {
                   getcurtimer.running = false
                   if(settParam.femurAngl <= 269)
                   {
                       settParam.femurAngl = settParam.femurAngl + 1
                       femurH = settParam.femurAngl >> 8 & 0x00ff
                       femurL = settParam.femurAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 1) + " " + femurL + " " + femurH);
                   }
                    getcurtimer.running = true
               }
           }

           Label {
               id: label3
               width: page_setting_foot.width
               wrapMode: Label.Wrap
               horizontalAlignment: Qt.AlignHCenter
               anchors.verticalCenter: left_left_femur.verticalCenter
               text: "угол сервы femur" + "      " + Math.round(settParam.femurAngl)
           }

           Slider {
               id: slider2
               anchors.top: left_left_femur.top
               anchors.topMargin: left_left_femur.height * 0.5
               value: settParam.femurAngl
               from: 0
               to: 270
               anchors.left: parent.left
               anchors.right: parent.right
               onMoved: {settParam.femurAngl = value}

               Component.onCompleted: {
                   settParam.femurAnglChanged.connect(modelChanged)
               }

               onPressedChanged: {
                   if(!pressed)
                   {
                       getcurtimer.running = false
                       femurH = settParam.femurAngl >> 8 & 0x00ff
                       femurL = settParam.femurAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 1) + " " + femurL + " " + femurH);
                       getcurtimer.running = true
                   }
              }

               function modelChanged(){
                   this.value = value
               }
           }

           //tiba
           Button {
               id: left_left_tiba
               anchors.top: left_left_femur.bottom
               anchors.left: page_setting_foot.left
               anchors.topMargin: left_left_tiba.height * 0.9
               width: page_setting_foot.width * 0.12
               height: page_setting_foot.height * 0.08
               background: Rectangle{
                   property var normalColor: "#11a9d4"
                   property var pressedColor: "#0c6f8a"
                   color: left_left_tiba.pressed ? pressedColor : normalColor
               }
               text: "<<"
               onClicked: {
                   getcurtimer.running = false
                   if(settParam.tibaAngl >= 10)
                   {
                       settParam.tibaAngl = settParam.tibaAngl - 10
                       tibaH = settParam.tibaAngl >> 8 & 0x00ff
                       tibaL = settParam.tibaAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 2) + " " + tibaL + " " + tibaH);
                   }
                    getcurtimer.running = true
               }
           }

           Button {
               id: left_tiba
               anchors.top: left_left_tiba.top
               anchors.left: left_left_tiba.right
               anchors.leftMargin: 5
               width: page_setting_foot.width * 0.12
               height: page_setting_foot.height * 0.08
               background: Rectangle{
                   property var normalColor: "#11a9d4"
                   property var pressedColor: "#0c6f8a"
                   color: left_tiba.pressed ? pressedColor : normalColor
               }
               text: "<"
               onClicked: {
                   getcurtimer.running = false
                   if(settParam.tibaAngl >= 1)
                   {
                       settParam.tibaAngl = settParam.tibaAngl - 1
                       tibaH = settParam.tibaAngl >> 8 & 0x00ff
                       tibaL = settParam.tibaAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 2) + " " + tibaL + " " + tibaH);
                   }
                    getcurtimer.running = true
               }
           }

           Button {
               id: right_right_tiba
               anchors.top: left_left_tiba.top
               anchors.right: parent.right
               width: page_setting_foot.width * 0.12
               height: page_setting_foot.height * 0.08
               background: Rectangle{
                   property var normalColor: "#11a9d4"
                   property var pressedColor: "#0c6f8a"
                   color: right_right_tiba.pressed ? pressedColor : normalColor
               }
               text: ">>"
               onClicked: {
                   getcurtimer.running = false
                   if(settParam.tibaAngl <= 260)
                   {
                       settParam.tibaAngl = settParam.tibaAngl + 10
                       tibaH = settParam.tibaAngl >> 8 & 0x00ff
                       tibaL = settParam.tibaAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 2) + " " + tibaL + " " + tibaH);
                   }
                    getcurtimer.running = true
               }
           }

           Button {
               id: right_tiba
               anchors.top: right_right_tiba.top
               anchors.right: right_right_tiba.left
               anchors.rightMargin: 5
               width: page_setting_foot.width * 0.12
               height: page_setting_foot.height * 0.08
               background: Rectangle{
                   property var normalColor: "#11a9d4"
                   property var pressedColor: "#0c6f8a"
                   color: right_tiba.pressed ? pressedColor : normalColor
               }
               text: ">"
               onClicked: {
                   getcurtimer.running = false
                   if(settParam.tibaAngl <= 269)
                   {
                       settParam.tibaAngl = settParam.tibaAngl + 1
                       tibaH = settParam.tibaAngl >> 8 & 0x00ff
                       tibaL = settParam.tibaAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 2) + " " + tibaL + " " + tibaH);
                   }
                    getcurtimer.running = true
               }
           }

           Label {
               id: label4
               width: page_setting_foot.width
               wrapMode: Label.Wrap
               horizontalAlignment: Qt.AlignHCenter
               anchors.verticalCenter: left_left_tiba.verticalCenter
               text: "угол сервы tiba" + "      " + Math.round(settParam.tibaAngl)
           }


           Slider {
               id: slider3
               anchors.top: left_left_tiba.top
               anchors.topMargin: left_left_tiba.height * 0.5
               value: settParam.tibaAngl
               from: 0
               to: 270
               anchors.left: parent.left
               anchors.right: parent.right
               onMoved: {settParam.tibaAngl = value}

               Component.onCompleted: {
                   settParam.tibaAnglChanged.connect(modelChanged)
               }

               onPressedChanged: {
                   if(!pressed)
                   {
                       getcurtimer.running = false
                       tibaH = settParam.tibaAngl >> 8 & 0x00ff
                       tibaL = settParam.tibaAngl & 0x00ff

                       tx_commands.setServoAnglee((butClick * 3 + 2) + " " + tibaL + " " + tibaH);
                       getcurtimer.running = true
                   }
              }

               function modelChanged(){
                   this.value = value
               }
           }

           Button {
               id: reset_foot_param
               anchors.bottom: parent.bottom
               width: page_setting_foot.width
               height: page_setting_foot.height * 0.08
               text: "Reset foot param!"
               background: Rectangle{
                   property var normalColor: "#e31041"
                   property var pressedColor: "#f026dc"
                   color: reset_foot_param.pressed ? pressedColor : normalColor
               }

               onClicked: {
                   getcurtimer.running = false
                   tx_commands.resetServsFoot(butClick);
                   listView1.positionViewAtEnd()
                   getcurtimer.running = true
               }
           }
            }

    }
    PageIndicator {
        id: pageIndicator
        interactive: true
        count: swipeView.count
        currentIndex: swipeView.currentIndex
        anchors.bottom: swipeView.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: parent.height*0.06
    }

}


