

#include <qbluetoothaddress.h>
#include <qbluetoothdevicediscoveryagent.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QDebug>
#include <QList>
#include <QByteArray>
#include <QMetaEnum>
#include <QTimer>
#include <QThread>
#include <QRegularExpression>
#include <string.h>
#include <QTime>
#include <QCoreApplication>
#include <QSettings>

#include "mainmodel.h"
#include "device.h"
#include "appmanager.h"


MainModel::MainModel()
{
    versionExternalProgram.u32 = 0;
    versionInternalProgram.u32 = 0;
}

void
MainModel::setTimer1(int value)
{
    timer1 = value;

    emit timer1Changed(value);

    if (timer1 < timer2){
        timer2 = timer1;

        emit timer2Changed(timer2);
    }
}


void
MainModel::setTimer2(int value)
{
    timer2 = value;

    emit timer2Changed(value);

    if (timer2 > timer1){
        timer1 = timer2;

        emit timer1Changed(timer1);
    }
}


int
MainModel::getTimer1()
{
   return  timer1;
}


int
MainModel::getTimer2()
{
    return timer2;
}


QString
MainModel::getCurrentDeviceName()
{
    return currenDeviceName_;
}


void
MainModel::setCurrentDeviceName(QString name)
{
    currenDeviceName_ = name;

    if(rendering_flag)    emit onCurrentDeviceNameChanged(name);
}

float MainModel::getJoystickAmplitude() const
{
    return joystickAmplitude;
}


void MainModel::setJoystickAmplitude(float newJoystickAmplitude)
{
    if (qFuzzyCompare(joystickAmplitude, newJoystickAmplitude))
        return;
    joystickAmplitude = newJoystickAmplitude;
    emit joystickAmplitudeChanged();
}



float MainModel::getheightAmplitude() const
{
    return heightAmplitude;
}

void MainModel::setheightAmplitude(float newheightAmplitude)
{
    if (qFuzzyCompare(heightAmplitude, newheightAmplitude))
        return;
    heightAmplitude = newheightAmplitude;
    emit heightAmplitudeChanged();
}

float MainModel::getheightAmplitudemin() const
{
    return heightAmplitudemin;
}

void MainModel::setheightAmplitudemin(float newheightAmplitudemin)
{
    if (qFuzzyCompare(heightAmplitudemin, newheightAmplitudemin))
        return;
    heightAmplitudemin = newheightAmplitudemin;
    emit heightAmplitudeChangedmin();
}

int MainModel::getservisIndexMenu() const
{
    return servisIndexMenu;
}

void MainModel::setservisIndexMenu(int newservisIndexMenu)
{
    servisIndexMenu = newservisIndexMenu;
}



float MainModel::getVmin() const
{
    return Vmin;
}

void MainModel::setVmin(float newVmin)
{
    if (qFuzzyCompare(Vmin, newVmin))
        return;
    Vmin = newVmin;
    emit VminChanged();
}

float MainModel::getVmax() const
{
    return Vmax;
}

void MainModel::setVmax(float newVmax)
{
    if (qFuzzyCompare(Vmax, newVmax))
        return;
    Vmax = newVmax;
    emit VmaxChanged();
}

float MainModel::getVreal() const
{
    return Vreal;
}

void MainModel::setVreal(float newVreal)
{
    if (qFuzzyCompare(Vreal, newVreal))
        return;
    Vreal = newVreal;
    if(rendering_flag)    emit VrealChanged();
}

float MainModel::getCurReal() const
{
    return CurReal;
}

void MainModel::setCurReal(float newCurReal)
{
    if (qFuzzyCompare(CurReal, newCurReal))
        return;
    CurReal = newCurReal;
    emit CurRealChanged();
}

int MainModel::getcoxaAngl() const
{
    return coxaAngl;
}

void MainModel::setcoxaAngl(int newcoxaAngl)
{
    coxaAngl = newcoxaAngl;
    if(rendering_flag)    emit coxaAnglChanged();
}

