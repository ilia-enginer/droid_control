#ifndef UPDATEAPP_H
#define UPDATEAPP_H

#include <QObject>
#include <QVariant>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMetaEnum>
#include <QtWidgets>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QJniObject>
#include "appversion.h"
#include "../main/appmanager.h"


#if defined(Q_OS_ANDROID)
    #include <QtCore/private/qandroidextras_p.h>
#endif



class UpdateApp : public QObject
{
        Q_OBJECT

    public:
        Q_PROPERTY(QString updateText READ getUpdateText WRITE setUpdateText NOTIFY onUpdateTextChanged)
        Q_PROPERTY(double TotalBytes READ get_TotalBytes WRITE set_TotalBytes NOTIFY totalBytesChanged)
        Q_PROPERTY(double BytesRead READ get_BytesRead WRITE set_BytesRead NOTIFY bytesReadChanged)


void delayyy( int mill);

        static const QString kVersionUrl;
        static const QString kUpdateUrl;
        static const QString versionHeading;
        static const QString fileName;
        static const QString downloadFolderAdress;


        explicit UpdateApp(QObject *parent = 0);
        ~UpdateApp();

        void setAppManager(AppManager *newAppManager);

        void set_TotalBytes(double byte);
        void set_BytesRead(double byte);

        double get_TotalBytes() const;
        double get_BytesRead() const;


    public slots:
        void checkForUpdates(void);
        void on_NetworkReply(QNetworkReply *inReply);
        void on_CancelDownload();
        void on_HttpFinished();
        void on_HttpDataRead();
        void on_UpdateDataReadProgress(qint64 inBytesRead, qint64 inTotalBytes);

        void downloadFile();
        void install();

Q_SIGNALS:

        void onUpdateTextChanged(QString text);

        void startload();         //включение ползунка загрузки
        void statusLoadOFF();     //отключение ползунка загрузки

        void busyIndicatorON();     //включает крутилку загрузки
        void busyIndicatorOFF();    //отключает крутилку загрузки

        void totalBytesChanged();
        void bytesReadChanged();

        void windowloadOpen();           //открывает окно обновления app

        void but_Ok_On();            //включение кнопки повторной установки

    private:
        bool requestAndroidPermissions(void);
        QString getUpdateText();
        void setUpdateText(QString text);

        void checkVersion(QString inVersion);
        void startRequest(QUrl inUrl);

        AppManager * _appManager = nullptr;

        QProgressDialog *mProgressDialog;
        QNetworkAccessManager *mNamChecker;
        QNetworkAccessManager *mNamDownloader;
        QNetworkReply *mDownloaderReply;
        QFile *mFile;
        bool mHttpRequestAborted = true;


        QDir *dir;
        QFileInfo fileinfo;

        qint32 _verAppIn;               //версия уже установленного приложения

        QString updateText_ = "Проверка обновлений...";

        double TotalBytes = 0.0;             //кол-во байтов всего загружаемого приложения
        double BytesRead = 0.0;              //сколько уже загружено
};

#endif // UPDATEAPP_H
