TEMPLATE = app
TARGET = droid_stick

#версия на главном экране
VERSION = "1.1.4"
DEFINES += INSERT_VERSION_CODE=\\\"$$VERSION\\\"

QT += quick charts qml quickcontrols2 bluetooth core positioning widgets core-private network  #androidextras

SOURCES += \
#    btscaner.cpp \
    source/communication/crc.cpp \
    source/communication/rx/rx_commands.cpp \
    source/communication/tx/packing.cpp \
    source/communication/rx/unpacking.cpp \
    source/communication/tx/tx_commands.cpp \
    source/display_working/commun_display.cpp \
    source/main/appmanager.cpp \
    source/main/settings.cpp \
    source/update/appversion.cpp \
    source/device/device.cpp \
    source/device/deviceinfo.cpp \
#    devicemodel.cpp \
    source/main/main.cpp \
    source/main/mainmodel.cpp \
    source/update/updateapp.cpp \
    source/update/updatehex.cpp

RESOURCES += \
    Resurs.qrc \
    pages/UpdateDialogPage.qml \
    pages/DeviceScanerPage.qml \
    pages/main.qml \
    pages/SenderPage.qml \
    pages/SettingsPage.qml \
    pages/SettingFootPage.qml \
    pages/firmware_update.qml \
    pages/ServicePage.qml \
    qmldir \
    pages/ScrollablePage.qml \
    qtquickcontrols2.conf \
    icons/gallery/index.theme \
    icons/gallery/20x20/back.png \
    icons/gallery/20x20/drawer.png \
    icons/gallery/20x20/menu.png \
    icons/gallery/20x20@2/back.png \
    icons/gallery/20x20@2/drawer.png \
    icons/gallery/20x20@2/menu.png \
    icons/gallery/20x20@3/back.png \
    icons/gallery/20x20@3/drawer.png \
    icons/gallery/20x20@3/menu.png \
    icons/gallery/20x20@4/back.png \
    icons/gallery/20x20@4/drawer.png \
    icons/gallery/20x20@4/menu.png \
    images/play_store_512.png \
    images/arrow.png \
    images/arrow@2x.png \
    images/arrow@3x.png \
    images/arrow@4x.png \
    images/arrows.png \
    images/arrows@2x.png \
    images/arrows@3x.png \
    images/arrows@4x.png \
    images/qt-logo.png \
    images/qt-logo@2x.png \
    images/qt-logo@3x.png \
    images/qt-logo@4x.png \
    pages/ToolBar.qml \
    +Material/ToolBar.qml \
    images/bluetooth-512.png \
#    pages/joystick/Back.qml \
#    pages/joystick/Groove.qml \
#    pages/joystick/Handle.qml \
    pages/joystick/Joystick.qml \
#    pages/joystick/Lights.qml \
#    pages/joystick/Main.qml \
#    pages/joystick/SparseLight.qml \
    pages/joystick/finger.png \
    pages/joystick/background.png \
    pages/AboutPage.qml \
    pages/UpdateAppPage.qml \
    pages/Update_Hex.qml

target.path = $$[QT_INSTALL_EXAMPLES]/quickcontrols2/gallery
INSTALLS += target

DISTFILES += \
    pages/UpdateAppPage.qml \
    pages/Update_Hex.qml \
    android/AndroidManifest.xml \
    android/src/org/qtproject/example/InstallAPK.java \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/res/drawable-xhdpi/icon.png \
    android/res/drawable-xxhdpi/icon.png \
    android/res/drawable-xxxhdpi/icon.png \
    android/res/values/libs.xml \
    android/res/xml/provider_paths.xml \
    android/src/org/qtproject/example/InstallAPK.java \
    pages/SettingFootPage.qml \
    pages/firmware_update.qml


HEADERS += \
    source/communication/crc.h \
    source/communication/rx/rx_commands.h \
    source/communication/tx/packing.h \
    source/communication/rx/unpacking.h \
    source/communication/tx/tx_commands.h \
    source/display_working/commun_display.h \
    source/main/appmanager.h \
    source/main/settings.h \
    source/update/appversion.h \
    source/device/device.h \
    source/device/deviceinfo.h \
    source/main/mainmodel.h \
    source/update/updateapp.h \
    source/update/updatehex.h

#RESOURCES += qml.qrc

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    OTHER_FILES = \
    $$PWD/android/src/org/qtproject/example/InstallAPK.java\
    $$PWD/android/res/xml/provider_paths.xml

    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
 }

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/armeabi-v7a/libcrypto_1_1.so\
        $$PWD/armeabi-v7a/libssl_1_1.so
 }



