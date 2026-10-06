#ifndef UPDATEHEX_H
#define UPDATEHEX_H

#include <QObject>
#include <QWidget>
#include <QFile>
#include <QTimer>
#include <functional>

#if defined(Q_OS_ANDROID)
    #include <QtCore/QJniObject>
    #include <QtCore/private/qandroidextras_p.h>
#endif

#include "../Shar/communication/tx_commands.h"
#include "../communication/crc.h"
#include "../Shar/display_working/commun_display.h"
#include "../main/settings.h"

class AppManager;



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
    void setAppManager(AppManager *newAppManager);

    void setVersExt(quint32 version);
    quint32 getVersExt(void);
    void setVersBootLoaderExt(quint32 version);
    quint32 getVersBootLoaderExt(void);

    quint32 getVersInt(void);
    quint32 getVersBootLoaderInt(void);

    int on_pbOpenFile_clicked(QString name);

    void setPageTx(qint32 num);

    void checkUpdateHex(const std::function<void(int)> &done);           //проверка наличия обновлений

    void f_AdminChange(bool f);

public slots:
    void checkingUpdates(void);     //проверка наличия обновлений и подготовка к обновлению
    QString versionToString(quint32 vers);
    void on_pbWrite_clicked(bool flag);
    qint32 open_Update(void);
    qint32 openBootloaderUpdate(void);
    QString fileOpen(bool open);
    void on_pbStop_clicked(QString error);
    void write_page(void);

private slots:
    void waitTick();

Q_SIGNALS:
    void navigateBackActionOFF();
    void navigateBackActionON();

private:
    void sendPage(void);

    //асинхронное ожидание ответа прибора (вместо блокирующих delay)
    void waitResponse(int intervalMs, int maxAttempts,
                      const std::function<void()> &send,
                      const std::function<bool()> &ready,
                      const std::function<void()> &onDone,
                      const std::function<void()> &onFail);
    void checkVoltageThenVersion();
    void requestFirmwareVersion();
    void proceedUpdateCheck();
    void finishCheckUpdateHex(const std::function<void(int)> &done);
    void finishTransfer();
    void waitBootloaderVersion();
    void waitProgramVersion();
    QString versionReport();

    Tx_commands *_tx_commands = nullptr;
    Crc *_crc = nullptr;
    Commun_display *_commun_display = nullptr;
    Settings *_settings = nullptr;
    AppManager *_appManager = nullptr;
    QTimer *_timer = nullptr;

    f_value version_BootLoader_ExternalProgram;     //версия загрузчика
    f_value version_BootLoader_InternalProgram;     //версия загрузчика из apk
    f_value versionExternalProgram;     //версия HEX
    f_value versionInternalProgram;     //версия из apk
    f_value _crc32_Internal;
    bool load_param_ = false;           ///флаг того что загружается
                                        ///false - bootloader
                                        ///true - основная прошивка

    QByteArray _bin;                ///< массив данных
    int _page = 0;                  ///< текущая передаваемая страница страница
    qint32 _pageTx = -1;            ///< номер успешно переданной страницы страница
    int _pages = 0;                 ///< количество страниц для передачи
    int _size = 512;               ///< размер пакета
    int _unsuccessful_transfers = 0;    ///считает кол-во неудачных передач

    //состояние асинхронного ожидания ответа
    QTimer *_waitTimer = nullptr;
    int _waitCount = 0;
    int _waitMaxAttempts = 0;
    std::function<bool()> _waitCheck;
    std::function<void()> _waitSend;
    std::function<void()> _waitDone;
    std::function<void()> _waitFail;
    int _opGen = 0;                     //поколение операции: отмена устаревших continuation-ов

    #ifdef Q_OS_WIN
        bool _f_Admin = true;
    #elif defined(Q_OS_MACOS)
        bool _f_Admin = false;
    #elif defined(Q_OS_ANDROID)
        bool _f_Admin = false;
    #elif defined(Q_OS_LINUX)
        bool _f_Admin = true;
    #endif

};

#endif // UPDATEHEX_H
