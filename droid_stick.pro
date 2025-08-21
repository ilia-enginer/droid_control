TEMPLATE = app
TARGET = droid_stick

VERSION = "1.1.2"
DEFINES += INSERT_VERSION_CODE=\\\"$$VERSION\\\"

QT += quick charts qml quickcontrols2 bluetooth core positioning widgets core-private network #androidextras

SOURCES += \
#    btscaner.cpp \
    appmanager.cpp \
    appversion.cpp \
    device.cpp \
    deviceinfo.cpp \
#    devicemodel.cpp \
    gallery.cpp \
    mainmodel.cpp \
    updateapp.cpp

RESOURCES += \
    Resurs.qrc \
    gallery.qml \
    DeviceScanerPage.qml \
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
    ToolBar.qml \
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
    AboutPage.qml

target.path = $$[QT_INSTALL_EXAMPLES]/quickcontrols2/gallery
INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/build.gradle \
    android/gradle.properties \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew \
    android/gradlew.bat \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/res/values/libs.xml \
    pages/SettingFootPage.qml \
    pages/firmware_update.qml


HEADERS += \
    appmanager.h \
    appversion.h \
    device.h \
    deviceinfo.h \
    mainmodel.h \
    updateapp.h

#RESOURCES += qml.qrc

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
