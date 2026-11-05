import QtQuick 2.15
import QtQuick.Controls

Item {
    id: room

    // ─── Активность страницы ─────────────────────────────────────
    property bool active: true

    onActiveChanged: {
        if (!active) {
            robot.stopAll()
        }
    }

    Component.onCompleted: {
        robot.y = room.floorY - robot.height
    }
    onHeightChanged: {
        if (robot.state !== "jump")
            robot.y = room.floorY - robot.height
    }
    onFloorYChanged: {
        if (robot.state !== "jump")
            robot.y = room.floorY - robot.height
    }

    // ─── Палитра комнаты ──────────────────────────────────────────
    Rectangle {
        anchors.fill: parent
        color: "#1e1e2a"
    }
    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: parent.height * 0.72
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#2b2b3d" }
            GradientStop { position: 1.0; color: "#1a1a28" }
        }
    }
    Rectangle {
        id: floor
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: parent.height * 0.28
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#3a2f2a" }
            GradientStop { position: 1.0; color: "#1f1916" }
        }
    }
    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: floor.top
        height: 3
        color: "#5a4a40"
    }

    // ─── Границы «прогулки» ───────────────────────────────────────
    readonly property real walkMinX: 60
    readonly property real walkMaxX: width - 60
    readonly property real floorY: floor.y > 0 ? floor.y : height * 0.72

    // ─── Клик по сцене → робот меняет действие ───────────────────
    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (!room.active) return;
            robot.pickAction();
        }
    }

    // ─── Мяч ──────────────────────────────────────────────────────
    Item {
        id: ball
        z: 5
        width: 26; height: 26
        visible: true
        property real vx: 0
        property real vy: 0
        property real groundY: room.floorY - height

        x: room.width * 0.7
        y: groundY

        Rectangle {
            anchors.fill: parent
            radius: width / 2
            color: "#e74c3c"
            border.color: "#a93226"
            border.width: 2
            Rectangle {
                width: parent.width * 0.35
                height: width
                radius: width / 2
                color: "#ff9f9f"
                opacity: 0.6
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.margins: 4
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (!room.active) return;
                console.log("ball clicked")
                ball.vx = (Math.random() > 0.5 ? 1 : -1) * (4 + Math.random() * 3);
                ball.vy = -7 - Math.random() * 3;
                ball.visible = true;
                robot.userBallMode = true;
                robot.robotKicks = 0;
                ballCooldown.restart();
                if (!robot.chasing) {
                    robot.chasing = true;
                }
            }
        }

        Timer {
            interval: 16
            repeat: true
            running: room.active && ball.visible
            onTriggered: {
                if (!room.active) return;
                ball.vy += 0.6
                ball.x += ball.vx
                ball.y += ball.vy

                if (ball.y >= ball.groundY) {
                    ball.y = ball.groundY
                    ball.vy = -ball.vy * 0.55
                    ball.vx *= 0.9
                    if (Math.abs(ball.vy) < 0.8) {
                        ball.vy = 0
                        ball.vx = 0
                    }
                }
                if (ball.y <= 0) {
                    ball.y = 0
                    ball.vy = Math.abs(ball.vy) * 0.55
                }
                if (ball.x <= room.walkMinX * 0.5) {
                    ball.x = room.walkMinX * 0.5
                    ball.vx = -ball.vx * 0.7
                }
                if (ball.x + ball.width >= room.width - room.walkMinX * 0.5) {
                    ball.x = room.width - room.walkMinX * 0.5 - ball.width
                    ball.vx = -ball.vx * 0.7
                }
            }
        }
    }

    // ─── Робот ────────────────────────────────────────────────────
    Item {
        id: robot
        z: 10
        width: 70
        height: 110
        x: (room.walkMinX + room.walkMaxX) / 2 - width / 2
        y: room.floorY - height

        // ─── Визуал робота ────────────────────────────────────────
        Rectangle {
            id: head
            width: 34; height: 30
            radius: 8
            color: "#8ecae6"
            border.color: "#219ebc"
            border.width: 2
            anchors.horizontalCenter: parent.horizontalCenter
            y: 0
            transformOrigin: Item.Bottom
            Row {
                anchors.centerIn: parent
                spacing: 8
                Rectangle { width: 6; height: 6; radius: 3; color: "#023047" }
                Rectangle { width: 6; height: 6; radius: 3; color: "#023047" }
            }
            Rectangle {
                width: 3; height: 10
                color: "#fb8500"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.top
                Rectangle {
                    width: 8; height: 8; radius: 4
                    color: "#ffb703"
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.top
                    anchors.bottomMargin: -2
                }
            }
        }
        Rectangle {
            id: body
            width: 44; height: 50
            radius: 10
            color: "#adb5bd"
            border.color: "#6c757d"
            border.width: 2
            anchors.horizontalCenter: parent.horizontalCenter
            y: head.y + head.height + 4
            transformOrigin: Item.Bottom
        }
        Rectangle {
            id: leftArm
            width: 10; height: 34; radius: 5
            color: "#adb5bd"
            border.color: "#6c757d"
            border.width: 1
            x: body.x - 12
            y: body.y + 6
            transformOrigin: Item.Top
        }
        Rectangle {
            id: rightArm
            width: 10; height: 34; radius: 5
            color: "#adb5bd"
            border.color: "#6c757d"
            border.width: 1
            x: body.x + body.width + 2
            y: body.y + 6
            transformOrigin: Item.Top
        }
        Rectangle {
            id: leftLeg
            width: 12; height: 26; radius: 4
            color: "#6c757d"
            x: body.x + 4
            y: body.y + body.height - 2
            transformOrigin: Item.Top
        }
        Rectangle {
            id: rightLeg
            width: 12; height: 26; radius: 4
            color: "#6c757d"
            x: body.x + body.width - 16
            y: body.y + body.height - 2
            transformOrigin: Item.Top
        }

        // ─── Состояние ────────────────────────────────────────────
        property string state: "idle"
        property real targetX: x
        property real vx: 0
        property real jumpStartY: 0
        property int kickCount: 0
        property bool chasing: false
        property bool userBallMode: false
        property int robotKicks: 0

        // Флаг: робот реально перемещается по X в режиме "ball"
        property bool ballWalking: false

        readonly property bool moving: room.active && (
            state === "walk" || state === "run" || chasing || ballWalking
        )

        property int strideMs: 340
        readonly property real legAmp: (state === "run" || chasing) ? 32 : 20
        readonly property real armAmp: (state === "run" || chasing) ? 26 : 16
        readonly property real lean: (state === "run" || chasing) ? 8 : 3
        readonly property real bobAmp: (state === "run" || chasing) ? 3 : 2

        // ─── Сброс конечностей в ноль (плавно) ────────────────────
        SequentialAnimation {
            id: resetLimbsAnim
            running: false
            ParallelAnimation {
                NumberAnimation { target: leftArm;  property: "rotation"; to: 0; duration: 150; easing.type: Easing.OutQuad }
                NumberAnimation { target: rightArm; property: "rotation"; to: 0; duration: 150; easing.type: Easing.OutQuad }
                NumberAnimation { target: leftLeg;  property: "rotation"; to: 0; duration: 150; easing.type: Easing.OutQuad }
                NumberAnimation { target: rightLeg; property: "rotation"; to: 0; duration: 150; easing.type: Easing.OutQuad }
                NumberAnimation { target: body;     property: "y";        to: head.y + head.height + 4; duration: 150; easing.type: Easing.OutQuad }
                NumberAnimation { target: body;     property: "rotation"; to: 0; duration: 150; easing.type: Easing.OutQuad }
                NumberAnimation { target: head;     property: "rotation"; to: 0; duration: 150; easing.type: Easing.OutQuad }
                NumberAnimation { target: leftLeg;  property: "height";   to: 26; duration: 150; easing.type: Easing.OutQuad }
                NumberAnimation { target: rightLeg; property: "height";   to: 26; duration: 150; easing.type: Easing.OutQuad }
            }
        }

        function startMoveAnim() {
            resetLimbsAnim.stop();
            leftArm.rotation = 0;
            rightArm.rotation = 0;
            leftLeg.rotation = 0;
            rightLeg.rotation = 0;
            body.rotation = 0;
            head.rotation = 0;
            body.y = head.y + head.height + 4;
        }
        function stopMoveAnim() {
            resetLimbsAnim.stop();
            resetLimbsAnim.start();
        }

        onChasingChanged: {
            if (chasing && room.active) {
                startMoveAnim();
            } else if (!chasing && state !== "walk" && state !== "run" && !ballWalking) {
                stopMoveAnim();
            }
        }

        function stopAll() {
            stopMoveAnim();
            idleBob.stop();
            jumpAnim.stop();
            exerciseAnim.stop();
            kickLunge.stop();
            kickTimer.stop();
            kickDelay.stop();
            fleeTimer.stop();
            chaseTimer.stop();
            ballCooldown.stop();
            chasing = false;
            userBallMode = false;
            robotKicks = 0;
            ballWalking = false;
            state = "idle";
            y = room.floorY - height;
            scale = 1.0;
            body.y = head.y + head.height + 4;
            body.rotation = 0;
            head.rotation = 0;
            leftLeg.height = 26;
            rightLeg.height = 26;
            strideMs = 340;
            ball.visible = true;
        }

        Behavior on x { NumberAnimation { duration: 60; easing.type: Easing.Linear } }

        // ─── Таймер «пользователь перестал играть» ───────────────
        Timer {
            id: ballCooldown
            interval: 5000
            repeat: false
            onTriggered: {
                robot.userBallMode = false;
                if (robot.robotKicks >= 2 && robot.chasing) {
                    robot.robotKicks = 0;
                    robot.enterState("idle");
                }
            }
        }

        onStateChanged: {
            if (state === "run")      strideMs = 240;
            else if (state === "walk") strideMs = 340;
            else                       strideMs = 340;
        }

        // ─── Единая анимация ходьбы/бега ─────────────────────────
        SequentialAnimation {
            id: moveAnim
            loops: Animation.Infinite
            running: robot.moving

            // Шаг A
            ParallelAnimation {
                NumberAnimation { target: leftLeg;  property: "rotation"; from:  robot.legAmp; to: -robot.legAmp; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: rightLeg; property: "rotation"; from: -robot.legAmp; to:  robot.legAmp; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: leftArm;  property: "rotation"; from: -robot.armAmp; to:  robot.armAmp; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: rightArm; property: "rotation"; from:  robot.armAmp; to: -robot.armAmp; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: body; property: "y"; from: head.y + head.height + 4; to: head.y + head.height + 4 + robot.bobAmp; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: body; property: "rotation"; from: 0; to: robot.lean; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: head; property: "rotation"; from: 0; to: -robot.lean * 0.6; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: leftLeg;  property: "height"; from: 26; to: 22; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: rightLeg; property: "height"; from: 26; to: 30; duration: robot.strideMs; easing.type: Easing.InOutSine }
            }
            // Шаг B
            ParallelAnimation {
                NumberAnimation { target: leftLeg;  property: "rotation"; from: -robot.legAmp; to:  robot.legAmp; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: rightLeg; property: "rotation"; from:  robot.legAmp; to: -robot.legAmp; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: leftArm;  property: "rotation"; from:  robot.armAmp; to: -robot.armAmp; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: rightArm; property: "rotation"; from: -robot.armAmp; to:  robot.armAmp; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: body; property: "y"; from: head.y + head.height + 4 + robot.bobAmp; to: head.y + head.height + 4; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: body; property: "rotation"; from: robot.lean; to: 0; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: head; property: "rotation"; from: -robot.lean * 0.6; to: 0; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: leftLeg;  property: "height"; from: 22; to: 26; duration: robot.strideMs; easing.type: Easing.InOutSine }
                NumberAnimation { target: rightLeg; property: "height"; from: 30; to: 26; duration: robot.strideMs; easing.type: Easing.InOutSine }
            }
        }

        // ─── Idle ────────────────────────────────────────────────
        SequentialAnimation {
            id: idleBob
            loops: Animation.Infinite
            running: false
            ParallelAnimation {
                NumberAnimation { target: body; property: "y"; from: head.y + head.height + 4; to: head.y + head.height + 6; duration: 1100; easing.type: Easing.InOutSine }
                NumberAnimation { target: head; property: "rotation"; from: -1.5; to: 1.5; duration: 1400; easing.type: Easing.InOutSine }
            }
            ParallelAnimation {
                NumberAnimation { target: body; property: "y"; from: head.y + head.height + 6; to: head.y + head.height + 4; duration: 1100; easing.type: Easing.InOutSine }
                NumberAnimation { target: head; property: "rotation"; from: 1.5; to: -1.5; duration: 1400; easing.type: Easing.InOutSine }
            }
        }

        // ─── Прыжок ──────────────────────────────────────────────
        SequentialAnimation {
            id: jumpAnim
            loops: 1
            running: false

            // Прыжок 1 — руки вправо
            ParallelAnimation {
                NumberAnimation { target: body; property: "y"; to: head.y + head.height + 10; duration: 100; easing.type: Easing.OutQuad }
                NumberAnimation { target: leftArm;  property: "rotation"; to: 35; duration: 100 }
                NumberAnimation { target: rightArm; property: "rotation"; to: 35; duration: 100 }
            }
            ParallelAnimation {
                NumberAnimation { target: robot; property: "y"; from: robot.jumpStartY; to: robot.jumpStartY - 80; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: leftArm;  property: "rotation"; to: 130; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: rightArm; property: "rotation"; to: 130; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: leftLeg;  property: "rotation"; to: 25; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: rightLeg; property: "rotation"; to: -25; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: leftLeg;  property: "height"; to: 18; duration: 300 }
                NumberAnimation { target: rightLeg; property: "height"; to: 18; duration: 300 }
                NumberAnimation { target: body; property: "y"; to: head.y + head.height + 4; duration: 300 }
            }
            ParallelAnimation {
                NumberAnimation { target: robot; property: "y"; from: robot.jumpStartY - 80; to: robot.jumpStartY; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: leftArm;  property: "rotation"; to: 0; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: rightArm; property: "rotation"; to: 0; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: leftLeg;  property: "rotation"; to: 0; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: rightLeg; property: "rotation"; to: 0; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: leftLeg;  property: "height"; to: 26; duration: 300 }
                NumberAnimation { target: rightLeg; property: "height"; to: 26; duration: 300 }
            }

            // Прыжок 2 — руки влево
            ParallelAnimation {
                NumberAnimation { target: body; property: "y"; to: head.y + head.height + 10; duration: 100; easing.type: Easing.OutQuad }
                NumberAnimation { target: leftArm;  property: "rotation"; to: -35; duration: 100 }
                NumberAnimation { target: rightArm; property: "rotation"; to: -35; duration: 100 }
            }
            ParallelAnimation {
                NumberAnimation { target: robot; property: "y"; from: robot.jumpStartY; to: robot.jumpStartY - 80; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: leftArm;  property: "rotation"; to: -130; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: rightArm; property: "rotation"; to: -130; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: leftLeg;  property: "rotation"; to: 25; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: rightLeg; property: "rotation"; to: -25; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: leftLeg;  property: "height"; to: 18; duration: 300 }
                NumberAnimation { target: rightLeg; property: "height"; to: 18; duration: 300 }
                NumberAnimation { target: body; property: "y"; to: head.y + head.height + 4; duration: 300 }
            }
            ParallelAnimation {
                NumberAnimation { target: robot; property: "y"; from: robot.jumpStartY - 80; to: robot.jumpStartY; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: leftArm;  property: "rotation"; to: 0; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: rightArm; property: "rotation"; to: 0; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: leftLeg;  property: "rotation"; to: 0; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: rightLeg; property: "rotation"; to: 0; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: leftLeg;  property: "height"; to: 26; duration: 300 }
                NumberAnimation { target: rightLeg; property: "height"; to: 26; duration: 300 }
            }

            // Прыжок 3 — руки снова вправо
            ParallelAnimation {
                NumberAnimation { target: body; property: "y"; to: head.y + head.height + 10; duration: 100; easing.type: Easing.OutQuad }
                NumberAnimation { target: leftArm;  property: "rotation"; to: 35; duration: 100 }
                NumberAnimation { target: rightArm; property: "rotation"; to: 35; duration: 100 }
            }
            ParallelAnimation {
                NumberAnimation { target: robot; property: "y"; from: robot.jumpStartY; to: robot.jumpStartY - 80; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: leftArm;  property: "rotation"; to: 130; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: rightArm; property: "rotation"; to: 130; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: leftLeg;  property: "rotation"; to: 25; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: rightLeg; property: "rotation"; to: -25; duration: 300; easing.type: Easing.OutQuad }
                NumberAnimation { target: leftLeg;  property: "height"; to: 18; duration: 300 }
                NumberAnimation { target: rightLeg; property: "height"; to: 18; duration: 300 }
                NumberAnimation { target: body; property: "y"; to: head.y + head.height + 4; duration: 300 }
            }
            ParallelAnimation {
                NumberAnimation { target: robot; property: "y"; from: robot.jumpStartY - 80; to: robot.jumpStartY; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: leftArm;  property: "rotation"; to: 0; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: rightArm; property: "rotation"; to: 0; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: leftLeg;  property: "rotation"; to: 0; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: rightLeg; property: "rotation"; to: 0; duration: 300; easing.type: Easing.InQuad }
                NumberAnimation { target: leftLeg;  property: "height"; to: 26; duration: 300 }
                NumberAnimation { target: rightLeg; property: "height"; to: 26; duration: 300 }
            }

            onFinished: {
                robot.y = room.floorY - robot.height
                robot.state = "idle"
                idleBob.start()
            }
        }

        // ─── Зарядка ─────────────────────────────────────────────
        SequentialAnimation {
            id: exerciseAnim
            loops: 8
            running: false
            NumberAnimation {
                target: body
                property: "y"
                to: head.y + head.height + 14
                duration: 300
                easing.type: Easing.InOutQuad
            }
            NumberAnimation {
                target: body
                property: "y"
                to: head.y + head.height + 4
                duration: 300
                easing.type: Easing.InOutQuad
            }
            onFinished: {
                body.y = head.y + head.height + 4
                robot.state = "idle"
                idleBob.start()
            }
        }

        // ─── Выпад при ударе ─────────────────────────────────────
        SequentialAnimation {
            id: kickLunge
            running: false
            NumberAnimation {
                target: robot
                property: "scale"
                to: 1.15
                duration: 100
                easing.type: Easing.OutQuad
            }
            NumberAnimation {
                target: robot
                property: "scale"
                to: 1.0
                duration: 150
                easing.type: Easing.InQuad
            }
        }

        // ─── Смена действий (brain) ───────────────────────────────
        Timer {
            id: brain
            interval: 2500 + Math.random() * 2500
            repeat: true
            running: room.active
            onTriggered: {
                if (!room.active) return;
                if (robot.userBallMode || robot.chasing) return;
                if (robot.state === "jump" || robot.state === "ball" ||
                    robot.state === "exercise" || robot.state === "run")
                    return;
                robot.pickAction();
            }
        }

        function pickAction() {
            var actions = ["idle", "walk", "run", "jump", "ball", "exercise", "walk", "idle"];
            var a = actions[Math.floor(Math.random() * actions.length)];
            enterState(a);
        }

        function enterState(s) {
            console.log("enterState:", s)
            state = s;
            chasing = false;
            userBallMode = false;
            robotKicks = 0;
            ballWalking = false;
            ballCooldown.stop();
            stopMoveAnim();
            idleBob.stop();
            jumpAnim.stop(); exerciseAnim.stop();
            kickTimer.stop(); kickDelay.stop();
            fleeTimer.stop(); chaseTimer.stop();
            ball.visible = true;

            y = room.floorY - height;
            scale = 1.0;
            body.y = head.y + head.height + 4;
            body.rotation = 0;
            head.rotation = 0;
            leftLeg.height = 26;
            rightLeg.height = 26;

            switch (s) {
            case "idle":
                targetX = x;
                idleBob.start();
                break;
            case "walk":
                strideMs = 340;
                targetX = room.walkMinX + Math.random() * (room.walkMaxX - room.walkMinX);
                vx = 1.2;
                startMoveAnim();
                break;
            case "run":
                strideMs = 240;
                targetX = (x < room.width / 2) ? room.walkMaxX : room.walkMinX;
                vx = 3.0;
                startMoveAnim();
                fleeTimer.start();
                break;
            case "jump":
                doJump();
                break;
            case "ball":
                playBall();
                break;
            case "exercise":
                doExercise();
                break;
            }
        }

        function doJump() {
            jumpStartY = room.floorY - height;
            y = jumpStartY;
            jumpAnim.start();
        }

        function playBall() {
            ball.visible = true;
            ball.x = robot.x + robot.width / 2;
            ball.y = room.floorY - ball.height;
            ball.vx = 0; ball.vy = 0;
            kickCount = 0;
            kickTimer.start();
        }

        function doExercise() {
            body.y = head.y + head.height + 4;
            exerciseAnim.start();
        }

        Timer {
            id: fleeTimer
            interval: 4000
            onTriggered: {
                if (robot.state === "run") {
                    robot.state = "idle";
                    robot.stopMoveAnim();
                    idleBob.start();
                }
            }
        }

        // ─── Погоня за мячом ──────────────────────────────────────
        Timer {
            id: chaseTimer
            interval: 16
            repeat: true
            running: room.active && robot.chasing
            onTriggered: {
                if (!room.active) return;
                if (!ball.visible) {
                    robot.chasing = false;
                    return;
                }
                var dx = (ball.x + ball.width / 2) - (robot.x + robot.width / 2);
                var ballReachable = ball.y + ball.height >= robot.y;
                if (Math.abs(dx) < 30 && ballReachable) {
                    ball.vx = (Math.random() > 0.5 ? 1 : -1) * (4 + Math.random() * 3);
                    ball.vy = -7 - Math.random() * 3;
                    kickLunge.start();

                    robot.robotKicks++;

                    if (!robot.userBallMode && robot.robotKicks >= 2) {
                        robot.robotKicks = 0;
                        robot.enterState("idle");
                        return;
                    }
                } else {
                    if (Math.abs(dx) > 2) {
                        var step = Math.min(Math.abs(dx), 4.0);
                        robot.x += Math.sign(dx) * step;
                    }
                }
            }
        }

        // ─── Пинки мяча (автоматический режим «ball») ─────────────
        // Играем чуть дольше: 6 пинков с интервалом 1500 мс (~9 секунд).
        Timer {
            id: kickTimer
            interval: 1500
            repeat: true
            running: room.active
            onTriggered: {
                if (!room.active) return;
                if (robot.kickCount >= 6) {
                    kickTimer.stop();
                    robot.state = "idle";
                    robot.ballWalking = false;
                    robot.stopMoveAnim();
                    idleBob.start();
                    return;
                }
                if (ball.y + ball.height < robot.y) {
                    return;
                }
                robot.kickCount++;
                robot.targetX = ball.x - robot.width / 2;
                robot.vx = 4.0;
                kickDelay.start();
            }
        }

        Timer {
            id: kickDelay
            interval: 350
            onTriggered: {
                if (!room.active) return;

                var dx = (ball.x + ball.width / 2) - (robot.x + robot.width / 2);
                var ballReachable = ball.y + ball.height >= robot.y;
                var closeEnough = Math.abs(dx) < robot.width / 2 + ball.width / 2 + 15;

                if (!ballReachable || !closeEnough) {
                    robot.state = "idle";
                    robot.ballWalking = false;
                    robot.stopMoveAnim();
                    idleBob.start();
                    return;
                }

                ball.vx = (Math.random() > 0.5 ? 1 : -1) * (3 + Math.random() * 3);
                ball.vy = -6 - Math.random() * 3;
                kickLunge.start();
            }
        }

        // ─── Движение к цели ──────────────────────────────────────
        Timer {
            id: locomotionTimer
            interval: 16
            repeat: true
            running: room.active && (robot.state === "walk" || robot.state === "run" || robot.state === "ball")

            onTriggered: {
                if (!room.active) return;

                var dx = robot.targetX - robot.x;
                var atTarget = Math.abs(dx) < 2;

                if (atTarget) {
                    robot.vx = 0;
                    if (robot.state === "ball" && robot.ballWalking) {
                        robot.ballWalking = false;
                    }
                    return;
                }

                robot.x += Math.sign(dx) * Math.min(Math.abs(dx), robot.vx);

                if (robot.state === "ball" && !robot.ballWalking) {
                    robot.ballWalking = true;
                    robot.strideMs = 300;
                }
            }

            onRunningChanged: {
                if (!running) {
                    if (robot.ballWalking) robot.ballWalking = false;
                }
            }
        }
    }

    // ─── Подпись ─────────────────────────────────────────────────
    Rectangle {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 30
        width: statusRow.implicitWidth + 24
        height: statusRow.implicitHeight + 12
        radius: 10
        color: "#00000055"
        visible: room.active

        Row {
            id: statusRow
            anchors.centerIn: parent
            spacing: 8

            Rectangle {
                width: 10; height: 10; radius: 5
                anchors.verticalCenter: parent.verticalCenter
                color: robot.chasing ? "#4cc9f0"
                     : robot.state === "idle" ? "#adb5bd"
                     : robot.state === "walk" ? "#8ecae6"
                     : robot.state === "run" ? "#4cc9f0"
                     : robot.state === "jump" ? "#ffb703"
                     : robot.state === "ball" ? "#e74c3c"
                     : robot.state === "exercise" ? "#90be6d"
                     : "#adb5bd"
            }

            Text {
                id: statusText
                anchors.verticalCenter: parent.verticalCenter
                color: "#e0e0e0"
                font.pixelSize: 13
                text: {
                    if (robot.chasing) return "бежит за мячом";
                    switch (robot.state) {
                    case "idle":     return "отдыхает";
                    case "walk":     return "идёт";
                    case "run":      return "бежит";
                    case "jump":     return "прыгает";
                    case "ball":     return "играет в мяч";
                    case "exercise": return "делает зарядку";
                    default:         return "";
                    }
                }
            }
        }
    }
}
