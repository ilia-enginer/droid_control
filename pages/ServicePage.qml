
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material


Page {
    id: page

//    property int paramIndex: -1

        Label {
            id: senderPageLabel
            anchors.top: parent.top
            width: parent.width
            wrapMode: Label.Wrap
       //     anchors.horizontalCenter: page.horizontalCenter
            horizontalAlignment: Qt.AlignHCenter
            text: mainModel.currentDeviceName
        }

        Rectangle {
            id: senderBackground
//            anchors.fill: parent
            anchors.top: senderPageLabel.bottom
            color: "#111111"
            width: page.width
            height: 150
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
                   width: scrolViewLogArea.width
                   clip: true

                   Connections {
                       target: device
                       onLogServis: {
                           logListModel.append({msg: type + msg})
                           listView1.positionViewAtEnd()
                       }
                   }
                   delegate: Column {
                       Text {
                           width: listView1.width * 0.95
                           text: msg
                           font.family: "Courier New"
                           wrapMode: Text.Wrap
                           font.pixelSize: 14
                           color: "lawngreen"
                       }
                    }
                   // Сама модель, в которой будут содержаться все элементы
                   model: ListModel {
                       id: logListModel // задаём ей id для обращения
                   }
               }
        }

        //очистка окна
        Button {
            id: clearButton_1
            width: senderBackground.width * 0.08
            height: senderBackground.height
            anchors.top: senderBackground.top
            anchors.right: senderBackground.right
            opacity: 0.2
            contentItem: Text{
                text: "Clear"
                font.pointSize: 6
            }

            onClicked: {
                logListModel.clear()
            }
        }

        CheckBox {
          id: loging
          anchors.top: senderBackground.bottom
          checked: device.loging
          text: qsTr("Включить логирование")

          onClicked: {
              device.loging = checked
          }
        }

        Flickable {

                id: scrolViewLogArea_1
                anchors.top: loging.bottom
                width: parent.width
                height: 500
                interactive: true
                clip: true
                flickableDirection: Flickable.VerticalFlick
                contentHeight: 700


                ComboBox {
                    id: paramBox
                    anchors.top: scrolViewLogArea_1.up
                    width: parent.width
                    currentIndex: mainModel.servisIndexMenu

                    model: ListModel {
                           id: model
                           ListElement { text: "..." }
                           ListElement { text: "Система" }
                           ListElement { text: "Сервы" }
                           ListElement { text: "Ток" }
                           ListElement { text: "Напряжение" }
                           ListElement { text: "Акселерометр" }
                           ListElement { text: "Светодиоды" }
                       }
                    onCurrentIndexChanged: {
                        mainModel.servisIndexMenu = paramBox.currentIndex

                            //пустая строка
                            if(currentIndex === 0)
                            {
                                //система
                                get_err.visible                 = false
                                clear_err.visible               = false
                                reboot.visible                  = false
                                get_vers.visible                = false
                                set_signed.visible              = false
                                resetButton.visible             = false
                                get_full_param.visible          = false
                                set_full_param.visible          = false

                                //сервы
                                servs_calibr.visible            = false
                                serv_start.visible              = false
                                serv_home.visible               = false
                                get_install_serv_num.visible    = false
                                get_install_serv_num_but.visible = false
                                get_serv_angl.visible           = false
                                get_serv_angl_but.visible       = false
                                set_serv_angl.visible            = false
                                set_serv_angl_but.visible       = false
                                set_serv_min_angl.visible       = false
                                set_serv_min_angl_but.visible    = false
                                serv_on_but.visible             = false
                                serv_off_but.visible             = false

                                //ток
                                get_cur.visible                 = false
                                get_cur_calibr.visible          = false
                                set_cur_value.visible           = false
                                set_cur_value_but.visible       = false
                                flag_cur_but_on.visible         = false
                                flag_cur_but_off.visible        = false
                                cur_calibr_auto.visible         = false

                                //напряжение
                                get_voltage.visible             = false

                                //акселерометр
                                get_angl_giroscope.visible      = false
                                get_acsel.visible               = false

                                //светодиоды
                                get_led.visible                 = false
                                get_led_but.visible             = false
                                set_led.visible                 = false
                                set_led_but.visible             = false
                            }
                            //Система
                            else if (currentIndex === 1)
                            {
                                //система
                                get_err.visible                 = true
                                clear_err.visible               = true
                                reboot.visible                  = true
                                get_vers.visible                = true
                                set_signed.visible              = true
                                resetButton.visible             = true
                                get_full_param.visible          = true
                                set_full_param.visible          = true

                                //сервы
                                servs_calibr.visible            = false
                                serv_start.visible              = false
                                serv_home.visible               = false
                                get_install_serv_num.visible    = false
                                get_install_serv_num_but.visible = false
                                get_serv_angl.visible           = false
                                get_serv_angl_but.visible       = false
                                set_serv_angl.visible            = false
                                set_serv_angl_but.visible       = false
                                set_serv_min_angl.visible       = false
                                set_serv_min_angl_but.visible    = false
                                serv_on_but.visible             = false
                                serv_off_but.visible             = false

                                //ток
                                get_cur.visible                 = false
                                get_cur_calibr.visible          = false
                                set_cur_value.visible           = false
                                set_cur_value_but.visible       = false
                                flag_cur_but_on.visible         = false
                                flag_cur_but_off.visible        = false
                                cur_calibr_auto.visible         = false

                                //напряжение
                                get_voltage.visible             = false

                                //акселерометр
                                get_angl_giroscope.visible      = false
                                get_acsel.visible               = false

                                //светодиоды
                                get_led.visible                 = false
                                get_led_but.visible             = false
                                set_led.visible                 = false
                                set_led_but.visible             = false
                            }
                            //Сервы
                            else if (currentIndex === 2)
                            {
                                //система
                                get_err.visible                 = false
                                clear_err.visible               = false
                                reboot.visible                  = false
                                get_vers.visible                = false
                                set_signed.visible              = false
                                resetButton.visible             = false
                                get_full_param.visible          = false
                                set_full_param.visible          = false

                                //сервы
                                servs_calibr.visible            = true
                                serv_start.visible              = true
                                serv_home.visible               = true
                                get_install_serv_num.visible    = true
                                get_install_serv_num_but.visible = true
                                get_serv_angl.visible           = true
                                get_serv_angl_but.visible       = true
                                set_serv_angl.visible            = true
                                set_serv_angl_but.visible       = true
                                set_serv_min_angl.visible       = true
                                set_serv_min_angl_but.visible    = true
                                serv_on_but.visible             = true
                                serv_off_but.visible             = true

                                //ток
                                get_cur.visible                 = false
                                get_cur_calibr.visible          = false
                                set_cur_value.visible           = false
                                set_cur_value_but.visible       = false
                                flag_cur_but_on.visible         = false
                                flag_cur_but_off.visible        = false
                                cur_calibr_auto.visible         = false

                                //напряжение
                                get_voltage.visible             = false

                                //акселерометр
                                get_angl_giroscope.visible      = false
                                get_acsel.visible               = false

                                //светодиоды
                                get_led.visible                 = false
                                get_led_but.visible             = false
                                set_led.visible                 = false
                                set_led_but.visible             = false
                            }
                            //Ток
                            else if (currentIndex === 3)
                            {
                                //система
                                get_err.visible                 = false
                                clear_err.visible               = false
                                reboot.visible                  = false
                                get_vers.visible                = false
                                set_signed.visible              = false
                                resetButton.visible             = false
                                get_full_param.visible          = false
                                set_full_param.visible          = false

                                //сервы
                                servs_calibr.visible            = false
                                serv_start.visible              = false
                                serv_home.visible               = false
                                get_install_serv_num.visible    = false
                                get_install_serv_num_but.visible = false
                                get_serv_angl.visible           = false
                                get_serv_angl_but.visible       = false
                                set_serv_angl.visible            = false
                                set_serv_angl_but.visible       = false
                                set_serv_min_angl.visible       = false
                                set_serv_min_angl_but.visible    = false
                                serv_on_but.visible             = false
                                serv_off_but.visible             = false

                                //ток
                                get_cur.visible                 = true
                                get_cur_calibr.visible          = true
                                set_cur_value.visible           = true
                                set_cur_value_but.visible       = true
                                flag_cur_but_on.visible         = true
                                flag_cur_but_off.visible        = true
                                cur_calibr_auto.visible         = true

                                //напряжение
                                get_voltage.visible             = false

                                //акселерометр
                                get_angl_giroscope.visible      = false
                                get_acsel.visible               = false

                                //светодиоды
                                get_led.visible                 = false
                                get_led_but.visible             = false
                                set_led.visible                 = false
                                set_led_but.visible             = false
                            }
                            //Напряжение
                            else if (currentIndex === 4)
                            {
                                //система
                                get_err.visible                 = false
                                clear_err.visible               = false
                                reboot.visible                  = false
                                get_vers.visible                = false
                                set_signed.visible              = false
                                resetButton.visible             = false
                                get_full_param.visible          = false
                                set_full_param.visible          = false

                                //сервы
                                servs_calibr.visible            = false
                                serv_start.visible              = false
                                serv_home.visible               = false
                                get_install_serv_num.visible    = false
                                get_install_serv_num_but.visible = false
                                get_serv_angl.visible           = false
                                get_serv_angl_but.visible       = false
                                set_serv_angl.visible            = false
                                set_serv_angl_but.visible       = false
                                set_serv_min_angl.visible       = false
                                set_serv_min_angl_but.visible    = false
                                serv_on_but.visible             = false
                                serv_off_but.visible             = false

                                //ток
                                get_cur.visible                 = false
                                get_cur_calibr.visible          = false
                                set_cur_value.visible           = false
                                set_cur_value_but.visible       = false
                                flag_cur_but_on.visible         = false
                                flag_cur_but_off.visible        = false
                                cur_calibr_auto.visible         = false

                                //напряжение
                                get_voltage.visible             = true

                                //акселерометр
                                get_angl_giroscope.visible      = false
                                get_acsel.visible               = false

                                //светодиоды
                                get_led.visible                 = false
                                get_led_but.visible             = false
                                set_led.visible                 = false
                                set_led_but.visible             = false
                            }
                            //Акселерометр
                            else if (currentIndex === 5)
                            {
                                //система
                                get_err.visible                 = false
                                clear_err.visible               = false
                                reboot.visible                  = false
                                get_vers.visible                = false
                                set_signed.visible              = false
                                resetButton.visible             = false
                                get_full_param.visible          = false
                                set_full_param.visible          = false

                                //сервы
                                servs_calibr.visible            = false
                                serv_start.visible              = false
                                serv_home.visible               = false
                                get_install_serv_num.visible    = false
                                get_install_serv_num_but.visible = false
                                get_serv_angl.visible           = false
                                get_serv_angl_but.visible       = false
                                set_serv_angl.visible            = false
                                set_serv_angl_but.visible       = false
                                set_serv_min_angl.visible       = false
                                set_serv_min_angl_but.visible    = false
                                serv_on_but.visible             = false
                                serv_off_but.visible             = false

                                //ток
                                get_cur.visible                 = false
                                get_cur_calibr.visible          = false
                                set_cur_value.visible           = false
                                set_cur_value_but.visible       = false
                                flag_cur_but_on.visible         = false
                                flag_cur_but_off.visible        = false
                                cur_calibr_auto.visible         = false

                                //напряжение
                                get_voltage.visible             = false

                                //акселерометр
                                get_angl_giroscope.visible      = true
                                get_acsel.visible               = true

                                //светодиоды
                                get_led.visible                 = false
                                get_led_but.visible             = false
                                set_led.visible                 = false
                                set_led_but.visible             = false
                            }
                            //Светодиоды
                            else if (currentIndex === 6)
                            {
                                //система
                                get_err.visible                 = false
                                clear_err.visible               = false
                                reboot.visible                  = false
                                get_vers.visible                = false
                                set_signed.visible              = false
                                resetButton.visible             = false
                                get_full_param.visible          = false
                                set_full_param.visible          = false

                                //сервы
                                servs_calibr.visible            = false
                                serv_start.visible              = false
                                serv_home.visible               = false
                                get_install_serv_num.visible    = false
                                get_install_serv_num_but.visible = false
                                get_serv_angl.visible           = false
                                get_serv_angl_but.visible       = false
                                set_serv_angl.visible            = false
                                set_serv_angl_but.visible       = false
                                set_serv_min_angl.visible       = false
                                set_serv_min_angl_but.visible    = false
                                serv_on_but.visible             = false
                                serv_off_but.visible             = false

                                //ток
                                get_cur.visible                 = false
                                get_cur_calibr.visible          = false
                                set_cur_value.visible           = false
                                set_cur_value_but.visible       = false
                                flag_cur_but_on.visible         = false
                                flag_cur_but_off.visible        = false
                                cur_calibr_auto.visible         = false

                                //напряжение
                                get_voltage.visible             = false

                                //акселерометр
                                get_angl_giroscope.visible      = false
                                get_acsel.visible               = false

                                //светодиоды
                                get_led.visible                 = true
                                get_led_but.visible             = true
                                set_led.visible                 = true
                                set_led_but.visible             = true
                            }

                        }

                    }

                //////////система/////////////
                Button {
                    id: get_err
                    anchors.top: paramBox.bottom
                    anchors.topMargin: 10
                    width: parent.width
                    height: parent.height * 0.04
                    text: "запрос ошибок"
                    background: Rectangle{
                        property var normalColor: "#7007e0"
                        property var pressedColor: "#5813a1"
                        color: get_err.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrapS("e5");
                        listView1.positionViewAtEnd()
                    }
                }

                Button {
                  id: clear_err
                  anchors.top: get_err.bottom
                  anchors.topMargin: 10
                  width: parent.width
                  height: parent.height * 0.04
                  text: "стирание ошибок"
                  background: Rectangle{
                      property var normalColor: "#7007e0"
                      property var pressedColor: "#5813a1"
                      color: clear_err.pressed ? pressedColor : normalColor
                  }
                   onClicked: {
                      device.sendMessageAndWrapS("eb");
                      listView1.positionViewAtEnd()
                   }
                }

                Button {
                  id: reboot
                  anchors.top: clear_err.bottom
                  anchors.topMargin: 10
                  width: parent.width
                  height: parent.height * 0.04
                  text: "перезагрузка шара"
                  background: Rectangle{
                      property var normalColor: "#7007e0"
                      property var pressedColor: "#5813a1"
                      color: reboot.pressed ? pressedColor : normalColor
                  }
                   onClicked: {
                      device.sendMessageAndWrapS("f0");
                      listView1.positionViewAtEnd()
                   }
                }

                Button {
                  id: get_vers
                  anchors.top: reboot.bottom
                  anchors.topMargin: 10
                  width: parent.width
                  height: parent.height * 0.04
                  text: "запрос версии"
                  background: Rectangle{
                      property var normalColor: "#7007e0"
                      property var pressedColor: "#5813a1"
                      color: get_vers.pressed ? pressedColor : normalColor
                  }
                   onClicked: {
                      device.sendMessageAndWrapS("f2");
                      listView1.positionViewAtEnd()
                   }
                }

                Button {
                  id: set_signed
                  anchors.top: get_vers.bottom
                  anchors.topMargin: 10
                  width: parent.width
                  height: parent.height * 0.04
                  text: "подписать приложение"
                  background: Rectangle{
                      property var normalColor: "#7007e0"
                      property var pressedColor: "#5813a1"
                      color: set_signed.pressed ? pressedColor : normalColor
                  }
                   onClicked: {
                      device.sendMessageAndWrap(0xF5, "");
                      listView1.positionViewAtEnd()
                   }
                }

                Button {
                    id: resetButton
                    anchors.top: set_signed.bottom
                    anchors.topMargin: 10
                    width: parent.width
                    height: parent.height * 0.04
                    text: "Сброс до заводских"
                    background: Rectangle{
                        property var normalColor: "#db1244"
                        property var pressedColor: "#5813a1"
                        color: resetButton.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrapS("e0");
                        listView1.positionViewAtEnd()
                    }
                }

                Button {
                    id: get_full_param
                    anchors.top: resetButton.bottom
                    anchors.topMargin: 10
                    width: parent.width
                    height: parent.height * 0.04
                    text: "запрос точки восстановления"
                    background: Rectangle{
                        property var normalColor: "#7007e0"
                        property var pressedColor: "#5813a1"
                        color: get_full_param.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrapS("a7");
                        listView1.positionViewAtEnd()
                    }
                }

                Button {
                    id: set_full_param
                    anchors.top: get_full_param.bottom
                    anchors.topMargin: 10
                    width: parent.width
                    height: parent.height * 0.04
                    text: "восстановить параметры"
                    background: Rectangle{
                        property var normalColor: "#7007e0"
                        property var pressedColor: "#5813a1"
                        color: set_full_param.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrapS("a8");
                        listView1.positionViewAtEnd()
                    }
                }


                //////////сервы/////////////

                Button {
                  id: servs_calibr
                  anchors.top: paramBox.bottom
                  anchors.topMargin: 10
                  width: parent.width
                  height: parent.height * 0.04
                  text: "автокалибровка серв"
                  background: Rectangle{
                      property var normalColor: "#123ddb"
                      property var pressedColor: "#052bb3"
                      color: servs_calibr.pressed ? pressedColor : normalColor
                  }
                   onClicked: {
                      device.sendMessageAndWrapS("e7");
                      listView1.positionViewAtEnd()
                   }
                 }

                Button {
                  id: serv_start
                  anchors.top: servs_calibr.bottom
                  anchors.topMargin: 10
                  width: parent.width
                  height: parent.height * 0.04
                  text: "сервы в старт"
                  background: Rectangle{
                      property var normalColor: "#123ddb"
                      property var pressedColor: "#052bb3"
                      color: serv_start.pressed ? pressedColor : normalColor
                  }
                   onClicked: {
                      device.sendMessageAndWrapS("ec");
                      listView1.positionViewAtEnd()
                   }
                }

                Button {
                  id: serv_home
                  anchors.top: serv_start.bottom
                  anchors.topMargin: 10
                  width: parent.width
                  height: parent.height * 0.04
                  text: "сервы в домашнее"
                  background: Rectangle{
                      property var normalColor: "#123ddb"
                      property var pressedColor: "#052bb3"
                      color: serv_home.pressed ? pressedColor : normalColor
                  }
                   onClicked: {
                      device.sendMessageAndWrapS("ed");
                      listView1.positionViewAtEnd()
                   }
                }

                TextField {
                    id: get_install_serv_num
                    placeholderText: "запрос уст. сервы №"
                    anchors.top: serv_home.bottom
                    anchors.topMargin: 10
                    color: "#123ddb"
                    validator: RegularExpressionValidator {regularExpression: /[0-9]+/}
                    width: parent.width * 0.75
                }
                Button {
                    id: get_install_serv_num_but
                    anchors.top: get_install_serv_num.top
                    width: parent.width * 0.25
                    anchors.bottom: get_install_serv_num.bottom
                    anchors.left: get_install_serv_num.right
                    anchors.leftMargin: 10
                    text: "отпр."
                    background: Rectangle{
                        property var normalColor: "#123ddb"
                        property var pressedColor: "#052bb3"
                        color: get_install_serv_num_but.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrap(0xE6, get_install_serv_num.text);
                        listView1.positionViewAtEnd()
                    }
                }

                TextField {
                    id: get_serv_angl
                    placeholderText: "запрос угла сервы №"
                    anchors.top: get_install_serv_num.bottom
                    anchors.topMargin: 10
                    color: "#123ddb"
                    validator: RegularExpressionValidator {regularExpression: /[0-9]+/}
                    width: parent.width * 0.75
                }
                Button {
                    id: get_serv_angl_but
                    anchors.top: get_serv_angl.top
                    width: parent.width * 0.25
                    anchors.bottom: get_serv_angl.bottom
                    anchors.left: get_serv_angl.right
                    anchors.leftMargin: 10
                    text: "отпр."
                    background: Rectangle{
                        property var normalColor: "#123ddb"
                        property var pressedColor: "#052bb3"
                        color: get_serv_angl_but.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrap(0xA5, get_serv_angl.text);
                        listView1.positionViewAtEnd()
                    }
                }

                TextField {
                    id: set_serv_angl
                    placeholderText: "установить серву в угол"
                    anchors.top: get_serv_angl.bottom
                    anchors.topMargin: 10
                    color: "#123ddb"
                    validator: RegularExpressionValidator {regularExpression: /[ 0-9]+/}
                    width: parent.width * 0.75
                }
                Button {
                    id: set_serv_angl_but
                    anchors.top: set_serv_angl.top
                    width: parent.width * 0.25
                    anchors.bottom: set_serv_angl.bottom
                    anchors.left: set_serv_angl.right
                    anchors.leftMargin: 10
                    text: "отпр."
                    background: Rectangle{
                        property var normalColor: "#123ddb"
                        property var pressedColor: "#052bb3"
                        color: set_serv_angl_but.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrap(0xE3, set_serv_angl.text);

                        listView1.positionViewAtEnd()
                    }
                }


                TextField {
                    id: set_serv_min_angl
                    placeholderText: "запись мин уст. сервы"
                    anchors.top: set_serv_angl.bottom
                    anchors.topMargin: 10
                    color: "#123ddb"
                    validator: RegularExpressionValidator {regularExpression: /[ 0-9]+/}
                    width: parent.width * 0.75
                }
                Button {
                    id: set_serv_min_angl_but
                    anchors.top: set_serv_min_angl.top
                    width: parent.width * 0.25
                    anchors.bottom: set_serv_min_angl.bottom
                    anchors.left: set_serv_min_angl.right
                    anchors.leftMargin: 10
                    text: "отпр."
                    background: Rectangle{
                        property var normalColor: "#123ddb"
                        property var pressedColor: "#052bb3"
                        color: set_serv_min_angl_but.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrap(0xE1, set_serv_min_angl.text);
                        listView1.positionViewAtEnd()
                    }
                }

                Button {
                    id: serv_off_but
                    anchors.top: set_serv_min_angl.bottom
                    width: parent.width * 0.47
                    height: parent.height * 0.04
                    anchors.left: parent.left
                    anchors.topMargin: 10
                    text: "откл. сервы"
                    background: Rectangle{
                        property var normalColor: "#123ddb"
                        property var pressedColor: "#052bb3"
                        color: serv_off_but.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrap(0xA6, 0x00);
                        listView1.positionViewAtEnd()
                    }
                }

                Button {
                    id: serv_on_but
                    anchors.top: set_serv_min_angl.bottom
                    width: parent.width * 0.47
                    height: parent.height * 0.04
                    anchors.right: parent.right
                    anchors.topMargin: 10
                    text: "вкл. сервы"
                    background: Rectangle{
                        property var normalColor: "#123ddb"
                        property var pressedColor: "#052bb3"
                        color: serv_on_but.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrap(0xA6, 0x01);
                        listView1.positionViewAtEnd()
                    }
                }

                //////////////ток///////////////

                Button {
                  id: get_cur
                  anchors.top: paramBox.bottom
                  anchors.topMargin: 10
                  width: parent.width
                  height: parent.height * 0.04
                  text: "запрос тока"
                  background: Rectangle{
                      property var normalColor: "#14deca"
                      property var pressedColor: "#0b7d72"
                      color: get_cur.pressed ? pressedColor : normalColor
                  }
                   onClicked: {
                      device.sendMessageAndWrapS("a2");
                      listView1.positionViewAtEnd()
                   }
                }

                Button {
                  id: get_cur_calibr
                  anchors.top: get_cur.bottom
                  anchors.topMargin: 10
                  width: parent.width
                  height: parent.height * 0.04
                  text: "запрос калибровки тока"
                  background: Rectangle{
                      property var normalColor: "#14deca"
                      property var pressedColor: "#0b7d72"
                      color: get_cur_calibr.pressed ? pressedColor : normalColor
                  }
                   onClicked: {
                      device.sendMessageAndWrapS("ea");
                      listView1.positionViewAtEnd()
                   }
                }

                TextField {
                    id: set_cur_value
                    placeholderText: "записать знач. калибр. тока"
                    anchors.top: get_cur_calibr.bottom
                    anchors.topMargin: 10
                    color: "#14deca"
                    validator: RegularExpressionValidator {regularExpression: /[.0-9]+/}
                    width: parent.width * 0.75
                }
                Button {
                    id: set_cur_value_but
                    anchors.top: set_cur_value.top
                    width: parent.width * 0.25
                    anchors.bottom: set_cur_value.bottom
                    anchors.left: set_cur_value.right
                    anchors.leftMargin: 10
                    text: "отпр."
                    background: Rectangle{
                        property var normalColor: "#14deca"
                        property var pressedColor: "#0b7d72"
                        color: set_cur_value_but.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrap(0xe9, set_cur_value.text);
                        listView1.positionViewAtEnd()
                    }
                }

                Button {
                    id: flag_cur_but_on
                    width: parent.width * 0.47
                    height: parent.height * 0.04
                    anchors.top: set_cur_value.bottom
                    anchors.left: parent.left
                    anchors.topMargin: 10
                    text: "вкл. блок."
                    background: Rectangle{
                        property var normalColor: "#14deca"
                        property var pressedColor: "#0b7d72"
                        color: flag_cur_but_on.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrap(0xE4, 0x01);
                        listView1.positionViewAtEnd()
                    }
                }
                Button {
                    id: flag_cur_but_off
                    width: parent.width * 0.47
                    height: parent.height * 0.04
                    anchors.right: parent.right
                    anchors.top: set_cur_value.bottom
                    anchors.topMargin: 10
                    text: "откл. блок."
                    background: Rectangle{
                        property var normalColor: "#14deca"
                        property var pressedColor: "#0b7d72"
                        color: flag_cur_but_off.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrap(0xE4, 0x00);
                        listView1.positionViewAtEnd()
                    }
                }

                Button {
                  id: cur_calibr_auto
                  anchors.top: flag_cur_but_off.bottom
                  anchors.topMargin: 10
                  width: parent.width
                  height: parent.height * 0.04
                  text: "автокалибр. датчика тока"
                  background: Rectangle{
                      property var normalColor: "#14deca"
                      property var pressedColor: "#0b7d72"
                      color: cur_calibr_auto.pressed ? pressedColor : normalColor
                  }
                   onClicked: {
                      device.sendMessageAndWrapS("e8");
                      listView1.positionViewAtEnd()
                   }
                }

                ///////////напряжение//////////////

                Button {
                  id: get_voltage
                  anchors.top: paramBox.bottom
                  anchors.topMargin: 10
                  width: parent.width
                  height: parent.height * 0.04
                  text: "запрос напряжения"
                  background: Rectangle{
                      property var normalColor: "#16e049"
                      property var pressedColor: "#10a335"
                      color: get_voltage.pressed ? pressedColor : normalColor
                  }
                   onClicked: {
                      device.sendMessageAndWrapS("a1");
                      listView1.positionViewAtEnd()
                   }
                }

                ///////////акселерометр//////////////

                Button {
                  id: get_angl_giroscope
                  anchors.top: paramBox.bottom
                  anchors.topMargin: 10
                  width: parent.width
                  height: parent.height * 0.04
                  text: "запрос углов гироскопа"
                  background: Rectangle{
                      property var normalColor: "#6ac714"
                      property var pressedColor: "#529415"
                      color: get_angl_giroscope.pressed ? pressedColor : normalColor
                  }
                   onClicked: {
                      device.sendMessageAndWrapS("a3");
                      listView1.positionViewAtEnd()
                   }
                }

                Button {
                  id: get_acsel
                  anchors.top: get_angl_giroscope.bottom
                  anchors.topMargin: 10
                  width: parent.width
                  height: parent.height * 0.04
                  text: "запрос ускорения"
                  background: Rectangle{
                      property var normalColor: "#6ac714"
                      property var pressedColor: "#529415"
                      color: get_acsel.pressed ? pressedColor : normalColor
                  }
                   onClicked: {
                      device.sendMessageAndWrapS("a4");
                      listView1.positionViewAtEnd()
                   }
                }

                ///////////светодиоды//////////////

                TextField {
                    id: get_led
                    placeholderText: "опрос светодиода №"
                    anchors.top: paramBox.bottom
                    anchors.topMargin: 10
                    color: "#a3a614"
                    validator: RegularExpressionValidator {regularExpression: /[0-9]+/}
                    width: parent.width * 0.75
                }
                Button {
                    id: get_led_but
                    anchors.top: get_led.top
                    width: parent.width * 0.25
                    anchors.bottom: get_led.bottom
                    anchors.left: get_led.right
                    anchors.leftMargin: 10
                    text: "отпр."
                    background: Rectangle{
                        property var normalColor: "#a3a614"
                        property var pressedColor: "#ecf011"
                        color: get_led_but.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrap(0xA0, get_led.text);
                        listView1.positionViewAtEnd()
                    }
                }

                TextField {
                    id: set_led
                    placeholderText: "запись уст. для светодиодов"
                    anchors.top: get_led.bottom
                    anchors.topMargin: 10
                    color: "#a3a614"
                    validator: RegularExpressionValidator {regularExpression: /[ 0-9]+/}
                    width: parent.width * 0.75
                }
                Button {
                    id: set_led_but
                    anchors.top: set_led.top
                    width: parent.width * 0.25
                    anchors.bottom: set_led.bottom
                    anchors.left: set_led.right
                    anchors.leftMargin: 10
                    text: "отпр."
                    background: Rectangle{
                        property var normalColor:  "#a3a614"
                        property var pressedColor: "#ecf011"
                        color: set_led_but.pressed ? pressedColor : normalColor
                    }
                    onClicked: {
                        device.sendMessageAndWrap(0xE2, set_led.text);

                        listView1.positionViewAtEnd()
                    }
                }

        }
}
