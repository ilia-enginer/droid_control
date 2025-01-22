#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <QObject>

class MainModel: public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(int timer1 READ getTimer1 WRITE setTimer1 NOTIFY timer1Changed)
    Q_PROPERTY(int timer2 READ getTimer2 WRITE setTimer2 NOTIFY timer2Changed)
    Q_PROPERTY(QString currentDeviceName READ getCurrentDeviceName WRITE setCurrentDeviceName NOTIFY onCurrentDeviceNameChanged)
    Q_PROPERTY(float joystickAmplitude READ getJoystickAmplitude WRITE setJoystickAmplitude NOTIFY joystickAmplitudeChanged)


    MainModel();

    void setTimer1(int value);
    void setTimer2(int value);


    int getTimer1();
    int getTimer2();

    QString getCurrentDeviceName();
    void setCurrentDeviceName(QString name);



    float getJoystickAmplitude() const;
    void setJoystickAmplitude(float newJoystickAmplitude);
    void resetJoystickAmplitude();

Q_SIGNALS:
    void timer1Changed(int value);
    void timer2Changed(int value);
    void onCurrentDeviceNameChanged(QString name);

    void joystickAmplitudeChanged();

private:
    int timer1 = 100;
    int timer2 = 80;
    float joystickAmplitude = 10;

    QString currenDeviceName_ = "Отсутствует подключение";

};

#endif // MAINMODEL_H
