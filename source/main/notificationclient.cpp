#include "notificationclient.h"
#include "appmanager.h"

#if defined(Q_QDOC) ||                                                         \
    (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))
#include <QtConcurrent/QtConcurrent>
#include <QtCore/private/qandroidextras_p.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qjniobject.h>
#include <QtCore/qjnienvironment.h>
#include <QThread>
#include <QDebug>
#include <QDateTime>

// Глобальный указатель на экземпляр NotificationClient для JNI callback
static NotificationClient* g_notificationClientInstance = nullptr;

// JNI callback функция, вызываемая из Java KeepAliveService
extern "C" JNIEXPORT void JNICALL
Java_org_qtproject_example_KeepAliveService_nativeOnTimerTick(JNIEnv* env, jobject obj) {
    Q_UNUSED(env);
    Q_UNUSED(obj);
    
    qDebug() << "[NotificationClient] ===== JNI callback from Java =====";
    qDebug() << "[NotificationClient] Thread:" << QThread::currentThread();
    
    if (g_notificationClientInstance) {
        // Вызываем метод C++ объекта
        g_notificationClientInstance->onTestTimerTimeout();
    } else {
        qWarning() << "[NotificationClient] ERROR: g_notificationClientInstance is NULL!";
    }
}

#endif

NotificationClient::NotificationClient(QObject *parent) : QObject(parent) {
#if defined(Q_QDOC) ||                                                         \
    (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))
  // Регистрируем глобальный экземпляр для JNI callback
  g_notificationClientInstance = this;
  qDebug() << "[NotificationClient] Instance registered for JNI callbacks";
  
  // Android 13+ (API 33): запрашиваем разрешение POST_NOTIFICATIONS
  if (QNativeInterface::QAndroidApplication::sdkVersion() >= 33) {
    const auto notificationPermission = "android.permission.POST_NOTIFICATIONS";
    auto requestResult =
        QtAndroidPrivate::requestPermission(notificationPermission);
    if (requestResult.result() != QtAndroidPrivate::Authorized) {
      qWarning() << "Failed to acquire permission to post notifications "
                    "(required for Android 13+)";
    }
  }

  connect(this, &NotificationClient::notificationChanged, this,
          &NotificationClient::updateAndroidNotification);
#endif
}

NotificationClient::~NotificationClient() {
#if defined(Q_QDOC) ||                                                         \
    (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))
  // Отменяем регистрацию
  if (g_notificationClientInstance == this) {
    g_notificationClientInstance = nullptr;
    qDebug() << "[NotificationClient] Instance unregistered from JNI callbacks";
  }
#endif
}

void NotificationClient::setNotification(const QString &notification) {
  if (notification.isEmpty())
    return;
  if (m_notification == notification)
    return;

  m_notification = notification;
  emit notificationChanged();
}

QString NotificationClient::notification() const { return m_notification; }

void NotificationClient::updateAndroidNotification() {
#if defined(Q_QDOC) ||                                                         \
    (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))
  
  QString msg = m_notification;
  
  qDebug() << "[NotificationClient] updateAndroidNotification called, msg:" << msg;
  qDebug() << "[NotificationClient] Current thread:" << QThread::currentThread();
  qDebug() << "[NotificationClient] Main thread:" << QCoreApplication::instance()->thread();
  
  // ИСПРАВЛЕНИЕ #1: Используем Application Context вместо Activity Context
  // Application Context живёт всё время работы приложения и не зависит от Activity lifecycle
  QJniObject appContext = QNativeInterface::QAndroidApplication::context();
  if (appContext.isValid()) {
    // Получаем Application Context через getApplicationContext()
    appContext = appContext.callObjectMethod("getApplicationContext", "()Landroid/content/Context;");
  }
  
  if (!appContext.isValid()) {
    qCritical() << "[NotificationClient] ERROR: Cannot get Application Context!";
    return;
  }
  
  qDebug() << "[NotificationClient] Application Context obtained successfully";
  
  // ИСПРАВЛЕНИЕ #2: Запускаем в worker thread с правильным JNI thread attachment
  QtConcurrent::run([msg, appContext]() {
    qDebug() << "[NotificationClient] Inside QtConcurrent::run lambda";
    qDebug() << "[NotificationClient] Worker thread:" << QThread::currentThread();
    
    // КРИТИЧНО: QJniEnvironment автоматически прикрепляет текущий поток к JVM
    // При выходе из scope (деструктор) поток будет откреплён
    QJniEnvironment env;
    
    // Проверяем, что поток успешно прикреплён
    if (!env.javaVM()) {
      qCritical() << "[NotificationClient] ERROR: Failed to attach thread to JVM!";
      return;
    }
    
    qDebug() << "[NotificationClient] Thread attached to JVM successfully";
    
    // Проверяем валидность контекста
    if (!appContext.isValid()) {
      qWarning() << "[NotificationClient] ERROR: Application Context is invalid in worker thread!";
      return;
    }
    
    // Создаём Java строку
    QJniObject javaMsg = QJniObject::fromString(msg);
    qDebug() << "[NotificationClient] Java message created, valid:" << javaMsg.isValid();
    
    if (!javaMsg.isValid()) {
      qWarning() << "[NotificationClient] ERROR: Failed to create Java string!";
      return;
    }
    
    // Вызываем Java метод с Application Context
    qDebug() << "[NotificationClient] Calling Java NotificationClient.notify()...";
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/NotificationClient", "notify",
        "(Landroid/content/Context;Ljava/lang/String;)V",
        appContext.object(),
        javaMsg.object<jstring>());
    
    // Проверяем JNI исключения
    if (env->ExceptionCheck()) {
      qCritical() << "[NotificationClient] JNI EXCEPTION occurred!";
      env->ExceptionDescribe();
      env->ExceptionClear();
    } else {
      qDebug() << "[NotificationClient] Java method call completed successfully";
    }
    
    // QJniEnvironment деструктор автоматически открепит поток от JVM
  });
  
  qDebug() << "[NotificationClient] QtConcurrent::run scheduled";
