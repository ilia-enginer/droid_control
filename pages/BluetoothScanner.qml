//import QtQuick
//import QtQuick.Layouts
//import QtQuick.Controls

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ScrollablePage {
    id: page


    ColumnLayout {
        Label {
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: "Scan Device"
        }

        Button {
            id: dev1
            text: ""
            highlighted: false
            Layout.fillWidth: true
        }
        Button {
            id: dev2
            text: ""
            highlighted: false
            Layout.fillWidth: true
        }

        Button {
            id: dev3
            text: ""
            highlighted: false
            Layout.fillWidth: true
        }

        Button {
            id: dev4
            text: ""
            highlighted: false
            Layout.fillWidth: true
        }
        Button {
            id: dev5
            text: ""
            highlighted: false
            Layout.fillWidth: true
        }

        anchors.horizontalCenter: parent.horizontalCenter

        Button {
            text: "Start scan"
            highlighted: true
            Layout.fillWidth: true
        }
        Button {
            text: "Stop scan"
            highlighted: true
            Layout.fillWidth: true
        }
        TextField {
            id: field
            placeholderText: "1f2f55"
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Component.onCompleted:  {
        
    }
}
