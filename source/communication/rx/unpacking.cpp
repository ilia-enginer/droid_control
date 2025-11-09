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
    int res;
    const std::string empty;

    if(!data.isEmpty())
    {
        dataRes = data;
        //вывод лога, если надо
        if(_settings->getLoging())
        {
            _commDisplay->logJoy("<- ", QString ("%1 (%2 size)").arg(QString(data.toHex())).arg(size));
            _commDisplay->logServis("<- ", QString ("%1 (%2 size)").arg(QString(data.toHex())).arg(size));
        }
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
        f_value val;
        val.data[0] = dataRes[size-2];
        val.data[1] = dataRes[size-1];
        quint16 crc_recieved = val.U16;

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

//проверка данных в массиве темп
int
Unpacking::checkBalance()
{
    const std::string empty;

    //если есть остаток и первый символ - начало пакета
    if(Temp.isEmpty())      return 0;

    for (quint8 i = 0; i < Temp.size(); i++)
    {
        if(Temp[i] == 0x55 &&
           Temp[i-1] == 0x2F)
        {
            //поместить часть сообщения из Temp в начало recievedData
            dataRes.replace(0, dataRes.size(), empty);
            for(qint32 k = i; k >= 0; k--) dataRes.prepend(Temp[k]);

            //если дальше не лежит еще один пакет
            if(Temp.size() > i+2 &&
               Temp[i + 1] != 0x1F)
            {
                Temp.replace(0, Temp.size(), empty);
            }
            else    Temp.replace(0, i+1, empty);

            return 1;
        }
    }

    return 0;
}

//склейка разорванных пакетов
int
Unpacking::gluingPackages()
{

   const std::string empty;

   for (quint8 i = 0; i < dataRes.size(); i++)
   {
     //если пакет не начат ищу начало
     if (statys == false)
     {
       if (dataRes[i] == 0x1F)
       {
         //удалить все до 1F
         dataRes.replace(0, i, empty);
         statys = true;
       }
     }
     //если пакет начат
     else if (statys == true)
     {
       //если найден конец сообщения
       if (dataRes[i] == 0x55 &&
           dataRes[i - 1] == 0x2F &&
           !split &&
           dataRes.size() < max_rx_size)
       {
           //если дальше идет начало посылки
           if(dataRes.size() > i+2 &&
                   dataRes[i+1] == 0x1F)
           {
               Temp.replace(0, Temp.size(), empty);
               //поместить recievedData в Temp
               for(qint32 k = i+1; k < dataRes.size(); k++) Temp.append(dataRes[k]);
               //удаляю в dataRes ненужный хвост
               dataRes.replace(i+1, dataRes.size()-i, empty);
               split = true;
               return 1;
           }
           //если дальше идет не начало посылки
           if(dataRes.size() > i+2 &&
                   dataRes[i+1] != 0x1F)
           {
               dataRes.replace(i+1, dataRes.size(), empty);

               split = false;
               return 1;
           }
           else
           {
               split = false;
               return 1;
           }
       }
       //если найден конец сообщения, сообщение было разделено
       else if(dataRes[i] == 0x55 &&
               dataRes[i - 1] == 0x2F &&
               split &&
               (dataRes.size() + Temp.size()) < max_rx_size)
       {

            //переместить все из temp
           if(!Temp.isEmpty())
           {
                for(qint32 k = Temp.size()-1; k >= 0; k--) dataRes.prepend(Temp[k]);
                Temp.replace(0, Temp.size(), empty);
           }
             //поиск целикового пакета
            for(qint32 k = 0; k < dataRes.size(); k++)
            {
                //если найден конец сообщения
                if (dataRes[k] == 0x55 &&
                    dataRes[k - 1] == 0x2F)
                {
                    //все что после поместить в темп
                    if(dataRes.size() > k+1)
                    {
                        for(qint32 q = k+2; q < dataRes.size(); q++) Temp.append(dataRes[q]);
                        split = true;
                    }
                    else    split = false;

                    dataRes.replace(k+1, dataRes.size(), empty);
                    return 1;
                }

            }
       }
       //если превышена максимальная длина пакета
       else if(dataRes.size() + Temp.size() >= max_rx_size)
       {
           dataRes.replace(0, dataRes.size(), empty);
           Temp.replace(0, Temp.size(), empty);
           split = false;
           statys = false;  
           return -1;
       }
       //если конец пакета - надо склеить
       else if (dataRes.size() == i + 1)
       {
           //поместить recievedData в Temp
           for(qint32 k = 0; k < dataRes.size(); k++) Temp.append(dataRes[k]);

            dataRes.replace(0, dataRes.size(), empty);
            split = true;
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

