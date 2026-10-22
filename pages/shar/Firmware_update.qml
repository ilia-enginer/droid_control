import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.settings
import QtQuick.Controls.Material


Page {
    id: firmwareUpdate
    focus: false

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
    readonly property color cDanger:    "#ef5350"
    readonly property color cDangerD:   "#c62828"

    onActiveFocusChanged: {
        if (!activeFocus) {
            window_focus = false
        } else {
            window_focus = true
        }
     }

    property bool window_focus : false
    property bool addingLog : true
    property bool selectionActive: false

    // Фон страницы
    Rectangle {
        anchors.fill: parent
        color: cBg
        z: -1
    }

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
            }
            clipboardHelperCpp.copyText(text)
        } else {
            clipboardHelperCpp.copyText("")
            selectionActive = false
        }
    }

    Label {
        id: senderPageLabel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 6
        anchors.rightMargin: 6
        width: parent.width
        wrapMode: Label.Wrap
        horizontalAlignment: Qt.AlignHCenter
        text: commun_display.curDeviceName_
        color: cText
        padding: 6
    }

    Label {
        id: senderPageLabel2
        anchors.topMargin: 8
        anchors.top: senderPageLabel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 6
        anchors.rightMargin: 6
        width: parent.width
        wrapMode: Label.Wrap
        horizontalAlignment: Qt.AlignHCenter
        text: "Проверьте обновление"
        font.pointSize: parent.height * 0.025
        color: cTextSoft
        padding: 4

        Component.onCompleted: {
                commun_display.onCurrenUpdateChanged.connect(onCurrenUpdateChanged)
            }
        function onCurrenUpdateChanged(msg){
            text = msg
        }
    }

    Button {
        id: get_firmware_update
        width: parent.width * 0.8
        height: parent.height * 0.07
        anchors.top: senderPageLabel2.bottom
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
        visible: Qt.platform.os === "windows" ? false : true
        text: "Проверить обновление"

        background: Rectangle {
            radius: 8
            color: get_firmware_update.pressed ? cMintD
                                               : (get_firmware_update.hovered ? "#81c784" : cMint)
            border.width: 1
            border.color: cMintD
            Behavior on color { ColorAnimation { duration: 120 } }
        }
        contentItem: Text {
            text: get_firmware_update.text
            color: "#ffffff"
            font.pixelSize: 13
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        Component.onCompleted: {
                    commun_display.onCheckUpdate.connect(onCheckUpdate)
                }
        function onCheckUpdate(){
            busyIndicator.visible = false
            update_load.visible = false
            update_bootloader_load.visible = false
            stop_load.visible = false
            get_firmware_update.visible = true

            if(Qt.platform.os === "windows")
            {
                get_firmware_update.visible = false
                update_bootloader_load.visible = true
                update_load.visible = true
            }
        }
        onClicked: {
             updateHexx.checkingUpdates();
        }
    }

    Shortcut{
        sequence: "F8"
        onActivated: {
            update_load.visible = false
            update_bootloader_load.visible = false
            busyIndicator.visible = true
            updateHexx.on_pbWrite_clicked(true)
        }
    }

    Button {
        id: update_load
        width: parent.width * 0.8
        height: parent.height * 0.07
        anchors.top: senderPageLabel2.bottom
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
        visible: Qt.platform.os === "windows" ? true : false
        text: Qt.platform.os === "windows" ? "Загрузить обновление (F8)" : "Загрузить обновление"

        background: Rectangle {
            radius: 8
            color: update_load.pressed ? cAmberD
                                       : (update_load.hovered ? "#ffca7a" : cAmber)
            border.width: 1
            border.color: cAmberD
            Behavior on color { ColorAnimation { duration: 120 } }
        }
        contentItem: Text {
            text: update_load.text
            color: "#ffffff"
            font.pixelSize: 13
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        Component.onCompleted: {
                    commun_display.onUpdateAvailable.connect(onUpdateAvailable)
                }
        function onUpdateAvailable(){
            get_firmware_update.visible = false
            busyIndicator.visible = false
            update_load.visible = true
            stop_load.visible = false
        }
        onClicked: {
            update_load.visible = false
            update_bootloader_load.visible = false
            busyIndicator.visible = true
            updateHexx.on_pbWrite_clicked(true)
        }
    }

    Shortcut{
        sequence: "F9"
        onActivated: {
            update_load.visible = false
            update_bootloader_load.visible = false
            busyIndicator.visible = true
            updateHexx.on_pbWrite_clicked(false)
        }
    }

    Button {
        id: update_bootloader_load
        width: parent.width * 0.8
        height: parent.height * 0.07
        anchors.top: update_load.bottom
        anchors.topMargin: 6
        anchors.horizontalCenter: parent.horizontalCenter
        visible: Qt.platform.os === "windows" ? true : false
        text: Qt.platform.os === "windows" ? "Загрузить bootloader (F9)" : "Загрузить bootloader"

        background: Rectangle {
            radius: 8
            color: update_bootloader_load.pressed ? cCoralD
                                                  : (update_bootloader_load.hovered ? "#ff8a80" : cCoral)
            border.width: 1
            border.color: cCoralD
            Behavior on color { ColorAnimation { duration: 120 } }
        }
        contentItem: Text {
            text: update_bootloader_load.text
            color: "#ffffff"
            font.pixelSize: 13
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        Component.onCompleted: {
                    commun_display.onUpdateBootloaderAvailable.connect(onUpdateBootloaderAvailable)
                }
        function onUpdateBootloaderAvailable(){
            get_firmware_update.visible = false
            busyIndicator.visible = false
            update_load.visible = true
            stop_load.visible = false
            update_bootloader_load.visible = true
        }
        onClicked: {
            update_load.visible = false
            update_bootloader_load.visible = false
            busyIndicator.visible = true
            updateHexx.on_pbWrite_clicked(false)
        }
    }

    BusyIndicator {
        id: busyIndicator
        Material.accent: cPrimary
        anchors.top: senderPageLabel2.bottom
        anchors.topMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
        height: get_firmware_update.height * 0.95

        Component.onCompleted: {
                    commun_display.onCheckUpdateProgress.connect(onCheckUpdateProgress)
                }
        function onCheckUpdateProgress(){
            get_firmware_update.visible = false
            update_load.visible = false
            busyIndicator.visible = true
            stop_load.visible = false
        }
    }

    Shortcut{
        sequence: "F10"
        onActivated: updateHexx.on_pbStop_clicked("")
    }

    Button {
        id: stop_load
        width: parent.width * 0.8
        height: parent.height * 0.07
        anchors.top: busyIndicator.bottom
        anchors.topMargin: 6
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
        text: Qt.platform.os === "windows" ? "Остановить обновление (F10)" : "Остановить обновление"

        background: Rectangle {
            radius: 8
            color: stop_load.pressed ? cDangerD
                                     : (stop_load.hovered ? "#ff6659" : cDanger)
            border.width: 1
            border.color: cDangerD
            Behavior on color { ColorAnimation { duration: 120 } }
        }
        contentItem: Text {
            text: stop_load.text
            color: "#ffffff"
            font.pixelSize: 13
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        Component.onCompleted: {
                    commun_display.onStopUpdate.connect(onStopUpdate)
                }
        onClicked: {
            updateHexx.on_pbStop_clicked("")
        }
        function onStopUpdate(){
            get_firmware_update.visible = false
            update_load.visible = false
            busyIndicator.visible = true
            stop_load.visible = true
        }
    }

    CheckBox {
      id: loging
      anchors.top: update_bootloader_load.bottom
      anchors.topMargin: 8
      anchors.left: parent.left
      anchors.leftMargin: 16
      checked: settParam.loging
      text: qsTr("Включить логирование")
      visible: false
      palette.windowText: cText
      palette.highlight: cPrimary
      palette.highlightedText: "#ffffff"

      onClicked: {
          settParam.loging = checked
      }
    }

    CheckBox {
      id: loging_visible
      anchors.top: loging.top
      anchors.right: parent.right
      anchors.rightMargin: 16
      checked: false
      text: qsTr("Вкл. экран")
      visible: mainModel.adminFlag
      palette.windowText: cText
      palette.highlight: cPrimary
      palette.highlightedText: "#ffffff"

      onClicked: {
          loging.visible = loging_visible.checked
           senderBackground.visible = loging_visible.checked
           scrolViewLogArea.visible = loging_visible.checked
           listView1.visible = loging_visible.checked
           clearButton_1.visible = loging_visible.checked
           permissLog.visible = loging_visible.checked
      }
    }

    Rectangle {
        id: senderBackground
        anchors.top: loging.bottom
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 6
        anchors.rightMargin: 6
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 6
        color: cCard
        radius: 8
        border.width: 1
        border.color: cBorder
        visible: false
    }

    ScrollView {
        id: scrolViewLogArea
        anchors.fill: senderBackground
        anchors.margins: 1
        clip: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ScrollBar.vertical: ScrollBar {
            id: styledScrollBar
            parent: scrolViewLogArea
            x: scrolViewLogArea.mirrored ? 0 : scrolViewLogArea.width - width
            y: scrolViewLogArea.topPadding
            height: scrolViewLogArea.availableHeight
            width: 10

            contentItem: Rectangle {
                implicitWidth: 4
                radius: 2
                color: styledScrollBar.pressed ? cPrimary : "#cfd8dc"
            }

            background: Rectangle {
                implicitWidth: 10
                color: "transparent"
            }
        }
        visible: false

        ListView {
            id: listView1
            anchors.fill: parent
            anchors.margins: 2
            anchors.rightMargin: 12
            snapMode: ListView.SnapToItem
            width: parent.width
            clip: true
            spacing: 2
            visible: false
            model: logListModel

            Connections {
                target: commun_display
                function onLogServis(type, msg) {
                    if(window_focus && loging_visible.visible)
                    {
                        if(!addingLog)   return;
                        logListModel.append({ msg: type + msg, selected: false })
                        listView1.positionViewAtEnd()
                    }
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

    // очистка лога
    Button {
        id: clearButton_1
        width: senderBackground.width * 0.1
        height: senderBackground.height * 0.42
        anchors.top: senderBackground.top
        anchors.topMargin: 6
        anchors.right: senderBackground.right
        anchors.rightMargin: 6
        opacity: 0.9
        visible: false

        background: Rectangle {
            radius: 6
            color: clearButton_1.pressed ? cCoralD
                                         : (clearButton_1.hovered ? "#ff8a80" : cCoral)
            border.width: 1
            border.color: cCoralD
            Behavior on color { ColorAnimation { duration: 120 } }
        }
        contentItem: Text {
            text: "Clear"
            font.pointSize: 6
            color: "#ffffff"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: {
            logListModel.clear()
            clipboardHelperCpp.copyText("")
            selectionActive = false
        }
    }

    // остановка выпадения лога
    Button {
        id: permissLog
        width: senderBackground.width * 0.1
        height: senderBackground.height * 0.42
        anchors.top: clearButton_1.bottom
        anchors.topMargin: 4
        anchors.right: senderBackground.right
        anchors.rightMargin: 6
        opacity: 0.9
        visible: false

        background: Rectangle {
            radius: 6
            color: addingLog
                ? (permissLog.pressed ? cAmberD
                                      : (permissLog.hovered ? "#ffca7a" : cAmber))
                : (permissLog.pressed ? cMintD
                                      : (permissLog.hovered ? "#81c784" : cMint))
            border.width: 1
            border.color: addingLog ? cAmberD : cMintD
            Behavior on color { ColorAnimation { duration: 120 } }
        }
        contentItem: Text {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: addingLog ? "Стоп" : "Старт"
            font.pointSize: 6
            color: "#ffffff"
        }

        onClicked: {
            addingLog ? addingLog = false : addingLog = true
        }
    }

    Shortcut{
        sequence: "F7"
        onActivated: updateHexx.fileOpen(false);
    }

    Button {
        id: open_file
        width: parent.width * 0.8
        height: parent.height * 0.07
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        visible: Qt.platform.os === "windows"
        opacity: 0.9
        text: "Открыть файл (F7)"

        background: Rectangle {
            radius: 8
            color: open_file.pressed ? cPrimaryD
                                     : (open_file.hovered ? "#7c96ff" : cPrimary)
            border.width: 1
            border.color: cPrimaryD
            Behavior on color { ColorAnimation { duration: 120 } }
        }
        contentItem: Text {
            text: open_file.text
            color: "#ffffff"
            font.pixelSize: 13
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
        onClicked: {
            updateHexx.fileOpen(true);
        }
    }
}
