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
    setUpdateText("Подготовка к загрузке...");

    QFileInfo tServerFileInfo(kUpdateUrl);
    QString tServerFileName = tServerFileInfo.fileName();

    if (tServerFileName.isEmpty())
    {
        setUpdateText(tr("Ошибка!\nФайл обновления %1 отсутствует.").arg(tServerFileName));
        if (rendering_flag) emit busyIndicatorOFF();
        return;
    }

    requestAndroidPermissions();

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
        if (rendering_flag) emit busyIndicatorOFF();
        delete mFile;
        return;
    }

    if (rendering_flag) emit busyIndicatorOFF();
    if (rendering_flag) emit startload();       

    setUpdateText("Загрузка...\nНе сворачивайте приложение во время загрузки");

    mHttpRequestAborted = false;
    startRequest(kUpdateUrl);
}


void UpdateApp::set_TotalBytes(double byte)
{
    TotalBytes = byte;
    if (rendering_flag) emit totalBytesChanged();
}

void UpdateApp::set_BytesRead(double byte)
{
    BytesRead = byte;
    if (rendering_flag) emit bytesReadChanged();
}

double UpdateApp::get_TotalBytes() const
{
    return TotalBytes;
}

double UpdateApp::get_BytesRead() const
{
    return BytesRead;
}

void UpdateApp::install()
{
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
            setUpdateText(QStringLiteral("Все еще не хватает разрешений на установку. Вы можете изменить это в настройках приложения. Раздел 'Разрешения'"));
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
    #endif
}

bool UpdateApp::requestAndroidPermissions()
{
#if defined(Q_OS_ANDROID)
    // Приватные директории приложения не требуют рантайм-разрешений
    return true;
#else
    return true;
#endif
}

//------------------------------------------------------------------------------

void UpdateApp::startRequest(QUrl inUrl)
{
//    if (!mNamDownloader)
//        mNamDownloader = new QNetworkAccessManager(this);

//    QNetworkRequest req(inUrl);

//#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
//    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
//                     QNetworkRequest::NoLessSafeRedirectPolicy);
//#endif

//    req.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
//                     QNetworkRequest::AlwaysNetwork);

//    req.setHeader(QNetworkRequest::UserAgentHeader,
//                  QByteArray("DroidStickUpdater/1.0 (Qt)"));
//    req.setRawHeader("Accept", "*/*");

//#if QT_CONFIG(ssl)
//    QSslConfiguration ssl = QSslConfiguration::defaultConfiguration();
//    ssl.setProtocol(QSsl::TlsV1_2OrLater);
//    req.setSslConfiguration(ssl);
//#endif

//    mDownloaderReply = mNamDownloader->get(req);

//    connect(mDownloaderReply, &QNetworkReply::finished,
//            this, &UpdateApp::on_HttpFinished);
//    connect(mDownloaderReply, &QIODevice::readyRead,
//            this, &UpdateApp::on_HttpDataRead);
//    connect(mDownloaderReply, &QNetworkReply::downloadProgress,
//            this, &UpdateApp::on_UpdateDataReadProgress);

//#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
//    connect(mDownloaderReply, &QNetworkReply::errorOccurred, this,
//            [this](QNetworkReply::NetworkError code) {
//                Q_UNUSED(code);
//                setUpdateText(tr("Ошибка сети: %1").arg(mDownloaderReply->errorString()));
//            });
//#endif

//#if QT_CONFIG(ssl)
//    connect(mDownloaderReply, &QNetworkReply::sslErrors, this,
//            [this](const QList<QSslError>& errs) {
//                if (!errs.isEmpty())
//                    setUpdateText(tr("SSL ошибка: %1").arg(errs.first().errorString()));
//            });
//#endif

    mNamDownloader = new QNetworkAccessManager(this);

    mDownloaderReply = mNamDownloader->get(QNetworkRequest(inUrl));

    connect(mDownloaderReply, SIGNAL(finished()),
            this, SLOT(on_HttpFinished()));

    connect(mDownloaderReply, SIGNAL(readyRead()),
            this, SLOT(on_HttpDataRead()));

    connect(mDownloaderReply, SIGNAL(downloadProgress(qint64, qint64)),
            this, SLOT(on_UpdateDataReadProgress(qint64, qint64)));

    //чтоб экран не гас
    AppManager app;
    app.keepScreenOn(true);
}



//------------------------------------------------------------------------------

void UpdateApp::on_CancelDownload()
{
    qDebug() << "on_CancelDownload";

    if(rendering_flag) emit statusLoadOFF();    //отключение ползунка загрузки

    if(!mHttpRequestAborted)
    {
        if(mDownloaderReply)
        {
            //mDownloaderReply->abort();
            mDownloaderReply->deleteLater();
            mDownloaderReply = nullptr;
        }
    }
    if (mFile) {
        delete mFile;
        mFile = nullptr;
    }
    mHttpRequestAborted = true;

    //чтоб экран не гас
    AppManager app;
    app.keepScreenOn(false);
}

//------------------------------------------------------------------------------

void UpdateApp::on_HttpFinished()
{
    qDebug() << "on_HttpFinished";

    //чтоб экран не гас
    AppManager app;
    app.keepScreenOn(false);

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
        if (rendering_flag) emit statusLoadOFF();

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
            if (rendering_flag) emit statusLoadOFF();

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
    setUpdateText("Обновление скачано");
    delayyy(300);
    if (rendering_flag) emit statusLoadOFF();
    setUpdateText("Установка обновления");
    if (rendering_flag) emit busyIndicatorON();
    delayyy(500);

#if defined(Q_OS_ANDROID)
    {
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
//        setUpdateText(QStringLiteral("Код установки: %1").arg(ret));
        if(ret == 0)
        {
            setUpdateText("Процесс обновления успешно запущен");
            delayyy(1500);
        }
        else if(ret == -1)
        {
            if (rendering_flag) emit busyIndicatorOFF();
            setUpdateText("Что-то пошло не по плану. Попробуйте обновиться чуть позже");
            delayyy(1500);
        }
        else if(ret == -2)
        {
            if (rendering_flag) emit busyIndicatorOFF();
            setUpdateText("Странные вещи происходят. Установочный файл пустой. Попробуйте перезапустить приложение.");
            delayyy(1500);
        }
        else if(ret == -3)
        {
            if (rendering_flag) emit busyIndicatorOFF();
            setUpdateText("Странные вещи происходят. Установочный файл куда-то пропал. Попробуйте перезапустить приложение.");
            delayyy(1500);
        }
        else if(ret == -4)
        {
            if (rendering_flag) emit busyIndicatorOFF();
            setUpdateText(QStringLiteral("Не хватает разрешений на установку, попробую еще раз. Для продолжения нажмите 'Ок'"));
            delayyy(3000);
            emit but_Ok_On();
            return;
        }
        else if(ret == -5)
        {
            setUpdateText("Неудача. Попробуйте перезапустить приложение.");
            delayyy(1500);
        }
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

    // Аккуратная очистка
    mDownloaderReply->deleteLater();
    mDownloaderReply = nullptr;

    if (mFile) {
        delete mFile;
        mFile = nullptr;
    }
}



//------------------------------------------------------------------------------

void UpdateApp::on_HttpDataRead()
{
    if (!mFile || !mDownloaderReply) return;
    mFile->write(mDownloaderReply->readAll());
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

