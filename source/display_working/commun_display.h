#ifndef COMMUN_DISPLAY_H
#define COMMUN_DISPLAY_H

#include <QObject>
#include <QWidget>
#include <QString>


class Commun_display: public QObject
{
    Q_OBJECT

public:
    explicit Commun_display(QObject *parent = 0);
    ~Commun_display();

    enum statusUpd {
        checkUpdateProgr,        //индикатор загрузки
        updateAvailab,            //включение кнопки загрузки
        updateBootloaderAvailab,  //кнопка обновления бутлоадера
        checkUpd,                //включение кнопки проверки обновления
        stopUpd                 //включение кнопки остановки обновления
    };

    enum statusDevic {
        disconnect,
        searchInProgr,
        searchFinish
    };

    enum updApp {
        null,
        startloadStat,          //включение ползунка загрузки
        statusLoadOFFStat,      //отключение ползунка загрузки
        busyIndicatorONStat,    //включает крутилку загрузки
        busyIndicatorOFFStat,   //отключает крутилку загрузки
        windowloadOpenStat,     //открывает окно обновления app
        but_Ok_OnStat           //включение кнопки повторной установки
    };

    Q_PROPERTY(QString currUpd READ getCurrenUpd WRITE setCurrenUpd NOTIFY CurrenUpdateChanged)
    Q_PROPERTY(QString update READ getUpdate WRITE setUpdatee NOTIFY updateChanged)
    Q_PROPERTY(QString curDeviceName_ READ getCurDeviceName WRITE setCurDeviceName NOTIFY onCurDeviceNameChanged)
    Q_PROPERTY(float Volt READ getVolt WRITE vrealChang NOTIFY vrealChanged)
    Q_PROPERTY(float Cur READ getCur WRITE curRealChang NOTIFY curRealChanged)
    Q_PROPERTY(QString updateAppText READ getUpdateAppText WRITE setUpdateAppText NOTIFY UpdateAppTextChanged)
    Q_PROPERTY(QString loadTextApp READ getLoadTextApp WRITE setLoadTextApp NOTIFY onLoadTextAppChanged)
    Q_PROPERTY(double TotalBytes READ get_TotalBytes WRITE set_TotalBytes NOTIFY totalBytesChanged)
    Q_PROPERTY(double BytesRead READ get_BytesRead WRITE set_BytesRead NOTIFY bytesReadChanged)

    int get_rendering_flag();
    void set_rendering_flag(int flag);
    int allUpdate();

    int log_out_T(QString type, QString msg);     //вывод в терминал
    int log_out_S(QString type, QString msg);     //вывод в сервисное окно
    int log_out_J(QString type, QString msg);     //вывод в окно джойстика

    int vrealChang(float V); 
    float getVolt();
    int curRealChang(float C);
    float getCur();

    int setCurrenUpd(const QString &message);
    QString getCurrenUpd();
    int statusUpdate(int status);

    QString getUpdate();
    int setUpdatee(const QString &message);
    int statusDevicee(int status);

    int setCurDeviceName(QString name);
    QString getCurDeviceName();

    int windloadHexOpen();

    void set_connected(bool connect);
    bool get_connected(void);

    void graphsOutput(float volt, float cur, float tilt_angle,
                      float tilt_direction, float boost,
                      float angular_velocity, float angleX,
                      float angleY, float angleZ);


    int statusUpdateApp(int status);

    int setUpdateAppText(const QString &message);
    QString getUpdateAppText();

    QString getLoadTextApp();
    int setLoadTextApp(QString text);

    void set_TotalBytes(double byte);
    void set_BytesRead(double byte);
    double get_TotalBytes() const;
    double get_BytesRead() const;

Q_SIGNALS:
    void logT(QString type, QString msg);
    void logServis(QString type, QString msg);
    void logJoy(QString type, QString msg);

    void vrealChanged(float V);
    void curRealChanged(float Cur);
    void CurrenUpdateChanged(QString msg);

    void checkUpdateProgress();     //индикатор загрузки
    void updateAvailable();         //включение кнопки загрузки
    void updateBootloaderAvailable();//кнопка обновления бутлоадера
    void checkUpdate();             //включение кнопки проверки обновления
    void stopUpdate();             //включение кнопки остановки обновления

    void updateChanged(QString msg);

    void onCurDeviceNameChanged(QString name);

    void disconnected();
    void searchInProgress();
    void searchFinished();

    void onWindowloadHexOpen();       //открывает окно-напоминание обновления hex шара

    void chart_data(float volt_, float cur_,
                    float tilt_angle_, float tilt_direction_,
                    float boost_, float angular_velocity_,
                    float angleX_, float angleY_,
                    float angleZ_);              //вывод графиков

    void UpdateAppTextChanged(QString msg);
    void onLoadTextAppChanged(QString text);

    void startload();         //включение ползунка загрузки
    void statusLoadOFF();     //отключение ползунка загрузки
    void busyIndicatorON();     //включает крутилку загрузки
    void busyIndicatorOFF();    //отключает крутилку загрузки
    void windowloadOpen();           //открывает окно обновления app
    void but_Ok_On();            //включение кнопки повторной установки

    void totalBytesChanged();
    void bytesReadChanged();

private:
    int vrealUpdate();
    int curRealUpdate();
    int currenUpd();
    int statusRefresh();
    int updateRefresh();
    int statusDeviceRefresh();
    int CurDeviceNameRefresh();
    int updateAppTextRefresh();
    int LoadTextAppRefresh();
    int statusUpdateAppRefresh();
    int TotalBytesRefresh();
    int BytesReadRefresh();

    int rendering_flag;
    bool connected = false;

    float Volt = 0.0;
    float Cur = 0.0;

    double TotalBytes = 0.0;             //кол-во байтов всего загружаемого приложения
    double BytesRead = 0.0;              //сколько уже загружено

    QString currUpd = "Проверьте обновление";
    QString messagee = " ";
    QString curDeviceName_ = "Устройство отключено";

    QString updateAppText_ = "Проверка обновлений...";
    QString loadTextApp_ = "";

    int statusUpd = statusUpd::checkUpd;
    int statusDevic = statusDevic::searchFinish;
    int statusUpdApp = updApp::null;
};

#endif // COMMUN_DISPLAY_H


