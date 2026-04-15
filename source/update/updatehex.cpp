
#include <QFileDialog>

#include "updatehex.h"
#include "../main/appmanager.h"
#include "source/communication/device/device.h"


UpdateHex::UpdateHex(QObject *parent)
    : QObject{parent}
{
    versionExternalProgram.u32 = 0;
    versionInternalProgram.u32 = 0;
    version_BootLoader_ExternalProgram.u32 = 0;
    version_BootLoader_InternalProgram.u32 = 0; 
}

UpdateHex::~UpdateHex()
{
}

void
UpdateHex::setTx_commands(Tx_commands *newTx_commands)
{
    _tx_commands = newTx_commands;
}

void
UpdateHex::setCrc(Crc *newCrc)
{
    _crc = newCrc;
}

void
UpdateHex::setCommun_display(Commun_display *newCommun_display)
{
    _commun_display = newCommun_display;
}

void
UpdateHex::setSettings(Settings *newSettings)
{
    _settings = newSettings;
}

void
UpdateHex::setAppManager(AppManager *newAppManager)
{
    _appManager = newAppManager;
}

void
UpdateHex::setVersExt(quint32 version)
{
    versionExternalProgram.u32 = version;
}

quint32
UpdateHex::getVersExt()
{
    return versionExternalProgram.u32;
}

void
UpdateHex::setVersBootLoaderExt(quint32 version)
{
    version_BootLoader_ExternalProgram.u32 = version;
}

quint32
UpdateHex::getVersBootLoaderExt()
{
    return version_BootLoader_ExternalProgram.u32;
}

quint32
UpdateHex::getVersInt()
{
    return versionInternalProgram.u32;
}

quint32
UpdateHex::getVersBootLoaderInt()
{
    return version_BootLoader_InternalProgram.u32;
}

void
UpdateHex::setPageTx(qint32 num)
{
    _pageTx = num;
}

//проверка обновления
int
UpdateHex::checkUpdateHex()
{
    //запрос версии прошивки
    _tx_commands->getVersion();
    //ожидание ответа
    delay(90);
    qint32 a = 0;
    while(versionExternalProgram.u32 == 0)
    {
        _tx_commands->getVersion();
        if(a++ > 10)
        {
            return -1;
        }
        delay(90);
    }

    #if defined(Q_QDOC) ||                                                         \
        (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))

        //открываю файл и читаю версии прошивки и бутлоадера
        if(!open_Update())  return -2;
        if(!openBootloaderUpdate())  return -2;

        //если основная версия или версия загрузчика из apk новее - предложить обновить провишку
        if(((versionInternalProgram.u32 > versionExternalProgram.u32)
                || (version_BootLoader_InternalProgram.u32 > version_BootLoader_ExternalProgram.u32)) &&
                (versionExternalProgram.u32 != 0))
        {
            //открыть всплывающее окно с предложением обновиться
            return 1;
        }
    #endif
    return 0;
}

void
UpdateHex::f_AdminChange(bool f)
{
    _f_Admin = f;
}


