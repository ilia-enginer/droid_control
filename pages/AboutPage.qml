import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material


Dialog {
    id: aboutDialog
    modal: true
    focus: true
    title: "О программе..."
    width: window.width * 0.6
    height: window.height * 0.3
    x: (window.width - aboutDialog.width) / 2
    y: (window.height - aboutDialog.height) / 5


    Column {
        width: parent.width
        height: parent.height
        spacing: 5

        Label {
            width: aboutDialog.availableWidth
            text: "Droid Stick - предназначена для управления роботизированныйми устройствами по Bluetooch."
            wrapMode: Label.Wrap
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
        }
/*
        Flickable {
             id: flick
             width: parent.width * 0.95
             height: parent.height * 0.7
             contentWidth: edit.contentWidth
             contentHeight: edit.contentHeight
             clip: true

             function ensureVisible(r)
             {
                 if (contentX >= r.x)
                     contentX = r.x;
                 else if (contentX+width <= r.x+r.width)
                     contentX = r.x+r.width-width;
                 if (contentY >= r.y)
                     contentY = r.y;
                 else if (contentY+height <= r.y+r.height)
                     contentY = r.y+r.height-height;
             }

             TextEdit {
                 id: edit
                 width: flick.width
                 color: "#7387d1"
                 focus: false
                 font: font.pixelSize = 13
                 wrapMode: TextEdit.Wrap
                 onCursorRectangleChanged: flick.ensureVisible(cursorRectangle)

                 Text {
                     width: parent.width
                     text: "Здесь вы можете оставить пожелания или отзыв о работе приложения..."
                     wrapMode: TextEdit.Wrap
                     color: "#7387d1"
                     visible: !edit.text && !edit.activeFocus // <----------- ;-)
                 }
             }
         }

        Button {
            id: seng_feedback
            width: parent.width * 0.85
            height: parent.height * 0.1
            anchors.horizontalCenter: parent.horizontalCenter
            text: "отправить"
            background: Rectangle{
                property var normalColor: "#7387d1"
                property var pressedColor: "#052bb3"
                color: seng_feedback.pressed ? pressedColor : normalColor
            }
            onClicked: {
                seng_feedback.visible = false;
                busyIndicator.visible = true;

                feedback.sendFedback(edit.text);
            }
        }

        BusyIndicator {
            id: busyIndicator
            Material.accent: Material.Light
            anchors.bottom: seng_feedback.bottom
            visible: false
            height: seng_feedback.height * 0.9
            anchors.horizontalCenter: seng_feedback.horizontalCenter

            Component.onCompleted: {
                feedback.onMessFinish.connect(onMessFinish)
            }

            function onMessFinish(){
                busyIndicator.visible = false;
                sendLabel.visible = true;
            }
        }

        Label {
            id: sendLabel
            width: seng_feedback.width
            height: seng_feedback.height
            text: "Отзыв отправлен"
            wrapMode: Label.Wrap
            anchors.horizontalCenter: seng_feedback.horizontalCenter
            font.pixelSize: 14
            color: "#7387d1"
            visible: false
        }
      */
    }
}

