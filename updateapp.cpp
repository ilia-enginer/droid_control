#include "updateapp.h"



//------------------------------------------------------------------------------

const QString UpdateApp::kVersionUrl =
    "https://github.com/ilia-enginer/droid_stick/blob/main/version.txt";

const QString UpdateApp::kUpdateUrl =
    //"https://raw.githubusercontent.com/ilia-enginer/droid_stick/blob/main/droid_stick.apk";     //1 вариант
    "https://github.com/ilia-enginer/droid_stick/blob/main/droid_stick.apk?raw=true";   //2 вариант

const QString UpdateApp::versionHeading =
    "4294967295";                           //0xFFFFFFFF

const QString UpdateApp::fileName =
        "droid_stick.apk";

const QString UpdateApp::downloadFolderAdress =
        "/storage/emulated/0/Download/";



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

void UpdateApp::checkForUpdates(void)
{
    AppVersion *AppVer = new AppVersion();
    qint32 ver = AppVer->getAppVersion();

    if(ver == 0)    return;

    _verAppIn = ver;

//    qDebug() << _verAppIn;

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
        if (inVersion.toInt() > _verAppIn)
        {
            if(rendering_flag)
            {
                emit windowloadOpen();
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
                        "Ошибка!\nФайл обновления %1 отсутствует."
                    ).arg(tServerFileName));

        if(rendering_flag) emit busyIndicatorOFF();

        return;
    }

    /*
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

*/



//    dir = new QDir("/");
//    dir->setPath(QDir::currentPath());
//    mFile = new QFile(fileName);

      requestAndroidPermissions();

#if defined(Q_OS_ANDROID)
     mFile = new QFile(downloadFolderAdress + fileName);
#elif defined(Q_OS_WINDOWS)
    mFile = new QFile(fileName);
#endif

    fileinfo = QFileInfo(*mFile);

    if(!mFile->open(QIODevice::WriteOnly)) // Just write the way to open the file
    {
        setUpdateText(
                    tr(
                        "Ошибка!\nОшибка сохранения файла %1: %2."
                    ).arg(fileName).arg(mFile->errorString())
                );

        if(rendering_flag) emit busyIndicatorOFF();

        delete mFile;
        return;
    }

    if(rendering_flag) emit busyIndicatorOFF();

    if(rendering_flag) emit startload();         //включение ползунка загрузки

   // setUpdateText(tr("Загрузка %1.").arg(tServerFileName));
    setUpdateText("Загрузка " + fileName);

    mHttpRequestAborted = false;
    startRequest(kUpdateUrl);
}


void UpdateApp::set_TotalBytes(double byte)
{
    TotalBytes = byte;
    if(rendering_flag) emit totalBytesChanged();
}

void UpdateApp::set_BytesRead(double byte)
{
    BytesRead = byte;
    if(rendering_flag) emit bytesReadChanged();
}

double UpdateApp::get_TotalBytes() const
{
    return TotalBytes;
}

double UpdateApp::get_BytesRead() const
{
    return BytesRead;
}

bool UpdateApp::requestAndroidPermissions()
{
#if defined(Q_OS_ANDROID)
   QFuture permission_request = QtAndroidPrivate::requestPermission("android.permission.WRITE_EXTERNAL_STORAGE");
   switch(permission_request.result())
   {
   case QtAndroidPrivate::Undetermined:
       return false;
       break;
   case QtAndroidPrivate::Authorized:
       break;
   case QtAndroidPrivate::Denied:
       return false;
       break;
   }

   permission_request = QtAndroidPrivate::requestPermission("android.permission.READ_EXTERNAL_STORAGE");
   switch(permission_request.result())
   {
   case QtAndroidPrivate::Undetermined:
       return false;
       break;
   case QtAndroidPrivate::Authorized:
       break;
   case QtAndroidPrivate::Denied:
       return false;
       break;
   }

   permission_request = QtAndroidPrivate::requestPermission("android.permission.INSTALL_PACKAGES");
   switch(permission_request.result())
   {
   case QtAndroidPrivate::Undetermined:
       return false;
       break;
   case QtAndroidPrivate::Authorized:
       break;
   case QtAndroidPrivate::Denied:
       return false;
       break;
   }
#endif
    return true;
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

    if(rendering_flag) emit statusLoadOFF();    //отключение ползунка загрузки

    if(!mHttpRequestAborted)
    {
        if(mDownloaderReply)    mDownloaderReply->abort();
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
        mFile = NULL;

        setUpdateText(
                    tr(
                        "Ошибка!\nОшибка загрузки: %1."
                    ).arg(mDownloaderReply->errorString()));

        if(rendering_flag) emit statusLoadOFF();    //отключение ползунка загрузки
    }
    else if (!tRedirectionTarget.isNull())
    {
        setUpdateText("6");
        delayyy(3000);
        QUrl tUrl = QUrl(kUpdateUrl);
        QUrl tNewUrl = tUrl.resolved(tRedirectionTarget.toUrl());
        tUrl = tNewUrl;

        mFile->open(QIODevice::WriteOnly);
        mFile->resize(0);

        set_BytesRead(0.0);

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

        setUpdateText("Скачано");
        delayyy(1000);

        if(rendering_flag) emit statusLoadOFF();    //отключение ползунка загрузки

        setUpdateText("Установка приложения");
        if(rendering_flag) emit busyIndicatorON();

        #if defined(Q_OS_ANDROID)
            QAndroidJniObject intent("InstallAPK");
            QAndroidJniObject jsText = QAndroidJniObject::fromString("/storage/emulated/0/Android/data/com.hznk/files/droid_stick.apk");
            jint ret = intent.callMethod<jint>("installApp","(Ljava/lang/String;)I",jsText.object<jstring>());
        #endif

 //       QApplication::quit();
    }

    mDownloaderReply->deleteLater();
    mDownloaderReply = NULL;

    mNamDownloader -> deleteLater();
    mNamDownloader = NULL;

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

    set_TotalBytes((double)inTotalBytes);
    set_BytesRead((double)inBytesRead);
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

