#ifndef TX_COMMANDSPYLT_H
#define TX_COMMANDSPYLT_H

#include <QObject>

class Tx_commandsPylt: public QObject
{
    Q_OBJECT

public:
    explicit Tx_commandsPylt(QObject *parent = 0);
    ~Tx_commandsPylt();


public slots:
    int joysticActivity(quint8 y1, quint8 x1, quint8 y2, quint8 x2, bool boot1, bool boot2);    //активность джойстика

private:

};

#endif // TX_COMMANDSPYLT_H
