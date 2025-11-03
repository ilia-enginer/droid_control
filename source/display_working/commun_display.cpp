#include "commun_display.h"
#include "QDebug"
#include <QList>
#include <QByteArray>
#include <QMetaEnum>
#include <QCoreApplication>


Commun_display::Commun_display(QObject *parent) :
    QObject(parent)
{

}

Commun_display::~Commun_display()
{

}

int Commun_display::get_rendering_flag()
{
    return rendering_flag;
}

void Commun_display::set_rendering_flag(int flag)
{
    rendering_flag = flag;
}

int
Commun_display::log_out_S(QString type, QString msg)
{
    if(rendering_flag == Qt::ApplicationState::ApplicationActive)
    {
        emit logServis(type, msg);
        return 0;
    }
    return -1;
}

int
Commun_display::log_out_J(QString type, QString msg)
{
    if(rendering_flag == Qt::ApplicationState::ApplicationActive)
    {
        emit logJoy(type, msg);
        return 0;
    }
    return -1;
}

int
Commun_display::vrealChang(float V)
{
    int res = -1;
    if(V != V)
        return -2;
    Volt = V;
    res = vrealUpdate();
    return res;
}

float
Commun_display::getVolt()
{
    return Volt;
}

int
Commun_display::vrealUpdate()
{
    if(rendering_flag == Qt::ApplicationState::ApplicationActive)
    {
        emit vrealChanged(Volt);
        return 0;
    }
    return -1;
}

int
Commun_display::curRealChang(float C)
{  
    int res = -1;
    if(C != C)
        return -2;
    Cur = C;
    res = curRealUpdate();
    return res;
}

float
Commun_display::getCur()
{
    return Cur;
}

int Commun_display::setCurrenUpd(const QString &message)
{
    int res = -1;
    if(message.isEmpty())
        return res;
    currUpd = message;
    res = currenUpd();
    return res;
}

QString
Commun_display::getCurrenUpd()
{
    return currUpd;
}

int
Commun_display::statusUpdate(int status)
{
    int res = -1;
    statusUpd = status;
    res = statusRefresh();
    return res;
}

QString
Commun_display::getUpdate()
{
    return messagee;
}

int
Commun_display::setUpdatee(const QString &message)
{
    if(message.isEmpty())   return -1;
    messagee = message;
    updateRefresh();
    return 0;
}

int
Commun_display::statusDevicee(int status)
{
    int res = -1;
    statusDevic = status;
    res = statusDeviceRefresh();
    return res;
}

int Commun_display::setCurDeviceName(QString name)
{
    curDeviceName_ = name;
    return CurDeviceNameRefresh();;
}

QString Commun_display::getCurDeviceName()
{
    return curDeviceName_;
}

int
Commun_display::windloadHexOpen()
{
    if(rendering_flag == Qt::ApplicationState::ApplicationActive)
    {
        emit windowloadHexOpen();
        return 0;
    }
    return -1;
}

void
Commun_display::set_connected(bool connect)
{
    connected = connect;
}

bool
Commun_display::get_connected()
{
    return connected;
}

void
Commun_display::graphsOutput(float volt, float cur, float tilt_angle, float tilt_direction, float boost, float angular_velocity, float angleX, float angleY, float angleZ)
{
    emit chart_data(volt,
                    cur, tilt_angle,
                    tilt_direction, boost,
                    angular_velocity, angleX,
                    angleY, angleZ);
}


int
Commun_display::allUpdate()
{
    vrealUpdate();      //напряжение
    curRealUpdate();    //ток
    currenUpd();        //строка на странице обновления hex
    statusRefresh();    //обновить состояние загрузочной страницы
    updateRefresh();    //состояние поисковой страницы
    statusDeviceRefresh();//обновить состояние поисковой страницы
    CurDeviceNameRefresh();//обновление имени подключенного устройства
    return 0;
}

int
Commun_display::curRealUpdate()
{
    if(rendering_flag == Qt::ApplicationState::ApplicationActive)
    {
        emit curRealChanged(Cur);
        return 0;
    }
    return -1;
}

int
Commun_display::currenUpd()
{
    if(rendering_flag == Qt::ApplicationState::ApplicationActive)
    {
        emit CurrenUpdateChanged(currUpd);
        return 0;
    }
    return -1;
}

int
Commun_display::statusRefresh()
{
    if(rendering_flag == Qt::ApplicationState::ApplicationActive)
    {
        switch(statusUpd)
        {
            case statusUpd::checkUpd:
                emit checkUpdate();             //включение кнопки проверки обновления
                break;
            case statusUpd::checkUpdateProgr:
                emit checkUpdateProgress();     //индикатор загрузки
                break;
            case statusUpd::stopUpd:
                emit stopUpdate();             //включение кнопки остановки обновления
                break;

            case statusUpd::updateAvailab:
                emit updateAvailable();         //включение кнопки загрузки
                break;

            case statusUpd::updateBootloaderAvailab:
                emit updateBootloaderAvailable();//кнопка обновления бутлоадера
                break;

            default:
                break;
        }
        return 0;
    }
    return -1;
}

int
Commun_display::updateRefresh()
{
    if(rendering_flag == Qt::ApplicationState::ApplicationActive)
    {
        if(messagee.isEmpty())   return -2;
        emit updateChanged(messagee);
        return 0;
    }
    return -1;
}

int
Commun_display::statusDeviceRefresh()
{
    if(rendering_flag == Qt::ApplicationState::ApplicationActive)
    {
        switch(statusDevic)
        {
            case statusDevic::disconnect:
                emit disconnected();
                break;
            case statusDevic::searchInProgr:
                emit searchInProgress();
                break;
            case statusDevic::searchFinish:
                emit searchFinished();
                break;
            default:
                break;
        }
        return 0;
    }
    return -1;
}

int Commun_display::CurDeviceNameRefresh()
{
    if(rendering_flag == Qt::ApplicationState::ApplicationActive)
    {
        emit onCurDeviceNameChanged(curDeviceName_);
        return 0;
    }
    return -1;
}


int
Commun_display::log_out_T(QString type, QString msg)
{
    if(rendering_flag == Qt::ApplicationState::ApplicationActive)
    {
        emit logT(type, msg);
        return 0;
    }
    return -1;
}
