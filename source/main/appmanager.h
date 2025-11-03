#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>
#include "../display_working/commun_display.h"


class AppManager : public QObject
{
    Q_OBJECT
public:

    explicit AppManager(QObject *parent = nullptr);

    void setCommun_display(Commun_display *newCommun_display);

    void keepScreenOn(bool on);
    
    Q_INVOKABLE int installApk(const QString& absPath);
signals:

private:
    Commun_display * _commun_display = nullptr;


public slots:
    void onApplicationStateChanged(Qt::ApplicationState state);

};



#endif // APPMANAGER_H
