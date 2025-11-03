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
    Q_PROPERTY(int adminTapCount READ getAdminTapCount WRITE incAdminTapCount NOTIFY onAdminTapCountChanged)
    Q_PROPERTY(int servisIndexMenu READ getservisIndexMenu WRITE setservisIndexMenu NOTIFY servisIndexMenuChanged)

    MainModel();
    void setDevice(Device *device);
    void setSettings(Settings *newSettings);
    void setTx_commands(Tx_commands *newTx_commands);
    void setRx_commands(Rx_commands *newRx_commands);
    void setUpdateHex(UpdateHex *newUpdateHex);
    void setCommun_display(Commun_display *newCommun_display);

    int getservisIndexMenu() const;
    void setservisIndexMenu(int newservisIndexMenu);

    int checkingParameters();

public slots:
    void incAdminTapCount(int value);
    int  getAdminTapCount();  

Q_SIGNALS:
    void servisIndexMenuChanged();
    void onAdminTapCountChanged();

private:
    Device *device_ = nullptr;
    Settings *_settings = nullptr;
    Tx_commands *_tx_commands = nullptr;
    UpdateHex *_updateHex = nullptr;
    Commun_display *_commun_display = nullptr;
    Rx_commands *_rx_commands = nullptr;


    int servisIndexMenu = 0;
    int adminTapCount = 0;      ///флаг админа
                                /// если -1, то полный доступ ко всем функциям
};
#endif // MAINMODEL_H