///! проверка обновлений
void
UpdateHex::checkingUpdates(void)
{
    emit navigateBackActionOFF();

    #if defined(Q_QDOC) || (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))
    int a = 0;

    if (!_commun_display->get_connected()) {
      emit navigateBackActionON();
      return;
    }

    _commun_display->setCurrenUpd("Подготовка к обновлению...");
    _commun_display->statusUpdate(_commun_display->statusUpd::checkUpdateProgr);

    ///запрос напряжения
    _commun_display->vrealChang(0.0);
    _tx_commands->voltage_read(false);
    delay(100);
    a = 0;
    ///ожидание ответа
    //если напряжение меньше 20% заряда - защита
    while(_commun_display->getVolt() == 0)
    {
        _tx_commands->voltage_read(false);
        delay(70);
        if(a++ > 10)
        {
            _commun_display->setCurrenUpd("Непредвиденная ошибка. Попробуйте еще раз.");
            _commun_display->statusUpdate(_commun_display->statusUpd::checkUpd);
            emit navigateBackActionON();
            return;
        }
    }

    if(_commun_display->getVolt() < (_settings->getVmax() * 0.8))
    {
        if(_f_Admin == false)
        {
            _commun_display->setCurrenUpd("Слишком низкое напряжение. Обновление невозможно");
            _commun_display->statusUpdate(_commun_display->statusUpd::checkUpd);
            emit navigateBackActionON();
            return;
        }
        else
        {
            _commun_display->setCurrenUpd("Внимание! Низкое напряжение");
            delay(3000);
        }
    }

    _commun_display->setCurrenUpd("Проверка обновлений...");
    versionExternalProgram.u32 = 0;
    version_BootLoader_ExternalProgram.u32 = 0;
    //запрос версии прошивки
    _tx_commands->getVersion();
    //ожидание ответа
    delay(90);
    while(versionExternalProgram.u32 == 0)
    {
        _tx_commands->getVersion();
        if(a++ > 7)
        {
            _commun_display->setCurrenUpd("Ошибка\nпроверьте подключение к устройству");
            _commun_display->statusUpdate(_commun_display->statusUpd::checkUpd);
            emit navigateBackActionON();
            return;
        }
        delay(90);
    }
    #endif

    if(!openBootloaderUpdate())
    {
        emit navigateBackActionON();
        return;
    }
    if(!open_Update())
    {
        emit navigateBackActionON();
        return;
    }

   //если обычный режим
    if(!_f_Admin)
    {
        //если прошивка из apk свежее
        //или бутлоадер свежее
        if((versionInternalProgram.u32 > versionExternalProgram.u32) || (version_BootLoader_InternalProgram.u32 > version_BootLoader_ExternalProgram.u32))
        {
            _commun_display->setCurrenUpd("Доступно обновление\nОбновить?");
            //включить кнопку загрузки прошивки
            _commun_display->statusUpdate(_commun_display->statusUpd::updateAvailab);
        }
        else
        {
            _commun_display->setCurrenUpd("Установлена последняя версия");
            _commun_display->statusUpdate(_commun_display->statusUpd::checkUpd);
        }
    }
    //если режим админа
    else
    {
        _commun_display->setCurrenUpd("Версия:\n" + versionToString(versionExternalProgram.u32) + "\nДоступна версия:\n" + versionToString(versionInternalProgram.u32) + "\n"
                            "Версия загрузчика:\n" + versionToString(version_BootLoader_ExternalProgram.u32) + "\nДоступна версия:\n" + versionToString(version_BootLoader_InternalProgram.u32));

        //включить кнопку загрузки бутлоадера
        _commun_display->statusUpdate(_commun_display->statusUpd::updateBootloaderAvailab);
    }
    emit navigateBackActionON();
}


QString
UpdateHex::versionToString(quint32 vers)
{
    f_value val;
    val.u32 = vers;

    return (QString("%1.%2.%3.%4.%5.%6")
                              .arg(val.data[3] >> 4, 1, 10)                            // версия
                              .arg((val.u16[1] >> 4) & 0x0FF, 2, 10, QChar('0'))       // год
                              .arg(val.data[2] & 0x0F, 2, 10, QChar('0'))              // месяц
                              .arg((val.data[1] >> 3) & 0x1F, 2, 10, QChar('0'))       // день
                              .arg((val.u16[0] >> 6) & 0x01F, 2, 10, QChar('0'))       // час
                              .arg(val.data[0] & 0x3F, 2, 10, QChar('0')));            // минуты
}


//! Включить запись
void
UpdateHex::on_pbWrite_clicked(bool flag)
{
    emit navigateBackActionOFF();

#ifdef Q_OS_ANDROID
    //если обычный режим
    if(!_f_Admin)
    {
//        qDebug() << version_BootLoader_InternalProgram.u32;
//        qDebug() << version_BootLoader_ExternalProgram.u32;
        //если надо записывать бутлоадер
        if(version_BootLoader_InternalProgram.u32 > version_BootLoader_ExternalProgram.u32)
        {
            if(!openBootloaderUpdate()) return;
            load_param_ = false;
        }
        //если надо записывать основную прошивку
        else
        {
            //открываю файл прошивки
            if(!open_Update()) return;
            load_param_ = true;
        }
    }
    //если режим админа
    else
    {
        //если надо записывать бутлоадер
        if(flag == false)
        {
            if(!openBootloaderUpdate()) return;
            load_param_ = false;
        }
        //если надо записывать основную прошивку
        else
        {
            //открываю файл прошивки
            if(!open_Update()) return;
            load_param_ = true;
        }
    }

#elif defined Q_OS_WIN
    //если надо записывать бутлоадер
    if(flag == false)
    {
        if(!openBootloaderUpdate()) return;
        load_param_ = false;
    }
    //если надо записывать основную прошивку
    else
    {
        //открываю файл прошивки
        if(!open_Update()) return;
        load_param_ = true;
    }
#endif

    _pages = _bin.size() / _size;
    _page = 0;
    _pageTx = -1;
    _unsuccessful_transfers = 0;

    QByteArray start;
    //если надо грузить бутлоадер
    if(load_param_ == false)
    {
        //посылаю команду начала передачи
        _tx_commands->writeBootloader(start);
        delay(70);
        _tx_commands->writeBootloader(start);
        delay(3000);
    }
    //если надо грузить основную прошивку
    else
    {
        //посылаю команду начала передачи
        _tx_commands->writeProgram(start);
        delay(70);
        _tx_commands->writeProgram(start);
        delay(3000);
    }

     if(_appManager)    _appManager->startBackgroundService();

     //включить индикатор загрузки
     //и кнопку остановки
     _commun_display->statusUpdate(_commun_display->statusUpd::stopUpd);

    //включить таймер
     _timer = new QTimer(this);
     _timer->setInterval(200);
     connect(_timer, &QTimer::timeout, this, &UpdateHex::write_page);

     _timer->start();
}


