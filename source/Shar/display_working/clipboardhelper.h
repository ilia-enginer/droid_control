#pragma once

#include <QObject>
#include <QString>

class ClipboardHelper : public QObject
{
    Q_OBJECT

public:
    explicit ClipboardHelper(QObject *parent = nullptr);

    // Копирует текст в системный буфер обмена.
    // Возвращает true при успехе.
    Q_INVOKABLE bool copyText(const QString &text);

    // Дополнительно: прочитать текущий текст из буфера (может пригодиться).
    Q_INVOKABLE QString text() const;
};