int MainModel::getfemurAngl() const
{
    return femurAngl;
}

void MainModel::setfemurAngl(int newfemurAngl)
{
    femurAngl = newfemurAngl;
    if(rendering_flag)    emit femurAnglChanged();
}

int MainModel::gettibaAngl() const
{
    return tibaAngl;
}

void MainModel::settibaAngl(int newtibaAngl)
{
    tibaAngl = newtibaAngl;
    if(rendering_flag)    emit tibaAnglChanged();
}

void
MainModel::incAdminTapCount(int value)
{
    Q_UNUSED(value)

    if(adminTapCount > 4)
    {
        adminTapCount = -1;
    }
    else if(adminTapCount >= 0)
    {
     adminTapCount ++;
    }
    emit onAdminTapCountChanged();
}

int
MainModel::getAdminTapCount()
{
    return adminTapCount;
}

QString MainModel::getCurrenUpdate()
{
    return currenUpdate;
}

///! проверка обновлений
void MainModel::checkingUpdates(void)
{
    QString empty;
    int a = 0;

    #if defined(Q_QDOC) || (defined(Q_OS_ANDROID) && !defined(Q_OS_ANDROID_EMBEDDED))
    if (!device_) {
      return;
    }


    if(empty == device_->getLastConnectedDevice()) return;

    if(rendering_flag)    setCurrenUpdate("Подготовка к обновлению...");
    if(rendering_flag)    emit checkUpdateProgress();


        //запрос напряжения шара
        Vreal = 0.0;
        device_->sendMessageAndWrap(0xa1, "");
        delay(50);
        a = 0;
        //ожидание ответа
        //если напряжение меньше 20% заряда - защита
        while((Vreal < (Vmax * 0.8)) && (a < 3))
        {
            device_->sendMessageAndWrap(0xa1, "");
            delay(50);
            if(a++ == 2)
            {
                if(adminTapCount != -1)
                {
                    if(rendering_flag)    setCurrenUpdate("Слишком низкое напряжение. Обновление невозможно");
                    if(rendering_flag)    emit checkUpdate();
                    return;
                }
                else
                {
                    if(rendering_flag)    setCurrenUpdate("Внимание! Низкое напряжение");
                    delay(3000);
                }
            }
        }

        if(rendering_flag)    setCurrenUpdate("Проверка обновлений...");
        versionExternalProgram.u32 = 0;
        version_BootLoader_ExternalProgram.u32 = 0;
        //запрос версии прошивки шара
        device_->sendMessageAndWrap(0xf2, "");
        //ожидание ответа
        delay(50);
        while(versionExternalProgram.u32 == 0)
        {
            device_->sendMessageAndWrap(0xf2, "");
            if(a++ > 6)
            {
                if(rendering_flag)    setCurrenUpdate("Ошибка\nпроверьте подключение к устройству");
                if(rendering_flag)    emit checkUpdate();
                return;
            }
            delay(50);
        }
    #endif

    if(!openBootloaderUpdate()) return;
    if(!open_Update()) return;

    //если обычный режим
    if(adminTapCount != -1)
    {
        //если прошивка из apk свежее
        //или бутлоадер свежее
        if((versionInternalProgram.u32 > versionExternalProgram.u32) || (version_BootLoader_InternalProgram.u32 > version_BootLoader_ExternalProgram.u32))
        {
            if(rendering_flag)
            {
        //        setCurrenUpdate("Доступно обновление\nВерсия шара:\n" + versionToString(versionExternalProgram.u32) + "\nДоступна версия:\n" + versionToString(versionInternalProgram.u32) + "\nОбновить?");
                setCurrenUpdate("Доступно обновление\nОбновить?");
            }
            //включить кнопку загрузки прошивки
           if(rendering_flag)     emit updateAvailable();
        }
        else
        {
            if(rendering_flag)
            {
         //       setCurrenUpdate("Установлена последняя версия\nВерсия шара:\n" + versionToString(versionExternalProgram.u32) + "\nДоступна версия:\n" + versionToString(versionInternalProgram.u32));
                setCurrenUpdate("Установлена последняя версия");
            }
            if(rendering_flag)    emit checkUpdate();
        }
    }
    //если режим админа
    else
    {
        if(rendering_flag)
        {
            setCurrenUpdate("Версия шара:\n" + versionToString(versionExternalProgram.u32) + "\nДоступна версия:\n" + versionToString(versionInternalProgram.u32) + "\n"
                            "Версия загрузчика:\n" + versionToString(version_BootLoader_ExternalProgram.u32) + "\nДоступна версия:\n" + versionToString(version_BootLoader_InternalProgram.u32));
        }

        if(rendering_flag)
        {
//            //включить кнопку загрузки прошивки
//            emit updateAvailable();
            //включить кнопку загрузки бутлоадера
            emit updateBootloaderAvailable();
        }
    }
}


