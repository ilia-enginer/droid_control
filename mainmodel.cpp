#include "mainmodel.h"

MainModel::MainModel()
{

}

void
MainModel::setTimer1(int value)
{
    timer1 = value;

    emit timer1Changed(value);

    if (timer1 < timer2){
        timer2 = timer1;

        emit timer2Changed(timer2);
    }
}


void
MainModel::setTimer2(int value)
{
    timer2 = value;

    emit timer2Changed(value);

    if (timer2 > timer1){
        timer1 = timer2;

        emit timer1Changed(timer1);
    }
}


int
MainModel::getTimer1()
{
   return  timer1;
}


int
MainModel::getTimer2()
{
    return timer2;
}


QString
MainModel::getCurrentDeviceName()
{
    return currenDeviceName_;
}


void
MainModel::setCurrentDeviceName(QString name)
{
    currenDeviceName_ = name;

    emit onCurrentDeviceNameChanged(name);
}

float MainModel::getJoystickAmplitude() const
{
    return joystickAmplitude;
}

void MainModel::setJoystickAmplitude(float newJoystickAmplitude)
{
    if (qFuzzyCompare(joystickAmplitude, newJoystickAmplitude))
        return;
    joystickAmplitude = newJoystickAmplitude;
    emit joystickAmplitudeChanged();
}



