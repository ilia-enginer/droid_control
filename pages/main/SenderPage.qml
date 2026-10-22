import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Page {
    id: terminalPage

    // ---- Палитра ----
    readonly property color cBg:        "#eef3f8"
    readonly property color cCard:      "#ffffff"
    readonly property color cBorder:    "#e0e6ed"
    readonly property color cText:      "#263238"
    readonly property color cTextSoft:  "#546e7a"
    readonly property color cPrimary:   "#5b7cfa"
    readonly property color cPrimaryD:  "#3f5fd6"
    readonly property color cCoral:     "#ff7a7a"
    readonly property color cCoralD:    "#e05555"
    readonly property color cAmber:     "#ffb74d"
    readonly property color cAmberD:    "#e09a2f"
    readonly property color cMint:      "#66bb6a"
    readonly property color cMintD:     "#43a047"
    readonly property color cLime:      "#c0ca33"
    readonly property color cLimeD:     "#9e9d24"

    property bool addingLog : true
    property bool wrap : false
    property bool selectionActive: false

    // Фон страницы
    Rectangle {
        anchors.fill: parent
        color: cBg
        z: -1
    }

    // Копирование всего лога (кнопка «Копировать всё»)
    function copyLog() {
        var parts = []
        for (var i = 0; i < logListModel.count; ++i) {
            parts.push(logListModel.get(i).msg)
        }
        var text = parts.join("\n").trim()

        clipboardHelperCpp.copyText("")
        if (text.length > 0)
            clipboardHelperCpp.copyText(text)
    }

    // Копирование выделенных строк (клик по строке)
    function updateClipboardFromSelection() {
        var parts = []
        for (var i = 0; i < logListModel.count; ++i) {
            var item = logListModel.get(i)
            if (item.selected)
                parts.push(item.msg)
        }
        var text = parts.join("\n").trim()

        if (text.length > 0) {
            if (!selectionActive) {
                clipboardHelperCpp.copyText("")
                selectionActive = true
                toast.toastOn("буфер обновлён")
            }
            clipboardHelperCpp.copyText(text)
        } else {
            clipboardHelperCpp.copyText("")
            selectionActive = false
            toast.toastOn("выделение снято, буфер очищен")
        }
    }

    Label {
        id: senderPageLabel
        width: parent.width
        wrapMode: Label.Wrap
        anchors.top: parent.top
        horizontalAlignment: Qt.AlignHCenter
        text: commun_display.curDeviceName_
        color: cText
        padding: 6
    }

    Rectangle {
        id: senderBackground
        anchors.top: senderPageLabel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 6
        anchors.rightMargin: 6
        color: cCard
        height: parent.height * 0.5
        radius: 8
        border.width: 1
        border.color: cBorder
    }

    ScrollView {
        id: scrolViewLogArea
        anchors.fill: senderBackground
        anchors.margins: 1
        clip: true

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
            width: 10

            contentItem: Rectangle {
                implicitWidth: 4
                radius: 2
                color: scrollBar.pressed ? cPrimary : "#cfd8dc"
            }

            background: Rectangle {
                implicitWidth: 10
                color: "transparent"
            }

            Button {
                anchors.top: parent.top
                width: parent.width
                height: 22
                background: Rectangle {
                    radius: 4
                    color: parent.pressed ? "#bbdefb" : "#e3f2fd"
                    border.width: 1
                    border.color: "#90caf9"
                }
                contentItem: Text {
                    text: "^"
                    color: cPrimaryD
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: { scrollBar.decrease() }
            }
            Button {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 22
                background: Rectangle {
                    radius: 4
                    color: parent.pressed ? "#bbdefb" : "#e3f2fd"
                    border.width: 1
                    border.color: "#90caf9"
                }
                contentItem: Text {
                    text: "v"
                    color: cPrimaryD
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: { scrollBar.increase() }
            }
        }

        ListView {
            id: listView1
            anchors.fill: parent
            anchors.margins: 2
            anchors.rightMargin: 12
            snapMode: ListView.SnapToItem
            clip: true
            spacing: 2
            model: logListModel

            Connections {
                target: commun_display
                function onLogT(type, msg) {
                    if(!addingLog)   return;
                    logListModel.append({ msg: type + msg, selected: false })
                    listView1.positionViewAtEnd()
                }
            }

            delegate: Rectangle {
                id: rowRoot
                width: listView1.width * 0.97
                height: rowText.implicitHeight + 6
                radius: 4
                color: selected ? "#d6eaff"
                                  : (index % 2 === 0 ? "#f7f9fc" : "transparent")
                border.width: selected ? 1 : 0
                border.color: cPrimary

                required property int index
                required property string msg
                required property bool selected

                Text {
                    id: rowText
                    anchors.left: parent.left
                    anchors.leftMargin: 6
                    anchors.right: parent.right
                    anchors.rightMargin: 6
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width - 12
                    text: msg
                    font.family: "Courier New"
                    font.pixelSize: 14
                    wrapMode: Text.Wrap
                    color: selected ? "#0d47a1" : cText
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton

                    onClicked: {
                        logListModel.setProperty(index, "selected", !selected)
                        updateClipboardFromSelection()
                    }

                    onDoubleClicked: {
                        logListModel.setProperty(index, "selected", true)
                        updateClipboardFromSelection()
                    }
                }
            }

            ListModel {
                id: logListModel
            }
        }
    }

    // уведомление о копировании
    ToolTip {
        id: toast
        delay: 0
        timeout: 1000
        x: (parent.width - width) / 2
        y: senderBackground.y + senderBackground.height

        enter: Transition {
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 450 }
        }
        exit: Transition {
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 250 }
        }

        background: Rectangle {
            color: "#7f856f"
            radius: 15
            border.width: 1
            border.color: "#aab39a"
        }
        contentItem: Text {
            text: toast.text
            color: "#ffffff"
            wrapMode: Text.Wrap
        }
        function toastOn(msg){
            toast.text = qsTr(msg)
            toast.visible = true
        }
    }

    TextField {
        id: messageToSend
        placeholderText: "..."
        anchors.top: senderBackground.bottom
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 6
        anchors.rightMargin: 6
        validator: RegularExpressionValidator {regularExpression: /[0-9A-Fa-f]+/}
        color: cText
        padding: 8

        background: Rectangle {
            radius: 6
            color: cCard
            border.width: 1
            border.color: cBorder
        }
    }

    Button {
        id: sendButton
        anchors.top: messageToSend.bottom
        anchors.topMargin: 6
        anchors.left: parent.left
        anchors.leftMargin: 6
        height: parent.height * 0.08
        width: parent.width * 0.49 - 6
        text: "Отправить"

        background: Rectangle {
            radius: 8
            color: sendButton.pressed ? cMintD : (sendButton.hovered ? "#81c784" : cMint)
            border.width: 1
            border.color: cMintD
            Behavior on color { ColorAnimation { duration: 120 } }
        }
        contentItem: Text {
            text: sendButton.text
            color: "#ffffff"
            font.pixelSize: 13
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        onClicked: {
            packing.sendMessage(messageToSend.text, wrap);
            if (needClear.checked) {
                messageToSend.text = ""
            }
            listView1.positionViewAtEnd()
        }
    }

    Button {
        id: clearButton
        anchors.top: messageToSend.bottom
        anchors.topMargin: 6
        anchors.right: parent.right
        anchors.rightMargin: 6
        height: parent.height * 0.08
        width: parent.width * 0.49 - 6
        text: "Очистить поле вывода"

        background: Rectangle {
            radius: 8
            color: clearButton.pressed ? cCoralD : (clearButton.hovered ? "#ff8a80" : cCoral)
            border.width: 1
            border.color: cCoralD
            Behavior on color { ColorAnimation { duration: 120 } }
        }
        contentItem: Text {
            text: clearButton.text
            color: "#ffffff"
            font.pixelSize: 13
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        onClicked: {
            logListModel.clear()
            clipboardHelperCpp.copyText("")
            selectionActive = false
        }
    }

    // остановка выпадения лога
    Button {
        id: startStopLog
        anchors.top: sendButton.bottom
        anchors.topMargin: 6
        anchors.right: sendButton.right
        anchors.left: sendButton.left
        height: parent.height * 0.08
        text: addingLog ? "Стоп лог" : "Старт лог"

        background: Rectangle {
            radius: 8
            color: addingLog
                ? (startStopLog.pressed ? cAmberD
                                        : (startStopLog.hovered ? "#ffca7a" : cAmber))
                : (startStopLog.pressed ? cMintD
                                        : (startStopLog.hovered ? "#81c784" : cMint))
            border.width: 1
            border.color: addingLog ? cAmberD : cMintD
            Behavior on color { ColorAnimation { duration: 120 } }
        }
        contentItem: Text {
            text: startStopLog.text
            color: "#ffffff"
            font.pixelSize: 13
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        onClicked: {
            addingLog ? addingLog = false : addingLog = true
        }
    }

    Button {
        id: copyLogButton
        anchors.top: clearButton.bottom
        anchors.topMargin: 6
        anchors.left: clearButton.left
        anchors.right: clearButton.right
        height: parent.height * 0.08
        text: "Копировать всё"

        background: Rectangle {
            radius: 8
            color: copyLogButton.pressed ? cLimeD
                                         : (copyLogButton.hovered ? "#d4e157" : cLime)
            border.width: 1
            border.color: cLimeD
            Behavior on color { ColorAnimation { duration: 120 } }
        }
        contentItem: Text {
            text: copyLogButton.text
            color: "#ffffff"
            font.pixelSize: 13
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        onClicked: {
            copyLog()
            if (logListModel.count > 0)
                toast.toastOn("буфер обновлён")
            else
                toast.toastOn("лог пуст")
        }
    }

    CheckBox {
        id: needWrap
        anchors.top: startStopLog.bottom
        anchors.topMargin: 6
        anchors.left: parent.left
        anchors.leftMargin: 16
        checked: false
        text: qsTr("Оборачивать протоколом")
        palette.windowText: cText
        palette.highlight: cPrimary
        palette.highlightedText: "#ffffff"

        onClicked: {
            wrap = checked
        }
    }

    CheckBox {
        id: needClear
        checked: false
        anchors.top: needWrap.bottom
        anchors.left: parent.left
        anchors.leftMargin: 16
        text: qsTr("Очищать поле ввода")
        palette.windowText: cText
        palette.highlight: cPrimary
        palette.highlightedText: "#ffffff"
    }
}