QString MainModel::versionToString(quint32 vers)
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
void MainModel::on_pbWrite_clicked(bool flag)
{
    //если обычный режим
    if(adminTapCount != -1)
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

    _pages = _bin.size() / _size;
    _page = 0;
    _pageTx = -1;
    _unsuccessful_transfers = 0;

    //если надо грузить бутлоадер
    if(load_param_ == false)
    {
        //посылаю команду начала передачи
        device_->sendMessageAndWrap(0xf4, "");
        delay(50);
        device_->sendMessageAndWrap(0xf4, "");
        delay(3000);
    }
    //если надо грузить основную прошивку
    else
    {
        //посылаю команду начала передачи
        device_->sendMessageAndWrap(0xf3, "");
        delay(50);
        device_->sendMessageAndWrap(0xf3, "");
        delay(3000);
    }

     //чтоб экран не гас
     AppManager app;
     app.keepScreenOn(true);

     //включить индикатор загрузки
     //и кнопку остановки
     if(rendering_flag)    emit stopUpdate();

    //включить таймер
     txPageTimerOn();
}

qint32 MainModel::open_Update()
{
    //считывание основной вервии
    //открытие файла с прошивкой из apk
    QString _fileName = ":/bin/roboshar_v.bin"; // (из ресурсов)
    if(!on_pbOpenFile_clicked(_fileName))
    {
        if(rendering_flag)    setCurrenUpdate("Ошибка\nне удалось открыть файл прошивки");
        if(rendering_flag)    emit checkUpdate();
        return false;
    }
    //поиск версии прошивки в файле
    if (_bin.contains(getVersionLabel())) {
//            qDebug() << "address" << _bin.indexOf(getVersionLabel());
        QByteArray newVersion = _bin.mid(_bin.indexOf(getVersionLabel()) + getVersionLabel().size() + 1, 4);
        versionInternalProgram.data[3] = newVersion.at(3);
        versionInternalProgram.data[2] = newVersion.at(2);
        versionInternalProgram.data[1] = newVersion.at(1);
        versionInternalProgram.data[0] = newVersion.at(0);
//           qDebug() << QString("0x%1").arg(versionInternalProgram.u32, 8, 16, QChar('0'));
    }

    //расчет срс прошивки
//    qDebug() << _bin.size();
    int fullSize = ceil((double)_bin.size() / _size) * _size;
//    qDebug() << fullSize << "*" << _size;
    QByteArray pack_crc(fullSize, 0xFF);
    for (int i = 0; i < pack_crc.size(); i++) {
        int binPtr = i;
        if (binPtr < _bin.size()) pack_crc[i] = _bin[binPtr];
        else {
            break;
        }
    }
    _crc32_Internal.int32 = device_->crc32(pack_crc, 8);
//    qDebug() <<_crc32_Internal.int32;

    return true;
}

