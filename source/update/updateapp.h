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
#include "../display_working/commun_display.h"


#if defined(Q_OS_ANDROID)
    #include <QtCore/private/qandroidextras_p.h>
#endif



class UpdateApp : public QObject
{
        Q_OBJECT

    public:
        static const QString kVersionUrl;
        static const QString kUpdateUrl;
        static const QString versionHeading;
        static const QString fileName;
        static const QString downloadFolderAdress;


        explicit UpdateApp(QObject *parent = 0);
        ~UpdateApp();

        void setAppManager(AppManager *newAppManager);
        void setCommun_display(Commun_display *newCommun_display);

        void set_TotalBytes(double byte);
        void set_BytesRead(double byte);

    public slots:
        void checkForUpdates(void);
        void on_NetworkReply(QNetworkReply *inReply);
        void on_CancelDownload();
        void on_HttpFinished();
        void on_HttpDataRead();
        void on_UpdateDataReadProgress(qint64 inBytesRead, qint64 inTotalBytes);

        void downloadFile();
        void install();

    private:
        void delayyy( int mill);
        void setLoadText(QString text);

        void setUpdateText(QString text);

        void checkVersion(QString inVersion);
        void startRequest(QUrl inUrl);

        AppManager * _appManager = nullptr;
        Commun_display *_commun_display = nullptr;

        QProgressDialog *mProgressDialog;
        QNetworkAccessManager *mNamChecker;
        QNetworkAccessManager *mNamDownloader;
        QNetworkReply *mDownloaderReply;
        QFile *mFile;
        bool mHttpRequestAborted = true;


        QDir *dir;
        QFileInfo fileinfo;

        qint32 _verAppIn;               //версия уже установленного приложения

};

#endif // UPDATEAPP_H
