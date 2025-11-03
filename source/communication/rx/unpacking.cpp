#include "unpacking.h"
#include <QByteArray>
#include <string.h>
#include <QCoreApplication>


Unpacking::Unpacking(QObject *parent) :
    QObject(parent)
{

}

Unpacking::~Unpacking()
{

}

void
Unpacking::setCrc(Crc *newCrc)
{
    _crc = newCrc;
}

void
Unpacking::setCommun_display(Commun_display *newCommun_display)
{
    _commDisplay = newCommun_display;
}

void
Unpacking::setRx_commands(Rx_commands *newRx_commands)
{
    _rx_commands = newRx_commands;
}

void
Unpacking::setSettings(Settings *newSettings)
{
    _settings = newSettings;
}

int
Unpacking::unpack(QByteArray data, int size)
{
    Q_UNUSED(size);
    dataRes = data;
    int res;
    const std::string empty;

    //вывод лога, если надо
    if(_settings->getLoging())
    {
        _commDisplay->logJoy("<- ", QString ("%1 (%2 size)").arg(QString(data.toHex())).arg(size));
        _commDisplay->logServis("<- ", QString ("%1 (%2 size)").arg(QString(data.toHex())).arg(size));
    }

    //склейка разорванных пакетов
    res = gluingPackages();

    //превышена длина пакета
    if(res == -1)
    {
        _commDisplay->logServis("<- превышена длина входящего пакета", dataRes.toHex());
        dataRes.replace(0, dataRes.size(), empty);  //очистка массива
    }
    //проверка crc
    else if(res == 1)
    {
        //удаление авроры
        auroraDelete();

        //проверка crc
        size = dataRes.size();
        quint16 crc_recieved = (dataRes[size-1] << 8) | dataRes[size-2];
        dataRes.replace(size-2, 2, empty);
        quint16 crc = _crc->crc16(dataRes);

        if (crc == crc_recieved)
        {
            //попытка распарсить посылку
            if(_rx_commands->searchCommand(dataRes) == -1)
            {
                //если не удалось распарсить ответ
                _commDisplay->logServis("<- неизвестный ответ на команду", dataRes.toHex());
                res = -3;
            }
        }
        else
        {
            //ошибка crc
            _commDisplay->logServis("<- no crc ", dataRes.toHex());
            res = -2;
        }
    }
    else
      res = 0;

    dataRes.replace(0, dataRes.size(), empty);  //очистка массива
    return res;
}

//склейка разорванных пакетов
int
Unpacking::gluingPackages()
{
   const std::string empty;

   for (quint8 i = 0; i < dataRes.size(); i++)
   {
     //если пакет не начат
     if (statys == false)
     {
       if (dataRes[i] == 0x1F)
       {
         startByteIndex = i;
         statys = true;
       }
     }
     //если пакет начат
     else if (statys == true)
     {
       //если найден конец первой посылки
       if (i + 1 < dataRes.size() &&
           dataRes[i] == 0x2F &&
           dataRes[i + 1] == 0x55 &&
           i > (startByteIndex + 1) && split == false &&
           dataRes.size() < max_rx_size)
       {
        //удалить все до 1F в recievedData
        if(startByteIndex > 0)
        {
            dataRes.replace(0, startByteIndex, empty);
        }
         statys = false;
         startByteIndex = 0;
         i = dataRes.size();
         return 1;
       }
       //если найден конец и пакет был разделен
       else if(i + 1 < dataRes.size() &&
               dataRes[i] == 0x2F &&
               dataRes[i + 1] == 0x55 &&
               split == true &&
               (dataRes.size() + Temp.size()) < max_rx_size)
       {
            //поместить Temp в начало recievedData
      //     for(qint32 k = Temp.size()-1; k >= 0; k--) recievedData.insert(0, Temp[k]);
           for(qint32 k = 0; k < dataRes.size(); k++) Temp.append(dataRes[k]);
           dataRes.replace(0, dataRes.size(), empty);
           for(qint32 k = 0; k < Temp.size(); k++) dataRes.append(Temp[k]);

            split = false;
            statys = false;
            startByteIndex = 0;
            Temp.replace(0, Temp.size(), empty);
            i = dataRes.size();
            return 1;
       }
       //если превышена максимальная длина пакета
       else if(dataRes.size() + Temp.size() >= max_rx_size)
       {
    //       Temp.replace(0, Temp.size(), std::empty);
           Temp.replace(0, Temp.size(), empty);
           startByteIndex = 0;
           split = false;
           statys = false;
           i = dataRes.size();
           dataRes.replace(0, dataRes.size(), empty);
           return -1;
       }
       //если конец пакета - надо склеить
       else if (i + 2 >= dataRes.size())
       {
           //удалить все до 1F в recievedData
           if(startByteIndex > 0)
           {
               dataRes.replace(0, startByteIndex, empty);
           }
           //поместить recievedData в Temp
     //      for(qint32 k = recievedData.size()-1; k >= 0; k--) Temp.insert(0, recievedData[k]);
           for(qint32 k = 0; k < dataRes.size(); k++) Temp.append(dataRes[k]);

            i = dataRes.size();
            dataRes.replace(0, dataRes.size(), empty);
            split = true;
            startByteIndex = 0;
            return 0;
       }
     }
   }
   return 0;
}

//удаление старт байта, стоп байтов
//удаление задваивания 1f, 2f
void
Unpacking::auroraDelete()
{
    const std::string empty;

   //удаление старт байта, стоп байтов
   if(dataRes[0] == dataRes[1] &&
           dataRes[0] == 0x1F && dataRes.size() > 1)
    {
        dataRes.replace(0, 2, empty);
    }
    else if(dataRes[0] == 0x1F)
    {
        dataRes.replace(0, 1, empty);
    }

    if(dataRes[dataRes.size() - 1] == 0x55)
    {
        dataRes.replace(dataRes.size()-1, 1, empty);
    }
    if(dataRes[dataRes.size() - 1] == 0x2F)
    {
        dataRes.replace(dataRes.size()-1, 1, empty);
    }       

    //удаление задваивания 1f, 2f
    qint32 size = dataRes.size();
    for(qint32 i = 0; i < size-1; i++)
    {
        if(dataRes[i] == dataRes[i+1] && dataRes[i] == 0x1f)
        {
            dataRes.replace(i, 1, empty);
            size--;
            i++;
        }
        else if(dataRes[i] == dataRes[i+1] && dataRes[i] == 0x2f)
        {
            dataRes.replace(i, 1, empty);
            size--;
            i++;
        }
    }
}

