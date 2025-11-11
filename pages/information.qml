import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material


Page {

    ComboBox {
        id: paramBox
        anchors.top: parent.up
        width: parent.width
        currentIndex: info.infoIndexMenu_

        model: ListModel {
               id: model
               ListElement { text: "..." }
               ListElement { text: "Включение" }
               ListElement { text: "Начало работы" }
               ListElement { text: "Режимы работы" }
               ListElement { text: "Настройка и калибровка" }
               ListElement { text: "Обновление прошивки шара" }
               ListElement { text: "Выключение" }
               ListElement { text: "Возможные ошибки" }
               ListElement { text: "Зарядка" }
           }
        onCurrentIndexChanged: {
            info.infoIndexMenu_ = paramBox.currentIndex

            //пустая строка
            if(currentIndex === 0)  info.clear();
            //Включение
            else if(currentIndex === 1) info.enabling();
            //Начало работы
            else if(currentIndex === 2) info.gettingStarted();
            //Режимы работы
            else if(currentIndex === 3) info.operatingModes();
            //Настройка и калибровка
            else if(currentIndex === 4) info.setupAndCalibration();
            //Обновление прошивки шара
            else if(currentIndex === 5) info.updatingHex();
            //Выключение
            else if(currentIndex === 6) info.shutdown();
            //Возможные ошибки
            else if(currentIndex === 7) info.possibleErrors();
            //Зарядка
            else if(currentIndex === 8) info.charging();
        }
    }

    Flickable {
         id: flick_text
         width: parent.width
         anchors.top: paramBox.bottom
         anchors.bottom: parent.bottom
         contentWidth: edit.contentWidth
         contentHeight: txt.contentHeight
         clip: true

         TextEdit {
             id: edit
             width: flick_text.width
             color: "#7387d1"
             focus: false
             activeFocusOnPress: false
             font: font.pixelSize = 13
             wrapMode: TextEdit.Wrap

             Text {
                 id: txt
                 width: parent.width
                 text: info.info_str_
                 wrapMode: TextEdit.Wrap
                 color: "#7387d1"
             }
         }
     }
}
