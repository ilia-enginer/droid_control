import QtQuick 2.15
import QtQuick.Controls 2.0
import QtCharts 2.15
import QtQml
import QtQuick.Controls.Material


SwipeView {
    id: joystick_shar
    // Свайп разрешён, только если внутренний элемент не "захватил" жест
    interactive: !childBusy
    currentIndex: 0
    focus: false

    // true, пока какой-то внутренний элемент (график/джойстик/слайдер/удержание)
    // обрабатывает жест. Пока true — SwipeView не листает страницы.
    property bool childBusy: false

    onActiveFocusChanged: {
        if (!activeFocus) {
            // Пользователь нажал или перешёл away от этого поля
            console.log("!!!!!!!!!!No active.")
            joystick_timer.running = false
            radioGroup.checkState = Qt.Unchecked
            window_focus = false
        } else {
            // Пользователь сосредоточился на этом поле
            console.log("!!!!!!!!Focus active.")
            window_focus = true
            joystick_timer.running = true
            tx_commands.getCheck();         // запрашивать ошибки при активации окна
        }
     }

    property bool window_focus : false

    property bool addingLog : true

    property bool verticalOnly : false
    property bool horizontalOnly : false
    property real offsetX : 0
    property real offsetY : 0
    property real azimuth : 0
    property real amplitude : 0
    property int mode : 0
    property real level : settParam.heightAmplitude/2
    property bool ctrl : false

    signal joystick_moved(double x, double y);

    property double valueYMin: -1.0
    property double valueYMax: 1.0

    property int count_volt: 0
    property int count_cur: 0
    property int count_tilt_angle: 0
    property int count_tilt_direction: 0
    property int count_boost: 0
    property int count_angular_velocity: 0
    property int count_angleX: 0
    property int count_angleY: 0
    property int count_angleZ: 0
    property int temp: 0
    property bool chartViewVisible : true

    property string copiedLogText: ""

    function updateClipboardFromSelection() {
        var text = ""
        for (var i = 0; i < logListModel_2.count; ++i) {
            var item = logListModel_2.get(i)
            if (item.selected) {
                text += item.msg + "\n"
                toast.toastOn("буфер обновлён")
            }
        }
        copiedLogText = text.trim()
        clipboardHelper.text = copiedLogText
        clipboardHelper.selectAll()
        clipboardHelper.copy()
    }

    //страница управления
    Item {
        id: page_joystick

        PageIndicator {
            id: pageIndicator
            interactive: true
            count: joystick_shar.count
            currentIndex: joystick_shar.currentIndex
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: parent.height * 0.03
        }

        Timer {
            id: joystick_timer
            interval: settParam.timer1
            running: false
            repeat: true
            onTriggered: { tx_commands.joysticActivity(mode, azimuth, amplitude, level, ctrl); }
        }

        TextEdit {
            id: clipboardHelper
            opacity: 0
            width: 1
            height: 1
            text: ""
            readOnly: true
            selectByMouse: false
        }

        //окно лога
        Rectangle {
            id: senderBackground_2
            anchors.top: parent.top
            height: parent.height * 0.33
            anchors.left: parent.left
            anchors.right: parent.right
            color: "#f7ebee"
        }

        ScrollView {
            id: scrolViewLogArea_2
            anchors.fill: senderBackground_2
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical: ScrollBar {
                id: scrollBar
                hoverEnabled: true
                active: hovered || pressed
                orientation: Qt.Vertical
                policy: ScrollBar.AlwaysOn
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                Image {
                    anchors.top: parent.top
                    width: parent.width
                    height: 15
                    source: "back.png" // Путь к файлу с иконкой-стрелкой вверх
                    rotation: 90
                    MouseArea {
                        anchors.fill: parent
                        onClicked: scrollBar.decrease() // Прокрутка вверх
                    }
                }
                Image {
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 15
                    source: "back.png" // Путь к файлу с иконкой-стрелкой вниз
                    rotation: 270
                    MouseArea {
                        anchors.fill: parent
                        onClicked: scrollBar.increase() // Прокрутка вниз
                    }
                }
            }

            ListView {
                   id: listView1
                   anchors.fill: scrolViewLogArea_2
                   snapMode:ListView.SnapOneItem
                   clip: true

                   spacing: -1
                   reuseItems: true
                   model: logListModel_2

                   // Пока пользователь скроллит лог — не даём SwipeView
                   // перехватить горизонтальный жест.
                   onMovementStarted: joystick_shar.childBusy = true
                   onMovementEnded:   joystick_shar.childBusy = false

                   delegate: Rectangle {
                       id: rowRoot
                       width: listView1.width * 0.95
                       height: rowText.implicitHeight //+ 8
                       radius: 4
                       color: selected ? "#cfe8ff" : "transparent"
                       border.width: selected ? 1 : 0
                       border.color: "#4a90e2"

                       required property int index
                       required property string msg
                       required property bool selected

                       Text {
                           id: rowText
                           anchors.left: parent.left
                           anchors.leftMargin: 14
                           anchors.right: parent.right
                           anchors.rightMargin: 4
                           anchors.verticalCenter: parent.verticalCenter
                           text: msg
                           wrapMode: Text.Wrap
                           color: "black"
                           font.pixelSize: 14
                       }

                       MouseArea {
                           anchors.fill: parent
                           acceptedButtons: Qt.LeftButton

                           onClicked: {
                               logListModel_2.setProperty(index, "selected", !selected)
                               updateClipboardFromSelection()
                           }

                           onDoubleClicked: {
                               logListModel_2.setProperty(index, "selected", true)
                               updateClipboardFromSelection()
                           }
                       }
                   }

                   Connections {
                       target: commun_display
                       function onLogJoy(type, msg) {
                           if(!addingLog)   return;
                           if (window_focus) {
                               if (mainModel.adminFlag === false) {
                                   logListModel_2.append({ msg: type + msg, selected: false })
                                   listView1.positionViewAtEnd()
                               }
                           }
                       }
                       function onLogServis(type, msg) {
                           if(!addingLog)   return;
                           if (window_focus) {
                               if (mainModel.adminFlag === true) {
                                   logListModel_2.append({ msg: type + msg, selected: false })
                                   listView1.positionViewAtEnd()
                               }
                           }
                       }
                   }

                   // Сама модель, в которой будут содержаться все элементы
                   ListModel {
                       id: logListModel_2 // задаём ей id для обращения
                   }
               }
        }

        // уведомление о копировании
        ToolTip {
            id: toast
            delay: 0  // задержка перед показом
            timeout: 1000  // время скрытия
            x: (parent.width - width) / 2  // по горизонтали по центру
            y: senderBackground_2.y + senderBackground_2.height //(parent.height - 100)  // по вертикали снизу

            // появление
            enter: Transition {
                    NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 450 }
                }
            // изчезновение
            exit: Transition {
                    NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 250 }
                }

            background: Rectangle {
                color: "#7f856f"
                radius: 15  // скруглённые углы
            }
            function toastOn(msg){
                toast.text = qsTr(msg)
                toast.visible = true
            }
        }


        //очистка лога
        Button {
            id: clearButton
            width: senderBackground_2.width * 0.08
            height: senderBackground_2.height * 0.45
            anchors.top: senderBackground_2.top
            anchors.right: senderBackground_2.right
          //  anchors.rightMargin: Qt.platform.os === "windows" ? 30 : 0
            anchors.rightMargin: 30
            opacity: 0.4

            contentItem: Text{
                horizontalAlignment: Qt.AlignHCenter
                text: "Clear"
                font.pointSize: 6
            }

            onClicked: {
                logListModel_2.clear()
                copiedLogText = ""
                clipboardHelper.text = ""
            }

            // остановка выпадения лога
            Button {
                width: senderBackground_2.width * 0.08
                height: senderBackground_2.height * 0.45
                anchors.top: clearButton.bottom
                anchors.topMargin: 3
                anchors.right: senderBackground_2.right
                anchors.rightMargin: 30
                opacity: 0.8

                contentItem: Text{
                    horizontalAlignment: Qt.AlignHCenter
                    text: addingLog ? "Стоп" : "Старт"
                    font.pointSize: 6
                }

                onClicked: {
                    addingLog ? addingLog = false : addingLog = true
                }
            }
        }

        //напряжение
        ProgressBar{
            id: voltage
            height: parent.height * 0.02
            anchors.top: senderBackground_2.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width * 0.98
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
                    if(window_focus)
                        voltage.value = V
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

        //ток
        ProgressBar{
            id: current
            height: parent.height * 0.02
            anchors.top: voltage.bottom
            anchors.topMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width * 0.98
            from: 0
            to: 5

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
            Connections {
                target: commun_display
               function onCurRealChanged(Cur) {
                   if(window_focus)
                    current.value = Cur
                }
            }
        }

        //ток
        Label {
            id: currentLabel
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            anchors.verticalCenter: current.verticalCenter
            text: commun_display.Cur.toFixed(2) + "А"
            color: "black"
        }

        //имя приемника
        Label {
            id: senderPageLabel
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            anchors.top: current.bottom
            anchors.topMargin: 10
            text: commun_display.curDeviceName_
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
                // Блокируем SwipeView, пока палец на джойстике
                joystick_shar.childBusy = true
            }

            onReleased: {
                    returnAnimation.restart()
                    joystick_moved(0, 0);
                    offsetX = 0
                    offsetY = 0
                    amplitude = 0
                    azimuth = 0
                    offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
                    // Отпускаем SwipeView
                    joystick_shar.childBusy = false
            }

            onCanceled: {
                // Страховка: если жест перехвачен — снимаем блокировку
                joystick_shar.childBusy = false
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

            width: 200//parent.width * 0.5
            height: 200// parent.height * 0.35
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: senderPageLabel.bottom
            anchors.topMargin: 10
            visible: false

            RoundButton {
                id: upBut
                radius: 8
                text: "˄"
                highlighted: true
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter

                onPressed: {
                    amplitude = 0.5
                    azimuth = 1.57
                    offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
                    joystick_shar.childBusy = true
                }
                onReleased: {
                    amplitude = 0
                    azimuth = 0
                    offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
                    joystick_shar.childBusy = false
                }
            }
            RoundButton {
                id: downBut
                radius: 8
                text: "˅"
                highlighted: true
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottomMargin: 30

                onPressed: {
                    amplitude = 0.5
                    azimuth = -1.57
                    offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
                    joystick_shar.childBusy = true
                }
                onReleased: {
                    amplitude = 0
                    azimuth = 0
                    offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
                    joystick_shar.childBusy = false
                }
            }
            RoundButton {
                radius: 8
                text: "˂"
                highlighted: true
                anchors.right: downBut.left
                anchors.rightMargin: 15
                anchors.top: upBut.bottom
                anchors.topMargin: 15

                onPressed: {
                    amplitude = 0.5
                    azimuth = -3.14
                    offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
                    joystick_shar.childBusy = true
                }
                onReleased: {
                    amplitude = 0
                    azimuth = 0
                    offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
                    joystick_shar.childBusy = false
                }
            }
            RoundButton {
                radius: 8
                text: "˃"
                highlighted: true
                anchors.left: downBut.right
                anchors.leftMargin: 15
                anchors.top: upBut.bottom
                anchors.topMargin: 15

                onPressed: {
                    amplitude = 0.5
                    azimuth = 0
                    offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
                    joystick_shar.childBusy = true
                }
                onReleased: {
                    amplitude = 0
                    azimuth = 0
                    offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
                    joystick_shar.childBusy = false
                }
            }
        }

        //высота
        Slider {
            id: levelSlider
            from: settParam.heightAmplitudemin
            to: settParam.heightAmplitude
            orientation: Qt.Vertical
            value: (((settParam.heightAmplitude - settParam.heightAmplitudemin) / 2) + settParam.heightAmplitudemin)
            anchors.right: parent.right
            anchors.verticalCenter: joystick.verticalCenter
            onMoved: {level = value}
            Component.onCompleted: {
                            level = value
                        }

            // Пока слайдер в нажатом состоянии — блокируем SwipeView,
            // чтобы вертикальный drag не уезжал в листание страниц.
            onPressedChanged: joystick_shar.childBusy = pressed
        }

        //высота
        Label {
            id: leveltext
            horizontalAlignment: Qt.AlignHCenter
            anchors.top: joystick.bottom
            anchors.right: parent.right
            anchors.rightMargin: 15
            anchors.topMargin: 20
            text: "Высота"
        }

        //режим работы
        ButtonGroup {
            id: radioGroup
            Component.onCompleted: {
                mode = Number("0")
            }
        }
        Column {
            spacing: 3
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: joystick.verticalCenter

             RadioButton {
                 text: qsTr("1")
                 ButtonGroup.group: radioGroup
                 onClicked: {
                     mode = Number("1")
                     ctrl_check.enabled = true
                     settParam.ctrl_ = ctrl
                     ctrl_check.checked = ctrl
                 }
             }

             RadioButton {
                 text: qsTr("2")
                 ButtonGroup.group: radioGroup
                 onClicked: {
                     mode = Number("2")
                     settParam.ctrl_ = false
                     ctrl_check.checked = false
                     ctrl_check.enabled = false
                 }
             }

             RadioButton {
                 text: qsTr("3")
                 ButtonGroup.group: radioGroup
                 onClicked: {
                     mode = Number("3")
                     ctrl_check.enabled = true
                     settParam.ctrl_ = ctrl
                     ctrl_check.checked = ctrl
                 }
             }

             RadioButton {
                 text: qsTr("0")
                 ButtonGroup.group: radioGroup
                 onClicked: {
                     mode = Number("0")
                     settParam.ctrl_ = false
                     ctrl_check.checked = false
                     ctrl_check.enabled = false
                 }
             }

             Label {
                text: qsTr("режим\nработы")
                }

        }

        //углы джойстика
        Label {
            id: offsetInfo
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            anchors.top: joystick.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: joystick.horizontalCenter
            text: "0/0"
            visible: mainModel.adminFlag
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
            }
        }

        //стабилизация
        CheckBox {
          id: ctrl_check
          anchors.top: joystick_mode_swith.bottom
          anchors.topMargin: 15
          anchors.left: parent.left
          anchors.leftMargin: 15
          checked: settParam.ctrl_
          enabled: false
          text: qsTr("Вкл. стабилизацию")

          onClicked: {
            settParam.ctrl_ = ctrl_check.checked
            ctrl = ctrl_check.checked
          }
        }

        //выключение шара
        DelayButton{
            id: power_off
            anchors.bottom: parent.bottom
            anchors.topMargin: 15
            anchors.right: parent.right
            anchors.rightMargin: 5
            width: parent.width * 0.3
            delay: 3000     //3000 ms

            // Удержание (3 секунды) не должно листать страницы
            onPressedChanged: joystick_shar.childBusy = pressed

            onActivated: {
                joystick_timer.running = false
                tx_commands.shutdown();
                tx_commands.shutdown();

                power_off.progress = 0.0
                radioGroup.checkState = Qt.Unchecked
            }

        }
        Rectangle{
            property var normalColor: "#1cc976"
            property var pressedColor: "#FFA07A"
            anchors.top: power_off.top
            anchors.right: power_off.right
            anchors.left: power_off.left
            height: power_off.height * 0.8
            radius: 4
            Text {
                id: name
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Выкл. шар")
            }
            color: power_off.pressed ? pressedColor : normalColor
        }
    }

    //страница графиков
    ChartController {
        id: chartPage
        // Пока пользователь тащит/щипает график — SwipeView не листает
        onInteractionStarted: joystick_shar.childBusy = true
        onInteractionEnded:   joystick_shar.childBusy = false
    }
}
