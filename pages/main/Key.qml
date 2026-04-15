
import QtQuick
import QtQuick.Controls 2.0
import QtCharts 2.15
import QtQml
import QtQuick.Controls.Material


Page {
    id: pageKey
    width: parent.width / 2
    height: parent.height / 3
    anchors.centerIn: parent
    anchors.verticalCenterOffset: -(parent.height * 0.08)
    opacity: 0.0

    property string key : "159"
    property string inputKey : ""

    function addNumber(num){
        inputKey = inputKey + num;
        console.log("inputKey = " + inputKey)
    }
    function checkKey(){
        if(key === inputKey){ deviceSelectionDialog.open(); inputKey = "" }
        else{
            console.log("key OFF")
            inputKey = ""
        }
    }

    Shortcut{
        sequence: "1"
        onActivated: addNumber("1")
    }
    Shortcut{
        sequence: "2"
        onActivated: addNumber("2")
    }
    Shortcut{
        sequence: "3"
        onActivated: addNumber("3")
    }
    Shortcut{
        sequence: "4"
        onActivated: addNumber("4")
    }
    Shortcut{
        sequence: "5"
        onActivated: addNumber("5")
    }
    Shortcut{
        sequence: "6"
        onActivated: addNumber("6")
    }
    Shortcut{
        sequence: "7"
        onActivated: addNumber("7")
    }
    Shortcut{
        sequence: "8"
        onActivated: addNumber("8")
    }
    Shortcut{
        sequence: "9"
        onActivated: addNumber("9")
    }
    Shortcut{
        sequence: "enter"
        onActivated: checkKey()
    }
    //1 строка
    //1
    MouseArea {
        id: zone1
        width: pageKey.width * 0.3
        height: pageKey.height * 0.3
        anchors.left: parent.left
        anchors.top: parent.top
       hoverEnabled: true

        onPressed: { addNumber("1") }
        onPressAndHold: { checkKey() }
    }
    //2
    MouseArea {
        id: zone2
        width: pageKey.width * 0.3
        height: pageKey.height * 0.3
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        hoverEnabled: true

        onPressed: { addNumber("2") }
        onPressAndHold: { checkKey() }
    }
    //3
    MouseArea {
        id: zone3
        width: pageKey.width * 0.3
        height: pageKey.height * 0.3
        anchors.right: parent.right
        anchors.top: parent.top
        hoverEnabled: true

        onPressed: { addNumber("3") }
        onPressAndHold: { checkKey() }
    }
    //2 строка
    //4
    MouseArea {
        id: zone4
        width: pageKey.width * 0.3
        height: pageKey.height * 0.3
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        hoverEnabled: true

        onPressed: { addNumber("4") }
        onPressAndHold: { checkKey() }
    }
    //5
    MouseArea {
        id: zone5
        width: pageKey.width * 0.3
        height: pageKey.height * 0.3
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        hoverEnabled: true

        onPressed: { addNumber("5") }
        onPressAndHold: { checkKey() }
    }
    //6
    MouseArea {
        id: zone6
        width: pageKey.width * 0.3
        height: pageKey.height * 0.3
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        hoverEnabled: true

        onPressed: { addNumber("6") }
        onPressAndHold: { checkKey() }
    }
    // 3 строчка
    //7
    MouseArea {
        id: zone7
        width: pageKey.width * 0.3
        height: pageKey.height * 0.3
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        hoverEnabled: true

        onPressed: { addNumber("7") }
        onPressAndHold: { checkKey() }
    }
    //8
    MouseArea {
        id: zone8
        width: pageKey.width * 0.3
        height: pageKey.height * 0.3
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        hoverEnabled: true

        onPressed: { addNumber("8") }
        onPressAndHold: { checkKey() }
    }
    //9
    MouseArea {
        id: zone9
        width: pageKey.width * 0.3
        height: pageKey.height * 0.3
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        hoverEnabled: true

        onPressed: { addNumber("9") }
        onPressAndHold: { checkKey() }
    }


    DeviceSelection{
        id: deviceSelectionDialog
        visible: false
    }
}