qint32
UpdateHex::open_Update()
{
    QString _fileName;

    //считывание основной вервии
    //открытие файла с прошивкой из apk
    #ifdef Q_OS_ANDROID
        _fileName = ":/bin/roboshar_v.bin"; // (из ресурсов)
    #elif defined Q_OS_WIN
        _fileName = fileOpen(false);
    #endif

    if(!on_pbOpenFile_clicked(_fileName))
    {
        _commun_display->setCurrenUpd("Ошибка\nне удалось открыть файл прошивки");
        _commun_display->statusUpdate(_commun_display->statusUpd::checkUpd);
        return false;
    }
    //поиск версии прошивки в файле
    if (_bin.contains(getVersionLabel())) {
        QByteArray newVersion = _bin.mid(_bin.indexOf(getVersionLabel()) + getVersionLabel().size() + 1, 4);
        versionInternalProgram.data[3] = newVersion.at(3);
        versionInternalProgram.data[2] = newVersion.at(2);
        versionInternalProgram.data[1] = newVersion.at(1);
        versionInternalProgram.data[0] = newVersion.at(0);
    }

    //расчет срс прошивки
    int fullSize = ceil((double)_bin.size() / _size) * _size;
    QByteArray pack_crc(fullSize, 0xFF);
    for (int i = 0; i < pack_crc.size(); i++) {
        int binPtr = i;
        if (binPtr < _bin.size()) pack_crc[i] = _bin[binPtr];
        else {
            break;
        }
    }
    _crc32_Internal.int32 = _crc->crc32(pack_crc, 8);
    return true;
}


qint32
UpdateHex::openBootloaderUpdate()
{
    QString _fileName;

    //считывание бутлоадера
    //открытие файла с прошивкой из apk
    #ifdef Q_OS_ANDROID
        _fileName = ":/bin/boot_loader.bin"; // (из ресурсов)
    #elif defined Q_OS_WIN
        _fileName = fileOpen(false);
    #endif

    if(!on_pbOpenFile_clicked(_fileName))
    {
        _commun_display->setCurrenUpd("Ошибка\nне удалось открыть файл прошивки");
        _commun_display->statusUpdate(_commun_display->statusUpd::checkUpd);
        return false;
    }
    //поиск версии прошивки в файле
    if (_bin.contains(getVersionLabel())) {
//            qDebug() << "address" << _bin.indexOf(getVersionLabel());
        QByteArray newVersion = _bin.mid(_bin.indexOf(getVersionLabel()) + getVersionLabel().size() + 1, 4);
        version_BootLoader_InternalProgram.data[3] = newVersion.at(3);
        version_BootLoader_InternalProgram.data[2] = newVersion.at(2);
        version_BootLoader_InternalProgram.data[1] = newVersion.at(1);
        version_BootLoader_InternalProgram.data[0] = newVersion.at(0);
//           qDebug() << QString("0x%1").arg(versionInternalProgram.u32, 8, 16, QChar('0'));
    }

    //расчет срс прошивки
    int fullSize = ceil((double)_bin.size() / _size) * _size;
    QByteArray pack_crc(fullSize, 0xFF);
    for (int i = 0; i < pack_crc.size(); i++) {
        int binPtr = i;
        if (binPtr < _bin.size()) pack_crc[i] = _bin[binPtr];
        else {
            break;
        }
    }
    _crc32_Internal.int32 = _crc->crc32(pack_crc, 8);

    return true;
}

