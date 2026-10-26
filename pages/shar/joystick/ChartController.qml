import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCharts 2.15

Item {
    id: root

    // Сигналы для внешнего мира (SwipeView)
    signal interactionStarted()
    signal interactionEnded()

    // Текущее состояние: true, пока пользователь тащит/щипает график
    property bool interacting: false

    readonly property var channelColors: [
        "#e05a5a", "#4caf50", "#f0c419", "#5b8def", "#ef8b3a",
        "#9b6bd6", "#3bbcc4", "#e06fb8", "#7cb342"
    ]
    readonly property var channelNames: [
        "Напряжение", "Ток", "Угол наклона",
        "Направл. накл.", "Ускорение", "Угл. скор.",
        "Угол X", "Угол Y", "Угол Z"
    ]

    readonly property color btnZoomColor:   "#7fa8d8"
    readonly property color btnClearColor:  "#e0a08f"
    readonly property color btnFollowColor: "#8fc98f"
    readonly property color btnTextColor:   "#1f2d3d"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 4
        spacing: 4

        // ------------------ График ------------------
        ChartView {
            id: chartView
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 60          // маленький минимум — график сможет сжаться
            Layout.preferredHeight: 300
            antialiasing: true
            animationOptions: ChartView.NoAnimation

            ValueAxis {
                id: axisX
                min: chartController.xMin
                max: chartController.xMax
            }
            ValueAxis {
                id: axisY
                min: chartController.yMin
                max: chartController.yMax

                // до 2 знаков после запятой; при больших диапазонах — 1 или 0
                labelFormat: {
                    var range = Math.abs(max - min)
                    if (range >= 100) return "%.0f"
                    if (range >= 10)  return "%.1f"
                    return "%.3f"
                }
            }

            LineSeries { id: s0; axisX: axisX; axisY: axisY; color: channelColors[0] }
            LineSeries { id: s1; axisX: axisX; axisY: axisY; color: channelColors[1] }
            LineSeries { id: s2; axisX: axisX; axisY: axisY; color: channelColors[2] }
            LineSeries { id: s3; axisX: axisX; axisY: axisY; color: channelColors[3] }
            LineSeries { id: s4; axisX: axisX; axisY: axisY; color: channelColors[4] }
            LineSeries { id: s5; axisX: axisX; axisY: axisY; color: channelColors[5] }
            LineSeries { id: s6; axisX: axisX; axisY: axisY; color: channelColors[6] }
            LineSeries { id: s7; axisX: axisX; axisY: axisY; color: channelColors[7] }
            LineSeries { id: s8; axisX: axisX; axisY: axisY; color: channelColors[8] }

            Component.onCompleted: {
                chartController.registerSeries(s0, 0)
                chartController.registerSeries(s1, 1)
                chartController.registerSeries(s2, 2)
                chartController.registerSeries(s3, 3)
                chartController.registerSeries(s4, 4)
                chartController.registerSeries(s5, 5)
                chartController.registerSeries(s6, 6)
                chartController.registerSeries(s7, 7)
                chartController.registerSeries(s8, 8)
            }

            DragHandler {
                id: chartDrag
                target: null  // не перемещаем сам ChartView, а используем для вычисления смещения

                property real lastX: 0
                property real lastY: 0
                property bool dragged: false

                onActiveChanged: {
                    if (active) {
                        // При начале перетаскивания запоминаем начальную позицию
                        lastX = centroid.scenePosition.x
                        lastY = centroid.scenePosition.y
                        dragged = false
                        chartController.notifyUserInteraction()
                        root.interacting = true
                        root.interactionStarted()
                    } else {
                        // При завершении перетаскивания
                        if (dragged) {
                            root.interacting = false
                            root.interactionEnded()
                        }
                    }
                }

                onCentroidChanged: {
                    if (!active) return

                    var dx = centroid.scenePosition.x - lastX
                    var dy = centroid.scenePosition.y - lastY

                    // Порог для фильтрации случайных касаний
                    if (!dragged && Math.abs(dx) < 4 && Math.abs(dy) < 4)
                        return

                    if (!dragged) {
                        dragged = true
                    }

                    chartController.panByPixels(dx, dy, chartView.width, chartView.height)
                    lastX = centroid.scenePosition.x
                    lastY = centroid.scenePosition.y
                }
            }
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                // z ниже, чем у DragHandler, чтобы не мешать перетаскиванию
                z: 4

                onDoubleClicked: chartController.autoScaleAll()

                onWheel: function(wheel) {
                    chartController.notifyUserInteraction()
                    var steps = wheel.angleDelta.y / 120.0
                    var factor = Math.pow(0.85, steps)
                    var nx = mouseX / width
                    var ny = 1.0 - mouseY / height
                    chartController.zoomX(factor, nx)
                    chartController.zoomY(factor, ny)
                }
            }

            // --- PinchArea: только multi-touch ---
            PinchArea {
                anchors.fill: parent
                z: 10
                pinch.minimumScale: 0.1
                pinch.maximumScale: 20.0

                property real startScale: 1.0

                onPinchStarted: {
                    startScale = 1.0
                    chartController.notifyUserInteraction()
                    root.interacting = true
                    root.interactionStarted()
                }
                onPinchUpdated: {
                    var factor = startScale / pinch.scale
                    startScale = pinch.scale
                    chartController.zoomBoth(factor)
                }
                onPinchFinished: {
                    root.interacting = false
                    root.interactionEnded()
                }
            }
        }

        // ------------------ Панель: ряд 1 ------------------
        RowLayout {
            Layout.fillWidth: true
            Layout.minimumHeight: 28
            Layout.preferredHeight: 32
            spacing: 4

            StyledButton {
                text: "+"
                baseColor: root.btnZoomColor
                Layout.minimumWidth: 30
                Layout.preferredWidth: 44
                Layout.fillWidth: false
                onClicked: { chartController.notifyUserInteraction(); chartController.zoomBoth(0.8) }
            }
            StyledButton {
                text: "−"
                baseColor: root.btnZoomColor
                Layout.minimumWidth: 30
                Layout.preferredWidth: 44
                Layout.fillWidth: false
                onClicked: { chartController.notifyUserInteraction(); chartController.zoomBoth(1.25) }
            }
            StyledButton {
                text: "Очистить"
                baseColor: root.btnClearColor
                Layout.fillWidth: true
                Layout.minimumWidth: 40
                onClicked: chartController.clear()
            }
            CheckBox {
                id: followCheck
                text: "Следовать"
                checked: chartController.autoFollow
                palette.text: root.btnTextColor
                Layout.fillWidth: false
                Layout.minimumWidth: 0
                onToggled: chartController.autoFollow = checked

                contentItem: Text {
                    text: followCheck.text
                    color: root.btnTextColor
                    font: followCheck.font
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    leftPadding: followCheck.indicator.width + followCheck.spacing
                }
            }
        }

        // ------------------ Панель: ряд 2 (только количество точек) ------------------
        RowLayout {
            Layout.fillWidth: true
            Layout.minimumHeight: 28
            Layout.preferredHeight: 32
            spacing: 4

            Label {
                text: "Точек:"
                color: root.btnTextColor
                Layout.minimumWidth: 0
                elide: Text.ElideRight
            }
            ComboBox {
                id: pointsCombo
                Layout.fillWidth: true
                Layout.minimumWidth: 50
                Layout.preferredHeight: 30
                model: ["50", "100", "200", "Все"]
                currentIndex: 3
                Component.onCompleted: chartController.maxPoints = 0
                onActivated: {
                    if (currentText === "Все") chartController.maxPoints = 0
                    else chartController.maxPoints = parseInt(currentText)
                }
            }
        }

        // ------------------ Панель: ряд 3 (9 чекбоксов) ------------------
        // Используем Flow вместо GridLayout, чтобы чекбоксы переносились
        // на новую строку при нехватке ширины.
        Flow {
            Layout.fillWidth: true
            Layout.minimumHeight: 20
            Layout.preferredHeight: implicitHeight
            spacing: 4

            Repeater {
                model: 9
                CheckBox {
                    id: channelCheck
                    checked: false
                    text: channelNames[index]
                    height: 24
                    // ширина по содержимому, но не больше доступной
                    width: Math.min(implicitWidth, root.width / 2)

                    Component.onCompleted:
                        chartController.setChannelEnabled(index, false)

                    contentItem: Text {
                        text: channelCheck.text
                        color: channelColors[index % channelColors.length]
                        font: channelCheck.font
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                        leftPadding: channelCheck.indicator.width + channelCheck.spacing
                    }

                    indicator: Rectangle {
                        implicitWidth: 18
                        implicitHeight: 18
                        x: channelCheck.leftPadding
                        y: parent.height / 2 - height / 2
                        radius: 3
                        border.color: channelColors[index % channelColors.length]
                        border.width: 2
                        color: "transparent"

                        Text {
                            visible: channelCheck.checked
                            text: "✓"
                            anchors.centerIn: parent
                            font.pixelSize: 12
                            color: channelColors[index % channelColors.length]
                        }
                    }

                    onToggled: chartController.setChannelEnabled(index, checked)
                }
            }
        }
    }

    // ------------------ Стилизованная кнопка ------------------
    component StyledButton: Button {
        id: sb
        property color baseColor: "#7fa8d8"

        implicitHeight: 30
        implicitWidth: 40
        leftPadding: 6
        rightPadding: 6
        topPadding: 2
        bottomPadding: 2

        contentItem: Text {
            text: sb.text
            color: sb.palette.buttonText
            font.pixelSize: 13
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            radius: 6
            color: {
                if (sb.pressed) return Qt.darker(sb.baseColor, 1.15)
                if (sb.hovered) return Qt.lighter(sb.baseColor, 1.10)
                return sb.baseColor
            }
            border.color: Qt.darker(sb.baseColor, 1.25)
            border.width: 1

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 2
                radius: parent.radius
                color: Qt.darker(sb.baseColor, 1.30)
                opacity: sb.pressed ? 0.0 : 0.6
            }

            Behavior on color { ColorAnimation { duration: 80 } }
        }

        palette.buttonText: root.btnTextColor
    }
}
