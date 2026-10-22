import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material


Page {
    id: informationPage

    // ---- Палитра ----
    readonly property color cBg:        "#eef3f8"
    readonly property color cCard:      "#ffffff"
    readonly property color cBorder:    "#e0e6ed"
    readonly property color cText:      "#263238"
    readonly property color cTextSoft:  "#546e7a"
    readonly property color cPrimary:   "#5b7cfa"
    readonly property color cPrimaryD:  "#3f5fd6"

    // Фон страницы
    Rectangle {
        anchors.fill: parent
        color: cBg
        z: -1
    }

    ComboBox {
        id: paramBox
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.98
        height: 40

        model: ListModel {
               id: model
               ListElement { text: "..." }
               ListElement { text: "Включение" }
               ListElement { text: "Начало работы" }
               ListElement { text: "Режимы работы" }
               ListElement { text: "Настройка и калибровка" }
               ListElement { text: "Обновление прошивки шара" }
               ListElement { text: "Выключение" }
               ListElement { text: "Возможные ошибки" }
               ListElement { text: "Зарядка" }
               ListElement { text: "Меры предосторожности" }
           }

        palette.windowText: cText
        palette.highlight: cPrimary
        palette.highlightedText: "#ffffff"

        background: Rectangle {
            radius: 6
            color: cCard
            border.width: 1
            border.color: cBorder
        }

        contentItem: Text {
            text: paramBox.displayText
            color: cText
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
            leftPadding: 8
        }

        indicator: Rectangle {
            implicitWidth: 24
            implicitHeight: 24
            x: paramBox.width - width - 6
            y: paramBox.topPadding + (paramBox.availableHeight - height) / 2
            radius: 4
            color: "transparent"
            border.width: 0

            Text {
                anchors.centerIn: parent
                text: "▾"
                color: cPrimaryD
                font.pixelSize: 14
            }
        }

        onCurrentIndexChanged: {
            //пустая строка
            if(currentIndex === 0)  info.clear();
            //Включение
            else if(currentIndex === 1) info.enabling();
            //Начало работы
            else if(currentIndex === 2) info.gettingStarted();
            //Режимы работы
            else if(currentIndex === 3) info.operatingModes();
            //Настройка и калибровка
            else if(currentIndex === 4) info.setupAndCalibration();
            //Обновление прошивки шара
            else if(currentIndex === 5) info.updatingHex();
            //Выключение
            else if(currentIndex === 6) info.shutdown();
            //Возможные ошибки
            else if(currentIndex === 7) info.possibleErrors();
            //Зарядка
            else if(currentIndex === 8) info.charging();
            //Меры предосторожности
            else if(currentIndex === 9) info.precautionaryMeasures();
        }
    }

    // Карточка с текстом — показывается только когда есть текст
    Rectangle {
        id: textCard
        anchors.top: paramBox.bottom
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 0.98
        color: cCard
        radius: 8
        border.width: 1
        border.color: cBorder
        clip: true

        // Показываем только при непустом тексте
        visible: txt.text.length > 0

        // Высота по содержимому, но не больше доступного места
        height: Math.min(txt.contentHeight + 24,
                         informationPage.height - paramBox.height - 24)
    }

    Flickable {
        id: flick_text
        anchors.fill: textCard
        anchors.margins: 1
        contentWidth: width
        contentHeight: txt.contentHeight + 16
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        ScrollBar.vertical: ScrollBar {
            id: vbar
            policy: ScrollBar.AsNeeded
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            width: 10

            contentItem: Rectangle {
                implicitWidth: 4
                radius: 2
                color: vbar.pressed ? cPrimary : "#cfd8dc"
            }

            background: Rectangle {
                implicitWidth: 10
                color: "transparent"
            }
        }

        Text {
            id: txt
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.leftMargin: 8
            width: parent.width - 18
            text: info.info_str_
            wrapMode: Text.Wrap
            font.family: "Courier New"
            font.pixelSize: 13
            color: cTextSoft
            lineHeight: 1.15
        }
    }
}
