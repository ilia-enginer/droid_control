#ifndef INFO_H
#define INFO_H

#include <QObject>
#include <QWidget>

class Info: public QObject
{
    Q_OBJECT

public:
    explicit Info(QObject *parent = 0);
    ~Info();

    Q_PROPERTY(QString info_str_ READ getInfo_str WRITE setInfo_str NOTIFY onInfo_strChanged)
    Q_PROPERTY(int infoIndexMenu_ READ getInfoIndexMenu WRITE setInfoIndexMenu NOTIFY infoIndexMenuChanged)

    void setInfo_str(QString name);
    QString getInfo_str();

    int getInfoIndexMenu() const;
    void setInfoIndexMenu(int newInfoIndexMenu);

public slots:
    void clear();
    void enabling();
    void gettingStarted();
    void operatingModes();
    void setupAndCalibration();
    void updatingHex();
    void shutdown();
    void possibleErrors();
    void charging();


Q_SIGNALS:
    void onInfo_strChanged(QString name);
    void infoIndexMenuChanged();

private:
    QString info_str_ = " ";
    int infoIndexMenu_ = 0;

};

#endif // INFO_H


