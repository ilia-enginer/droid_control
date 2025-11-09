#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QObject>
#include <QWidget>

class Feedback: public QObject
{
    Q_OBJECT

public:
    explicit Feedback(QObject *parent = 0);
    ~Feedback();


public slots:
    int sendFedback(QString text);

Q_SIGNALS:

    void messFinish();


private:


};

#endif // FEEDBACK_H


