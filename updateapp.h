#ifndef UPDATEAPP_H
#define UPDATEAPP_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QFile>
#include <QFileInfo>
#include <QDir>

//#include <QtNetwork>
//#include <QNetworkReply>


#include <QtWidgets>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#if defined(Q_OS_MACOS)
    #include <QtCore/private/qandroidextras_p.h>
#endif



class UpdateApp : public QObject
{
        Q_OBJECT

    public:

        Q_PROPERTY(QString updateText READ getUpdateText WRITE setUpdateText NOTIFY onUpdateTextChanged)
        Q_PROPERTY(qint64 totalBytes_ READ get_TotalBytes WRITE set_TotalBytes NOTIFY TotalBytesChanged)
        Q_PROPERTY(qint64 bytesRead_ READ get_BytesRead WRITE set_BytesRead NOTIFY BytesReadChanged)

void delayyy( int mill);
        void set_rendering_flag(bool fl);

        static const QString kVersionUrl;
        static const QString kUpdateUrl;
        static const QString versionHeading;


        explicit UpdateApp(QObject *parent = 0);
        ~UpdateApp();



    public slots:
        void checkForUpdates(qint32);
        void on_NetworkReply(QNetworkReply *inReply);
        void on_CancelDownload();
        void on_HttpFinished();
        void on_HttpDataRead();
        void on_UpdateDataReadProgress(qint64 inBytesRead, qint64 inTotalBytes);

        void downloadFile();

        void set_TotalBytes(qint64 byte);
        void set_BytesRead(qint64 byte);

        qint64 get_TotalBytes() const;
        qint64 get_BytesRead() const;


Q_SIGNALS:

        void onUpdateTextChanged(QString text);

        void startload();         //включение ползунка загрузки

        void TotalBytesChanged();
        void BytesReadChanged();

        void windowloadOpen();           //открывает окно обновления app

    private:
        QString getUpdateText();
        void setUpdateText(QString text);

        void checkVersion(QString inVersion);
        void startRequest(QUrl inUrl);


        QProgressDialog *mProgressDialog;
        QNetworkAccessManager *mNamChecker;
        QNetworkAccessManager *mNamDownloader;
        QNetworkReply *mDownloaderReply;
        QFile *mFile;
        bool mHttpRequestAborted = true;

        qint32 _verAppIn = 0;               //версия уже установленного приложения

        QString updateText_ = "Проверка обновлений...";

        qint64 totalBytes_ = 0;             //кол-во байтов всего загружаемого приложения
        qint64 bytesRead_ = 0;              //сколько уже загружено


        bool rendering_flag = false;        //флаг выводы сообщений на экран
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
