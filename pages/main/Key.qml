
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material


/**58632***/

Item {
    id: pageKey
    width: parent.width / 2
    height: parent.height / 3
    anchors.centerIn: parent
    anchors.verticalCenterOffset: -(parent.height * 0.08)

    property int cnt : 0
    property int but : -1

    //1 строка
    //1
    MouseArea {
        id: zone1
        width: pageKey.width * 0.1
        height: pageKey.height * 0.1
        anchors.left: parent.left
        anchors.top: parent.top
        hoverEnabled: true
        onEntered: {
            but = -1
            cnt = 0
        }
    }
    //2
    MouseArea {
        id: zone2
        width: pageKey.width * 0.1
        height: pageKey.height * 0.1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        hoverEnabled: true
        onEntered: {
            if((but === 3) && (cnt === 5)){
                but = 3
                cnt++
            }
            else{
                but = -1
                cnt = 0
            }
        }
    }
    //3
    MouseArea {
        id: zone3
        width: pageKey.width * 0.1
        height: pageKey.height * 0.1
        anchors.right: parent.right
        anchors.top: parent.top
        hoverEnabled: true
        onEntered: {
            if((but === 6) && (cnt === 4)){
                but = 3
                cnt++
            }
            else{
                but = -1
                cnt = 0
            }
        }
    }
    //2 строка
    //4
    MouseArea {
        id: zone4
        width: pageKey.width * 0.1
        height: pageKey.height * 0.1
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        hoverEnabled: true
        onEntered: {
            but = -1
            cnt = 0
        }
    }
    //5
    MouseArea {
        id: zone5
        width: pageKey.width * 0.1
        height: pageKey.height * 0.1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        hoverEnabled: true
        onEntered: {
            if((but === -1) && (cnt === 0)){
                but = 5
                cnt++
            }
            else{
                but = -1
                cnt = 0
            }
        }
    }
    //6
    MouseArea {
        id: zone6
        width: pageKey.width * 0.1
        height: pageKey.height * 0.1
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        onEntered: {
            if((but === 8) && (cnt === 2)){
                but = 6
                cnt++
            }
            else{
                but = -1
                cnt = 0
            }
        }
    }
    // 3 строчка
    //7
    MouseArea {
        id: zone7
        width: pageKey.width * 0.1
        height: pageKey.height * 0.1
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        hoverEnabled: true
        onEntered: {
            but = -1
            cnt = 0
        }
    }
    //8
    MouseArea {
        id: zone8
        width: pageKey.width * 0.1
        height: pageKey.height * 0.1
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        hoverEnabled: true
        onEntered: {
            if((but === 5) && (cnt === 1)){
                but = 8
                cnt++
            }
            else{
                but = -1
                cnt = 0
            }
        }
    }
    //9
    MouseArea {
        id: zone9
        width: pageKey.width * 0.1
        height: pageKey.height * 0.1
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        hoverEnabled: true
        onEntered: {
            but = -1
            cnt = 0
        }
    }
}
