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

#if defined(Q_OS_ANDROID)
    #include <QtCore/private/qandroidextras_p.h>
#endif



class UpdateApp : public QObject
{
        Q_OBJECT

    public:

        Q_PROPERTY(QString updateText READ getUpdateText WRITE setUpdateText NOTIFY onUpdateTextChanged)
        Q_PROPERTY(double totalBytes READ get_TotalBytes WRITE set_TotalBytes NOTIFY TotalBytesChanged)
        Q_PROPERTY(double bytesRead READ get_BytesRead WRITE set_BytesRead NOTIFY BytesReadChanged)

void delayyy( int mill);

        void set_rendering_flag(bool fl);

        static const QString kVersionUrl;
        static const QString kUpdateUrl;
        static const QString versionHeading;
        static const QString fileName;
        static const QString downloadFolderAdress;


        explicit UpdateApp(QObject *parent = 0);
        ~UpdateApp();

        void set_TotalBytes(double byte);
        void set_BytesRead(double byte);

        double get_TotalBytes() const;
        double get_BytesRead() const;



    public slots:
        void checkForUpdates(qint32);
        void on_NetworkReply(QNetworkReply *inReply);
        void on_CancelDownload();
        void on_HttpFinished();
        void on_HttpDataRead();
        void on_UpdateDataReadProgress(qint64 inBytesRead, qint64 inTotalBytes);

        void downloadFile();

Q_SIGNALS:

        void onUpdateTextChanged(QString text);

        void startload();         //включение ползунка загрузки

        void TotalBytesChanged();
        void BytesReadChanged();

        void windowloadOpen();           //открывает окно обновления app

    private:
        bool requestAndroidPermissions(void);
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


        QDir *dir;
        QFileInfo fileinfo;

        qint32 _verAppIn = 0;               //версия уже установленного приложения

        QString updateText_ = "Проверка обновлений...";

        double totalBytes = 0.0;             //кол-во байтов всего загружаемого приложения
        double bytesRead = 0.0;              //сколько уже загружено


        bool rendering_flag = false;        //флаг выводы сообщений на экран
};

#endif // UPDATEAPP_H