qint32 MainModel::openBootloaderUpdate()
{
    //считывание бутлоадера
    //открытие файла с прошивкой из apk
      QString _fileName = ":/bin/boot_loader.bin"; // (из ресурсов)
    if(!on_pbOpenFile_clicked(_fileName))
    {
        if(rendering_flag)    setCurrenUpdate("Ошибка\nне удалось открыть файл прошивки");
        if(rendering_flag)    emit checkUpdate();
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
//    qDebug() << _bin.size();
    int fullSize = ceil((double)_bin.size() / _size) * _size;
//    qDebug() << fullSize << "*" << _size;
    QByteArray pack_crc(fullSize, 0xFF);
    for (int i = 0; i < pack_crc.size(); i++) {
        int binPtr = i;
        if (binPtr < _bin.size()) pack_crc[i] = _bin[binPtr];
        else {
            break;
        }
    }
    _crc32_Internal.int32 = device_->crc32(pack_crc, 8);
//    qDebug() <<_crc32_Internal.int32;

    return true;
}

//! Прервать запись
void MainModel::on_pbStop_clicked(QString error)
{
    //
    AppManager app;
    app.keepScreenOn(false);

    //выключить таймер
    txPageTimerOff();
    if(rendering_flag)    checkUpdate();
    if(rendering_flag)    setCurrenUpdate(error + "Проверьте обновление");
}

//вызов по таймеру
void MainModel::write_page()
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
            if(rendering_flag)    setCurrenUpdate("Неизвестная ошибка передачи");
            txPageTimerOff();
            if(rendering_flag)    emit checkUpdate();
            return;
        }
        _unsuccessful_transfers++;
        if(_unsuccessful_transfers > 40)
        {
            if(rendering_flag)    setCurrenUpdate("Ошибка: превышено количество попыток передачи");
            txPageTimerOff();
            if(rendering_flag)    emit checkUpdate();
            return;
        }
        sendPage();
    }
    else if(_pageTx > _pages)
    {
        if(rendering_flag)    setCurrenUpdate("Странная ошибка передачи");
        txPageTimerOff();
        if(rendering_flag)    emit checkUpdate();
        return;
    }

    //если все пакеты переданы
    if((_pageTx == _page) && (_pageTx == _pages))
    {
        //выключить таймер
        txPageTimerOff();

        //если обычный режим
        if(adminTapCount != -1)
        {
            //если передача бутлоадера
            if(!load_param_)
            {
                setCurrenUpdate("Проверка ключей");
                delay(1000);
                setCurrenUpdate("Настройка логики");
                delay(1000);
                //запрос версии прошивки шара
                device_->sendMessageAndWrap(0xf2, "");
                delay(50);
                int a = 0;
                version_BootLoader_ExternalProgram.u32 = 0;
                //ожидание ответа
                while(version_BootLoader_ExternalProgram.u32 != version_BootLoader_InternalProgram.u32)
                {
                    device_->sendMessageAndWrap(0xf2, "");
                    if(a++ > 15)
                    {
                        if(rendering_flag)    setCurrenUpdate("Что-то поломалось. Попробуйте еще раз");
                        if(rendering_flag)    emit checkUpdate();
                        //
                        AppManager app;
                        app.keepScreenOn(false);
                        return;
                    }
                    delay(50);
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
                    if(rendering_flag)    setCurrenUpdate("Обновление установлено");
                    if(rendering_flag)    emit checkUpdate();
                    //
                    AppManager app;
                    app.keepScreenOn(false);
                    return;
                }
            }
            //если загрузка основной программы
            else
            {
                setCurrenUpdate("Установка обновления");
                delay(5000);    //5c

                //запрос версии прошивки шара
                device_->sendMessageAndWrap(0xf2, "");
                delay(50);
                int a = 0;
                versionExternalProgram.u32 = 0;
                //ожидание ответа
                while(versionExternalProgram.u32 != versionInternalProgram.u32)
                {
                    device_->sendMessageAndWrap(0xf2, "");
                    if(a++ > 15)
                    {
                        if(rendering_flag)    setCurrenUpdate("Ошибка установки. Попробуйте еще раз");
                        if(rendering_flag)    emit checkUpdate();
                        //
                        AppManager app;
                        app.keepScreenOn(false);
                        return;
                    }
                    delay(50);
                }
                if(rendering_flag)    setCurrenUpdate("Обновление установлено");
                if(rendering_flag)    emit checkUpdate();

                //
                AppManager app;
                app.keepScreenOn(false);
            }
        }
        //если режим админа
        else
        {
            //если передача бутлоадера
            if(!load_param_)
            {
                setCurrenUpdate("Проверка ключей");
                delay(1000);
                setCurrenUpdate("Настройка логики");
                delay(1000);
                //запрос версии прошивки шара
                device_->sendMessageAndWrap(0xf2, "");
                delay(50);
                int a = 0;
                version_BootLoader_ExternalProgram.u32 = 0;
                //ожидание ответа
                while(version_BootLoader_ExternalProgram.u32 != version_BootLoader_InternalProgram.u32)
                {
                    device_->sendMessageAndWrap(0xf2, "");
                    if(a++ > 15)
                    {
                        if(rendering_flag)
                        {
                            setCurrenUpdate("Что-то поломалось. Попробуйте еще раз\n"
                                            "Версия шара:\n" +
                                            versionToString(versionExternalProgram.u32) +
                                            "\nДоступна версия:\n" +
                                            versionToString(versionInternalProgram.u32) + "\n\n"
                                             "Версия загрузчика:\n" +
                                            versionToString(version_BootLoader_ExternalProgram.u32) +
                                            "\nДоступна версия:\n" + versionToString(version_BootLoader_InternalProgram.u32));
                        }
                        if(rendering_flag)    emit checkUpdate();
                        //
                        AppManager app;
                        app.keepScreenOn(false);
                        return;
                    }
                    delay(50);
                }
                if(rendering_flag)
                {
                    setCurrenUpdate("Загрузчик установлен\n"
                                    "Версия шара:\n" +
                                    versionToString(versionExternalProgram.u32) +
                                    "\nДоступна версия:\n" +
                                    versionToString(versionInternalProgram.u32) + "\n\n"
                                     "Версия загрузчика:\n" +
                                    versionToString(version_BootLoader_ExternalProgram.u32) +
                                    "\nДоступна версия:\n" + versionToString(version_BootLoader_InternalProgram.u32));
                }
                if(rendering_flag)    emit checkUpdate();
                //
                AppManager app;
                app.keepScreenOn(false);
                return;

            }
            //передача основной прошивки
            else
            {
                setCurrenUpdate("Установка обновления");
                delay(5000);    //5c

                //запрос версии прошивки шара
                device_->sendMessageAndWrap(0xf2, "");
                delay(50);
                int a = 0;
                versionExternalProgram.u32 = 0;
                //ожидание ответа
                while(versionExternalProgram.u32 != versionInternalProgram.u32)
                {
                    device_->sendMessageAndWrap(0xf2, "");
                    if(a++ > 15)
                    {
                        if(rendering_flag)
                        {
                            setCurrenUpdate("Что-то поломалось. Попробуйте еще раз\n"
                                            "Версия шара:\n" +
                                            versionToString(versionExternalProgram.u32) +
                                            "\nДоступна версия:\n" +
                                            versionToString(versionInternalProgram.u32) + "\n\n"
                                             "Версия загрузчика:\n" +
                                            versionToString(version_BootLoader_ExternalProgram.u32) +
                                            "\nДоступна версия:\n" + versionToString(version_BootLoader_InternalProgram.u32));
                        }
                        if(rendering_flag)    emit checkUpdate();
                        //
                        AppManager app;
                        app.keepScreenOn(false);
                        return;
                    }
                    delay(50);
                }
                if(rendering_flag)
                {
                    setCurrenUpdate("Обновление установлено\n"
                                    "Версия шара:\n" +
                                    versionToString(versionExternalProgram.u32) +
                                    "\nДоступна версия:\n" +
                                    versionToString(versionInternalProgram.u32) + "\n\n"
                                     "Версия загрузчика:\n" +
                                    versionToString(version_BootLoader_ExternalProgram.u32) +
                                    "\nДоступна версия:\n" + versionToString(version_BootLoader_InternalProgram.u32));
                }
                if(rendering_flag)    emit checkUpdate();

                //
                AppManager app;
                app.keepScreenOn(false);
            }
        }
    }
}