QString
UpdateHex::fileOpen(bool open)
{
    static QString localPath;

    if((open) || (localPath.isEmpty()))
    {
        QUrl fileUrl = QFileDialog::getOpenFileUrl(nullptr,
                                                   "Выберите файл",
                                                   QUrl("file:///roboshar_v.bin"),
                                                   "Бинарный файл (*.bin);;All files (*)");
        if (!fileUrl.isEmpty()) {
            qDebug() << "Выбранный URL:" << fileUrl.toString();
            if (fileUrl.isLocalFile()) {
                localPath = fileUrl.toLocalFile();
                qDebug() << "Локальный путь:" << localPath;
                return localPath;
            }
        }
    }
    qDebug() << "Локальный путь:" << localPath;
    return localPath;
}


//! Прервать запись
void
UpdateHex::on_pbStop_clicked(QString error)
{
    emit navigateBackActionON();

    if(_appManager) _appManager->stopBackgroundService();

    //выключить таймер
    if(_timer)  _timer->stop();

    _commun_display->statusUpdate(_commun_display->statusUpd::checkUpd);
    _commun_display->setCurrenUpd(error);
}

//вызов по таймеру
void
UpdateHex::write_page()
{
    //пока успешно переданные станицы меньше кол-ва страниц
    if(_pageTx < _pages)
    {
        if(_pageTx == _page)
        {
            _page++;
            _unsuccessful_transfers = 0;
        }
        else if(_pageTx > _page)
        {
            on_pbStop_clicked("Неизвестная ошибка передачи");
            return;
        }
        _unsuccessful_transfers++;
        if(_unsuccessful_transfers > 45)
        {
           on_pbStop_clicked("Ошибка: превышено количество попыток передачи");
            return;
        }
        sendPage();
    }
    else if(_pageTx > _pages)
    {
        on_pbStop_clicked("Странная ошибка передачи");
        return;
    }

    //если все пакеты переданы
    if((_pageTx == _page) && (_pageTx == _pages))
    {
        //выключить таймер
        _timer->stop();

        //если обычный режим
        if(!_f_Admin)
        {
            //если передача бутлоадера
            if(!load_param_)
            {
                _commun_display->setCurrenUpd("Проверка ключей");
                delay(1500);
                _commun_display->setCurrenUpd("Настройка логики");
                delay(1500);
                _commun_display->setCurrenUpd("Ещё чуть-чуть");
                delay(1500);
                //запрос версии прошивки
                _tx_commands->getVersion();
                delay(100);
                int a = 0;
                version_BootLoader_ExternalProgram.u32 = 0;
                //ожидание ответа
                while(version_BootLoader_ExternalProgram.u32 != version_BootLoader_InternalProgram.u32)
                {
                    _tx_commands->getVersion();
                    if(a++ > 15)
                    {
                        on_pbStop_clicked("Что-то поломалось. Попробуйте еще раз");
                        return;
                    }
                    delay(70);
                }
                //когда бутлоадер будет установлен
                //загрузить основную программу, если необходимо
                if((versionExternalProgram.u32 < versionInternalProgram.u32))
                {
                    load_param_ = true;
                    openBootloaderUpdate();
                    on_pbWrite_clicked(load_param_);
                }
                else
                {
                    on_pbStop_clicked("Обновление установлено");
                    return;
                }
            }
            //если загрузка основной программы
            else
            {
                _commun_display->setCurrenUpd("Установка обновления");
                delay(7000);    //7c

                //запрос версии прошивки
                _tx_commands->getVersion();
                delay(100);
                int a = 0;
                versionExternalProgram.u32 = 0;
                //ожидание ответа
                while(versionExternalProgram.u32 != versionInternalProgram.u32)
                {
                    _tx_commands->getVersion();
                    if(a++ > 15)
                    {
                        on_pbStop_clicked("Ошибка установки. Попробуйте еще раз");
                        return;
                    }
                    delay(70);
                }
                on_pbStop_clicked("Обновление установлено");
                return;
            }
        }
        //если режим админа
        else
        {
            //если передача бутлоадера
            if(!load_param_)
            {
                _commun_display->setCurrenUpd("Проверка ключей");
                delay(1500);
                _commun_display->setCurrenUpd("Настройка логики");
                delay(1500);
                _commun_display->setCurrenUpd("Ещё чуть-чуть");
                delay(1500);
                //запрос версии прошивки
                _tx_commands->getVersion();
                delay(100);
                int a = 0;
                version_BootLoader_ExternalProgram.u32 = 0;
                //ожидание ответа
                while(version_BootLoader_ExternalProgram.u32 != version_BootLoader_InternalProgram.u32)
                {
                    _tx_commands->getVersion();
                    if(a++ > 15)
                    {
                        on_pbStop_clicked("Что-то поломалось. Попробуйте еще раз\n"
                                        "Версия:\n" +
                                        versionToString(versionExternalProgram.u32) +
                                        "\nДоступна версия:\n" +
                                        versionToString(versionInternalProgram.u32) + "\n\n"
                                         "Версия загрузчика:\n" +
                                        versionToString(version_BootLoader_ExternalProgram.u32) +
                                        "\nДоступна версия:\n" + versionToString(version_BootLoader_InternalProgram.u32));
                        return;
                    }
                    delay(70);
                }
                on_pbStop_clicked("Загрузчик установлен\n"
                                "Версия:\n" +
                                versionToString(versionExternalProgram.u32) +
                                "\nДоступна версия:\n" +
                                versionToString(versionInternalProgram.u32) + "\n\n"
                                 "Версия загрузчика:\n" +
                                versionToString(version_BootLoader_ExternalProgram.u32) +
                                "\nДоступна версия:\n" + versionToString(version_BootLoader_InternalProgram.u32));
                return;

            }
            //передача основной прошивки
            else
            {
                _commun_display->setCurrenUpd("Установка обновления");
                delay(7000);    //7c

                //запрос версии прошивки
                _tx_commands->getVersion();
                delay(100);
                int a = 0;
                versionExternalProgram.u32 = 0;
                //ожидание ответа
                while(versionExternalProgram.u32 != versionInternalProgram.u32)
                {
                    _tx_commands->getVersion();
                    if(a++ > 15)
                    {
                        on_pbStop_clicked("Что-то поломалось. Попробуйте еще раз\n"
                                        "Версия:\n" +
                                        versionToString(versionExternalProgram.u32) +
                                        "\nДоступна версия:\n" +
                                        versionToString(versionInternalProgram.u32) + "\n\n"
                                         "Версия загрузчика:\n" +
                                        versionToString(version_BootLoader_ExternalProgram.u32) +
                                        "\nДоступна версия:\n" + versionToString(version_BootLoader_InternalProgram.u32));
                        return;
                    }
                    delay(70);
                }
                on_pbStop_clicked("Обновление установлено\n"
                                "Версия:\n" +
                                versionToString(versionExternalProgram.u32) +
                                "\nДоступна версия:\n" +
                                versionToString(versionInternalProgram.u32) + "\n\n"
                                 "Версия загрузчика:\n" +
                                versionToString(version_BootLoader_ExternalProgram.u32) +
                                "\nДоступна версия:\n" + versionToString(version_BootLoader_InternalProgram.u32));
                return;
            }
        }
    }
}


