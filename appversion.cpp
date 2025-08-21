#include "appversion.h"



AppVersion::AppVersion(QObject *parent) :
    QObject(parent)
{

}

//------------------------------------------------------------------------------

AppVersion::~AppVersion()
{

}


qint32 AppVersion::getAppVersion()
{
    return _appVersion;
}
