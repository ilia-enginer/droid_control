#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QObject>
#include <functional>
#include <QTimer>
#include "settings.h"
#include "../Shar/communication/tx_commands.h"
#include "../update/updatehex.h"
#include "../Shar/display_working/commun_display.h"
#include "../communication/serialComPort/mainSerialPort.h"
#include "../communication/packing.h"
#include "../Pylt/communication/tx_commandsPylt.h"
#include "../Pylt/pylt_settings.h"


class Device;
class MainSerialPort;

class MainModel: public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(bool adminFlag READ getAdminFlag WRITE setAdminFlag NOTIFY onAdminFlagChanged)

    explicit MainModel(QObject *parent = nullptr);
    ~MainModel();

    void setDevice(Device *device);
    void setSettings(Settings *newSettings);
    void setTx_commands(Tx_commands *newTx_commands);
    void setTx_commandsPylt(Tx_commandsPylt *newTx_commandsPylt);
    void setUpdateHex(UpdateHex *newUpdateHex);
    void setCommun_display(Commun_display *newCommun_display);
    void setMainSerialComPort(MainSerialPort *newMainSerialPort);
    void setPacking(Packing *newPacking);
    void setPylt_settings(Pylt_settings * newPylt_settings);

    void checkingParameters(const std::function<void()> &done);
    void checkID(const std::function<void(int)> &done);
    void checkUpdate();

public slots:
    void setAdminFlag(bool value);
    bool getAdminFlag();
    void deviceConnect(QString type, QString name);

Q_SIGNALS:
    void onAdminFlagChanged();

private slots:
    void retryTick();

private:
    Device              * device_ = nullptr;
    Settings            * _settings = nullptr;
    Tx_commands         * _tx_commands = nullptr;
    Tx_commandsPylt     * _tx_commandsPylt = nullptr;
    Pylt_settings       * _pylt_settings = nullptr;
    UpdateHex           * _updateHex = nullptr;
    Commun_display      * _commun_display = nullptr;
    MainSerialPort      * _mainserialport = nullptr;
    Packing             * _packing;

    //асинхронное ожидание ответа прибора (вместо блокирующих delay)
    void startRetry(const std::function<void()> &send,
                    const std::function<bool()> &ready,
                    const std::function<void()> &onDone,
                    const std::function<void()> &onFail,
                    int maxAttempts, int intervalMs);
    QTimer              * _retryTimer = nullptr;
    int                   _retryCount = 0;
    int                   _retryMaxAttempts = 0;
    std::function<bool()> _retryCheck;
    std::function<void()> _retrySend;
    std::function<void()> _retryDone;
    std::function<void()> _retryFail;


    ///флаг админа
    /// если -1, то полный доступ ко всем функциям
    #ifdef Q_OS_WIN
        bool adminFlag = true;
    #elif defined(Q_OS_MACOS)
        bool adminFlag = false;
    #elif defined(Q_OS_ANDROID)
        bool adminFlag = false;
    #elif defined(Q_OS_LINUX)
        bool adminFlag = true;
    #endif

};
#endif // MAINMODEL_H
