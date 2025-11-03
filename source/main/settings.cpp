
#include "settings.h"


Settings::Settings(QObject *parent)
    : QObject{parent}
{

}

Settings::~Settings()
{

}

void
Settings::setCommun_display(Commun_display *newCommun_display)
{
    _commun_display = newCommun_display;
}

void
Settings::setTimer1(int value)
{
    timer1 = value;
    emit timer1Changed(value);
}


int
Settings::getTimer1()
{
   return  timer1;
}


float
Settings::getJoystickAmplitude() const
{
    return joystickAmplitude;
}


void
Settings::setJoystickAmplitude(float newJoystickAmplitude)
{
    if (qFuzzyCompare(joystickAmplitude, newJoystickAmplitude))
        return;
    joystickAmplitude = newJoystickAmplitude;
    emit joystickAmplitudeChanged();
}



float
Settings::getheightAmplitude() const
{
    return heightAmplitude;
}

void
Settings::setheightAmplitude(float newheightAmplitude)
{
    if (qFuzzyCompare(heightAmplitude, newheightAmplitude))
        return;
    heightAmplitude = newheightAmplitude;
    emit heightAmplitudeChanged();
}

float
Settings::getheightAmplitudemin() const
{
    return heightAmplitudemin;
}

void
Settings::setheightAmplitudemin(float newheightAmplitudemin)
{
    if (qFuzzyCompare(heightAmplitudemin, newheightAmplitudemin))
        return;
    heightAmplitudemin = newheightAmplitudemin;
    emit heightAmplitudeChangedmin();
}

float
Settings::getVmin() const
{
    return Vmin;
}

void
Settings::setVmin(float newVmin)
{
    if (qFuzzyCompare(Vmin, newVmin))
        return;
    Vmin = newVmin;
    emit VminChanged();
}

float
Settings::getVmax() const
{
    return Vmax;
}

void
Settings::setVmax(float newVmax)
{
    if (qFuzzyCompare(Vmax, newVmax))
        return;
    Vmax = newVmax;
    emit VmaxChanged();
}


int
Settings::getcoxaAngl() const
{
    return coxaAngl;
}

void
Settings::setcoxaAngl(int newcoxaAngl)
{
    coxaAngl = newcoxaAngl;
    emit coxaAnglChanged();
}

int
Settings::getfemurAngl() const
{
    return femurAngl;
}

void
Settings::setfemurAngl(int newfemurAngl)
{
    femurAngl = newfemurAngl;
    emit femurAnglChanged();
}

int
Settings::gettibaAngl() const
{
    return tibaAngl;
}

void
Settings::settibaAngl(int newtibaAngl)
{
    tibaAngl = newtibaAngl;
    emit tibaAnglChanged();
}

bool
Settings::getctrl() const
{
    return ctrl_;
}

void
Settings::setctrl(bool newctrl)
{
    ctrl_ = newctrl;
}

bool
Settings::getLoging()
{
    return loging;
}

void Settings::setLoging(bool newloging)
{
    loging = newloging;
    emit logingChanged();
}


QByteArray
Settings::get_full_param()
{
    //если параметры не прочитаны
    if(!_full_Param.isEmpty())
    {
        QSettings setting;
        _full_Param = setting.value("fullParam", QByteArray()).toByteArray();
    }
    return _full_Param;
}

int
Settings::set_full_param(QByteArray &param)
{
    if(param.isEmpty())
        return -1;

    _full_Param = param;
    QSettings setting;
    setting.setValue("fullParam", _full_Param);
    return 0;
}


/////проверка сохранееных параметров
int
Settings::full_param_check()
{
    QSettings setting;
    _full_Param = setting.value("fullParam", QByteArray()).toByteArray();

    //если параметры не прочитаны
    if((_full_Param.isEmpty()) || (_full_Param.size() < 10))
    {
        //читаю из памяти
         _full_Param = setting.value("fullParam", QByteArray()).toByteArray();

         //если в памяти их нет
         if((_full_Param.isEmpty()) || (_full_Param.size() < 10))
         {
             return 0;
         }
    }
    return 1;
}