//! Открыть файл
int
UpdateHex::on_pbOpenFile_clicked(QString name)
{
      QFile file(name);
      file.open(QIODevice::ReadOnly);
      if (file.isOpen()) {
              _bin = file.readAll();
              return 1;
      } else {
              name = QString();
              _bin.clear();
              return 0;
      }
}

//! Формировать пакет
void
UpdateHex::sendPage()
{
    QByteArray pack(_size, 0xFF);
    for (int i = 0; i < _size; i++) {
        int binPtr = i + _page * _size;
        if (binPtr < _bin.size()) pack[i] = _bin[binPtr];
        else break;
    }

    f_value header;
    header.u16[1] = _page;
    header.u16[0] = _pages;
    pack.prepend(header.data[0]).prepend(header.data[1]).prepend(header.data[2]).prepend(header.data[3]);
    pack.prepend(_crc32_Internal.data[0]).prepend(_crc32_Internal.data[1]).prepend(_crc32_Internal.data[2]).prepend(_crc32_Internal.data[3]);
    //если загрузка бутлоадера
    if(load_param_ == false)
    {
        _tx_commands->writeBootloader(pack);   // послать пакет
    }
    //если загрузка основной прошивки
    else
    {
        _tx_commands->writeProgram(pack);   // послать пакет
    }

    //если обычный режим
    if(!_f_Admin)
    {
        //если передача основной прошивки
        if(load_param_)
        {
            _commun_display->setCurrenUpd(tr("Передача пакета [ %1 / %2 ]").arg(_page + 1).arg(_pages + 1));
        }
        //если передача бутлоадера
        else
        {
            _commun_display->setCurrenUpd("Подготовка устройства...");
        }

    }
    //режим админа
    else
    {
        _commun_display->setCurrenUpd(tr("Передача пакета [ %1 / %2 ]").arg(_page + 1).arg(_pages + 1) + "\n" + tr("Кол-во повторов [ %1 ]").arg(_unsuccessful_transfers));
    }

}