#endif
}

// ─── AppManager ──────────────────────────────────────────────────────────────

void NotificationClient::setAppManager(AppManager *am) { m_appManager = am; }

// ─── Тестовый таймер ─────────────────────────────────────────────────────────

void NotificationClient::startTestTimer(int intervalMs) {
  m_testCounter = 0;
  
  qDebug() << "[NotificationClient] ===== Starting background notifications =====";
  qDebug() << "[NotificationClient] Interval:" << intervalMs << "ms";
  qDebug() << "[NotificationClient] Phrases count:" << m_testPhrases.size();

#if defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED)
  // Запросить исключение из батарейной оптимизации (Doze mode).
  QNativeInterface::QAndroidApplication::runOnAndroidMainThread([] {
    qDebug() << "[NotificationClient] Requesting battery optimization ignore...";
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/NotificationClient",
        "requestIgnoreBatteryOptimizations", "(Landroid/content/Context;)V",
        QNativeInterface::QAndroidApplication::context());
  });
  
  // Запускаем KeepAliveService с таймером
  // Handler в сервисе будет вызывать наш C++ callback через JNI
  QNativeInterface::QAndroidApplication::runOnAndroidMainThread([intervalMs] {
    qDebug() << "[NotificationClient] Starting KeepAliveService with timer...";
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/KeepAliveService",
        "startServiceWithTimer",
        "(Landroid/content/Context;I)V",
        QNativeInterface::QAndroidApplication::context(),
        intervalMs);
    qDebug() << "[NotificationClient] KeepAliveService with timer started";
  });
#endif
}

void NotificationClient::stopTestTimer() {
  qDebug() << "[NotificationClient] ===== Stopping background notifications =====";
  
#if defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED)
  // Останавливаем таймер в KeepAliveService
  QNativeInterface::QAndroidApplication::runOnAndroidMainThread([] {
    qDebug() << "[NotificationClient] Stopping KeepAliveService timer...";
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/KeepAliveService",
        "stopServiceTimer",
        "(Landroid/content/Context;)V",
        QNativeInterface::QAndroidApplication::context());
    qDebug() << "[NotificationClient] KeepAliveService timer stopped";
  });
#endif
}

void NotificationClient::onTestTimerTimeout() {
  ++m_testCounter;
  qDebug() << "[NotificationClient] ===== Timer callback from Java ===== Counter:" << m_testCounter;
  qDebug() << "[NotificationClient] Current time:" << QDateTime::currentDateTime().toString("hh:mm:ss");
  qDebug() << "[NotificationClient] Thread:" << QThread::currentThread();
  
  // Циклически перебираем фразы из массива
  int phraseIndex = (m_testCounter - 1) % m_testPhrases.size();
  QString message = m_testPhrases[phraseIndex] + QString(" #%1").arg(m_testCounter);
  
  qDebug() << "[NotificationClient] Using phrase[" << phraseIndex << "]:" << m_testPhrases[phraseIndex];
  qDebug() << "[NotificationClient] Full message:" << message;
  
#if defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED)
  // КРИТИЧНО: НЕ используем setNotification() и signal/slot!
  // Signal ставится в очередь Qt main thread, который приостановлен в фоне.
  // Вместо этого вызываем Java метод НАПРЯМУЮ из текущего потока (Handler thread).
  
  qDebug() << "[NotificationClient] Sending notification DIRECTLY from JNI callback thread...";
  
  // Получаем Application Context
  QJniObject appContext = QNativeInterface::QAndroidApplication::context();
  if (appContext.isValid()) {
    appContext = appContext.callObjectMethod("getApplicationContext", "()Landroid/content/Context;");
  }
  
  if (!appContext.isValid()) {
    qCritical() << "[NotificationClient] ERROR: Cannot get Application Context!";
    return;
  }
  
  // Прикрепляем текущий поток к JVM (Handler thread)
  QJniEnvironment env;
  if (!env.javaVM()) {
    qCritical() << "[NotificationClient] ERROR: Failed to attach thread to JVM!";
    return;
  }
  
  qDebug() << "[NotificationClient] Thread attached to JVM, sending notification...";
  
  // Создаём Java строку
  QJniObject javaMsg = QJniObject::fromString(message);
  
  // Вызываем Java метод НАПРЯМУЮ
  QJniObject::callStaticMethod<void>(
      "org/qtproject/example/NotificationClient", "notify",
      "(Landroid/content/Context;Ljava/lang/String;)V",
      appContext.object(),
      javaMsg.object<jstring>());
  
  // Проверяем JNI исключения
  if (env->ExceptionCheck()) {
    qCritical() << "[NotificationClient] JNI EXCEPTION occurred!";
    env->ExceptionDescribe();
    env->ExceptionClear();
  } else {
    qDebug() << "[NotificationClient] Notification sent successfully from JNI callback";
  }
#else
  // На не-Android платформах используем обычный механизм
  setNotification(message);
#endif
}
