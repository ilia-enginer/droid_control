#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QObject>
#include "settings.h"
#include "../communication/tx/tx_commands.h"
#include "../communication/rx/rx_commands.h"
#include "../update/updatehex.h"
#include "../display_working/commun_display.h"


class Device;

class MainModel: public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(bool adminFlag READ getAdminFlag WRITE setAdminFlag NOTIFY onAdminFlagChanged)

    MainModel();
    void setDevice(Device *device);
    void setSettings(Settings *newSettings);
    void setTx_commands(Tx_commands *newTx_commands);
    void setRx_commands(Rx_commands *newRx_commands);
    void setUpdateHex(UpdateHex *newUpdateHex);
    void setCommun_display(Commun_display *newCommun_display);

    int checkingParameters();
    int checkID();
    int checkUpdate();

public slots:

    void setAdminFlag(bool value);
    bool  getAdminFlag();

Q_SIGNALS:
    void onAdminFlagChanged();

private:
    Device *device_ = nullptr;
    Settings *_settings = nullptr;
    Tx_commands *_tx_commands = nullptr;
    UpdateHex *_updateHex = nullptr;
    Commun_display *_commun_display = nullptr;
    Rx_commands *_rx_commands = nullptr;

    bool adminFlag = false;      ///флаг админа
                                /// если -1, то полный доступ ко всем функциям
};
#endif // MAINMODEL_H
