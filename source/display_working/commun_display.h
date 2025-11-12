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

    Q_PROPERTY(QString currUpd READ getCurrenUpd WRITE setCurrenUpd NOTIFY CurrenUpdateChanged)
    Q_PROPERTY(QString update READ getUpdate WRITE setUpdatee NOTIFY updateChanged)
    Q_PROPERTY(QString curDeviceName_ READ getCurDeviceName WRITE setCurDeviceName NOTIFY onCurDeviceNameChanged)
    Q_PROPERTY(float Volt READ getVolt WRITE vrealChang NOTIFY vrealChanged)
    Q_PROPERTY(float Cur READ getCur WRITE curRealChang NOTIFY curRealChanged)

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

private:
    int vrealUpdate();
    int curRealUpdate();
    int currenUpd();
    int statusRefresh();
    int updateRefresh();
    int statusDeviceRefresh();
    int CurDeviceNameRefresh();

    int rendering_flag;
    bool connected = false;

    float Volt = 0.0;
    float Cur = 0.0;

    QString currUpd = "Проверьте обновление";
    QString messagee = " ";
    QString curDeviceName_ = "Устройство отключено";

    int statusUpd = statusUpd::checkUpd;
    int statusDevic = statusDevic::searchFinish;
};

#endif // COMMUN_DISPLAY_H