//! Формировать пакет
void MainModel::sendPage()
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
        pack.prepend(0xF4);
    }
    //если загрузка основной прошивки
    else
    {
        pack.prepend(0xF3);
    }

//    qDebug() << pack.toHex();

    device_->sendUpdate(pack);        // послать пакет

    //если обычный режим
    if(adminTapCount != -1)
    {
        //если передача основной прошивки
        if(load_param_)
        {
            if(rendering_flag)    setCurrenUpdate(tr("Передача пакета [ %1 / %2 ]").arg(_page + 1).arg(_pages + 1));
        }
        //если передача бутлоадера
        else
        {
            if(rendering_flag)    setCurrenUpdate("Подготовка устройства...");
        }

    }
    //режим админа
    else
    {
        if(rendering_flag)    setCurrenUpdate(tr("Передача пакета [ %1 / %2 ]").arg(_page + 1).arg(_pages + 1) + "\n" + tr("Кол-во повторов [ %1 ]").arg(_unsuccessful_transfers));
    }

}

void MainModel::setDevice(Device *device)
{
    device_ = device;
}


//! Открыть файл
int MainModel::on_pbOpenFile_clicked(QString name)
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

void MainModel::setPageTx(qint32 num)
{
    _pageTx = num;
}

void MainModel::set_rendering_flag(bool fl)
{
    rendering_flag = fl;
}

