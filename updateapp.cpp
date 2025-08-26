#include "updateapp.h"



//------------------------------------------------------------------------------

const QString UpdateApp::kVersionUrl =
    "https://github.com/ilia-enginer/droid_stick/blob/main/version.txt";

const QString UpdateApp::kUpdateUrl =
    "https://github.com/ilia-enginer/droid_stick/blob/main/droid_stick.apk";

const QString UpdateApp::versionHeading =
    "4294967295";                           //0xFFFFFFFF


//------------------------------------------------------------------------------

///???
 void  UpdateApp::delayyy( int mill)
{
    QTime dieTime = QTime::currentTime().addMSecs( mill );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

QString UpdateApp::getUpdateText()
{
    return updateText_;
}

void UpdateApp::setUpdateText(QString text)
{
    updateText_ = text;

    if(rendering_flag)    emit onUpdateTextChanged(text);
}

void UpdateApp::set_rendering_flag(bool fl)
{
    rendering_flag = fl;
}


UpdateApp::UpdateApp(QObject *parent) :
    QObject(parent)
{
//    QStringList permission;
//    permission.append("android.permission.WRITE_EXTERNAL_STORAGE");
//    permission.append("android.permission.INSTALL_PACKAGES");
//    permission.append("android.permission.REQUEST_INSTALL_PACKAGES");
//    QtAndroid::requestPermissionsSync(permission);

//    auto r = QtAndroidPrivate::checkPermission(QtAndroidPrivate::Storage).result();
//    if (r == QtAndroidPrivate::Denied)
//    {
//        r = QtAndroidPrivate::requestPermission(QtAndroidPrivate::Storage).result();

//    }

    mNamChecker = new QNetworkAccessManager(this);

    connect(
        mNamChecker,
        SIGNAL(finished(QNetworkReply *)),
        this,
        SLOT(on_NetworkReply(QNetworkReply *))
    );
}

//------------------------------------------------------------------------------

UpdateApp::~UpdateApp()
{
    delete mNamChecker;
}

//------------------------------------------------------------------------------

void UpdateApp::checkForUpdates(qint32 ver)
{
#if defined(Q_OS_MACOS)

    QFuture permission_request = QtAndroidPrivate::requestPermission("android.permission.INTERNET");
#endif
//    switch (permission_request.result()) {
//    case QtAndroidPrivate::Undetermined:
//        setUpdateText("01 Undetermined");

//        break;
//    case QtAndroidPrivate::Authorized:
//        setUpdateText("02 Authorized");

//        break;
//    case QtAndroidPrivate::Denied:
//        setUpdateText("03 Denied");

//        break;
//    default:
//        break;
//    }
//delayyy(1000);

    if(ver != 0)    _verAppIn = ver;
setUpdateText("1 to checkForUpdates");
delayyy(1000);

    QUrl tUrl(kVersionUrl);

    mNamChecker->get(QNetworkRequest(tUrl));

 setUpdateText("2 to mNamChecker->get");
 delayyy(1500);
}

//------------------------------------------------------------------------------

void UpdateApp::checkVersion(QString inVersion)
{
//    qDebug() << inVersion.toInt();
//    qDebug() << inVersion;

    if ((inVersion.toInt() != 0) && (_verAppIn != 0))
    {
      ////???  if (inVersion.toInt() > _verAppIn)
        if (inVersion.toInt() > 3)
        {
            if(rendering_flag)
            {
                ///?????
        //        emit windowloadOpen();
            }

            setUpdateText("Доступно обновление программы\n Обновить сейчас?");
        }
    }
}

//------------------------------------------------------------------------------

void UpdateApp::downloadFile()
{
    QFileInfo tServerFileInfo(kUpdateUrl);
    QString tServerFileName = tServerFileInfo.fileName();

    if (tServerFileName.isEmpty())
    {
        setUpdateText(tr(
                        "Ошибка!\nФайл %1 отсутствует."
                    ).arg(tServerFileName));
        return;
    }

    QString tLocalFileName =
        QDir::toNativeSeparators(
            QCoreApplication::applicationDirPath() + "/" + tServerFileName);

    if (QFile::exists(tLocalFileName))
    {
        QFile::remove(tLocalFileName);
    }

    mFile = new QFile(tLocalFileName);
    if (!mFile->open(QIODevice::WriteOnly))
    {
        setUpdateText(
                    tr(
                        "Ошибка!\nНевозможно сохранить файл %1: %2."
                    ).arg(tLocalFileName).arg(mFile->errorString())
                );

        delete mFile;
        return;
    }


    emit startload();         //включение ползунка загрузки

    setUpdateText(tr("Загрузка %1.").arg(tServerFileName));

    mHttpRequestAborted = false;
    startRequest(kUpdateUrl);
}

void UpdateApp::set_TotalBytes(qint64 byte)
{
    totalBytes_ = byte;
}

void UpdateApp::set_BytesRead(qint64 byte)
{
    bytesRead_ = byte;
}

qint64 UpdateApp::get_TotalBytes() const
{
    return totalBytes_;
}

qint64 UpdateApp::get_BytesRead() const
{
    return bytesRead_;
}

//------------------------------------------------------------------------------

void UpdateApp::startRequest(QUrl inUrl)
{
    mNamDownloader = new QNetworkAccessManager(this);

    mDownloaderReply = mNamDownloader->get(QNetworkRequest(inUrl));

    connect(mDownloaderReply, SIGNAL(finished()),
            this, SLOT(on_HttpFinished()));
    connect(mDownloaderReply, SIGNAL(readyRead()),
            this, SLOT(on_HttpDataRead()));
    connect(mDownloaderReply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(on_UpdateDataReadProgress(qint64, qint64)));
}

//------------------------------------------------------------------------------

void UpdateApp::on_CancelDownload()
{
    qDebug() << "on_CancelDownload";

    if(!mHttpRequestAborted)
    {
        mDownloaderReply->abort();
    }
    mHttpRequestAborted = true;
}

//------------------------------------------------------------------------------

void UpdateApp::on_HttpFinished()
{
    qDebug() << "on_HttpFinished";

    if (mHttpRequestAborted)
    {
        if (mFile)
        {
            mFile->close();
            mFile->remove();

            delete mFile;
        }

        mDownloaderReply->deleteLater();
        set_BytesRead(get_TotalBytes());

        return;
    }

    set_BytesRead(get_TotalBytes());
    mFile->flush();
    mFile->close();

    QVariant tRedirectionTarget =
        mDownloaderReply->attribute(
            QNetworkRequest::RedirectionTargetAttribute);

    if (mDownloaderReply->error())
    {
        mFile->remove();

        setUpdateText(
                    tr(
                        "Ошибка!\nОшибка загрузки: %1."
                    ).arg(mDownloaderReply->errorString()));
    }
    else if (!tRedirectionTarget.isNull())
    {
        QUrl tUrl = QUrl(kUpdateUrl);
        QUrl tNewUrl = tUrl.resolved(tRedirectionTarget.toUrl());
        tUrl = tNewUrl;

        mFile->open(QIODevice::WriteOnly);
        mFile->resize(0);

        startRequest(tUrl);

        return;
    }
    else
    {
        QString tLocalFileName =
            QDir::toNativeSeparators(
                QCoreApplication::applicationDirPath() + "/" + QFileInfo(
                    QUrl(kUpdateUrl).path()).fileName());

        QDesktopServices::openUrl(QUrl::fromLocalFile(tLocalFileName));

        QApplication::quit();
    }

    mDownloaderReply->deleteLater();

    delete mFile;
}

//------------------------------------------------------------------------------

void UpdateApp::on_HttpDataRead()
{
    if (mFile)
    {
        mFile->write(mDownloaderReply->readAll());
    }
}

//------------------------------------------------------------------------------

void UpdateApp::on_UpdateDataReadProgress(
    qint64 inBytesRead,
    qint64 inTotalBytes)
{
    if (mHttpRequestAborted)
    {
        return;
    }

    set_TotalBytes(inTotalBytes);
    set_BytesRead(inBytesRead);
}

//------------------------------------------------------------------------------

void UpdateApp::on_NetworkReply(QNetworkReply *inReply)
{
    setUpdateText("3 to on_NetworkReply");
    delayyy(1000);
    if (inReply->error() == QNetworkReply::NoError)
    {
        int tHttpStatusCode =
            inReply->attribute(
                QNetworkRequest::HttpStatusCodeAttribute).toUInt();

        setUpdateText("4 inReply->error() == QNe");
        delayyy(1000);
        if (tHttpStatusCode >= 200 && tHttpStatusCode < 300)
        {
            setUpdateText("5 tHttpStatusCode >= 200 && tHtt");
            delayyy(1000);
            if (inReply->isReadable())
            {
                setUpdateText("6 inReply->isReadable(");
                delayyy(1000);
                //читаю построчно
                //ищу в каждой строке место где записана версия
                //с помощью "лейбла"
                QString tReplyString;
                QString temp;
                while(!inReply->atEnd()) {
                    temp = inReply->readLine();
                    if (temp.contains(UpdateApp::versionHeading)) {
                        tReplyString = temp.mid(temp.indexOf(UpdateApp::versionHeading) + UpdateApp::versionHeading.size() + 1, 9);

                 //       qDebug() << temp;
                    }
                }
                checkVersion(tReplyString);
            }
        }
        else if (tHttpStatusCode >= 300 && tHttpStatusCode < 400)
        {
            setUpdateText("7 tHttpStatusCode >= 300 && tHttpStatusCode");
            delayyy(1000);
            // Получите URL-адрес для перенаправления
            QUrl tNewUrl =
                inReply->attribute(
                    QNetworkRequest::RedirectionTargetAttribute).toUrl();
            // Потому что URL-адрес перенаправления может быть относительным,
            // мы должны использовать предыдущий вариант, чтобы решить эту проблему
            tNewUrl = inReply->url().resolved(tNewUrl);

            QNetworkAccessManager *tManager = inReply->manager();
            QNetworkRequest tRedirection(tNewUrl);
            //            QNetworkReply *tNewReply = tManager->get(tRedirection);
            tManager->get(tRedirection);

            return; // чтобы удержать менеджера для следующего запроса
        }
        else
        {
            setUpdateText("8 Error!;");
            delayyy(1000);
            qDebug() << "Error!";
        }
    }
    else
    {
        setUpdateText("006 inReply->isReadable(");
        delayyy(1000);
        //читаю построчно
        //ищу в каждой строке место где записана версия
        //с помощью "лейбла"
        QString tReplyString;
        QString temp;
        while(!inReply->atEnd()) {
            temp = inReply->readLine();
            if (temp.contains(UpdateApp::versionHeading)) {
                tReplyString = temp.mid(temp.indexOf(UpdateApp::versionHeading) + UpdateApp::versionHeading.size() + 1, 9);

         //       qDebug() << temp;

            }
            setUpdateText(temp);
            delayyy(1000);
        }
        setUpdateText("Версия");
        delayyy(3000);
        setUpdateText(tReplyString);
        delayyy(3000);


        setUpdateText(
                    tr(
                        "10 %1."
                    ).arg(inReply->error()));

        delayyy(2000);
    }

    setUpdateText("9");
    delayyy(1000);
    inReply->deleteLater();
}




/*
UpdateApp::UpdateApp(QObject *parent)
    : QObject{parent}
{

}


void UpdateApp::httpDownload()
{
    dir = new QDir("/");
    qDebug()<< "mkpath:" << dir->mkpath("/storage/emulated/0/Android/data/com.hznk/files");
    qDebug()<< "setCurrent:" << QDir::setCurrent("/storage/emulated/0/Android/data/com.hznk/files");
    qDebug() << "QDir::currentPath():"<<QDir::currentPath();
    dir->setPath(QDir::currentPath());
    file = new QFile(fileName);
    fileinfo = QFileInfo(*file);
    qDebug() << "QFileInfo" << fileinfo.absoluteFilePath();

    bool suc = file->open(QIODevice::WriteOnly); // Just write the way to open the file
    if(suc){
                 qDebug () << "File opens successfully";
    }else{
                 qDebug () << "File fails";
    }

    accessManager = new QNetworkAccessManager(this);
    request.setUrl(url);
    // ******************** Set the header *****************************************************
    // request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
    // request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
    // request.setRawHeader("Content-Disposition","form-data;name='doc';filename='a.txt'");
    //request.setHeader(QNetworkRequest::ContentLengthHeader,post_data.length());

    reply = accessManager->get(request); // By sending data, the return value is stored in the repy pointer.
  //  connect(accessManager, signal (FINISHED (QNetworkReply)), this, Slot (Replyfinished (QNetworkReply*)); // FINISH is a signal that comes with Manager. Definition
  //  connect (reply, Signal (DOWNLOADPROGress (Qint64, Qint64)), this, slot (onDownloadProgress (Qint64, Qint64))); // Download the schedule schedule progress progress progress progress schedule)
    connect((QObject *)reply, SIGNAL(readyRead()),this, SLOT(onReadyRead()));
}

void UpdateApp::replyFinished(QNetworkReply *reply)
{
    // Get the information of the response, the status code is 200 to indicate normal
    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug () << "The network response is complete:" << status_code;

    // Return without mistakes
    if(reply->error() == QNetworkReply::NoError)
    {
      file->flush();
      file->close();
      delete file;
      file=NULL;
    }else
    {
             // Process errors
    }

    reply->deleteLater(); // Delete reply, but you cannot directly delete in the repyfinished.
    reply = NULL;

    accessManager->deleteLater();
    accessManager = NULL;

//    downloadFinished();
}

void UpdateApp::onDownloadProgress(qint64 bytesSent, qint64 bytesTotal)
{
//    double pre = double(bytesSent)/bytesTotal;
//    emit progressPosition(pre);
}

void UpdateApp::onReadyRead()
{
    file->write(reply->readAll());
}

*/
