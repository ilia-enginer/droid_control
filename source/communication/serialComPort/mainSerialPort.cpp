
#include "mainSerialPort.h"
#include "settingsdialog.h"

#include <QMessageBox>

//! [0]
MainSerialPort::MainSerialPort(QWidget *parent) :
    QWidget(parent),    //QMainWindow(parent),
    m_settings(new SettingsDialog),
    m_serial(new QSerialPort(this)),
    _commun_display(new Commun_display(this))
{
    connect(m_serial, &QSerialPort::errorOccurred, this, &MainSerialPort::handleError);
//! [2]
    connect(m_serial, &QSerialPort::readyRead, this, &MainSerialPort::readData);
}

MainSerialPort::~MainSerialPort()
{
    delete m_settings;
}

void MainSerialPort::settingsWindowOpen()
{
    m_settings->fillPortsInfo();
    m_settings->show();
}

//! [4]
void MainSerialPort::openSerialPort()
{
    const SettingsDialog::Settings p = m_settings->settings();
    m_serial->setPortName(p.name);
    m_serial->setBaudRate(p.baudRate);
    m_serial->setDataBits(p.dataBits);
    m_serial->setParity(p.parity);
    m_serial->setStopBits(p.stopBits);
    m_serial->setFlowControl(p.flowControl);
    if (m_serial->open(QIODevice::ReadWrite)) {
        _commun_display->set_connected(true);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
        emit connected("comport");
    }
    else{
        QMessageBox::critical(this, tr("Error"), m_serial->errorString());
        showStatusMessage(tr("Open error"));
    }
}
//! [4]

//! [5]
void MainSerialPort::closeSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();

    emit connected("none");
    showStatusMessage(tr("Устройство отключено"));
    _commun_display->set_connected(false);
}
//! [5]

//! [6]
int MainSerialPort::writeData(const QByteArray &data)
{
    return m_serial->write(data);
}
//! [6]

//! [7]
void MainSerialPort::readData()
{
    QByteArray data = m_serial->readAll();
    if(data.isEmpty())  return;
    emit messageReceived(data);
}
//! [7]

//! [8]
void MainSerialPort::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), m_serial->errorString());
        closeSerialPort();
    }
}

void MainSerialPort::showStatusMessage(const QString &message)
{
    _commun_display->setCurDeviceName(message);
}
