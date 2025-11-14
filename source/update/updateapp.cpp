#include "source/update/updateapp.h"
#include "source/main/appmanager.h"
#include <QStandardPaths>
#include <QDir>


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

//------------------------------------------------------------------------------


void
UpdateApp::delayyy( int mill)
{
    QTime dieTime = QTime::currentTime().addMSecs( mill );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void
UpdateApp::setUpdateText(QString text)
{
    _commun_display->setUpdateAppText(text);
}

void
UpdateApp::setLoadText(QString text)
{
    _commun_display->setLoadTextApp(text);
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

void
UpdateApp::setAppManager(AppManager *newAppManager)
{
    _appManager = newAppManager;
}

void
UpdateApp::setCommun_display(Commun_display *newCommun_display)
{
    _commun_display = newCommun_display;
}

//------------------------------------------------------------------------------

void
UpdateApp::checkForUpdates(void)
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

void
UpdateApp::checkVersion(QString inVersion)
{
//    qDebug() << inVersion.toInt();
//    qDebug() << inVersion;

    if ((inVersion.toInt() != 0) && (_verAppIn != 0))
    {
        if (inVersion.toInt() > _verAppIn)
        {
            //открыть всплывающее окно с предложением обновиться
            _commun_display->statusUpdateApp(_commun_display->updApp::windowloadOpenStat);

            setUpdateText("Доступно обновление программы\n Обновить сейчас?");
        }
    }
}

//------------------------------------------------------------------------------

void
UpdateApp::downloadFile()
{
    setUpdateText("Подготовка к загрузке...");

    QFileInfo tServerFileInfo(kUpdateUrl);
    QString tServerFileName = tServerFileInfo.fileName();

    if (tServerFileName.isEmpty())
    {
        setUpdateText(tr("Ошибка!\nФайл обновления %1 отсутствует.").arg(tServerFileName));
        _commun_display->statusUpdateApp(_commun_display->updApp::busyIndicatorOFFStat);
        return;
    }

#if defined(Q_OS_ANDROID)
{
    QString base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(base);
    mFile = new QFile(QDir(base).filePath(fileName)); // .../files/<org>/AppName/droid_stick.apk
}
#elif defined(Q_OS_WINDOWS)
{
    mFile = new QFile(fileName);
}
#endif

    fileinfo = QFileInfo(*mFile);

    if (!mFile->open(QIODevice::WriteOnly))
    {
        setUpdateText(tr("Ошибка!\nОшибка сохранения файла %1: %2.")
                          .arg(fileName).arg(mFile->errorString()));
        _commun_display->statusUpdateApp(_commun_display->updApp::busyIndicatorOFFStat);
        delete mFile;
        return;
    }

    _commun_display->statusUpdateApp(_commun_display->updApp::busyIndicatorOFFStat);
    _commun_display->statusUpdateApp(_commun_display->updApp::startloadStat);

    setUpdateText("Загрузка...\nНе сворачивайте приложение во время загрузки");
    setLoadText("");

    _appManager->startBackgroundService();

    mHttpRequestAborted = false;
    startRequest(kUpdateUrl);
}


void
UpdateApp::set_TotalBytes(double byte)
{
    _commun_display->set_TotalBytes(byte);
}

void
UpdateApp::set_BytesRead(double byte)
{
    _commun_display->set_BytesRead(byte);
}

void
UpdateApp::install()
{
    _commun_display->statusUpdateApp(_commun_display->updApp::busyIndicatorONStat);

    #if defined(Q_OS_ANDROID)
        QString base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        const QString apkPath = QDir(base).filePath(fileName);

        QJniObject jPath = QJniObject::fromString(apkPath);
        jint ret = QJniObject::callStaticMethod<jint>(
            "org/qtproject/example/InstallAPK",
            "execute",
            "(Landroid/content/Context;Ljava/lang/String;)I",
            nullptr,
            jPath.object<jstring>()
        );

        if(ret == 0)
        {
            setUpdateText("Процесс обновления успешно запущен");
            delayyy(1500);
        }
        else if(ret == -1)
        {
            setUpdateText("Что-то пошло не по плану. Попробуйте обновиться чуть позже");
            delayyy(1500);
        }
        else if(ret == -2)
        {
            setUpdateText("Странные вещи происходят. Установочный файл пустой. Попробуйте перезапустить приложение.");
            delayyy(1500);
        }
        else if(ret == -3)
        {
            setUpdateText("Странные вещи происходят. Установочный файл куда-то пропал. Попробуйте перезапустить приложение.");
            delayyy(1500);
        }
        else if(ret == -4)
        {
            setUpdateText(QStringLiteral("Не хватает разрешений на установку. Вы можете изменить это в настройках приложения. Раздел 'Разрешения'"));
            delayyy(3000);
        }
        else if(ret == -5)
        {
            setUpdateText("Неудача. Попробуйте перезапустить приложение.");
            delayyy(1500);
        }


        // Аккуратная очистка
        mDownloaderReply->deleteLater();
        mDownloaderReply = nullptr;

        if (mFile) {
            delete mFile;
            mFile = nullptr;
        }
#elif defined(Q_OS_WINDOWS)
    {
        QString tLocalFileName =
            QDir::toNativeSeparators(
                QCoreApplication::applicationDirPath() + "/" +
                QFileInfo(QUrl(kUpdateUrl).path()).fileName());
        QDesktopServices::openUrl(QUrl::fromLocalFile(tLocalFileName));
        QApplication::quit();
    }
#endif
    _commun_display->statusUpdateApp(_commun_display->updApp::busyIndicatorOFFStat);
}

//------------------------------------------------------------------------------

void
UpdateApp::startRequest(QUrl inUrl)
{
    mNamDownloader = new QNetworkAccessManager(this);

    mDownloaderReply = mNamDownloader->get(QNetworkRequest(inUrl));

    connect(mDownloaderReply, SIGNAL(finished()),
            this, SLOT(on_HttpFinished()));

    connect(mDownloaderReply, SIGNAL(readyRead()),
            this, SLOT(on_HttpDataRead()));

    connect(mDownloaderReply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(on_UpdateDataReadProgress(qint64, qint64)));

    //чтоб экран не гас
    _appManager->keepScreenOn(true);
}



//------------------------------------------------------------------------------

void
UpdateApp::on_CancelDownload()
{
    qDebug() << "on_CancelDownload";

    _commun_display->statusUpdateApp(_commun_display->updApp::statusLoadOFFStat);
    setLoadText("");

    if(!mHttpRequestAborted)
    {
        if(mDownloaderReply)
        {
            //mDownloaderReply->abort();
            mDownloaderReply->deleteLater();
            mDownloaderReply = nullptr;
        }
    }

    if(mFile)   mFile = nullptr;

    mHttpRequestAborted = true;

    //чтоб экран не гас
    _appManager->keepScreenOn(false);
    _appManager->stopBackgroundService();
}

//------------------------------------------------------------------------------

void
UpdateApp::on_HttpFinished()
{
    qDebug() << "on_HttpFinished";

    //чтоб экран не гас
    _appManager->keepScreenOn(false);
    _appManager->stopBackgroundService();
    setLoadText("");

    if (!mDownloaderReply) {
        // Нечего обрабатывать
        return;
    }

    if (mHttpRequestAborted) {
        if (mFile) {
            mFile->close();
            mFile->remove();
            delete mFile;
            mFile = nullptr;
        }
        mDownloaderReply->deleteLater();
        mDownloaderReply = nullptr;
        return;
    }

    if (mFile) {
        mFile->flush();
        mFile->close();
    }

    const QVariant redirTarget =
        mDownloaderReply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (mDownloaderReply->error() != QNetworkReply::NoError) {
        if (mFile) {
            mFile->remove();
            delete mFile;
            mFile = nullptr;
        }
        setUpdateText(tr("Ошибка!\nОшибка загрузки: %1.")
                          .arg(mDownloaderReply->errorString()));
        _commun_display->statusUpdateApp(_commun_display->updApp::statusLoadOFFStat);
        _appManager->stopBackgroundService();

        mDownloaderReply->deleteLater();
        mDownloaderReply = nullptr;
        return;
    }

    if (!redirTarget.isNull()) {
        // Обработка редиректа: перезапускаем запрос и выходим,
        // чтобы не удалить новый reply ниже.
        QUrl nextUrl = QUrl(kUpdateUrl).resolved(redirTarget.toUrl());

        if (mFile && mFile->open(QIODevice::WriteOnly)) {
            mFile->resize(0);
            set_BytesRead(0.0);
            mDownloaderReply->deleteLater();
            mDownloaderReply = nullptr;

            startRequest(nextUrl);
            return; // <<< ОБЯЗАТЕЛЬНО
        } else {
            setUpdateText(tr("Ошибка!\nНе удалось переоткрыть файл для записи."));
            _commun_display->statusUpdateApp(_commun_display->updApp::statusLoadOFFStat);
            _appManager->stopBackgroundService();

            if (mFile) {
                delete mFile;
                mFile = nullptr;
            }
            mDownloaderReply->deleteLater();
            mDownloaderReply = nullptr;
            return;
        }
    }

    // ==== УСПЕШНО СКАЧАНО ====
    setUpdateText("Обновление скачано. Установить?");
    _commun_display->statusUpdateApp(_commun_display->updApp::statusLoadOFFStat);
    _commun_display->statusUpdateApp(_commun_display->updApp::but_Ok_OnStat);
}

//------------------------------------------------------------------------------

void
UpdateApp::on_HttpDataRead()
{
    if (!mFile || !mDownloaderReply) return;
    mFile->write(mDownloaderReply->readAll());
}

//------------------------------------------------------------------------------

void
UpdateApp::on_UpdateDataReadProgress(
    qint64 inBytesRead,
    qint64 inTotalBytes)
{
    if (mHttpRequestAborted)
    {
        return;
    }

    qint64 Mbyte = inBytesRead / 1024 / 1024;
    qint64 Mbytes = inTotalBytes / 1024 / 1024;

    setLoadText(QString("Загружено %1 Мб / %2 Мб")
                .arg(Mbyte)
                .arg(Mbytes));

    set_TotalBytes((double)inTotalBytes);
    set_BytesRead((double)inBytesRead);
}

//------------------------------------------------------------------------------

void
UpdateApp::on_NetworkReply(QNetworkReply *inReply)
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

