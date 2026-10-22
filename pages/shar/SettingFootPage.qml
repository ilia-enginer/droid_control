import QtQuick 2.15
import QtQuick.Controls 2.0
import QtQuick.Layouts
import Qt.labs.settings
import QtQuick.Controls.Material
import QtQml

Page {
    id: settingFoot
    focus: true
    activeFocusOnTab: true

    // Принудительно забираем фокус при появлении страницы
    Component.onCompleted: settingFoot.forceActiveFocus()

    // Если страница создаётся заранее (StackView/SwipeView), ловим момент,
    // когда она реально стала видимой
    onVisibleChanged: {
        if (visible)
            settingFoot.forceActiveFocus()
    }

    onActiveFocusChanged: {
        if (!activeFocus) {
            getcurtimer.running = false
            get_foot_param.visible = false
            set_foot_param.visible = false
            box1.checked = false
            box2.checked = false
            box3.checked = false
            box4.checked = false
            box5.checked = false
            box6.checked = false

            window_focus = false
        } else {
            getcurtimer.running = true
            window_focus = true
        }
    }

    property bool window_focus : false
    property bool addingLog : true

    property int butClick : 0
    property int coxaL : 0
    property int coxaH : 0
    property int femurL : 0
    property int femurH : 0
    property int tibaL : 0
    property int tibaH : 0

    property int pca1 : 0
    property int pca2 : 0
    property int speedServs : 0

    // ---- Приятная палитра ----
    readonly property color cPrimary:    "#5b7cfa"
    readonly property color cPrimaryD:   "#3f5fd6"
    readonly property color cTeal:       "#26a69a"
    readonly property color cTealD:      "#00796b"
    readonly property color cCoral:      "#ff7a7a"
    readonly property color cCoralD:     "#e05555"
    readonly property color cAmber:      "#ffb74d"
    readonly property color cAmberD:     "#e09a2f"
    readonly property color cMint:       "#66bb6a"
    readonly property color cMintD:      "#43a047"
    readonly property color cText:       "#263238"
    readonly property color cTextLight:  "#ffffff"
    readonly property color cBg:         "#f4f7fb"

    // Буфер обмена: очищается при старте нового выделения
    property bool selectionActive: false

    function updateClipboardFromSelection() {
        var parts = []
        for (var i = 0; i < logListModel.count; ++i) {
            var item = logListModel.get(i)
            if (item.selected)
                parts.push(item.msg)
        }
        var text = parts.join("\n").trim()

        if (text.length > 0) {
            // Если это первое выделение после «чистого» состояния —
            // очищаем буфер, чтобы не смешать с текстом из другого файла
            if (!selectionActive) {
                clipboardHelperCpp.copyText("")
                selectionActive = true
            }
            clipboardHelperCpp.copyText(text)
        } else {
            // Выделение снято полностью — очищаем буфер
            clipboardHelperCpp.copyText("")
            selectionActive = false
        }
    }

    Label {
        id: senderPageLabel
        anchors.top: parent.top
        width: parent.width
        wrapMode: Label.Wrap
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Qt.AlignHCenter
        text: commun_display.curDeviceName_
    }

    Label {
        id: warring
        anchors.top: senderPageLabel.bottom
        width: parent.width
        wrapMode: Label.Wrap
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Qt.AlignHCenter
        text: "Не меняйте настройки, если не уверены!"
        color: "#d32f2f"
        font.pointSize: 16
        font.bold: true
        padding: 6
    }

    Rectangle {
        id: senderBackground
        anchors.top: warring.bottom
        color: "#ffffff"
        width: parent.width
        height: parent.height * 0.25
        radius: 8
        border.width: 1
        border.color: "#e0e6ed"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 6
        anchors.rightMargin: 6
    }

    ScrollView {
        id: scrolViewLogArea
        anchors.fill: senderBackground
        anchors.margins: 1
        clip: true

        // Горизонтальный скроллбар отключаем
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        // Вертикальный — прижат к правому краю, реагирует на нажатия
        ScrollBar.vertical: ScrollBar {
            id: vbar
            orientation: Qt.Vertical
            policy: ScrollBar.AlwaysOn
            interactive: true

            // Явно прижимаем к правому краю ScrollView
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: 8

            // Ползунок: стандартный size/position Controls 2
            size: 0.3
            position: 0.0

            contentItem: Rectangle {
                implicitWidth: 4
                radius: 2
                color: vbar.pressed ? "#5b7cfa" : "#cfd8dc"
            }

            // Фон дорожки (опционально, чтобы видеть, куда тянуть)
            background: Rectangle {
                implicitWidth: 8
                color: "transparent"
            }
        }

        ListView {
            id: listView1
            anchors.fill: parent
            anchors.rightMargin: 10
            anchors.margins: 2
            snapMode: ListView.SnapToItem
            width: parent.width
            clip: true
            spacing: 1
            reuseItems: true
            model: logListModel

            Connections {
                target: commun_display
                function onLogJoy(type, msg) {
                    if(!addingLog)   return;
                    if(window_focus)
                    {
                        if (mainModel.adminFlag === false){
                             logListModel.append({msg: type + msg, selected: false})
                             listView1.positionViewAtEnd()
                        }
                    }
                 }
                function onLogServis(type, msg) {
                    if(!addingLog)   return;
                    if(window_focus)
                    {
                        if (mainModel.adminFlag === true){
                            logListModel.append({msg: type + msg, selected: false})
                            listView1.positionViewAtEnd()
                        }
                    }
                }
            }

            delegate: Rectangle {
                id: rowRoot
                width: listView1.width * 0.97
                height: rowText.implicitHeight + 6
                radius: 4
                color: selected ? "#d6eaff" : (index % 2 === 0 ? "#f7f9fc" : "transparent")
                border.width: selected ? 1 : 0
                border.color: "#1e88e5"

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
                    text: msg
                    font.pixelSize: 14
                    wrapMode: Text.Wrap
                    color: selected ? "#0d47a1" : "#263238"
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
    // очистка лога — прижата к правому краю
    Button {
        id: clearButton_1
        width: senderBackground.width * 0.1
        height: senderBackground.height * 0.42
        anchors.top: senderBackground.top
        anchors.topMargin: 6
        anchors.right: senderBackground.right
        anchors.rightMargin: 6
        opacity: 0.9

        background: Rectangle {
            radius: 6
            color: clearButton_1.pressed ? "#e05555" : (clearButton_1.hovered ? "#ff8a80" : "#ff7a7a")
            border.width: 1
            border.color: "#e05555"
            Behavior on color { ColorAnimation { duration: 120 } }
        }
        contentItem: Text{
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

    // остановка выпадения лога — прижата к правому краю
    Button {
        id: stopStartButton
        width: senderBackground.width * 0.1
        height: senderBackground.height * 0.42
        anchors.top: clearButton_1.bottom
        anchors.topMargin: 4
        anchors.right: senderBackground.right
        anchors.rightMargin: 6
        opacity: 0.9

        background: Rectangle {
            radius: 6
            color: addingLog
                ? (stopStartButton.pressed ? "#e09a2f" : (stopStartButton.hovered ? "#ffca7a" : "#ffb74d"))
                : (stopStartButton.pressed ? "#43a047" : (stopStartButton.hovered ? "#81c784" : "#66bb6a"))
            border.width: 1
            border.color: addingLog ? "#e09a2f" : "#43a047"
            Behavior on color { ColorAnimation { duration: 120 } }
        }

        contentItem: Text{
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            text: addingLog ? "Стоп" : "Старт"
            font.pointSize: 6
            color: "#ffffff"
        }

        onClicked: {
            addingLog ? addingLog = false : addingLog = true
        }
    }

    SwipeView {
        id: swipeView

        anchors.top: senderBackground.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        currentIndex: pageIndicator.currentIndex

        //страница основных настроек
        Item {
            id: page_settings

            Flickable {
                id: settingsFlick
                anchors.fill: parent
                contentWidth: width
                contentHeight: settingsColumn.height + 16
                clip: true
                boundsBehavior: Flickable.StopAtBounds

                Column {
                    id: settingsColumn
                    width: parent.width * 0.92
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    spacing: 6

                    Label {
                        id: label_1
                        width: parent.width
                        wrapMode: Label.Wrap
                        horizontalAlignment: Qt.AlignHCenter
                        text: "окно основных настроек"
                        color: cText
                        font.pointSize: 12
                        font.bold: true
                    }

                    // ---- Система ----
                    Label {
                        width: parent.width
                        text: "Система"
                        color: cPrimaryD
                        font.pointSize: 10
                        font.bold: true
                    }

                    StyledButton {
                        width: parent.width
                        text: "запрос ошибок"
                        baseColor: cPrimary
                        onClicked: {
                            getcurtimer.running = false
                            tx_commands.getCheck();
                            listView1.positionViewAtEnd()
                            getcurtimer.running = true
                        }
                    }

                    StyledButton {
                        width: parent.width
                        text: "стереть ошибки"
                        baseColor: cPrimary
                        onClicked: {
                            getcurtimer.running = false
                            tx_commands.checkReset();
                            listView1.positionViewAtEnd()
                            getcurtimer.running = true
                        }
                    }

                    StyledButton {
                        width: parent.width
                        text: "перезагрузка шара"
                        baseColor: cPrimary
                        onClicked: {
                            getcurtimer.running = false
                            tx_commands.reboot();
                            listView1.positionViewAtEnd()
                            getcurtimer.running = true
                        }
                    }

                    StyledButton {
                        width: parent.width
                        text: "запрос точки восстановления"
                        baseColor: cPrimary
                        onClicked: {
                            getcurtimer.running = false
                            tx_commands.readAllParams();
                            listView1.positionViewAtEnd()
                            getcurtimer.running = true
                        }
                    }

                    StyledButton {
                        width: parent.width
                        text: "восстановить параметры"
                        baseColor: cPrimary
                        onClicked: {
                            getcurtimer.running = false
                            tx_commands.writeAllParams();
                            listView1.positionViewAtEnd()
                            getcurtimer.running = true
                        }
                    }

                    // ---- Ток ----
                    Label {
                        width: parent.width
                        text: "Ток"
                        color: cCoralD
                        font.pointSize: 10
                        font.bold: true
                    }

                    Column {
                        width: parent.width
                        spacing: 4

                        Button {
                            id: cur_calibr_auto
                            width: parent.width
                            height: 38
                            property real holdProgress: 0.0
                            property bool fired: false

                            background: Rectangle {
                                radius: 8
                                color: cur_calibr_auto.pressed ? cCoralD : cCoral
                                Behavior on color { ColorAnimation { duration: 120 } }
                            }

                            contentItem: Text {
                                text: "автокалибровка датчика тока"
                                color: "#ffffff"
                                font.pointSize: 10
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }

                            Timer {
                                id: curHoldTimer
                                interval: 50
                                repeat: true
                                running: cur_calibr_auto.pressed && !cur_calibr_auto.fired
                                onTriggered: {
                                    cur_calibr_auto.holdProgress += 50 / 3000.0
                                    if (cur_calibr_auto.holdProgress >= 1.0) {
                                        cur_calibr_auto.holdProgress = 0.0
                                        cur_calibr_auto.fired = true
                                        getcurtimer.running = false;
                                        tx_commands.curAutoCalibration();
                                        listView1.positionViewAtEnd();
                                        getcurtimer.running = true;
                                    }
                                }
                            }

                            onPressedChanged: {
                                if (pressed) {
                                    holdProgress = 0.0
                                    fired = false
                                } else {
                                    holdProgress = 0.0
                                    fired = false
                                }
                            }
                        }

                        Rectangle {
                            width: parent.width
                            height: 8
                            radius: 4
                            color: "#f0d6d6"
                            border.width: 1
                            border.color: "#d8b4b4"
                            visible: cur_calibr_auto.holdProgress > 0

                            Rectangle {
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                width: parent.width * cur_calibr_auto.holdProgress
                                radius: 4
                                color: "#b71c1c"
                                Behavior on width { NumberAnimation { duration: 60 } }
                            }
                        }
                    }

                    // ---- Сервы ----
                    Label {
                        width: parent.width
                        text: "Сервы"
                        color: cTealD
                        font.pointSize: 10
                        font.bold: true
                    }

                    Column {
                        width: parent.width
                        spacing: 4

                        Button {
                            id: servscalibr
                            width: parent.width
                            height: 38
                            property real holdProgress: 0.0
                            property bool fired: false

                            background: Rectangle {
                                radius: 8
                                color: servscalibr.pressed ? cTealD : cTeal
                                Behavior on color { ColorAnimation { duration: 120 } }
                            }

                            contentItem: Text {
                                text: "старт/стоп автокалибровки серв"
                                color: "#ffffff"
                                font.pointSize: 10
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }

                            Timer {
                                id: servHoldTimer
                                interval: 50
                                repeat: true
                                running: servscalibr.pressed && !servscalibr.fired
                                onTriggered: {
                                    servscalibr.holdProgress += 50 / 1500.0
                                    if (servscalibr.holdProgress >= 1.0) {
                                        servscalibr.holdProgress = 0.0
                                        servscalibr.fired = true
                                        getcurtimer.running = false
                                        tx_commands.servoAutoCalibration();
                                        listView1.positionViewAtEnd()
                                        getcurtimer.running = true
                                    }
                                }
                            }

                            onPressedChanged: {
                                if (pressed) {
                                    holdProgress = 0.0
                                    fired = false
                                } else {
                                    holdProgress = 0.0
                                    fired = false
                                }
                            }
                        }

                        Rectangle {
                            width: parent.width
                            height: 8
                            radius: 4
                            color: "#d6ebe8"
                            border.width: 1
                            border.color: "#b4d8d3"
                            visible: servscalibr.holdProgress > 0

                            Rectangle {
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                width: parent.width * servscalibr.holdProgress
                                radius: 4
                                color: "#004d40"
                                Behavior on width { NumberAnimation { duration: 60 } }
                            }
                        }
                    }

                    // ---- Яркость ----
                    Label {
                        width: parent.width
                        text: "Яркость OLED"
                        color: cTealD
                        font.pointSize: 10
                        font.bold: true
                    }

                    Label {
                        id: brightnessOLEDlabel
                        width: parent.width
                        wrapMode: Label.Wrap
                        horizontalAlignment: Qt.AlignHCenter
                        text: Math.round(brightnessOLED.value)
                        color: cText
                        font.pointSize: 11
                    }

                    SwipeSlider {
                        id: brightnessOLED
                        from: 0
                        to: 255
                        width: parent.width
                        Component.onCompleted: { value = 255 }
                        onPressedChanged: {
                            if(!pressed)
                            {
                                tx_commands.setBrightness(Math.round(brightnessOLED.value));
                            }
                        }
                    }
                }
            }
        }

        //страница калибровок углов конечностей
        Item {
            id: page_setting_foot

            Timer {
                id: getcurtimer
                interval: 250; running: false; repeat: true
                onTriggered: {
                    tx_commands.current_read(false);
                }
            }

            Column {
                id: footCurrentBlock
                width: parent.width * 0.94
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 8
                spacing: 4

                Label {
                    width: parent.width
                    text: "Ток потребления"
                    color: cCoralD
                    font.pointSize: 10
                    font.bold: true
                }

                Item {
                    width: parent.width
                    height: 18

                    ProgressBar {
                        id: current
                        anchors.fill: parent
                        from: 0
                        to: 5
                        background: Rectangle {
                            color: "#e0e6ed"
                            radius: 8
                        }
                        contentItem: Item {
                            Rectangle {
                                width: current.visualPosition * parent.width
                                height: parent.height
                                radius: 8
                                color: cCoral
                                Behavior on width { NumberAnimation { duration: 150 } }
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

                    Label {
                        id: labelcur
                        anchors.centerIn: parent
                        z: 2
                        text: commun_display.Cur.toFixed(2) + " А"
                        color: cText
                        font.pointSize: 10
                        font.bold: true
                    }
                }
            }

            Flickable {
                id: footFlick
                anchors.top: footCurrentBlock.bottom
                anchors.topMargin: 6
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                contentWidth: width
                contentHeight: footColumn.height + 16
                clip: true
                boundsBehavior: Flickable.StopAtBounds

                Column {
                    id: footColumn
                    width: parent.width * 0.94
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 6
                    spacing: 6

                    Label {
                        id: label_2
                        width: parent.width
                        wrapMode: Label.Wrap
                        horizontalAlignment: Qt.AlignHCenter
                        text: "окно корректировки 'домашних' углов конечностей"
                        color: cText
                        font.pointSize: 12
                        font.bold: true
                    }

                    // ---- Выбор ноги ----
                    Label {
                        width: parent.width
                        text: "Какую ногу калибруем?"
                        color: cPrimaryD
                        font.pointSize: 10
                        font.bold: true
                    }

                    Row {
                        width: parent.width
                        spacing: 4
                        Repeater {
                            model: 6
                            StyledButton {
                                width: (footColumn.width - 5 * 4) / 6
                                text: (index + 1).toString()
                                baseColor: (box1.checked && index === 0) ||
                                           (box2.checked && index === 1) ||
                                           (box3.checked && index === 2) ||
                                           (box4.checked && index === 3) ||
                                           (box5.checked && index === 4) ||
                                           (box6.checked && index === 5)
                                           ? cPrimaryD : cPrimary
                                onClicked: {
                                    getcurtimer.running = false
                                    get_foot_param.visible = true
                                    set_foot_param.visible = true
                                    butClick = index
                                    box1.checked = (index === 0)
                                    box2.checked = (index === 1)
                                    box3.checked = (index === 2)
                                    box4.checked = (index === 3)
                                    box5.checked = (index === 4)
                                    box6.checked = (index === 5)
                                    tx_commands.getParamServsFoot(butClick);
                                    getcurtimer.running = true
                                }
                            }
                        }
                    }

                    // Скрытые радиокнопки — для совместимости
                    Item {
                        width: 0; height: 0
                        RadioButton { id: box1; visible: false }
                        RadioButton { id: box2; visible: false }
                        RadioButton { id: box3; visible: false }
                        RadioButton { id: box4; visible: false }
                        RadioButton { id: box5; visible: false }
                        RadioButton { id: box6; visible: false }
                    }

                    // ---- Параметры ----
                    Row {
                        width: parent.width
                        spacing: 6
                        StyledButton {
                            id: get_foot_param
                            width: (parent.width - 6) / 2
                            text: "Прочитать"
                            baseColor: cTeal
                            visible: false
                            onClicked: {
                                getcurtimer.running = false
                                tx_commands.getParamServsFoot(butClick);
                                listView1.positionViewAtEnd()
                                getcurtimer.running = true
                            }
                        }
                        StyledButton {
                            id: set_foot_param
                            width: (parent.width - 6) / 2
                            text: "Сохранить"
                            baseColor: cAmber
                            visible: false
                            onClicked: {
                                getcurtimer.running = false
                                coxaH = settParam.coxaAngl >> 8 & 0x00ff
                                coxaL = settParam.coxaAngl & 0x00ff
                                femurH = settParam.femurAngl >> 8 & 0x00ff
                                femurL = settParam.femurAngl & 0x00ff
                                tibaH = settParam.tibaAngl >> 8 & 0x00ff
                                tibaL = settParam.tibaAngl & 0x00ff
                                tx_commands.writeHomeAngleFoot(butClick + " " + coxaL + " " + coxaH + " " + femurL + " " + femurH + " " + tibaL + " " + tibaH);
                                listView1.positionViewAtEnd()
                                getcurtimer.running = true
                            }
                        }
                    }

                    // ---- Coxa ----
                    Label {
                        width: parent.width
                        text: "Coxa — " + Math.round(settParam.coxaAngl) + "°"
                        color: cMintD
                        font.pointSize: 10
                        font.bold: true
                    }

                    SwipeSlider {
                        id: slider1
                        width: parent.width
                        from: 0
                        to: 270
                        Component.onCompleted: {
                            value = settParam.coxaAngl
                            settParam.coxaAnglChanged.connect(modelChanged)
                        }
                        onMoved: {settParam.coxaAngl = value}
                        onPressedChanged: {
                            if(!pressed)
                            {
                                getcurtimer.running = false
                                coxaH = settParam.coxaAngl >> 8 & 0x00ff
                                coxaL = settParam.coxaAngl & 0x00ff
                                tx_commands.setServoAnglee((butClick * 3 + 0) + " " + coxaL + " " + coxaH);
                                getcurtimer.running = true
                            }
                        }
                        function modelChanged(){
                            if (!pressed)
                                value = settParam.coxaAngl
                        }
                    }

                    Row {
                        width: parent.width
                        spacing: 4
                        StyledButton {
                            width: (parent.width - 3 * 4) / 4
                            text: "<<"
                            baseColor: cMint
                            onClicked: {
                                getcurtimer.running = false
                                if(settParam.coxaAngl >= 10) {
                                    settParam.coxaAngl = settParam.coxaAngl - 10
                                    coxaH = settParam.coxaAngl >> 8 & 0x00ff
                                    coxaL = settParam.coxaAngl & 0x00ff
                                    tx_commands.setServoAnglee((butClick * 3 + 0) + " " + coxaL + " " + coxaH);
                                }
                                getcurtimer.running = true
                            }
                        }
                        StyledButton {
                            width: (parent.width - 3 * 4) / 4
                            text: "<"
                            baseColor: cMint
                            onClicked: {
                                getcurtimer.running = false
                                if(settParam.coxaAngl >= 1) {
                                    settParam.coxaAngl = settParam.coxaAngl - 1
                                    coxaH = settParam.coxaAngl >> 8 & 0x00ff
                                    coxaL = settParam.coxaAngl & 0x00ff
                                    tx_commands.setServoAnglee((butClick * 3 + 0) + " " + coxaL + " " + coxaH);
                                }
                                getcurtimer.running = true
                            }
                        }
                        StyledButton {
                            width: (parent.width - 3 * 4) / 4
                            text: ">"
                            baseColor: cMint
                            onClicked: {
                                getcurtimer.running = false
                                if(settParam.coxaAngl <= 269) {
                                    settParam.coxaAngl = settParam.coxaAngl + 1
                                    coxaH = settParam.coxaAngl >> 8 & 0x00ff
                                    coxaL = settParam.coxaAngl & 0x00ff
                                    tx_commands.setServoAnglee((butClick * 3 + 0) + " " + coxaL + " " + coxaH);
                                }
                                getcurtimer.running = true
                            }
                        }
                        StyledButton {
                            width: (parent.width - 3 * 4) / 4
                            text: ">>"
                            baseColor: cMint
                            onClicked: {
                                getcurtimer.running = false
                                if(settParam.coxaAngl <= 260) {
                                    settParam.coxaAngl = settParam.coxaAngl + 10
                                    coxaH = settParam.coxaAngl >> 8 & 0x00ff
                                    coxaL = settParam.coxaAngl & 0x00ff
                                    tx_commands.setServoAnglee((butClick * 3 + 0) + " " + coxaL + " " + coxaH);
                                }
                                getcurtimer.running = true
                            }
                        }
                    }

                    // ---- Femur ----
                    Label {
                        width: parent.width
                        text: "Femur — " + Math.round(settParam.femurAngl) + "°"
                        color: cAmberD
                        font.pointSize: 10
                        font.bold: true
                    }

                    SwipeSlider {
                        id: slider2
                        width: parent.width
                        from: 0
                        to: 270
                        Component.onCompleted: {
                            value = settParam.femurAngl
                            settParam.femurAnglChanged.connect(modelChanged)
                        }
                        onMoved: {settParam.femurAngl = value}
                        onPressedChanged: {
                            if(!pressed)
                            {
                                getcurtimer.running = false
                                femurH = settParam.femurAngl >> 8 & 0x00ff
                                femurL = settParam.femurAngl & 0x00ff
                                tx_commands.setServoAnglee((butClick * 3 + 1) + " " + femurL + " " + femurH);
                                getcurtimer.running = true
                            }
                        }
                        function modelChanged(){
                            if (!pressed)
                                value = settParam.femurAngl
                        }
                    }

                    Row {
                        width: parent.width
                        spacing: 4
                        StyledButton {
                            width: (parent.width - 3 * 4) / 4
                            text: "<<"
                            baseColor: cAmber
                            onClicked: {
                                getcurtimer.running = false
                                if(settParam.femurAngl >= 10) {
                                    settParam.femurAngl = settParam.femurAngl - 10
                                    femurH = settParam.femurAngl >> 8 & 0x00ff
                                    femurL = settParam.femurAngl & 0x00ff
                                    tx_commands.setServoAnglee((butClick * 3 + 1) + " " + femurL + " " + femurH);
                                }
                                getcurtimer.running = true
                            }
                        }
                        StyledButton {
                            width: (parent.width - 3 * 4) / 4
                            text: "<"
                            baseColor: cAmber
                            onClicked: {
                                getcurtimer.running = false
                                if(settParam.femurAngl >= 1) {
                                    settParam.femurAngl = settParam.femurAngl - 1
                                    femurH = settParam.femurAngl >> 8 & 0x00ff
                                    femurL = settParam.femurAngl & 0x00ff
                                    tx_commands.setServoAnglee((butClick * 3 + 1) + " " + femurL + " " + femurH);
                                }
                                getcurtimer.running = true
                            }
                        }
                        StyledButton {
                            width: (parent.width - 3 * 4) / 4
                            text: ">"
                            baseColor: cAmber
                            onClicked: {
                                getcurtimer.running = false
                                if(settParam.femurAngl <= 269) {
                                    settParam.femurAngl = settParam.femurAngl + 1
                                    femurH = settParam.femurAngl >> 8 & 0x00ff
                                    femurL = settParam.femurAngl & 0x00ff
                                    tx_commands.setServoAnglee((butClick * 3 + 1) + " " + femurL + " " + femurH);
                                }
                                getcurtimer.running = true
                            }
                        }
                        StyledButton {
                            width: (parent.width - 3 * 4) / 4
                            text: ">>"
                            baseColor: cAmber
                            onClicked: {
                                getcurtimer.running = false
                                if(settParam.femurAngl <= 260) {
                                    settParam.femurAngl = settParam.femurAngl + 10
                                    femurH = settParam.femurAngl >> 8 & 0x00ff
                                    femurL = settParam.femurAngl & 0x00ff
                                    tx_commands.setServoAnglee((butClick * 3 + 1) + " " + femurL + " " + femurH);
                                }
                                getcurtimer.running = true
                            }
                        }
                    }

                    // ---- Tiba ----
                    Label {
                        width: parent.width
                        text: "Tiba — " + Math.round(settParam.tibaAngl) + "°"
                        color: cPrimaryD
                        font.pointSize: 10
                        font.bold: true
                    }

                    SwipeSlider {
                        id: slider3
                        width: parent.width
                        from: 0
                        to: 270
                        Component.onCompleted: {
                            value = settParam.tibaAngl
                            settParam.tibaAnglChanged.connect(modelChanged)
                        }
                        onMoved: {settParam.tibaAngl = value}
                        onPressedChanged: {
                            if(!pressed)
                            {
                                getcurtimer.running = false
                                tibaH = settParam.tibaAngl >> 8 & 0x00ff
                                tibaL = settParam.tibaAngl & 0x00ff
                                tx_commands.setServoAnglee((butClick * 3 + 2) + " " + tibaL + " " + tibaH);
                                getcurtimer.running = true
                            }
                        }
                        function modelChanged(){
                            if (!pressed)
                                value = settParam.tibaAngl
                        }
                    }

                    Row {
                        width: parent.width
                        spacing: 4
                        StyledButton {
                            width: (parent.width - 3 * 4) / 4
                            text: "<<"
                            baseColor: cPrimary
                            onClicked: {
                                getcurtimer.running = false
                                if(settParam.tibaAngl >= 10) {
                                    settParam.tibaAngl = settParam.tibaAngl - 10
                                    tibaH = settParam.tibaAngl >> 8 & 0x00ff
                                    tibaL = settParam.tibaAngl & 0x00ff
                                    tx_commands.setServoAnglee((butClick * 3 + 2) + " " + tibaL + " " + tibaH);
                                }
                                getcurtimer.running = true
                            }
                        }
                        StyledButton {
                            width: (parent.width - 3 * 4) / 4
                            text: "<"
                            baseColor: cPrimary
                            onClicked: {
                                getcurtimer.running = false
                                if(settParam.tibaAngl >= 1) {
                                    settParam.tibaAngl = settParam.tibaAngl - 1
                                    tibaH = settParam.tibaAngl >> 8 & 0x00ff
                                    tibaL = settParam.tibaAngl & 0x00ff
                                    tx_commands.setServoAnglee((butClick * 3 + 2) + " " + tibaL + " " + tibaH);
                                }
                                getcurtimer.running = true
                            }
                        }
                        StyledButton {
                            width: (parent.width - 3 * 4) / 4
                            text: ">"
                            baseColor: cPrimary
                            onClicked: {
                                getcurtimer.running = false
                                if(settParam.tibaAngl <= 269) {
                                    settParam.tibaAngl = settParam.tibaAngl + 1
                                    tibaH = settParam.tibaAngl >> 8 & 0x00ff
                                    tibaL = settParam.tibaAngl & 0x00ff
                                    tx_commands.setServoAnglee((butClick * 3 + 2) + " " + tibaL + " " + tibaH);
                                }
                                getcurtimer.running = true
                            }
                        }
                        StyledButton {
                            width: (parent.width - 3 * 4) / 4
                            text: ">>"
                            baseColor: cPrimary
                            onClicked: {
                                getcurtimer.running = false
                                if(settParam.tibaAngl <= 260) {
                                    settParam.tibaAngl = settParam.tibaAngl + 10
                                    tibaH = settParam.tibaAngl >> 8 & 0x00ff
                                    tibaL = settParam.tibaAngl & 0x00ff
                                    tx_commands.setServoAnglee((butClick * 3 + 2) + " " + tibaL + " " + tibaH);
                                }
                                getcurtimer.running = true
                            }
                        }
                    }

                    // ---- Калибровка ----
                    Label {
                        width: parent.width
                        text: "Калибровка"
                        color: cText
                        font.pointSize: 10
                        font.bold: true
                    }

                    StyledButton {
                        id: calibr_foot
                        width: parent.width
                        text: "старт/стоп калибровки ноги"
                        baseColor: cTeal
                        onClicked: {
                            tx_commands.calibrServsFoot(butClick);
                            listView1.positionViewAtEnd();
                        }
                    }

                    StyledButton {
                        id: reset_foot_param
                        width: parent.width
                        text: "Reset foot param!"
                        baseColor: cCoral
                        onClicked: {
                            getcurtimer.running = false
                            tx_commands.resetServsFoot(butClick);
                            listView1.positionViewAtEnd()
                            getcurtimer.running = true
                        }
                    }
                }
            }
        }

        // настройка PCA
        Item {
            id: pca_Speed

            Component.onCompleted: {
                commun_display.onPca1_pca2_speed_Changed.connect(settingsChanged)
            }
            function settingsChanged(freqPca1, freqPca2, speed){
                pca1 = freqPca1;
                pca2 = freqPca2;
                speedServs = speed;
            }

            Column {
                id: pcaCurrentBlock
                width: parent.width * 0.9
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 8
                spacing: 4

                Label {
                    width: parent.width
                    text: "Ток потребления"
                    color: cCoralD
                    font.pointSize: 10
                    font.bold: true
                }

                Item {
                    width: parent.width
                    height: 18

                    ProgressBar {
                        id: current1
                        anchors.fill: parent
                        from: 0
                        to: 5
                        background: Rectangle {
                            color: "#e0e6ed"
                            radius: 8
                        }
                        contentItem: Item {
                            Rectangle {
                                width: current1.visualPosition * parent.width
                                height: parent.height
                                radius: 8
                                color: cCoral
                                Behavior on width { NumberAnimation { duration: 150 } }
                            }
                        }
                        Connections {
                            target: commun_display
                            function onCurRealChanged(Cur) {
                                if(window_focus)
                                    current1.value = Cur
                            }
                        }
                    }

                    Label {
                        id: labelcur1
                        anchors.centerIn: parent
                        z: 2
                        text: commun_display.Cur.toFixed(2) + " А"
                        color: cText
                        font.pointSize: 10
                        font.bold: true
                    }
                }
            }

            Flickable {
                id: pcaFlick
                anchors.top: pcaCurrentBlock.bottom
                anchors.topMargin: 6
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                contentWidth: width
                contentHeight: pcaColumn.height + 16
                clip: true
                boundsBehavior: Flickable.StopAtBounds

                Column {
                    id: pcaColumn
                    width: parent.width * 0.9
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 6
                    spacing: 6

                    Label {
                        width: parent.width
                        wrapMode: Label.Wrap
                        horizontalAlignment: Qt.AlignHCenter
                        text: "окно настройки драйверов сервоприводов"
                        color: cText
                        font.pointSize: 12
                        font.bold: true
                    }

                    Row {
                        width: parent.width
                        spacing: 6
                        StyledButton {
                            width: (parent.width - 6) / 2
                            text: "Прочитать"
                            baseColor: cTeal
                            onClicked: {
                                tx_commands.getSettingsServs();
                                listView1.positionViewAtEnd();
                            }
                        }
                        StyledButton {
                            width: (parent.width - 6) / 2
                            text: "Сохранить"
                            baseColor: cMint
                            onClicked: {
                                tx_commands.saveFlashSettings();
                                listView1.positionViewAtEnd();
                            }
                        }
                    }

                    Label {
                        width: parent.width
                        text: "Частота PCA 1 — " + pca1
                        color: cPrimaryD
                        font.pointSize: 10
                        font.bold: true
                    }

                    Row {
                        width: parent.width
                        spacing: 6
                        StyledButton {
                            width: (parent.width - 6) / 2
                            text: "−"
                            baseColor: cPrimary
                            onClicked: {
                                if(pca1 !== 24) pca1--;
                                tx_commands.setSettingsServo(pca1, pca2, speedServs);
                                listView1.positionViewAtEnd();
                            }
                        }
                        StyledButton {
                            width: (parent.width - 6) / 2
                            text: "+"
                            baseColor: cPrimary
                            onClicked: {
                                if(pca1 < 150) pca1++;
                                tx_commands.setSettingsServo(pca1, pca2, speedServs);
                                listView1.positionViewAtEnd();
                            }
                        }
                    }

                    Label {
                        width: parent.width
                        text: "Частота PCA 2 — " + pca2
                        color: cPrimaryD
                        font.pointSize: 10
                        font.bold: true
                    }

                    Row {
                        width: parent.width
                        spacing: 6
                        StyledButton {
                            width: (parent.width - 6) / 2
                            text: "−"
                            baseColor: cPrimary
                            onClicked: {
                                if(pca2 !== 24) pca2--;
                                tx_commands.setSettingsServo(pca1, pca2, speedServs);
                                listView1.positionViewAtEnd();
                            }
                        }
                        StyledButton {
                            width: (parent.width - 6) / 2
                            text: "+"
                            baseColor: cPrimary
                            onClicked: {
                                if(pca2 < 150) pca2++;
                                tx_commands.setSettingsServo(pca1, pca2, speedServs);
                                listView1.positionViewAtEnd();
                            }
                        }
                    }

                    Label {
                        width: parent.width
                        text: "Скорость серв — " + speedServs
                        color: cAmberD
                        font.pointSize: 10
                        font.bold: true
                    }

                    Row {
                        width: parent.width
                        spacing: 6
                        StyledButton {
                            width: (parent.width - 6) / 2
                            text: "−"
                            baseColor: cAmber
                            onClicked: {
                                if(speedServs !== 0) speedServs--;
                                tx_commands.setSettingsServo(pca1, pca2, speedServs);
                                listView1.positionViewAtEnd();
                            }
                        }
                        StyledButton {
                            width: (parent.width - 6) / 2
                            text: "+"
                            baseColor: cAmber
                            onClicked: {
                                if(speedServs < 70) speedServs++;
                                tx_commands.setSettingsServo(pca1, pca2, speedServs);
                                listView1.positionViewAtEnd();
                            }
                        }
                    }

                    Label {
                        width: parent.width
                        text: "Прочее"
                        color: cText
                        font.pointSize: 10
                        font.bold: true
                    }

                    StyledButton {
                        width: parent.width
                        text: "Центр. полож. серв"
                        baseColor: cTeal
                        onClicked: {
                            tx_commands.setMidPwmServs();
                            listView1.positionViewAtEnd();
                        }
                    }

                    StyledButton {
                        width: parent.width
                        text: "Проверка скорости"
                        baseColor: cMint
                        onClicked: {
                            tx_commands.checkSpeedServs();
                            listView1.positionViewAtEnd();
                        }
                    }
                }
            }
        }
    }

    PageIndicator {
        id: pageIndicator
        interactive: true
        count: swipeView.count
        currentIndex: swipeView.currentIndex
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: parent.height * 0.05
    }

    // =====================================================================
    // Стилизованная кнопка — компактная
    // =====================================================================
    component StyledButton: Button {
        id: sb
        property color baseColor: "#5b7cfa"

        implicitHeight: 38
        implicitWidth: 200
        leftPadding: 10
        rightPadding: 10
        topPadding: 6
        bottomPadding: 6

        contentItem: Text {
            text: sb.text
            color: "#ffffff"
            font.pixelSize: 11
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        background: Rectangle {
            radius: 8
            color: {
                if (sb.pressed) return Qt.darker(sb.baseColor, 1.18)
                if (sb.hovered) return Qt.lighter(sb.baseColor, 1.08)
                return sb.baseColor
            }
            border.color: Qt.darker(sb.baseColor, 1.25)
            border.width: 1

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height: 3
                radius: parent.radius
                color: Qt.darker(sb.baseColor, 1.30)
                opacity: sb.pressed ? 0.0 : 0.55
            }

            Behavior on color { ColorAnimation { duration: 120 } }
        }
    }

    // =====================================================================
    // SwipeSlider — тонкий кастомный слайдер
    // =====================================================================
    component SwipeSlider: Item {
        id: ssRoot

        property real value: 0
        property real from: 0
        property real to: 100
        property real stepSize: 0
        property bool pressed: false

        signal moved()

        implicitHeight: 30
        implicitWidth: 200

        readonly property color trackColor:  "#cfd8dc"
        readonly property color fillColor:   "#4dd0e1"
        readonly property color handleColor: "#26a69a"
        readonly property color handleBorder:"#00796b"

        Rectangle {
            id: ssTrack
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right
            height: 4
            radius: 2
            color: ssRoot.trackColor

            Rectangle {
                width: Math.max(0, Math.min(parent.width,
                    (ssRoot.value - ssRoot.from) / (ssRoot.to - ssRoot.from) * parent.width))
                height: parent.height
                radius: 2
                color: ssRoot.fillColor
            }
        }

        Rectangle {
            id: ssHandle
            width: 16
            height: 16
            radius: 8
            color: ssRoot.handleColor
            border.color: ssRoot.handleBorder
            border.width: 2
            anchors.verticalCenter: parent.verticalCenter
            x: Math.max(0, Math.min(parent.width - width,
                (ssRoot.value - ssRoot.from) / (ssRoot.to - ssRoot.from) * (parent.width - width)))
        }

        MouseArea {
            id: ssMouse
            anchors.fill: parent
            preventStealing: true
            acceptedButtons: Qt.LeftButton

            property real startX: 0
            property real startY: 0
            property bool dragging: false
            property bool horizontal: false

            onPressed: function(mouse) {
                startX = mouse.x
                startY = mouse.y
                dragging = false
                horizontal = false
                ssRoot.pressed = true
                swipeView.interactive = false
            }

            onPositionChanged: function(mouse) {
                if (!pressed) return
                var dx = mouse.x - startX
                var dy = mouse.y - startY

                if (!dragging && !horizontal) {
                    if (Math.abs(dx) > 8 && Math.abs(dx) > Math.abs(dy)) {
                        horizontal = true
                        ssRoot.pressed = false
                        swipeView.interactive = true
                        preventStealing = false
                        mouse.accepted = false
                        return
                    } else if (Math.abs(dy) > 4 || Math.abs(dx) > 4) {
                        dragging = true
                    }
                }

                if (dragging) {
                    var rel = Math.max(0, Math.min(1,
                        (mouse.x - ssHandle.width / 2) / (width - ssHandle.width)))
                    var newVal = ssRoot.from + rel * (ssRoot.to - ssRoot.from)
                    if (ssRoot.stepSize > 0)
                        newVal = Math.round(newVal / ssRoot.stepSize) * ssRoot.stepSize
                    ssRoot.value = newVal
                    mouse.accepted = true
                }
            }

            onReleased: function(mouse) {
                if (dragging) {
                    ssRoot.moved()
                }
                dragging = false
                horizontal = false
                ssRoot.pressed = false
                preventStealing = true
                swipeView.interactive = true
                mouse.accepted = false
            }

            onCanceled: {
                dragging = false
                horizontal = false
                ssRoot.pressed = false
                preventStealing = true
                swipeView.interactive = true
            }
        }
    }
}
