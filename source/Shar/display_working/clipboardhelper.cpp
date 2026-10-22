#include "ClipboardHelper.h"
#include <QGuiApplication>
#include <QClipboard>

ClipboardHelper::ClipboardHelper(QObject *parent)
    : QObject(parent)
{
}

bool ClipboardHelper::copyText(const QString &text)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    if (!clipboard)
        return false;

    // На Android/X11 установка текста может быть асинхронной,
    // но setText синхронно передаёт данные в системный буфер.
    clipboard->setText(text, QClipboard::Clipboard);

    // Дополнительно: на Linux/X11 имеет смысл продублировать в Selection,
    // чтобы работала вставка средней кнопкой мыши.
#ifdef Q_OS_LINUX
    if (clipboard->supportsSelection())
        clipboard->setText(text, QClipboard::Selection);
#endif

    return true;
}

QString ClipboardHelper::text() const
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    return clipboard ? clipboard->text(QClipboard::Clipboard) : QString();
}
