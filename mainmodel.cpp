

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

#include "mainmodel.h"
#include "device.h"


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

    emit onCurrentDeviceNameChanged(name);
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
    emit VrealChanged();
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
    emit coxaAnglChanged();
}

int MainModel::getfemurAngl() const
{
    return femurAngl;
}

void MainModel::setfemurAngl(int newfemurAngl)
{
    femurAngl = newfemurAngl;
    emit femurAnglChanged();
}

int MainModel::gettibaAngl() const
{
    return tibaAngl;
}

void MainModel::settibaAngl(int newtibaAngl)
{
    tibaAngl = newtibaAngl;
    emit tibaAnglChanged();
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

    if (!device_) {
      return;
    }


    if(empty == device_->getLastConnectedDevice()) return;

    setCurrenUpdate("Подготовка к обновлению...");
    emit checkUpdateProgress();

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
                setCurrenUpdate("Слишком низкое напряжение. Обновление невозможно");
                emit checkUpdate();
                return;
            }
            else
            {
                setCurrenUpdate("Внимание! Низкое напряжение");
                delay(3000);
            }
        }
    }

    setCurrenUpdate("Проверка обновлений...");
    versionExternalProgram.u32 = 0;
    //запрос версии прошивки шара
    device_->sendMessageAndWrap(0xf2, "");
    //ожидание ответа
    delay(50);
    while(versionExternalProgram.u32 == 0)
    {
        device_->sendMessageAndWrap(0xf2, "");
        if(a++ > 6)
        {
            setCurrenUpdate("Ошибка\nпроверьте подключение к устройству");
            emit checkUpdate();
            return;
        }
        delay(50);
    }

    //если версия еще не считана
    if(versionInternalProgram.u32 == 0)
    {
        //открытие файла с прошивкой из apk
        if(!on_pbOpenFile_clicked())
        {
            setCurrenUpdate("Ошибка\nне удалось открыть файл прошивки");
            emit checkUpdate();
            return;
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

    //если обычный режим
    if(adminTapCount != -1)
    {
        //если прошивка из apk свежее
        if(versionInternalProgram.u32 > versionExternalProgram.u32)
        {
            setCurrenUpdate("Доступно обновление\nВерсия шара:\n" + versionToString(versionExternalProgram.u32) + "\nДоступна версия:\n" + versionToString(versionInternalProgram.u32) + "\nОбновить?");
            //включить кнопку загрузки прошивки
            emit updateAvailable();
        }
        else
        {
            setCurrenUpdate("Установлена последняя версия\nВерсия шара:\n" + versionToString(versionExternalProgram.u32) + "\nДоступна версия:\n" + versionToString(versionInternalProgram.u32));
            emit checkUpdate();
        }
    }
    //если режим админа
    else
    {
        setCurrenUpdate("Версия шара:\n" + versionToString(versionExternalProgram.u32) + "\nДоступна версия:\n" + versionToString(versionInternalProgram.u32));
        //включить кнопку загрузки прошивки
        emit updateAvailable();
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
void MainModel::on_pbWrite_clicked()
{
    //включить индикатор загрузки
    //и кнопку остановки
    emit stopUpdate();

    _pages = _bin.size() / _size;
    _page = 0;
    _pageTx = -1;
    _unsuccessful_transfers = 0;

    //посылаю команду начала передачи
     device_->sendMessageAndWrap(0xf3, "");
     delay(50);
     device_->sendMessageAndWrap(0xf3, "");
     delay(3000);

    //включить таймер
    txPageTimerOn();
}

//! Прервать запись
void MainModel::on_pbStop_clicked(QString error)
{
    //выключить таймер
    txPageTimerOff();
    checkUpdate();
    setCurrenUpdate(error + "Проверьте обновление");
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
            setCurrenUpdate("Неизвестная ошибка передачи");
            txPageTimerOff();
            emit checkUpdate();
            return;
        }
        _unsuccessful_transfers++;
        if(_unsuccessful_transfers > 40)
        {
            setCurrenUpdate("Ошибка: превышено количество попыток передачи");
            txPageTimerOff();
            emit checkUpdate();
            return;
        }
        sendPage();
    }
    else if(_pageTx > _pages)
    {
        setCurrenUpdate("Странная ошибка передачи");
        txPageTimerOff();
        emit checkUpdate();
        return;
    }

    //если все пакеты переданы
    if((_pageTx == _page) && (_pageTx == _pages))
    {
        //выключить таймер
        txPageTimerOff();

        setCurrenUpdate("Установка обновления");
        delay(5000);    //5c

        //запрос версии прошивки шара
        device_->sendMessageAndWrap(0xf2, "");
        delay(50);
        int a = 0;
        //ожидание ответа
        while(versionExternalProgram.u32 != versionInternalProgram.u32)
        {
            device_->sendMessageAndWrap(0xf2, "");
            if(a++ > 8)
            {
                setCurrenUpdate("Ошибка установки");
                emit checkUpdate();
                return;
            }
            delay(50);
        }
        setCurrenUpdate("Обновление установлено");
        emit checkUpdate();
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
    pack.prepend(0xF3);
//    qDebug() << pack.toHex();

    device_->sendUpdate(pack);        // послать пакет

    //если обычный режим
    if(adminTapCount != -1)
    {
        setCurrenUpdate(tr("Передача пакета [ %1 / %2 ]").arg(_page + 1).arg(_pages + 1));
    }
    //режим админа
    else
    {
        setCurrenUpdate(tr("Передача пакета [ %1 / %2 ]").arg(_page + 1).arg(_pages + 1) + "\n" + tr("Кол-во повторов [ %1 ]").arg(_unsuccessful_transfers));
    }

}

void MainModel::setDevice(Device *device)
{
    device_ = device;
}


//! Открыть файл
int MainModel::on_pbOpenFile_clicked()
{
    QString _fileName = ":/bin/roboshar_v.bin"; // (из ресурсов)
      QFile file(_fileName);
      file.open(QIODevice::ReadOnly);
      if (file.isOpen()) {
              _bin = file.readAll();
              return 1;
      } else {
              _fileName = QString();
              _bin.clear();
              return 0;
      }
}

void MainModel::setPageTx(qint32 num)
{
    _pageTx = num;
}

void MainModel::setCurrenUpdate(const QString &message)
{
  currenUpdate = message;
    emit CurrenUpdateChanged(message);
}

void MainModel::setVersExt(quint32 version)
{
    versionExternalProgram.u32 = version;
}

void MainModel::setVersBootLoaderExt(quint32 version)
{
    version_BootLoader_ExternalProgram.u32 = version;
}

