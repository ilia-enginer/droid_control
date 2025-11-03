#include "appversion.h"



AppVersion::AppVersion(QObject *parent) :
    QObject(parent)
{

}

//------------------------------------------------------------------------------

AppVersion::~AppVersion()
{

}


qint32
AppVersion::getAppVersion()
{
    return _appVersion;
}


QString
AppVersion::getversion_app()
{
    return version_app;
}

void
AppVersion::setversion_app(QString &vers)
{
    version_app = vers;
}
