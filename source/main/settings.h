#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QWidget>
#include <QSettings>
#include <QVariant>
#include <QList>
#include <QMetaEnum>
#include <QString>
#include <QCoreApplication>
#include "../display_working/commun_display.h"




class Settings : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(int timer1 READ getTimer1 WRITE setTimer1 NOTIFY timer1Changed)
    Q_PROPERTY(float joystickAmplitude READ getJoystickAmplitude WRITE setJoystickAmplitude NOTIFY joystickAmplitudeChanged)
    Q_PROPERTY(float heightAmplitude READ getheightAmplitude WRITE setheightAmplitude NOTIFY heightAmplitudeChanged)
    Q_PROPERTY(float heightAmplitudemin READ getheightAmplitudemin WRITE setheightAmplitudemin NOTIFY heightAmplitudeChangedmin)
    Q_PROPERTY(float Vmin READ getVmin WRITE setVmin NOTIFY VminChanged)
    Q_PROPERTY(float Vmax READ getVmax WRITE setVmax NOTIFY VmaxChanged)
    Q_PROPERTY(int coxaAngl READ getcoxaAngl WRITE setcoxaAngl NOTIFY coxaAnglChanged)
    Q_PROPERTY(int femurAngl READ getfemurAngl WRITE setfemurAngl NOTIFY femurAnglChanged)
    Q_PROPERTY(int tibaAngl READ gettibaAngl WRITE settibaAngl NOTIFY tibaAnglChanged)
    Q_PROPERTY(bool ctrl_ READ getctrl WRITE setctrl NOTIFY ctrlChanged)
    Q_PROPERTY(bool loging READ getLoging WRITE setLoging NOTIFY logingChanged)

    explicit Settings(QObject *parent = nullptr);
    ~Settings(); 

    void setCommun_display(Commun_display *newCommun_display);     

    QByteArray get_full_param(void);
    int set_full_param(QByteArray &param);
    int full_param_check(void);

    void setTimer1(int value);
    int getTimer1();

    float getJoystickAmplitude() const;
    void setJoystickAmplitude(float newJoystickAmplitude);

    float getheightAmplitude() const;
    void setheightAmplitude(float newHeightAmplitude);

    float getheightAmplitudemin() const;
    void setheightAmplitudemin(float newHeightAmplitudemin);

    float getVmin() const;
    void setVmin(float newVmin);

    float getVmax() const;
    void setVmax(float newVmax);

    int getcoxaAngl() const;
    void setcoxaAngl(int newcoxaAngl);

    int getfemurAngl() const;
    void setfemurAngl(int newfemurAngl);

    int gettibaAngl() const;
    void settibaAngl(int newtibaAngl);

    bool getctrl() const;
    void setctrl(bool newctrl);

public slots:
    bool getLoging();
    void setLoging(bool newloging);

Q_SIGNALS:
    void timer1Changed(int value);

    void joystickAmplitudeChanged();
    void heightAmplitudeChanged();
    void heightAmplitudeChangedmin();

    void VminChanged();
    void VmaxChanged();

    void coxaAnglChanged();
    void femurAnglChanged();
    void tibaAnglChanged();

    void ctrlChanged();

    void logingChanged();

private:
    Commun_display * _commun_display = nullptr;

    int timer1 = 100;
    float joystickAmplitude = 100;
    float heightAmplitudemin = 20;
    float heightAmplitude = 55;

    float Vmin = 9.0;
    float Vmax = 12.4;

    int coxaAngl = 0;
    int femurAngl = 0;
    int tibaAngl = 0;

    bool ctrl_ = false;              //стабилизация шара

    bool loging = false;                //флаг логирования

    QByteArray _full_Param;         ///массив с полными параметрами
};

#endif // SETTINGS_H

