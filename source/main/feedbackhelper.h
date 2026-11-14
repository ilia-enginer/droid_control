#ifndef FEEDBACKHELPER_H
#define FEEDBACKHELPER_H

#include <QObject>
class FeedbackHelper : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE void openFeedbackScreen(); // Этот метод будет вызываться из QML
};


#endif // FEEDBACKHELPER_H