QString MainModel::getversion_app()
{
    return version_app;
}

void MainModel::setversion_app(QString &vers)
{
    version_app = vers;
}

QByteArray MainModel::get_full_param()
{
    if(!_full_Param.isEmpty())
    {
        return _full_Param;
    }
    QByteArray a = 0x00;
    return a;
}

void MainModel::set_full_param(QByteArray &param)
{
    _full_Param = param;
}

///проверка сохранееных параметров
/// если их нет - запросит
void MainModel::full_param_check()
{
    QSettings setting;

    //если параметры не прочитаны
    if(_full_Param.isEmpty())
    {
        //читаю из памяти
         _full_Param = setting.value("fullParam", QByteArray()).toByteArray();

         //если в памяти их нет
         if(_full_Param.isEmpty())
         {
             //на всякий. вдруг шар еще не включен
             delay(1000);
            //запрашиваю параметры с шара
            QString msg = "a7";

            device_->sendMessageAndWrapS(msg);
            delay(300);
            for(qint8 i = 0; i < 10; i++)
            {
                if(_full_Param.size() < 10) device_->sendMessageAndWrapS(msg);
                else                        i = 10;
                delay(70);
            }

            ///параметры сохранятся при получении
//            //если удалось получить с шара - сохраняю в память
//            if(_full_Param.size() > 10)
//            {
//                setting.setValue("fullParam", _full_Param);
//            }
         }
    }
}

void MainModel::setCurrenUpdate(const QString &message)
{
  currenUpdate = message;
  if(rendering_flag)    emit CurrenUpdateChanged(message);
}

void MainModel::setVersExt(quint32 version)
{
    versionExternalProgram.u32 = version;
}

void MainModel::setVersBootLoaderExt(quint32 version)
{
    version_BootLoader_ExternalProgram.u32 = version;
}

