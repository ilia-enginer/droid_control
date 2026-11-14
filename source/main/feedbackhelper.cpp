#include "feedbackhelper.h"

#if defined(Q_OS_ANDROID)
#include <QCoreApplication>
#include <QJniObject>
#include <QJniEnvironment>
#include <QDebug>
#include <QThread>
#include <QOperatingSystemVersion>

// Собираем информацию об устройстве через android.os.Build
static QString getDeviceInfo()
{
    QString info;
    QJniEnvironment env;

    qDebug() << "getDeviceInfo: start";

    // --- Модель ---
    QJniObject model = QJniObject::getStaticObjectField(
        "android/os/Build", "MODEL", "Ljava/lang/String;");
    if (env.checkAndClearExceptions()) {
        qWarning() << "getDeviceInfo: JNI exception on MODEL";
    } else if (model.isValid()) {
        info += "Модель: " + model.toString() + "\n";
    }

    // --- Бренд ---
    QJniObject brand = QJniObject::getStaticObjectField(
        "android/os/Build", "BRAND", "Ljava/lang/String;");
    if (env.checkAndClearExceptions()) {
        qWarning() << "getDeviceInfo: JNI exception on BRAND";
    } else if (brand.isValid()) {
        info += "Бренд: " + brand.toString() + "\n";
    }

    // --- Производитель ---
    QJniObject manufacturer = QJniObject::getStaticObjectField(
        "android/os/Build", "MANUFACTURER", "Ljava/lang/String;");
    if (env.checkAndClearExceptions()) {
        qWarning() << "getDeviceInfo: JNI exception on MANUFACTURER";
    } else if (manufacturer.isValid()) {
        info += "Производитель: " + manufacturer.toString() + "\n";
    }

    // Версия Android через Qt (надежный способ)
    QOperatingSystemVersion version = QOperatingSystemVersion::current();
    info += "Версия Android: " + QString::number(version.majorVersion())
            + "." + QString::number(version.minorVersion()) + "\n";

    // --- Реальная версия Android через API Level (Huawei не может изменить) ---
    int sdkInt = QNativeInterface::QAndroidApplication::sdkVersion();
    info += "API Level: " + QString::number(sdkInt) + "\n";

//    // Расшифровка API Level в версию Android (для удобства чтения)
//    QString androidVersion;
//    switch (sdkInt) {
//        case 21: androidVersion = "5.0"; break;
//        case 22: androidVersion = "5.1"; break;
//        case 23: androidVersion = "6.0"; break;
//        case 24: androidVersion = "7.0"; break;
//        case 25: androidVersion = "7.1"; break;
//        case 26: androidVersion = "8.0"; break;
//        case 27: androidVersion = "8.1"; break;
//        case 28: androidVersion = "9";   break;
//        case 29: androidVersion = "10";  break;
//        case 30: androidVersion = "11";  break;
//        case 31: androidVersion = "12";  break;
//        case 32: androidVersion = "12L"; break;
//        case 33: androidVersion = "13";  break;
//        case 34: androidVersion = "14";  break;
//        case 35: androidVersion = "15";  break;
//        default: androidVersion = "неизвестно"; break;
//    }
//    info += "Версия Android: " + androidVersion + "\n";

    qDebug() << "getDeviceInfo: end, result =" << info;
    return info;
}

void FeedbackHelper::openFeedbackScreen()
{
    qDebug() << "openFeedbackScreen: start";
    qDebug() << "openFeedbackScreen: thread =" << QThread::currentThread()
             << "isMainThread =" << (QThread::currentThread() == QCoreApplication::instance()->thread());

    //auto *androidApp = QCoreApplication::instance()
    //    ->nativeInterface<QNativeInterface::QAndroidApplication>();
//    auto *androidApp = qApp->nativeInterface<QNativeInterface::QAndroidApplication>();

//    qDebug() << "openFeedbackScreen: androidApp =" << (void*)androidApp;

//    if (!androidApp) {
//        qWarning() << "openFeedbackScreen: androidApp is nullptr";
//        return;
//    }

//    qDebug() << "openFeedbackScreen: isActivityContext =" << androidApp->isActivityContext();

//    if (!androidApp->isActivityContext()) {
//        qWarning() << "openFeedbackScreen: context is not an Activity";
//        return;
//    }

//    QJniObject activity = androidApp->context();
    QJniObject activity = QNativeInterface::QAndroidApplication::context();
    qDebug() << "openFeedbackScreen: activity.isValid =" << activity.isValid();

    if (!activity.isValid()) {
        qWarning() << "openFeedbackScreen: activity is invalid";
        return;
    }

    // 2. Создаём Intent и явно указываем полное имя класса
    QJniObject intent("android/content/Intent");
    QJniObject className = QJniObject::fromString(
        "org.qtproject.example.FeedbackActivity");

    intent.callObjectMethod("setClassName",
        "(Landroid/content/Context;Ljava/lang/String;)Landroid/content/Intent;",
        activity.object(),
        className.object());

    // 3. Собираем информацию об устройстве и кладём в extras
    qDebug() << "openFeedbackScreen: before getDeviceInfo";
    QString deviceInfoStr = getDeviceInfo();
    qDebug() << "openFeedbackScreen: after getDeviceInfo";

    QJniObject deviceInfo = QJniObject::fromString(deviceInfoStr);
    QJniObject extraKey = QJniObject::fromString("device_info");

    intent.callObjectMethod("putExtra",
        "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
        extraKey.object(),
        deviceInfo.object());

    qDebug() << "openFeedbackScreen: calling startActivity";

    // 4. Запускаем Activity напрямую через контекст
    activity.callMethod<void>("startActivity", "(Landroid/content/Intent;)V", intent.object());

    qDebug() << "openFeedbackScreen: end";
}

#else
// Заглушка для Windows/Linux/macOS
void FeedbackHelper::openFeedbackScreen()
{
    qWarning() << "FeedbackHelper: only available on Android";
}
#endif
