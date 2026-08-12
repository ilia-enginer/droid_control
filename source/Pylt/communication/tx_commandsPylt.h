#ifndef TX_COMMANDSPYLT_H
#define TX_COMMANDSPYLT_H

#include <QObject>
#include "../../communication/packing.h"
#include "../pylt_settings.h"


class Tx_commandsPylt: public QObject
{
    Q_OBJECT

public:
    explicit Tx_commandsPylt(QObject *parent = 0);
    ~Tx_commandsPylt();
    void setPacking(Packing *newPacking);
    void setPylt_settings(Pylt_settings *newPylt_settings);

public slots:
    int recalculatingParameters();
    int updateParameters();

    int joystic_Activity(float x_1, float y_1, float x_2, float y_2, bool but_1, bool but_2);       // активность джойстика

    int joystic_Tx();               // передача команды управления 0x0A
    int batteryTypeRequest();       // запрос типа аккамулятора 0xA1

private:
    int joysticTank_Activity(float* y, float* x);   // активность джойстика, если управление танком

    qint32 uint_32_changing_range(qint32 old, qint32 old_min, qint32 old_max, qint32 new_min, qint32 new_max);
    float float_changing_range(float old, float old_min, float old_max, float new_min, float new_max);
    qint8 uint_8_changing_range(qint8 old, qint8 old_min, qint8 old_max, qint8 new_min, qint8 new_max);


    // посчитанные значения для передачи
    qint8 y1        = 125;
    qint8 x1        = 125;
    qint8 y2        = 125;
    qint8 x2        = 125;
    qint8 but1      = 125;
    qint8 but2      = 125;

    bool flag_calculation       = false;    // флаг означающий что идет расчет
    bool flag_deferred_transfer = false;    // флаг означающий что после расчета надо отправить данные

    // минимальные и максимальные значения для пересчета, которые принимает пульт
    FullParam valuesParam;

    Packing * _packing = nullptr;
    Pylt_settings * _pylt_settings = nullptr;

};

#endif // TX_COMMANDSPYLT_H
