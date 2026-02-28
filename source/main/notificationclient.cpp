#include "notificationclient.h"

#if defined(Q_QDOC) || (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))
#include <QtCore/qjniobject.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/private/qandroidextras_p.h>
#endif

//using namespace Qt::StringLiterals;

NotificationClient::NotificationClient(QObject *parent)
    : QObject(parent)
{
#if defined(Q_QDOC) || (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))
    if (QNativeInterface::QAndroidApplication::sdkVersion() >= 33) {
        const auto notificationPermission = "android.permission.POST_NOTIFICATIONS";
        auto requestResult = QtAndroidPrivate::requestPermission(notificationPermission);
        if (requestResult.result() != QtAndroidPrivate::Authorized) {
            qWarning() << "Failed to acquire permission to post notifications "
                          "(required for Android 13+)";
        }
    }

    connect(this, &NotificationClient::notificationChanged,
            this, &NotificationClient::updateAndroidNotification);
#endif
}

void
NotificationClient::setNotification(const QString &notification)
{
    if(notification.isEmpty())              return;
    if (m_notification == notification)     return;

    //! [notification changed signal]
        m_notification = notification;
        emit notificationChanged();
    //! [notification changed signal]
}

QString
NotificationClient::notification() const
{
    return m_notification;
}

void
NotificationClient::updateAndroidNotification()
{

#if defined(Q_QDOC) || (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))

    QJniObject javaNotification = QJniObject::fromString(m_notification);
    QJniObject::callStaticMethod<void>(
                    "org/qtproject/example/NotificationClient",
                    "notify",
                    "(Landroid/content/Context;Ljava/lang/String;)V",
                    QNativeInterface::QAndroidApplication::context(),
                    javaNotification.object<jstring>());
#endif
}
