import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material


Dialog {
    id: aboutDialog
    modal: true
    focus: true
    header: null
    title: "О программе..."

    parent: Overlay.overlay
    z: 1000
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    width: window.width * 0.8
    // Высота по содержимому: колонка + внутренние отступы + рамка + запас
    height: contentColumn.implicitHeight + 28 + aboutDialog.frameThickness * 2
    x: (window.width - aboutDialog.width) / 2
    y: (window.height - aboutDialog.height) / 5

    readonly property color cCard: "#ffffff"
    readonly property color cText: "#263238"
    readonly property color cTextSoft: "#546e7a"

    readonly property int frameThickness: 3
    readonly property int glowMargin: 12

    background: Rectangle {
        id: dialogBg
        radius: 10
        color: cCard
        border.width: 0

        Rectangle {
            id: frameGradient
            anchors.fill: parent
            radius: parent.radius
            border.width: 0

            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    SequentialAnimation on color {
                        loops: Animation.Infinite
                        ColorAnimation { from: "#5b7cfa"; to: "#26a69a"; duration: 1000 }
                        ColorAnimation { from: "#26a69a"; to: "#5b7cfa"; duration: 1000 }
                    }
                }
                GradientStop {
                    position: 0.5
                    SequentialAnimation on color {
                        loops: Animation.Infinite
                        ColorAnimation { from: "#ff7a7a"; to: "#9c27b0"; duration: 1100 }
                        ColorAnimation { from: "#9c27b0"; to: "#ff7a7a"; duration: 1100 }
                    }
                }
                GradientStop {
                    position: 1.0
                    SequentialAnimation on color {
                        loops: Animation.Infinite
                        ColorAnimation { from: "#5b7cfa"; to: "#ffb74d"; duration: 1100 }
                        ColorAnimation { from: "#ffb74d"; to: "#5b7cfa"; duration: 1100 }
                    }
                }
            }

            Rectangle {
                anchors.fill: parent
                anchors.margins: aboutDialog.frameThickness
                radius: parent.radius - aboutDialog.frameThickness
                color: dialogBg.color
            }
        }
    }

    Item {
        id: glowLayer
        anchors.fill: parent
        anchors.margins: -aboutDialog.glowMargin
        clip: false
        z: 3

        readonly property real offsetX: aboutDialog.glowMargin
        readonly property real offsetY: aboutDialog.glowMargin
        readonly property real cardW: aboutDialog.width
        readonly property real cardH: aboutDialog.height

        component GlowDot: Rectangle {
            id: dot
            width: 6
            height: 6
            radius: 3
            border.width: 1
            border.color: "#ffffff"

            property color baseColor: "#5b7cfa"
            property int cycleDuration: 800

            color: baseColor

            Rectangle {
                anchors.centerIn: parent
                width: parent.width + 8
                height: width
                radius: width / 2
                color: dot.color
                opacity: 0.4
                z: -1
            }

            SequentialAnimation on color {
                loops: Animation.Infinite
                ColorAnimation { from: dot.baseColor; to: "#66bb6a"; duration: dot.cycleDuration }
                ColorAnimation { from: "#66bb6a"; to: "#ffb74d"; duration: dot.cycleDuration }
                ColorAnimation { from: "#ffb74d"; to: "#ff7a7a"; duration: dot.cycleDuration }
                ColorAnimation { from: "#ff7a7a"; to: "#26a69a"; duration: dot.cycleDuration }
                ColorAnimation { from: "#26a69a"; to: "#9c27b0"; duration: dot.cycleDuration }
                ColorAnimation { from: "#9c27b0"; to: dot.baseColor; duration: dot.cycleDuration }
            }
        }

        GlowDot { id: glow1; baseColor: "#5b7cfa"; cycleDuration: 600 }
        GlowDot { id: glow2; baseColor: "#66bb6a"; cycleDuration: 700 }
        GlowDot { id: glow3; baseColor: "#ffb74d"; cycleDuration: 650 }
        GlowDot { id: glow4; baseColor: "#ff7a7a"; cycleDuration: 750 }
        GlowDot { id: glow5; baseColor: "#26a69a"; cycleDuration: 620 }
        GlowDot { id: glow6; baseColor: "#9c27b0"; cycleDuration: 720 }
        GlowDot { id: glow7; baseColor: "#03a9f4"; cycleDuration: 680 }
        GlowDot { id: glow8; baseColor: "#e91e63"; cycleDuration: 740 }

        SequentialAnimation {
            loops: Animation.Infinite
            running: aboutDialog.visible
            PauseAnimation { duration: 0 }
            ParallelAnimation {
                NumberAnimation { target: glow1; property: "x"; from: glowLayer.offsetX - glow1.width; to: glowLayer.offsetX + glowLayer.cardW; duration: 1400 }
                NumberAnimation { target: glow1; property: "y"; from: glowLayer.offsetY - glow1.height; to: glowLayer.offsetY - glow1.height; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow1; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX + glowLayer.cardW; duration: 1000 }
                NumberAnimation { target: glow1; property: "y"; from: glowLayer.offsetY - glow1.height; to: glowLayer.offsetY + glowLayer.cardH; duration: 1000 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow1; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX - glow1.width; duration: 1400 }
                NumberAnimation { target: glow1; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY + glowLayer.cardH; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow1; property: "x"; from: glowLayer.offsetX - glow1.width; to: glowLayer.offsetX - glow1.width; duration: 1000 }
                NumberAnimation { target: glow1; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY - glow1.height; duration: 1000 }
            }
        }

        SequentialAnimation {
            loops: Animation.Infinite
            running: aboutDialog.visible
            PauseAnimation { duration: 400 }
            ParallelAnimation {
                NumberAnimation { target: glow2; property: "x"; from: glowLayer.offsetX - glow2.width; to: glowLayer.offsetX + glowLayer.cardW; duration: 1400 }
                NumberAnimation { target: glow2; property: "y"; from: glowLayer.offsetY - glow2.height; to: glowLayer.offsetY - glow2.height; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow2; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX + glowLayer.cardW; duration: 1000 }
                NumberAnimation { target: glow2; property: "y"; from: glowLayer.offsetY - glow2.height; to: glowLayer.offsetY + glowLayer.cardH; duration: 1000 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow2; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX - glow2.width; duration: 1400 }
                NumberAnimation { target: glow2; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY + glowLayer.cardH; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow2; property: "x"; from: glowLayer.offsetX - glow2.width; to: glowLayer.offsetX - glow2.width; duration: 1000 }
                NumberAnimation { target: glow2; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY - glow2.height; duration: 1000 }
            }
        }

        SequentialAnimation {
            loops: Animation.Infinite
            running: aboutDialog.visible
            PauseAnimation { duration: 800 }
            ParallelAnimation {
                NumberAnimation { target: glow3; property: "x"; from: glowLayer.offsetX - glow3.width; to: glowLayer.offsetX + glowLayer.cardW; duration: 1400 }
                NumberAnimation { target: glow3; property: "y"; from: glowLayer.offsetY - glow3.height; to: glowLayer.offsetY - glow3.height; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow3; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX + glowLayer.cardW; duration: 1000 }
                NumberAnimation { target: glow3; property: "y"; from: glowLayer.offsetY - glow3.height; to: glowLayer.offsetY + glowLayer.cardH; duration: 1000 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow3; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX - glow3.width; duration: 1400 }
                NumberAnimation { target: glow3; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY + glowLayer.cardH; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow3; property: "x"; from: glowLayer.offsetX - glow3.width; to: glowLayer.offsetX - glow3.width; duration: 1000 }
                NumberAnimation { target: glow3; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY - glow3.height; duration: 1000 }
            }
        }

        SequentialAnimation {
            loops: Animation.Infinite
            running: aboutDialog.visible
            PauseAnimation { duration: 1200 }
            ParallelAnimation {
                NumberAnimation { target: glow4; property: "x"; from: glowLayer.offsetX - glow4.width; to: glowLayer.offsetX + glowLayer.cardW; duration: 1400 }
                NumberAnimation { target: glow4; property: "y"; from: glowLayer.offsetY - glow4.height; to: glowLayer.offsetY - glow4.height; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow4; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX + glowLayer.cardW; duration: 1000 }
                NumberAnimation { target: glow4; property: "y"; from: glowLayer.offsetY - glow4.height; to: glowLayer.offsetY + glowLayer.cardH; duration: 1000 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow4; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX - glow4.width; duration: 1400 }
                NumberAnimation { target: glow4; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY + glowLayer.cardH; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow4; property: "x"; from: glowLayer.offsetX - glow4.width; to: glowLayer.offsetX - glow4.width; duration: 1000 }
                NumberAnimation { target: glow4; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY - glow4.height; duration: 1000 }
            }
        }

        SequentialAnimation {
            loops: Animation.Infinite
            running: aboutDialog.visible
            PauseAnimation { duration: 1600 }
            ParallelAnimation {
                NumberAnimation { target: glow5; property: "x"; from: glowLayer.offsetX - glow5.width; to: glowLayer.offsetX + glowLayer.cardW; duration: 1400 }
                NumberAnimation { target: glow5; property: "y"; from: glowLayer.offsetY - glow5.height; to: glowLayer.offsetY - glow5.height; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow5; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX + glowLayer.cardW; duration: 1000 }
                NumberAnimation { target: glow5; property: "y"; from: glowLayer.offsetY - glow5.height; to: glowLayer.offsetY + glowLayer.cardH; duration: 1000 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow5; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX - glow5.width; duration: 1400 }
                NumberAnimation { target: glow5; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY + glowLayer.cardH; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow5; property: "x"; from: glowLayer.offsetX - glow5.width; to: glowLayer.offsetX - glow5.width; duration: 1000 }
                NumberAnimation { target: glow5; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY - glow5.height; duration: 1000 }
            }
        }

        SequentialAnimation {
            loops: Animation.Infinite
            running: aboutDialog.visible
            PauseAnimation { duration: 2000 }
            ParallelAnimation {
                NumberAnimation { target: glow6; property: "x"; from: glowLayer.offsetX - glow6.width; to: glowLayer.offsetX + glowLayer.cardW; duration: 1400 }
                NumberAnimation { target: glow6; property: "y"; from: glowLayer.offsetY - glow6.height; to: glowLayer.offsetY - glow6.height; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow6; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX + glowLayer.cardW; duration: 1000 }
                NumberAnimation { target: glow6; property: "y"; from: glowLayer.offsetY - glow6.height; to: glowLayer.offsetY + glowLayer.cardH; duration: 1000 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow6; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX - glow6.width; duration: 1400 }
                NumberAnimation { target: glow6; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY + glowLayer.cardH; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow6; property: "x"; from: glowLayer.offsetX - glow6.width; to: glowLayer.offsetX - glow6.width; duration: 1000 }
                NumberAnimation { target: glow6; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY - glow6.height; duration: 1000 }
            }
        }

        SequentialAnimation {
            loops: Animation.Infinite
            running: aboutDialog.visible
            PauseAnimation { duration: 2400 }
            ParallelAnimation {
                NumberAnimation { target: glow7; property: "x"; from: glowLayer.offsetX - glow7.width; to: glowLayer.offsetX + glowLayer.cardW; duration: 1400 }
                NumberAnimation { target: glow7; property: "y"; from: glowLayer.offsetY - glow7.height; to: glowLayer.offsetY - glow7.height; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow7; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX + glowLayer.cardW; duration: 1000 }
                NumberAnimation { target: glow7; property: "y"; from: glowLayer.offsetY - glow7.height; to: glowLayer.offsetY + glowLayer.cardH; duration: 1000 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow7; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX - glow7.width; duration: 1400 }
                NumberAnimation { target: glow7; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY + glowLayer.cardH; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow7; property: "x"; from: glowLayer.offsetX - glow7.width; to: glowLayer.offsetX - glow7.width; duration: 1000 }
                NumberAnimation { target: glow7; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY - glow7.height; duration: 1000 }
            }
        }

        SequentialAnimation {
            loops: Animation.Infinite
            running: aboutDialog.visible
            PauseAnimation { duration: 2800 }
            ParallelAnimation {
                NumberAnimation { target: glow8; property: "x"; from: glowLayer.offsetX - glow8.width; to: glowLayer.offsetX + glowLayer.cardW; duration: 1400 }
                NumberAnimation { target: glow8; property: "y"; from: glowLayer.offsetY - glow8.height; to: glowLayer.offsetY - glow8.height; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow8; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX + glowLayer.cardW; duration: 1000 }
                NumberAnimation { target: glow8; property: "y"; from: glowLayer.offsetY - glow8.height; to: glowLayer.offsetY + glowLayer.cardH; duration: 1000 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow8; property: "x"; from: glowLayer.offsetX + glowLayer.cardW; to: glowLayer.offsetX - glow8.width; duration: 1400 }
                NumberAnimation { target: glow8; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY + glowLayer.cardH; duration: 1400 }
            }
            ParallelAnimation {
                NumberAnimation { target: glow8; property: "x"; from: glowLayer.offsetX - glow8.width; to: glowLayer.offsetX - glow8.width; duration: 1000 }
                NumberAnimation { target: glow8; property: "y"; from: glowLayer.offsetY + glowLayer.cardH; to: glowLayer.offsetY - glow8.height; duration: 1000 }
            }
        }
    }

    Column {
        id: contentColumn
        width: parent.width
        // Высота по содержимому
        height: implicitHeight
        spacing: 5
        padding: 14

        Label {
            width: parent.width
            text: "О программе..."
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            font.pixelSize: 14
            font.bold: true
            color: cText
            padding: 2
        }

        Label {
            width: parent.width
            text: "Droid Stick - предназначена для управления роботизированныйми устройствами по Bluetooch. Приложение активируется после подключения к устройству."
            wrapMode: Label.Wrap
            font.pixelSize: 12
            color: cTextSoft
            padding: 4
        }
    }
}
