
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtCharts 2.15
import QtQml
import QtQuick.Controls.Material



SwipeView {
    id: swipeView
    anchors.fill: parent
    interactive: false
    currentIndex: 0

    property bool flag_getcheck : false
    property bool verticalOnly : false
    property bool horizontalOnly : false
    property real offsetX : 0
    property real offsetY : 0
    property real azimuth : 0
    property real amplitude : 0
    property int mode : 0
    property real level : mainModel.heightAmplitude/2
    property bool ctrl : false

    signal joystick_moved(double x, double y);

    property int valueYMin: 0
    property int valueYMax: 1

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


    //страница управления
    Page {
        id: page_joystick
        anchors.fill: swipeView

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
                device.onJoysticActivity(mode, azimuth, amplitude, level, ctrl);
            }
        }


        //окно лога
        Rectangle {
            id: senderBackground_2
            anchors.top: parent.top
            height: parent.height * 0.25
            anchors.left: parent.left
            anchors.right: leveltext.right
            anchors.leftMargin: 10
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
        }

        //имя приемника
        Label {
            id: senderPageLabel
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            anchors.top: current.bottom
            anchors.topMargin: 10
            text: mainModel.OncurrentDeviceName
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

        //высота
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

        //высота
        Label {
            id: leveltext
            horizontalAlignment: Qt.AlignHCenter
            anchors.top: joystick.bottom
            anchors.left: joystick.right
            anchors.leftMargin: 30
            anchors.topMargin: 20
            text: "Высота"
        }


        //режим работы
        ButtonGroup {
            id: radioGroup
            onClicked: mode = Number(button.text)
            Component.onCompleted: {
                mode = Number("1")
            }
        }
        Column {
            spacing: 10
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: joystick.top
         //   anchors.topMargin: 20

             RadioButton {
                 checked: true
                 text: qsTr("1")
                 ButtonGroup.group: radioGroup
             }

             RadioButton {
                 text: qsTr("2")
                 ButtonGroup.group: radioGroup
             }

             RadioButton {
                 text: qsTr("3")
                 ButtonGroup.group: radioGroup
             }

             RadioButton {
                 text: qsTr("0")
                 ButtonGroup.group: radioGroup
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

        //стабилизация
        CheckBox {
          id: ctrl_check
          anchors.top: joystick_mode_swith.bottom
          anchors.topMargin: 15
          anchors.left: parent.left
          anchors.leftMargin: 15
          checked: mainModel.ctrl_
          text: qsTr("Вкл. стабилизацию")

          onClicked: {
            mainModel.ctrl_ = ctrl_check.checked
            ctrl = ctrl_check.checked
          }
        }

        //пролистывание
        Rectangle
        {
            id:plus
            width:parent.width * 0.3
            height:parent.height * 0.035
            anchors.bottom:parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            color:"#bdde1b"
            opacity: 0.3
            Label {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: parent.height * 0.7
                text: "->"
            }
            MouseArea
            {
                anchors.fill:parent
                onClicked:{
                    if(swipeView.currentIndex<1)
                        swipeView.currentIndex++
                }
            }
        }
    }

    //страница графиков
     Page {
        id: page_graph
        anchors.fill: swipeView

        Flickable {
            id: flick
            anchors.top: parent.top
            width: parent.width
            height: parent.height
            interactive: true
            clip: true
            flickableDirection: Flickable.VerticalFlick
            contentHeight: parent.height * 2

             ChartView {
                 id:chartView
               //  Title: QSTR ("1 side temperature trend")
                 titleFont.pixelSize: 15
                 titleFont.bold: true
                 //titleColor: Theme.darker(1.2)
                 width: parent.width
                 anchors.left: parent.left
                 height:parent.height * 0.35
                 antialiasing: true

                 //X-axis
                 ValuesAxis {
                     id: valueAxisX
                     min: 0
                     max: 1
             //        tickCount: 10  //The number of scales on the coordinate axis. So interval = 60/30 = 2
              //       labelFormat: "%.0f"//0 - there are several decimals after controlling the decimal point
                 }

                 //Y-axis
                 ValuesAxis {
                     id: valueAxisY
                     min: 0
                     max: 1
              //       tickCount: 10  //Number of scale on the coordinate axis
               //      labelFormat: "%.1f"//1 - There are several decimals after controlling the decimal point
                 }

                 //LINESERIES is a line, splineseries is a curve. Children are xypoint
                 SplineSeries {
                     id:line1
                     name: "1"  //Name of the line
                     axisX: valueAxisX  //Specify X-axis
                     axisY: valueAxisY  //Specify Y-axis
                     color: Qt.darker("#BB4444",1.2) //Line color, the default color of each line
                     width: 2//The width of the line, but after modifying the line wide, you need to reset the color, otherwise the wire modified by Width is black.
                 }
                 SplineSeries {
                     id:line2
                     name: "2"
                     axisX: valueAxisX
                     axisY: valueAxisY
                     color: Qt.darker("#B38A4D",1.2)
                     width: 2
                 }
                 SplineSeries {
                     id:line3
                     name: "3"
                     //STYLE: Qt.solidline // style
                     axisX: valueAxisX
                     axisY: valueAxisY
                     color: Qt.darker("#61B34D",1.2)
                     width: 2
                 }
                 SplineSeries {
                     id:line4
                     name: "4"
                     //STYLE: Qt.solidline // style
                     axisX: valueAxisX
                     axisY: valueAxisY
                     color: Qt.darker("#4D8AB3",1.2)
                     width: 2
                 }
                 SplineSeries {
                     id:line5
                     name: "5"
                     //STYLE: Qt.solidline // style
                     axisX: valueAxisX
                     axisY: valueAxisY
                     color: Qt.darker("#9E4DB3",1.2)
                     width: 2
                 }
                 SplineSeries {
                     id:line6
                     name: "6"
                     //STYLE: Qt.solidline // style
                     axisX: valueAxisX
                     axisY: valueAxisY
                     color: Qt.darker("#AA5566",1.2)
                     width: 2
                 }
                 SplineSeries {
                     id:line7
                     name: "7"
                     //STYLE: Qt.solidline // style
                     axisX: valueAxisX
                     axisY: valueAxisY
                     color: Qt.darker("#808080",1.2)
                     width: 2
                 }
                 SplineSeries {
                     id:line8
                     name: "8"
                     //STYLE: Qt.solidline // style
                     axisX: valueAxisX
                     axisY: valueAxisY
                     color: Qt.darker("#F79709",1.2)
                     width: 2
                 }
                 SplineSeries {
                     id:line9
                     name: "9"
                     //STYLE: Qt.solidline // style
                     axisX: valueAxisX
                     axisY: valueAxisY
                     color: Qt.darker("#F79709",1.2)
                     width: 2
                 }


                 //перетаскивание графика
                 PinchArea{
                    id: pa
                    anchors.fill: parent
                    property real currentPinchScaleX: 1
                    property real currentPinchScaleY: 1
                    property real pinchStartX : 0
                    property real pinchStartY : 0

                    onPinchStarted: {
                        // Pinching has started. Record the initial center of the pinch
                        // so relative motions can be reversed in the pinchUpdated signal
                        // handler
                        pinchStartX = pinch.center.x;
                        pinchStartY = pinch.center.y;
                    }

                    onPinchUpdated: {
                        chartView.zoomReset();

                        // Reverse pinch center motion direction
                        var center_x = pinchStartX + (pinchStartX - pinch.center.x);
                        var center_y = pinchStartY + (pinchStartY - pinch.center.y);

                        // Compound pinch.scale with prior pinch scale level and apply
                        // scale in the absolute direction of the pinch gesture
                        var scaleX = currentPinchScaleX * (1 + (pinch.scale - 1) * Math.abs(Math.cos(pinch.angle * Math.PI / 180)));
                        var scaleY = currentPinchScaleY * (1 + (pinch.scale - 1) * Math.abs(Math.sin(pinch.angle * Math.PI / 180)));

                        // Apply scale to zoom levels according to pinch angle
                        var width_zoom = height / scaleX;
                        var height_zoom = width / scaleY;

                        var r = Qt.rect(center_x - width_zoom / 2, center_y - height_zoom / 2, width_zoom, height_zoom);
                        chartView.zoomIn(r);
                    }

                    onPinchFinished: {
                        // Pinch finished. Record compounded pinch scale.
                        currentPinchScaleX = currentPinchScaleX * (1 + (pinch.scale - 1) * Math.abs(Math.cos(pinch.angle * Math.PI / 180)));
                        currentPinchScaleY = currentPinchScaleY * (1 + (pinch.scale - 1) * Math.abs(Math.sin(pinch.angle * Math.PI / 180)));
                    }

                    MouseArea{
                        anchors.fill: parent
                        drag.target: dragTarget
                        drag.axis: Drag.XAndYAxis

                        onDoubleClicked: {
                            chartView.zoomReset();
                            parent.currentPinchScaleX = 1;
                            parent.currentPinchScaleY = 1;
                        }
                    }

                    Item {
                        // A virtual item to receive drag signals from the MouseArea.
                        // When x or y properties are changed by the MouseArea's
                        // drag signals, the ChartView is scrolled accordingly.
                        id: dragTarget

                        property real oldX : x
                        property real oldY : y

                        onXChanged: {
                            chartView.scrollLeft( x - oldX );
                            oldX = x;
                        }
                        onYChanged: {
                            chartView.scrollUp( y - oldY );
                            oldY = y;
                        }
                     }
                 }

                 Connections {
                     target: device
                     onChart_data: {
                        if(volt.checked)
                        {
                            //масштабирование по у
                            if(volt_ < valueYMin){
                              valueYMin = volt_;
                              valueAxisY.min = valueYMin;
                            }
                            if(volt_ > valueYMax){
                              valueYMax = volt_;
                              valueAxisY.max = valueYMax;
                            }
                            line1.append(count_volt*2/10, volt_);
                            count_volt++;

                            if((count_volt*2/10) > valueAxisX.max){
                                valueAxisX.max += 0.2;
                            }
                        }

                        if(cur.checked)
                        {
                            //масштабирование по у
                            if(cur_ < valueYMin){
                              valueYMin = cur_;
                              valueAxisY.min = valueYMin;
                            }
                            if(cur_ > valueYMax){
                              valueYMax = cur_;
                              valueAxisY.max = valueYMax;
                            }
                            line2.append(count_cur*2/10, cur_);
                            count_cur++;

                            if((count_cur*2/10) > valueAxisX.max){
                                valueAxisX.max += 0.2;
                            }
                        }

                        if(tilt_angle.checked)
                        {
                            //масштабирование по у
                            if(tilt_angle_ < valueYMin){
                              valueYMin = tilt_angle_;
                              valueAxisY.min = valueYMin;
                            }
                            if(tilt_angle_ > valueYMax){
                              valueYMax = tilt_angle_;
                              valueAxisY.max = valueYMax;
                            }
                            line3.append(count_tilt_angle*2/10, tilt_angle_);
                            count_tilt_angle++;

                            if((count_tilt_angle*2/10) > valueAxisX.max){
                                valueAxisX.max += 0.2;
                            }
                        }

                        if(tilt_direction.checked)
                        {
                            //масштабирование по у
                            if(tilt_direction_ < valueYMin){
                              valueYMin = tilt_direction_;
                              valueAxisY.min = valueYMin;
                            }
                            if(tilt_direction_ > valueYMax){
                              valueYMax = tilt_direction_;
                              valueAxisY.max = valueYMax;
                            }
                            line4.append(count_tilt_direction*2/10, tilt_direction_);
                            count_tilt_direction++;

                            if((count_tilt_direction*2/10) > valueAxisX.max){
                                valueAxisX.max += 0.2;
                            }
                        }

                        if(boost.checked)
                        {
                            //масштабирование по у
                            if(boost_ < valueYMin){
                              valueYMin = boost_;
                              valueAxisY.min = valueYMin;
                            }
                            if(boost_ > valueYMax){
                              valueYMax = boost_;
                              valueAxisY.max = valueYMax;
                            }
                            line5.append(count_boost*2/10, boost_);
                            count_boost++;

                            if((count_boost*2/10) > valueAxisX.max){
                                valueAxisX.max += 0.2;
                            }
                        }

                        if(angular_velocity.checked)
                        {
                            //масштабирование по у
                            if(angular_velocity_ < valueYMin){
                              valueYMin = angular_velocity_;
                              valueAxisY.min = valueYMin;
                            }
                            if(angular_velocity_ > valueYMax){
                              valueYMax = angular_velocity_;
                              valueAxisY.max = valueYMax;
                            }
                            line6.append(count_angular_velocity*2/10, angular_velocity_);
                            count_angular_velocity++;

                            if((count_angular_velocity*2/10) > valueAxisX.max){
                                valueAxisX.max += 0.2;
                            }
                        }

                        if(angleX.checked)
                        {
                            //масштабирование по у
                            if(angleX_ < valueYMin){
                              valueYMin = angleX_;
                              valueAxisY.min = valueYMin;
                            }
                            if(angleX_ > valueYMax){
                              valueYMax = angleX_;
                              valueAxisY.max = valueYMax;
                            }
                            line7.append(count_angleX*2/10, angleX_);
                            count_angleX++;

                            if((count_angleX*2/10) > valueAxisX.max){
                                valueAxisX.max += 0.2;
                            }
                        }

                        if(angleY.checked)
                        {
                            //масштабирование по у
                            if(angleY_ < valueYMin){
                              valueYMin = angleY_;
                              valueAxisY.min = valueYMin;
                            }
                            if(angleY_ > valueYMax){
                              valueYMax = angleY_;
                              valueAxisY.max = valueYMax;
                            }
                            line8.append(count_angleY*2/10, angleY_);
                            count_angleY++;

                            if((count_angleY*2/10) > valueAxisX.max){
                                valueAxisX.max += 0.2;
                            }
                        }

                        if(angleZ.checked)
                        {
                            //масштабирование по у
                            if(angleZ_ < valueYMin){
                              valueYMin = angleZ_;
                              valueAxisY.min = valueYMin;
                            }
                            if(angleZ_ > valueYMax){
                              valueYMax = angleZ_;
                              valueAxisY.max = valueYMax;
                            }
                            line9.append(count_angleZ*2/10, angleZ_);
                            count_angleZ++;

                            if((count_angleZ*2/10) > valueAxisX.max){
                                valueAxisX.max += 0.2;
                            }
                        }
                     }
                 }

             }

       /*      Timer{
                      id:timer
                      property int count: 0
           //           property int temp: 0

                      //Generate random number (> = min, <= max)
                      function getRandomNum(Min,Max)
                      {
                          var Range = Max - Min;
                          var Rand = Math.random();
                          return(Min + Math.round(Rand * Range));
                      }
                      function timeChanged() {


                          temp = getRandomNum(-20,50);

                          //масштабирование по у
                          if(temp < valueYMin){
                            valueYMin = temp;
                            valueAxisY.min = valueYMin;
                          }
                          if(temp > valueYMax){
                            valueYMax = temp;
                            valueAxisY.max = valueYMax;
                          }
                          //мфсштабирование по х
                          valueAxisX.max += 0.2;


                          line1.append(count*2/10, temp);
                       //   line1.append(count, mainModel.CurReal);

                          count += 1;

                      }
                      interval: 200;
                      running: true;
                      repeat: true;
                      onTriggered: {
                          timeChanged();
                      }
                  }
*/

             Column {
                 spacing: 5
                 width: parent.width
                 anchors.top: chartView.bottom
                 anchors.bottom: flick.bottom

                 //очистка графика
                 Button {
                     id: clearButton_1
                     width: parent.width
                     //opacity: 0.2
                     text: "Очистить график"

                     onClicked: {
                         line1.clear();
                         line2.clear();
                         line3.clear();
                         line4.clear();
                         line5.clear();
                         line6.clear();
                         line7.clear();
                         line8.clear();
                         line9.clear();

                         count_volt = 0;
                         count_cur = 0;
                         count_tilt_angle = 0;
                         count_tilt_direction = 0;
                         count_boost = 0;
                         count_angular_velocity = 0;
                         count_angleX = 0;
                         count_angleY = 0;
                         count_angleZ = 0;

                         valueAxisX.min = 0;
                         valueAxisX.max = 1;

                         valueAxisY.min = 0;
                         valueAxisY.max = 1;

                         valueYMin = 0;
                         valueYMax = 1;

                     }
                 }

                 CheckBox {
                   id: volt
                   checked: false
                   contentItem: Rectangle {
                        width: parent.width
                        height: parent.height * 0.001
                        anchors.left: parent.right
                        anchors.leftMargin: parent.width * 0.5
                        Text {
                            text: qsTr("Напряжение")
                            color: "#BB4444"
                            anchors.left: parent.right
                        }
                   }
                 }
                 CheckBox {
                   id: cur
                   checked: false
                   contentItem: Rectangle {
                        width: parent.width
                        height: parent.height * 0.001
                        anchors.left: parent.right
                        anchors.leftMargin: parent.width * 0.5
                        Text {
                            text: qsTr("Ток")
                            color: "#B38A4D"
                            anchors.left: parent.right
                        }
                   }
                 }
                 CheckBox {
                   id: tilt_angle
                   checked: false
                   contentItem: Rectangle {
                        width: parent.width
                        height: parent.height * 0.001
                        anchors.left: parent.right
                        anchors.leftMargin: parent.width * 0.5
                        Text {
                            text: qsTr("Угол наклона")
                            color: "#61B34D"
                            anchors.left: parent.right
                        }
                   }
                 }
                 CheckBox {
                   id: tilt_direction
                   checked: false
                   contentItem: Rectangle {
                        width: parent.width
                        height: parent.height * 0.001
                        anchors.left: parent.right
                        anchors.leftMargin: parent.width * 0.5
                        Text {
                            text: qsTr("Направление наклона")
                            color: "#4D8AB3"
                            anchors.left: parent.right
                        }
                   }
                 }
                 CheckBox {
                   id: boost
                   checked: false
                   contentItem: Rectangle {
                        width: parent.width
                        height: parent.height * 0.001
                        anchors.left: parent.right
                        anchors.leftMargin: parent.width * 0.5
                        Text {
                            text: qsTr("Ускорение")
                            color: "#9E4DB3"
                            anchors.left: parent.right
                        }
                   }
                 }
                 CheckBox {
                   id: angular_velocity
                   checked: false
                   contentItem: Rectangle {
                        width: parent.width
                        height: parent.height * 0.001
                        anchors.left: parent.right
                        anchors.leftMargin: parent.width * 0.5
                        Text {
                            text: qsTr("Угловая скорость")
                            color: "#AA5566"
                            anchors.left: parent.right
                        }
                   }
                 }
                 CheckBox {
                   id: angleX
                   checked: false
                   contentItem: Rectangle {
                        width: parent.width
                        height: parent.height * 0.001
                        anchors.left: parent.right
                        anchors.leftMargin: parent.width * 0.5
                        Text {
                            text: qsTr("Угол X")
                            color: "#808080"
                            anchors.left: parent.right
                        }
                   }
                 }
                 CheckBox {
                   id: angleY
                   checked: false
                   contentItem: Rectangle {
                        width: parent.width
                        height: parent.height * 0.001
                        anchors.left: parent.right
                        anchors.leftMargin: parent.width * 0.5
                        Text {
                            text: qsTr("Угол Y")
                            color: "#F79709"
                            anchors.left: parent.right
                        }
                   }
                 }
                 CheckBox {
                   id: angleZ
                   checked: false
                   contentItem: Rectangle {
                        width: parent.width
                        height: parent.height * 0.001
                        anchors.left: parent.right
                        anchors.leftMargin: parent.width * 0.5
                        Text {
                            text: qsTr("Угол Z")
                            color: "#F79709"
                            anchors.left: parent.right
                        }
                   }
                 }
            }
        }


        //пролистывание
        Rectangle
        {
            id: minus
            width:parent.width * 0.3
            height:parent.height * 0.035
            anchors.bottom:parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            color:"#bdde1b"
            opacity: 0.3
            Label {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                font.pointSize: parent.height * 0.7
                text: "<-"
            }
            MouseArea
            {
                anchors.fill:parent
                onClicked:{
//                        timer.start();
                    if(swipeView.currentIndex>0)
                        swipeView.currentIndex--
                }
            }
        }

    }

}



