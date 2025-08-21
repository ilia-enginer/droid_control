#ifndef UPDATEAPP_H
#define UPDATEAPP_H

/*#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QFile>
#include <QFileInfo>
#include <QDir>

//#include <QtNetwork>
//#include <QNetworkReply>
*/

#include <QtWidgets>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
//#include <QtAndroid>



class UpdateApp : public QObject
{
        Q_OBJECT

    public:
        static const QString kVersionUrl;
        static const QString kUpdateUrl;
        static const QString versionHeading;


        explicit UpdateApp(QObject *parent = 0);
        ~UpdateApp();

        void checkForUpdates(qint32);

    public slots:
        void on_NetworkReply(QNetworkReply *inReply);
        void on_CancelDownload();
        void on_HttpFinished();
        void on_HttpDataRead();
        void on_UpdateDataReadProgress(qint64 inBytesRead, qint64 inTotalBytes);

    private:
        void checkVersion(QString inVersion);
        void startRequest(QUrl inUrl);
        void downloadFile();

        QProgressDialog *mProgressDialog;
        QNetworkAccessManager *mNamChecker;
        QNetworkAccessManager *mNamDownloader;
        QNetworkReply *mDownloaderReply;
        QFile *mFile;
        bool mHttpRequestAborted;

        qint32 _verAppIn = 0;               //версия уже установленного приложения

};

/*
class UpdateApp : public QObject

{
    Q_OBJECT
public:

    explicit UpdateApp(QObject *parent = nullptr);
    Q_INVOKABLE void httpDownload();

signals:
    void ProgressPosition (double PRE); // Update the progress bar signal
    void downloadfinished ();           // end the signal

protected slots:
    void replyFinished(QNetworkReply*reply);
    void onDownloadProgress(qint64 bytesSent, qint64 bytesTotal);
    void onReadyRead();

private:
    QNetworkAccessManager * accessManager = nullptr;
    QNetworkRequest request;
    QNetworkReply * reply = nullptr;
    QString url = "Fill your download address here";
    QString fileName = "506787841apk.apk";
    QFileInfo fileinfo;
    QDir *dir;
    QFile *file;

};
*/
#endif // UPDATEAPP_H
