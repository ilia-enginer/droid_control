#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QObject>

#include <qbluetoothlocaldevice.h>
#include <QObject>
#include <QVariant>
#include <QList>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include <QBluetoothAddress>
#include <QBluetoothUuid>
#include <QFileDialog>




union f_value {
    float f;
    quint8 data[4];
    quint16 u16[2];
    quint32 u32;
    int     int32;
};

class Device;


class MainModel: public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(int timer1 READ getTimer1 WRITE setTimer1 NOTIFY timer1Changed)
    Q_PROPERTY(int timer2 READ getTimer2 WRITE setTimer2 NOTIFY timer2Changed)
    Q_PROPERTY(QString currentDeviceName READ getCurrentDeviceName WRITE setCurrentDeviceName NOTIFY onCurrentDeviceNameChanged)
    Q_PROPERTY(float joystickAmplitude READ getJoystickAmplitude WRITE setJoystickAmplitude NOTIFY joystickAmplitudeChanged)
    Q_PROPERTY(float heightAmplitude READ getheightAmplitude WRITE setheightAmplitude NOTIFY heightAmplitudeChanged)
    Q_PROPERTY(float heightAmplitudemin READ getheightAmplitudemin WRITE setheightAmplitudemin NOTIFY heightAmplitudeChangedmin)
    Q_PROPERTY(float Vmin READ getVmin WRITE setVmin NOTIFY VminChanged)
    Q_PROPERTY(float Vmax READ getVmax WRITE setVmax NOTIFY VmaxChanged)
    Q_PROPERTY(float Vreal READ getVreal WRITE setVreal NOTIFY VrealChanged)
    Q_PROPERTY(int adiminTapCount READ getAdminTapCount WRITE incAdminTapCount NOTIFY onAdminTapCountChanged)
    Q_PROPERTY(int coxaAngl READ getcoxaAngl WRITE setcoxaAngl NOTIFY coxaAnglChanged)
    Q_PROPERTY(int femurAngl READ getfemurAngl WRITE setfemurAngl NOTIFY femurAnglChanged)
    Q_PROPERTY(int tibaAngl READ gettibaAngl WRITE settibaAngl NOTIFY tibaAnglChanged)
    Q_PROPERTY(float CurReal READ getCurReal WRITE setCurReal NOTIFY CurRealChanged)
    Q_PROPERTY(QString currenUpdate READ getCurrenUpdate WRITE setCurrenUpdate NOTIFY CurrenUpdateChanged)
    Q_PROPERTY(QString version_app READ getversion_app WRITE setversion_app NOTIFY version_appChanged)
    Q_PROPERTY(int servisIndexMenu READ getservisIndexMenu WRITE setservisIndexMenu NOTIFY servisIndexMenuChanged)
    Q_PROPERTY(bool ctrl_ READ getctrl WRITE setctrl NOTIFY ctrlChanged)


    MainModel();

    const QByteArray getVersionLabel() {return "version\0";} //{0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x00};   // version

    void setTimer1(int value);
    void setTimer2(int value);


    int getTimer1();
    int getTimer2();

    QString getCurrentDeviceName();
    void setCurrentDeviceName(QString name);

    float getJoystickAmplitude() const;
    void setJoystickAmplitude(float newJoystickAmplitude);
    void resetJoystickAmplitude();

    float getheightAmplitude() const;
    void setheightAmplitude(float newHeightAmplitude);
    void resetheightAmplitude();

    float getheightAmplitudemin() const;
    void setheightAmplitudemin(float newHeightAmplitudemin);
    void resetheightAmplitudemin();

    int getservisIndexMenu() const;
    void setservisIndexMenu(int newservisIndexMenu);


    float getVmin() const;
    void setVmin(float newVmin);
    void resetVmin();

    float getVmax() const;
    void setVmax(float newVmax);
    void resetVmax();

    float getVreal() const;
    void setVreal(float newVmax);
    void resetVreal();

    float getCurReal() const;
    void setCurReal(float newVmax);
    void resetCurReal();

    int getcoxaAngl() const;
    void setcoxaAngl(int newcoxaAngl);
    void resetcoxaAngl();

    int getfemurAngl() const;
    void setfemurAngl(int newfemurAngl);
    void resetfemurAngl();

    int gettibaAngl() const;
    void settibaAngl(int newtibaAngl);
    void resettibaAngl();

    bool getctrl() const;
    void setctrl(bool newctrl);

    void incAdminTapCount(int value);
    int  getAdminTapCount();

    QString getCurrenUpdate();
    void setCurrenUpdate(const QString &message);

    void setVersExt(quint32 version);
    quint32 getVersExt(void);
    void setVersBootLoaderExt(quint32 version);
    quint32 getVersBootLoaderExt(void);

    quint32 getVersInt(void);
    quint32 getVersBootLoaderInt(void);


    void setDevice(Device *device);

    int on_pbOpenFile_clicked(QString name);

    void setPageTx(qint32 num);

    void set_rendering_flag(bool fl);

    QString getversion_app();
    void setversion_app(QString &vers);

    QByteArray get_full_param(void);
    void set_full_param(QByteArray &param);

    void full_param_check(void);

