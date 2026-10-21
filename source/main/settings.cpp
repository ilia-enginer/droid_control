
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
Settings::setDevice(Device *device)
{
    device_ = device;
}

void
Settings::setPylt_settings(Pylt_settings *newPylt_settings)
{
    _pylt_settings = newPylt_settings;
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

void
Settings::setDevName(QString newName)
{
    _devName = newName;
}

int
Settings::setIdDevice(int id, bool refresh)
{
    if((idDevice == id) && (!refresh))  return 1;

    switch(id){
        case 0 :
            idDevice = NONE;
            emit clearDevice();
            break;

        case 1 :
            idDevice = SHAR;
            emit deviceShar();
            break;
        case 2 :
            idDevice = PYLT;
            emit devicePylt();
            break;

        default:
            idDevice = NONE;
            emit clearDevice();
            emit idDeviceChanged();
            return 0;
            break;
    }
    emit idDeviceChanged();
    return 1;
}

int
Settings::getIdDevice()
{
    return idDevice;
}

/// удаляет устройство из памяти
int
Settings::deleteDevice(QString idDev)
{
    if(idDev.isEmpty())
        return 1;

    // устройство в device.c
    device_->del_device(idDev);

    // настройки в pylt_setting.c
    _pylt_settings->del_settings(idDev);

    // настройки шара в settings.c
    del_paramShar(idDev);

    return 0;
}

/// перемещает устройство в начало списка
int
Settings::upListDevice(QString idDev)
{
    if(idDev.isEmpty())
        return 1;

    device_->upList_device(idDev);

    return 0;
}

/// вычитывание параметров шара из памяти
int
Settings::reading_fullParam_from_memory(QVector<QStringList> &lastParams, int &num)
{
    QSettings parameters;
    while (true)
    {
        if (parameters.contains(QString("SharParametrs%1").arg(num))) {
            lastParams.append(parameters.value(QString("SharParametrs%1").arg(num)).toString().split(";"));
            ++num;    // следующий номер прибора для сохранения в истории
        } else break;
    }
    return 0;
}

/// поиск всех параметров в списке
bool
Settings::searching_fullParam_in_memory(QVector<QStringList> lastDevices, QString id, int &num)
{
    QSettings setting;
    for(QStringList parametrs : lastDevices) {
        if(parametrs.at(0) == id)
        {
            return true;
        }
        num++;
    }
    return false;
}

/// удаляет параметры шара из памяти
int
Settings::del_paramShar(QString id)
{
    if(id.isEmpty())
        return 1;

    QSettings setting;
    QVector<QStringList> lastDevices;
    int deviceNum = 0;
    reading_fullParam_from_memory(lastDevices, deviceNum);
    deviceNum = 0;
    if(searching_fullParam_in_memory(lastDevices, id, deviceNum))
    {
        setting.remove(QString("SharParametrs%1").arg(deviceNum));
        // сдвинуть остальные чтоб не было промежутка
        while(true)
        {
            if (setting.contains(QString("SharParametrs%1").arg(++deviceNum))) {
                setting.setValue(QString("SharParametrs%1").arg(deviceNum-1), setting.value(QString("last%1").arg(deviceNum)));
            }
            else
            {
                if (setting.contains(QString("SharParametrs%1").arg(--deviceNum))) {
                    setting.remove(QString("SharParametrs%1").arg(deviceNum));
                }
                break;
            }
        }
    }
    return 0;
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
    QSettings parameters;
    QString dName;
    QByteArray _full_Param;
    QVector<QStringList> lastParams;

    if(_devName.isEmpty())   return _full_Param;

    int deviceNum = 0;
    reading_fullParam_from_memory(lastParams, deviceNum);

    int num = 0;
    // TODO проверка есть ли устройство в списке сохраненных
    if(searching_fullParam_in_memory(lastParams, _devName, num))
    {
        _full_Param = lastParams[num].at(1).toUtf8();
        return _full_Param;
    }

    return _full_Param;
}

int
Settings::set_full_param(QByteArray &param)
{
    if(param.isEmpty())
        return -1;
    // если имя не записано - выйти
    if(_devName.isEmpty())   return -2;

    QSettings parameters;
    QVector<QStringList> lastParams;
    int deviceNum = 0;
    //вычитываю все устройства
    reading_fullParam_from_memory(lastParams, deviceNum);

    int num = 0;
    // TODO проверка есть ли устройство в списке сохраненных
    searching_fullParam_in_memory(lastParams, _devName, num);

    //сохраняю
    QString paramString = QStringList({_devName,
                                  QString("%1").arg(param),
                                 }).join(";");
    parameters.setValue(QString("SharParametrs%1").arg(deviceNum), paramString);    // запись

    return 1;
}


///проверка сохранееных параметров
int
Settings::full_param_check()
{
    QByteArray _full_Param = get_full_param();

    // если в памяти их нет
    if((_full_Param.isEmpty()) || (_full_Param.size() < 10))
             return 0;

    return 1;
}
