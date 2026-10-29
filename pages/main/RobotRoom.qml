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
            if (robot.state === "jump" || robot.state === "ball" ||
                robot.state === "exercise" || robot.state === "run")
                return;
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

        // ─── Клик по мячу → отскок + погоня (если ещё не бежит) ───
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (!room.active) return;
                console.log("ball clicked")
                ball.vx = (Math.random() > 0.5 ? 1 : -1) * (4 + Math.random() * 3);
                ball.vy = -7 - Math.random() * 3;
                ball.visible = true;
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

                // ─── Отскок от пола ──────────────────────────────
                if (ball.y >= ball.groundY) {
                    ball.y = ball.groundY
                    ball.vy = -ball.vy * 0.55
                    ball.vx *= 0.9
                    if (Math.abs(ball.vy) < 0.8) {
                        ball.vy = 0
                        ball.vx = 0
                    }
                }

                // ─── Отскок от потолка (верх окна) ───────────────
                if (ball.y <= 0) {
                    ball.y = 0
                    ball.vy = Math.abs(ball.vy) * 0.55
                }

                // ─── Стены по X ──────────────────────────────────
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

        function stopAll() {
            armsAnim.stop();
            legsAnim.stop();
            idleBob.stop();
            jumpAnim.stop();
            exerciseAnim.stop();
            kickLunge.stop();
            kickTimer.stop();
            kickDelay.stop();
            fleeTimer.stop();
            chaseTimer.stop();
            chasing = false;
            state = "idle";
            y = room.floorY - height;
            scale = 1.0;
            leftArm.rotation = 0;
            rightArm.rotation = 0;
            leftLeg.rotation = 0;
            rightLeg.rotation = 0;
            body.y = head.y + head.height + 4;
            ball.visible = true;
        }

        Behavior on x { NumberAnimation { duration: 60; easing.type: Easing.Linear } }

        // ─── Анимации конечностей ─────────────────────────────────
        SequentialAnimation {
            id: armsAnim
            loops: Animation.Infinite
            running: room.active && (robot.state === "walk" || robot.state === "run" || robot.chasing)
            NumberAnimation { target: leftArm; property: "rotation"; from: -25; to: 25; duration: 300; easing.type: Easing.InOutSine }
            NumberAnimation { target: leftArm; property: "rotation"; from: 25; to: -25; duration: 300; easing.type: Easing.InOutSine }
        }
        SequentialAnimation {
            id: legsAnim
            loops: Animation.Infinite
            running: room.active && (robot.state === "walk" || robot.state === "run" || robot.chasing)
            NumberAnimation { target: leftLeg; property: "rotation"; from: -20; to: 20; duration: 300; easing.type: Easing.InOutSine }
            NumberAnimation { target: rightLeg; property: "rotation"; from: 20; to: -20; duration: 300; easing.type: Easing.InOutSine }
        }
        SequentialAnimation {
            id: idleBob
            loops: Animation.Infinite
            running: room.active && robot.state === "idle" && !robot.chasing
            NumberAnimation { target: body; property: "y"; from: head.y + head.height + 4; to: head.y + head.height + 6; duration: 900; easing.type: Easing.InOutSine }
            NumberAnimation { target: body; property: "y"; from: head.y + head.height + 6; to: head.y + head.height + 4; duration: 900; easing.type: Easing.InOutSine }
        }

        // ─── Прыжок (3 подряд) ───────────────────────────────────
        SequentialAnimation {
            id: jumpAnim
            loops: 3
            running: room.active
            NumberAnimation {
                target: robot
                property: "y"
                to: robot.jumpStartY - 80
                duration: 300
                easing.type: Easing.OutQuad
            }
            NumberAnimation {
                target: robot
                property: "y"
                to: robot.jumpStartY
                duration: 300
                easing.type: Easing.InQuad
            }
            onFinished: {
                robot.y = room.floorY - robot.height
                robot.state = "idle"
            }
        }

        // ─── Зарядка (8 приседаний) ──────────────────────────────
        SequentialAnimation {
            id: exerciseAnim
            loops: 8
            running: room.active
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
            }
        }

        // ─── Визуальный выпад при ударе ───────────────────────────
        SequentialAnimation {
            id: kickLunge
            running: room.active
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

        // ─── Смена действий ───────────────────────────────────────
        Timer {
            id: brain
            interval: 2500 + Math.random() * 2500
            repeat: true
            running: room.active
            onTriggered: {
                if (!room.active) return;
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
            armsAnim.stop(); legsAnim.stop(); idleBob.stop();
            jumpAnim.stop(); exerciseAnim.stop();
            kickTimer.stop(); kickDelay.stop();
            fleeTimer.stop(); chaseTimer.stop();
            ball.visible = true;
            leftArm.rotation = 0; rightArm.rotation = 0;
            leftLeg.rotation = 0; rightLeg.rotation = 0;
            body.y = head.y + head.height + 4;

            switch (s) {
            case "idle":
                targetX = x;
                break;
            case "walk":
                targetX = room.walkMinX + Math.random() * (room.walkMaxX - room.walkMinX);
                vx = 1.2;
                break;
            case "run":
                targetX = (x < room.width / 2) ? room.walkMaxX : room.walkMinX;
                vx = 3.0;
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
                if (robot.state === "run")
                    robot.state = "idle";
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
                if (Math.abs(dx) < 30) {
                    ball.vx = (Math.random() > 0.5 ? 1 : -1) * (4 + Math.random() * 3);
                    ball.vy = -7 - Math.random() * 3;
                    kickLunge.start();
                } else {
                    robot.x += Math.sign(dx) * 4.0;
                }
            }
        }

        // ─── Пинки мяча ───────────────────────────────────────────
        Timer {
            id: kickTimer
            interval: 1200
            repeat: true
            running: room.active
            onTriggered: {
                if (!room.active) return;
                if (robot.kickCount >= 4) {
                    kickTimer.stop();
                    robot.state = "idle";
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
                ball.vx = (Math.random() > 0.5 ? 1 : -1) * (3 + Math.random() * 3);
                ball.vy = -6 - Math.random() * 3;
                kickLunge.start();
            }
        }

        // ─── Движение к цели ──────────────────────────────────────
        Timer {
            interval: 16
            repeat: true
            running: room.active && (robot.state === "walk" || robot.state === "run" || robot.state === "ball")
            onTriggered: {
                if (!room.active) return;
                var dx = robot.targetX - robot.x;
                if (Math.abs(dx) < 2) {
                    robot.vx = 0;
                    return;
                }
                robot.x += Math.sign(dx) * Math.min(Math.abs(dx), robot.vx);
            }
        }
    }

    // ─── Подпись ─────────────────────────────────────────────────
    Text {
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 40
        color: "#9aa0b5"
        font.pixelSize: 13
        visible: room.active
        text: {
            if (robot.chasing) return "🏃 бежит за мячом";
            switch (robot.state) {
            case "idle":     return "🤖 отдыхает";
            case "walk":     return "🚶 идёт";
            case "run":      return "🏃 бежит";
            case "jump":     return "🦘 прыгает";
            case "ball":     return "⚽ играет в мяч";
            case "exercise": return "💪 делает зарядку";
            default:         return "";
            }
        }
    }
}
