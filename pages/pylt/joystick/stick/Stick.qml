import QtQuick 2.7
import QtQuick.Controls 2.0
import QtCharts 2.15
import QtQml
import QtQuick.Controls.Material 2.15


Item {
    id: stick

    property bool verticalOnly : false
    property bool horizontalOnly : false
    property real angle : 0

    signal joystick_moved(double x, double y);

    property real azimuth : 0
    property real amplitude : 0
    property real offsetX : 0
    property real offsetY : 0


    //углы джойстика
    Label {
        id: offsetInfo
        wrapMode: Label.Wrap
        anchors.bottom: joystick.top
        anchors.horizontalCenter: joystick.horizontalCenter
        text: "0/0"
        visible: mainModel.adminFlag
    }

    Image {
        id: joystick
        source: "background.png"
        anchors.centerIn: parent

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
        anchors.fill: joystick

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

        onReleased: { if(!fixed.checked) mouse_touch.joyReset(); }
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
        function joyReset() {
            returnAnimation.restart()
            joystick_moved(0, 0);
            offsetX = 0
            offsetY = 0
            amplitude = 0
            azimuth = 0
            offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
        }
    }

    Image {
        id: joystick_buttons
        anchors.fill: joystick
        visible: false

        RoundButton {
            id: upBut
            radius: 8
            highlighted: true
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            height: joystick_buttons.height * 0.3
            width: joystick_buttons.width * 0.3
            Text {
                anchors.centerIn: parent
                text: qsTr("˄")
                font.pixelSize: 25
                color: "white"
            }

            onPressed: {
                amplitude = 1.0
                azimuth = 1.57
                offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
            }
            onReleased: { if(!fixed.checked)  mouse_touch.joyReset(); }
        }
        RoundButton {
            id: downBut
            radius: 8
            highlighted: true
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            height: joystick_buttons.height * 0.3
            width: joystick_buttons.width * 0.3
            Text {
                anchors.centerIn: parent
                text: qsTr("˅")
                font.pixelSize: 25
                color: "white"
            }

            onPressed: {
                amplitude = 1.0
                azimuth = -1.57
                offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
            }
            onReleased: { if(!fixed.checked)    mouse_touch.joyReset(); }
        }
        RoundButton {
            radius: 8
            highlighted: true
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            height: joystick_buttons.height * 0.3
            width: joystick_buttons.width * 0.3
            Text {
                anchors.centerIn: parent
                text: qsTr("˂")
                font.pixelSize: 25
                color: "white"
            }

            onPressed: {
                amplitude = 1.0
                azimuth = -3.14
                offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
            }
            onReleased: { if(!fixed.checked)    mouse_touch.joyReset(); }
        }
        RoundButton {
            radius: 8
            highlighted: true
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            height: joystick_buttons.height * 0.3
            width: joystick_buttons.width * 0.3
            Text {
                anchors.centerIn: parent
                text: qsTr("˃")
                font.pixelSize: 25
                color: "white"
            }

            onPressed: {
                amplitude = 1.0
                azimuth = 0
                offsetInfo.text = Math.round(azimuth * 100) / 100 + "/" + Math.round(amplitude * 100) / 100
            }
            onReleased: { if(!fixed.checked)    mouse_touch.joyReset(); }
        }
    }

    Column{
        id: spac
        spacing: -3
        anchors.horizontalCenter: joystick.horizontalCenter
        anchors.top: joystick.bottom

        Switch {
            text: "джойстик"
            checked: true
            onCheckedChanged:  {
               joystick.visible = checked
               joystick_buttons.visible = !checked
            }
        }
        Switch {
            id: fixed
            text: "фиксация"
            checked: false
            onClicked: { if(!fixed.checked)  mouse_touch.joyReset(); }
        }
    }

    RoundButton {
        anchors.top: spac.top
        anchors.bottom: spac.bottom
        anchors.left: spac.right
        width: joystick.width * 0.25
        highlighted: true
        visible: fixed.checked
        radius: 8
        Text{
            anchors.centerIn: parent
            text: qsTr("reset")
            rotation: 270
            font.pixelSize: 17
        }
        onPressed: { mouse_touch.joyReset(); }
    }
}
