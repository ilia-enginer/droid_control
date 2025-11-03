#ifndef UPDATEHEX_H
#define UPDATEHEX_H

#include <QObject>
#include <QWidget>
#include <QFile>
#include "../communication/tx/tx_commands.h"
#include "../communication/crc.h"
#include "../display_working/commun_display.h"
#include "../main/settings.h"



class UpdateHex : public QObject
{
    Q_OBJECT
public:
    explicit UpdateHex(QObject *parent = nullptr);
    ~UpdateHex();

    const QByteArray getVersionLabel() {return "version\0";} //{0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x00};   // version

    void setTx_commands(Tx_commands *newTx_commands);
    void setCrc(Crc *newCrc);
    void setCommun_display(Commun_display *newCommun_display);
    void setSettings(Settings *newSettings);

    void setVersExt(quint32 version);
    quint32 getVersExt(void);
    void setVersBootLoaderExt(quint32 version);
    quint32 getVersBootLoaderExt(void);

    quint32 getVersInt(void);
    quint32 getVersBootLoaderInt(void);

    int on_pbOpenFile_clicked(QString name);

    void setPageTx(qint32 num);

    int checkUpdateHex();           //проверка наличия обновлений

    void f_AdminChange(bool f);

public slots:
    void checkingUpdates(void);     //проверка наличия обновлений и подготовка к обновлению
    QString versionToString(quint32 vers);
    void on_pbWrite_clicked(bool flag);
    qint32 open_Update(void);
    qint32 openBootloaderUpdate(void);
    void on_pbStop_clicked(QString error);
    void write_page(void);

Q_SIGNALS:
    void txPageTimerOn();
    void txPageTimerOff();

private:
    void sendPage(void);

    Tx_commands *_tx_commands = nullptr;
    Crc *_crc = nullptr;
    Commun_display *_commun_display = nullptr;
    Settings *_settings = nullptr;

    f_value version_BootLoader_ExternalProgram;     //версия загрузчика
    f_value version_BootLoader_InternalProgram;     //версия загрузчика из apk
    f_value versionExternalProgram;     //версия шара
    f_value versionInternalProgram;     //версия из apk
    f_value _crc32_Internal;
    bool load_param_ = false;           ///флаг того что загружается
                                        ///false - bootloader
                                        ///true - основная прошивка

    QByteArray _bin;                ///< массив данных
    int _page = 0;                  ///< текущая передаваемая страница страница
    qint32 _pageTx = -1;                ///< номер успешно переданной страницы страница
    int _pages = 0;                 ///< количество страниц для передачи
    int _size = 512;               ///< размер пакета
    int _unsuccessful_transfers = 0;    ///считает кол-во неудачных передач

    bool _f_Admin = false;               ///флаг режима админа
};

#endif // UPDATEHEX_H