public slots:
    void checkingUpdates(void);
    QString versionToString(quint32 vers);
    void on_pbWrite_clicked(bool flag);
    qint32 open_Update(void);
    qint32 openBootloaderUpdate(void);
    void on_pbStop_clicked(QString error);
    void write_page(void);


Q_SIGNALS:
    void timer1Changed(int value);
    void timer2Changed(int value);
    void onCurrentDeviceNameChanged(QString name);

    void joystickAmplitudeChanged();
    void heightAmplitudeChanged();
    void heightAmplitudeChangedmin();

    void servisIndexMenuChanged();


    void VminChanged();
    void VmaxChanged();
    void VrealChanged();

    void CurRealChanged();

    void coxaAnglChanged();
    void femurAnglChanged();
    void tibaAnglChanged();

    void ctrlChanged();


    void onAdminTapCountChanged();


    void CurrenUpdateChanged(QString msg);

    void checkUpdateProgress();     //индикатор загрузки
    void updateAvailable();         //включение кнопки загрузки
    void updateBootloaderAvailable();//кнопка обновления бутлоадера
    void checkUpdate();             //включение кнопки проверки обновления
    void stopUpdate();             //включение кнопки остановки обновления

    void txPageTimerOn();
    void txPageTimerOff();

    void version_appChanged();

private:

    void sendPage(void);

    int timer1 = 100;
    int timer2 = 80;
    float joystickAmplitude = 100;
    float heightAmplitudemin = 20;
    float heightAmplitude = 55;

    int servisIndexMenu = 0;

    float Vmin = 9.0;
    float Vreal = 0.0;
    float Vmax = 12.4;

    float CurReal = 0.0;

    int coxaAngl = 0;
    int femurAngl = 0;
    int tibaAngl = 0;

    bool ctrl_ = false;              //стабилизация шара

    QString currenDeviceName_ = "Отсутствует подключение";

    int adminTapCount = 0;      ///флаг админа
                                /// если -1, то полный доступ ко всем функциям

    QString currenUpdate = "Проверьте обновление";

    f_value version_BootLoader_ExternalProgram;     //версия загрузчика
    f_value version_BootLoader_InternalProgram;     //версия загрузчика из apk
    f_value versionExternalProgram;     //версия шара
    f_value versionInternalProgram;     //версия из apk
    f_value _crc32_Internal;
    bool load_param_ = false;           ///флаг того что загружается
                                        ///false - bootloader
                                        ///true - основная прошивка

    QByteArray _full_Param;         ///массив с полными параметрами

    Device *device_ = nullptr;

    QByteArray _bin;                ///< массив данных
    int _page = 0;                  ///< текущая передаваемая страница страница
    qint32 _pageTx = -1;                ///< номер успешно переданной страницы страница
    int _pages = 0;                 ///< количество страниц для передачи
    int _size = 512;               ///< размер пакета
    int _unsuccessful_transfers = 0;    ///считает кол-во неудачных передач

    bool rendering_flag = false;        //флаг выводы сообщений на экран

    QString version_app = INSERT_VERSION_CODE;
};


#endif // MAINMODEL_H
