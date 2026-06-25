import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material

import "." as App

import "joystick"

Item {
    Component.onCompleted: {
        settParam.devicePylt.connect(devPylt)
    }

    function devPylt(){
        stackView.pop();
        listView.model.clear();
        navigateBackAction.enabled = true;
        listView.currentIndex = -1;

        listView.model.append({ title: "Джойстик", source: "qrc:/pages/pylt/joystick/Joystick.qml" })
        listView.model.append({ title: "Настройки", source: "qrc:/pages/pylt/Settings_Pylt" })

        if (mainModel.adminFlag === true){
            listView.model.append({ title: "Терминал", source: "qrc:/pages/shar/SenderPage.qml" })            
        }
    }

    function loadPagePylt(page){
        switch(page){
            case 0 :
               stackView.push(joystick_pylt)
                break
            case 1:
                stackView.push(settings_Pylt)
                break
            case 2:
                stackView.push(terminalPage)
                break
            default:
                break
            }
    }

    Joystick {
        id: joystick_pylt
        visible: false
    }
    Settings_Pylt{
        id: settings_Pylt
    }

}
