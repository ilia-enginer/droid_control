#ifndef MAINSERIALPORT_H
#define MAINSERIALPORT_H

#include <QMainWindow>
#include <QSerialPort>
#include "../../display_working/commun_display.h"


class SettingsDialog;

class MainSerialPort : public QWidget   //QMainWindow
{
    Q_OBJECT

public:
    explicit MainSerialPort(QWidget *parent = nullptr);
    ~MainSerialPort();

    int writeData(const QByteArray &data);

signals:
    void connected(QString type);
    void messageReceived(QByteArray mess);

public slots:
    void settingsWindowOpen();
    void openSerialPort();
    void closeSerialPort();

private slots:
    void readData();
    void handleError(QSerialPort::SerialPortError error);

private:
    void showStatusMessage(const QString &message);

    SettingsDialog *m_settings = nullptr;
    QSerialPort *m_serial = nullptr;
    Commun_display *_commun_display = nullptr;

};


#endif // MAINSERIALPORT_H

