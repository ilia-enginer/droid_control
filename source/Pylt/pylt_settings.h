#ifndef PYLT_SETTINGS_H
#define PYLT_SETTINGS_H

#include <QObject>

struct FullParam {
    qint32 ch1_joy1_y_min;
    qint32 ch1_joy1_y_max;
    qint32 ch2_joy1_x_min;
    qint32 ch2_joy1_x_max;

    qint32 ch3_joy2_y_min;
    qint32 ch3_joy2_y_max;
    qint32 ch4_joy2_x_min;
    qint32 ch4_joy2_x_max;

    qint32 ch5_min;
    qint32 ch5_max;

    qint32 ch6_min;
    qint32 ch6_max;

    bool boot_fixing;
    bool tank_mode;

    //??? добавить строки с инверсией
};


class Pylt_settings: public QObject
{
    Q_OBJECT
public:
    explicit Pylt_settings(QObject *parent = 0);
    ~Pylt_settings();

    Q_PROPERTY(qint32 ch1_joy1_y_min READ get_ch1_joy1_y_min WRITE set_ch1_joy1_y_min NOTIFY ch1_joy1_y_min_chang)
    Q_PROPERTY(qint32 ch1_joy1_y_max READ get_ch1_joy1_y_max WRITE set_ch1_joy1_y_max NOTIFY ch1_joy1_y_max_chang)
    Q_PROPERTY(qint32 ch2_joy1_x_min READ get_ch2_joy1_x_min WRITE set_ch2_joy1_x_min NOTIFY ch2_joy1_x_min_chang)
    Q_PROPERTY(qint32 ch2_joy1_x_max READ get_ch2_joy1_x_max WRITE set_ch2_joy1_x_max NOTIFY ch2_joy1_x_max_chang)

    Q_PROPERTY(qint32 ch3_joy2_y_min READ get_ch3_joy2_y_min WRITE set_ch3_joy2_y_min NOTIFY ch3_joy2_y_min_chang)
    Q_PROPERTY(qint32 ch3_joy2_y_max READ get_ch3_joy2_y_max WRITE set_ch3_joy2_y_max NOTIFY ch3_joy2_y_max_chang)
    Q_PROPERTY(qint32 ch4_joy2_x_min READ get_ch4_joy2_x_min WRITE set_ch4_joy2_x_min NOTIFY ch4_joy2_x_min_chang)
    Q_PROPERTY(qint32 ch4_joy2_x_max READ get_ch4_joy2_x_max WRITE set_ch4_joy2_x_max NOTIFY ch4_joy2_x_max_chang)

    Q_PROPERTY(qint32 ch5_min READ get_ch5_min WRITE set_ch5_min NOTIFY ch5_min_chang)
    Q_PROPERTY(qint32 ch5_max READ get_ch5_max WRITE set_ch5_max NOTIFY ch5_max_chang)

    Q_PROPERTY(qint32 ch6_min READ get_ch6_min WRITE set_ch6_min NOTIFY ch6_min_chang)
    Q_PROPERTY(qint32 ch6_max READ get_ch6_max WRITE set_ch6_max NOTIFY ch6_max_chang)

    Q_PROPERTY(bool boot_fixing READ get_boot_fixing WRITE set_boot_fixing NOTIFY boot_fixing_chang)
    Q_PROPERTY(bool tank_mode READ get_tank_mode WRITE set_tank_mode NOTIFY tank_mode_chang)

    void set_ch1_joy1_y_min(qint32 value);
    qint32 get_ch1_joy1_y_min();
    void set_ch1_joy1_y_max(qint32 value);
    qint32 get_ch1_joy1_y_max();

    void set_ch2_joy1_x_min(qint32 value);
    qint32 get_ch2_joy1_x_min();
    void set_ch2_joy1_x_max(qint32 value);
    qint32 get_ch2_joy1_x_max();

    void set_ch3_joy2_y_min(qint32 value);
    qint32 get_ch3_joy2_y_min();
    void set_ch3_joy2_y_max(qint32 value);
    qint32 get_ch3_joy2_y_max();

    void set_ch4_joy2_x_min(qint32 value);
    qint32 get_ch4_joy2_x_min();
    void set_ch4_joy2_x_max(qint32 value);
    qint32 get_ch4_joy2_x_max();

    void set_ch5_min(qint32 value);
    qint32 get_ch5_min();
    void set_ch5_max(qint32 value);
    qint32 get_ch5_max();

    void set_ch6_min(qint32 value);
    qint32 get_ch6_min();
    void set_ch6_max(qint32 value);
    qint32 get_ch6_max();

    void set_boot_fixing(bool value);
    bool get_boot_fixing();
    void set_tank_mode  (bool value);
    bool get_tank_mode  ();

    FullParam getFullParam();

public slots:
    int saveFullParam();

signals:
    void ch1_joy1_y_min_chang();
    void ch1_joy1_y_max_chang();
    void ch2_joy1_x_min_chang();
    void ch2_joy1_x_max_chang();

    void ch3_joy2_y_min_chang();
    void ch3_joy2_y_max_chang();
    void ch4_joy2_x_min_chang();
    void ch4_joy2_x_max_chang();

    void ch5_min_chang();
    void ch5_max_chang();

    void ch6_min_chang();
    void ch6_max_chang();

    void boot_fixing_chang();
    void tank_mode_chang();

private:
    // дефолтные значения пользовательского вида из которых будут получены значения для пульта
    qint32 ch1_joy1_y_min = 1000;
    qint32 ch1_joy1_y_max = 2000;
    qint32 ch2_joy1_x_min = 1000;
    qint32 ch2_joy1_x_max = 2000;

    qint32 ch3_joy2_y_min = 1000;
    qint32 ch3_joy2_y_max = 2000;
    qint32 ch4_joy2_x_min = 1000;
    qint32 ch4_joy2_x_max = 2000;

    qint32 ch5_min        = 1000;
    qint32 ch5_max        = 2000;

    qint32 ch6_min        = 1000;
    qint32 ch6_max        = 2000;

    bool boot_fixing      = false;
    bool tank_mode        = false;

};

#endif // PYLT_SETTINGS_H
