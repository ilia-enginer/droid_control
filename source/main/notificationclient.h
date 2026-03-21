#ifndef NOTIFICATIONCLIENT_H
#define NOTIFICATIONCLIENT_H

#include <QObject>
#include <QTimer>
#include <QStringList>


class NotificationClient : public QObject {
  Q_OBJECT
public:
  explicit NotificationClient(QObject *parent = 0);
  ~NotificationClient();

  void setNotification(const QString &notification);
  QString notification() const;

  // Тестовый таймер: запустить/остановить отправку уведомлений раз в минуту
  Q_INVOKABLE void startTestTimer(int intervalMs = 60000);
  Q_INVOKABLE void stopTestTimer();
  
  // Callback метод, вызываемый из Java через JNI
  void onTestTimerTimeout();

signals:
  void notificationChanged();

private slots:
  void updateAndroidNotification();

private:
  QString m_notification;
  QTimer m_testTimer;
  int m_testCounter = 0;
  
  // Массив фраз для тестовых уведомлений
  QStringList m_testPhrases = {
    "Проверка связи с устройством",
    "Мониторинг фоновой работы",
    "Система работает нормально",
    "Обновление данных",
    "Синхронизация завершена",
    "Проверка соединения",
    "Фоновая задача активна",
    "Всё работает корректно"
  };
};

#endif // NOTIFICATIONCLIENT_H
