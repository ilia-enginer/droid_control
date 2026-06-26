import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtQuick.Controls.Material

import "." as App

import "joystick"


Item {
    Component.onCompleted: {
        settParam.deviceShar.connect(devShar)
    }

    function devShar(){
        stackView.pop();
        listView.model.clear();
        navigateBackAction.enabled = true;
        listView.currentIndex = -1;

        listView.model.append({ title: "Джойстик", source: "qrc:/pages/shar/joystick/Joystick.qml" })
        listView.model.append({ title: "Окно настроек\nи калибровок", source: "qrc:/pages/shar/SettingFootPage.qml" })
        listView.model.append({ title: "Обновление прошивки", source: "qrc:/pages/shar/Firmware_update.qml" })
        listView.model.append({ title: "Справка", source: "qrc:/pages/shar/Information.qml" })
        if (mainModel.adminFlag === true){
            listView.model.append({ title: "Сервис", source: "qrc:/pages/shar/ServicePage.qml" })
            listView.model.append({ title: "Терминал", source: "qrc:/pages/shar/SenderPage.qml" })
            listView.model.append({ title: "Настройки", source: "qrc:/pages/shar/SettingsPage.qml" })
        }
    }

    function loadPageShar(page){
        switch(page){
            case 0 :
               stackView.push(joystick_shar)
                break
            case 1 :
                stackView.push(settingFoot)
                break
            case 2:
                stackView.push(firmwareUpdate)
                break
            case 3:
                stackView.push(informationPage)
                break
            case 4:
                stackView.push(pageService)
                break
            case 5:
                stackView.push(terminalPage)
                break
            case 6:
                stackView.push(settingsPageShar)
                break
            default:
                break
            }
    }

    Joystick {
        id: joystick_shar
        visible: false
    }
    Firmware_update{
        id: firmwareUpdate
        visible: false
    }
    Information{
        id: informationPage
        visible: false
    }
    ServicePage {
        id: pageService
        visible: false
    }
    SettingFootPage{
        id: settingFoot
        visible: false
    }
    SettingsPageShar{
        id: settingsPageShar
        visible: false
    }
}
