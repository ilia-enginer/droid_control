
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material

//Pane {
Page{
    id: pane
    Image {
        id: logo
        width: pane.availableWidth / 1.9
        height: pane.availableHeight / 1.9
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -(parent.height * 0.08)
        fillMode: Image.PreserveAspectFit
        source: "../../images/play_store_512.png"
    }
    //имя приемника
    Text {
        id: devName
        width: parent.width
        wrapMode: Label.Wrap
        horizontalAlignment: Qt.AlignHCenter
        text: commun_display.curDeviceName_
        font.pixelSize: 16
    }
//    QtObject{}
}
