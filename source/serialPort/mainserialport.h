#ifndef MAINSERIALPORT_H
#define MAINSERIALPORT_H

#include <QMainWindow>
#include <QSerialPort>
#include "../display_working/commun_display.h"

QT_BEGIN_NAMESPACE

class QLabel;

//namespace Ui {
//class MainSerialPort;
//}

QT_END_NAMESPACE

class Console;
class SettingsDialog;

class MainSerialPort : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainSerialPort(QWidget *parent = nullptr);
    ~MainSerialPort();

    void writeData(const QByteArray &data);

public slots:
    void settingsWindowOpen();
    void openSerialPort();
    void closeSerialPort();

private slots:
    void about();
    void readData();
    void handleError(QSerialPort::SerialPortError error);

private:
    void initActionsConnections();

private:
    void showStatusMessage(const QString &message);

//    Ui::MainSerialPort *m_ui = nullptr;
    QLabel *m_status = nullptr;
    Console *m_console = nullptr;
    SettingsDialog *m_settings = nullptr;
    QSerialPort *m_serial = nullptr;
    Commun_display *_commun_display = nullptr;
};


#endif // MAINSERIALPORT_H


