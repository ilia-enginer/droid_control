#ifndef CRC_H
#define CRC_H

#include <QObject>
#include <QWidget>


union f_value {
    float f;
    quint8 data[4];
    quint16 u16[2];
    quint16 U16;
    quint32 u32;
    int     int32;
};

class Crc: public QObject
{
    Q_OBJECT

public:

    explicit Crc(QObject *parent = 0);
    ~Crc();

    qint16 crc16(const QByteArray &arr);
    uint_least32_t crc32(const QByteArray &buf, qint32 format_buf);

};

#endif // CRC_H
