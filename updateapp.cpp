#include "updateapp.h"


/*
//------------------------------------------------------------------------------

const QString UpdateApp::kVersionUrl =
    "https://github.com/ilia-enginer/droid_stick/blob/main/version.txt";

const QString UpdateApp::kUpdateUrl =
    "https://github.com/ilia-enginer/droid_stick/blob/main/droid_stick.apk";

const QString UpdateApp::versionHeading =
    "4294967295";                           //0xFFFFFFFF


//------------------------------------------------------------------------------

UpdateApp::UpdateApp(QObject *parent) :
    QObject(parent)
{
//    QStringList permission;
//    permission.append("android.permission.WRITE_EXTERNAL_STORAGE");
//    permission.append("android.permission.INSTALL_PACKAGES");
//    permission.append("android.permission.REQUEST_INSTALL_PACKAGES");
//    QtAndroid::requestPermissionsSync(permission);

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
    if(ver != 0)    _verAppIn = ver;

    QUrl tUrl(kVersionUrl);

    mNamChecker->get(QNetworkRequest(tUrl));
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
            QMessageBox msgBox;
            msgBox.setWindowModality(Qt::ApplicationModal);
            msgBox.setWindowTitle("Доступна новая версия приложения.");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(QString("Обновить сейчас?"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::No);
//            msgBox.setButtonText(QMessageBox::No, "Not now");
            int tRet = msgBox.exec();

            if (tRet == QMessageBox::Yes)
            {
                downloadFile();
            }
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
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Внимание!"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(
            tr(
                "Файл %1 отсутствует."
            ).arg(tServerFileName));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

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
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Внимание!"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(
            tr(
                "Невозможно сохранить файл %1: %2."
            ).arg(tLocalFileName).arg(mFile->errorString())
        );
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        delete mFile;

        return;
    }

    mProgressDialog = new QProgressDialog();
    mProgressDialog->setWindowModality(Qt::ApplicationModal);
    connect(mProgressDialog, SIGNAL(canceled()),
            this, SLOT(on_CancelDownload()));
    mProgressDialog->setWindowTitle(tr("MyApp"));
    mProgressDialog->setLabelText(tr("Загрузка %1.").arg(tServerFileName));

    mHttpRequestAborted = false;
    startRequest(kUpdateUrl);
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

    mHttpRequestAborted = true;
    mDownloaderReply->abort();
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
        mProgressDialog->hide();

        return;
    }

    mProgressDialog->hide();
    mFile->flush();
    mFile->close();

    QVariant tRedirectionTarget =
        mDownloaderReply->attribute(
            QNetworkRequest::RedirectionTargetAttribute);

    if (mDownloaderReply->error())
    {
        mFile->remove();

        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Внимание!"));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(
            tr(
                "Ошибка загрузки: %1."
            ).arg(mDownloaderReply->errorString()));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
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

    mProgressDialog->setMaximum(inTotalBytes);
    mProgressDialog->setValue(inBytesRead);
}

//------------------------------------------------------------------------------

void UpdateApp::on_NetworkReply(QNetworkReply *inReply)
{
    if (inReply->error() == QNetworkReply::NoError)
    {
        int tHttpStatusCode =
            inReply->attribute(
                QNetworkRequest::HttpStatusCodeAttribute).toUInt();

        if (tHttpStatusCode >= 200 && tHttpStatusCode < 300)
        {             
            if (inReply->isReadable())
            {
//                QString tReplyString =
//                    QString::fromUtf8(inReply->readAll().data());

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
            qDebug() << "Error!";
        }
    }

    inReply->deleteLater();
}
*/



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
