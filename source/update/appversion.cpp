#include "appversion.h"

#include <QClipboard>
#include <QMessageBox>
#include <QApplication>

AppVersion::AppVersion(QObject *parent) :
    QObject(parent)
{
}
AppVersion::~AppVersion()
{
}

qint32
AppVersion::getAppVersion()
{
    return _appVersion;
}

QString
AppVersion::getversion_app()
{
    return version_app;
}

void
AppVersion::setversion_app(QString &vers)
{
    version_app = vers;
}

void
AppVersion::appVersionVisible()
{
  #ifdef QT_DEBUG
    QClipboard *clipboard = QGuiApplication::clipboard();
    QMessageBox msgBox;

    qint32 ver = getAppVersion();

    msgBox.setWindowModality(Qt::WindowModal);
    msgBox.setWindowTitle("Версия приложения.");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(QString("%1\n\n%2.%3.%4.%5.%6.%7")
                   .arg(ver)                                         //версия полным числом
                   .arg(ver >> 28, 1, 10)                            // версия
                   .arg((ver >> 20) & 0x0FF, 2, 10, QChar('0'))      // год
                   .arg((ver >> 16) & 0x0F, 2, 10, QChar('0'))       // месяц
                   .arg((ver >> 11) & 0x1F, 2, 10, QChar('0'))       // день
                   .arg((ver >> 6) & 0x01F, 2, 10, QChar('0'))       // час
                   .arg(ver & 0x3F, 2, 10, QChar('0')));             // минуты

    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Ok );

    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::Save:
          // Save was clicked
        clipboard->setText(QString::number(ver), QClipboard::Clipboard);    //скопирует в буфер
          break;
      default:
          // should never be reached
          break;
    }
  #endif
}
