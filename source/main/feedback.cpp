
#include "feedback.h"
#include <QDebug>
#include "../smtp/mimetext.h"
#include "../smtp/mimemessage.h"
#include "../smtp/smtpclient.h"


Feedback::Feedback(QObject *parent) :
    QObject(parent)
{
    MimeMessage message;
    EmailAddress sender("ilia.zhuc@yandex.ru", "roboshar");
    message.setSender(sender);

    EmailAddress to("zhuckov.ilia@yandex.ru", "Илья");
    message.addRecipient(to);

//    message.setSubject("SmtpClient for Qt - Demo");

}

Feedback::~Feedback()
{

}

int
Feedback::sendFedback(QString text)
{
    MimeMessage message;
    MimeText messageText;

    qDebug() << text;

    messageText.setText(text);
    message.addPart(&messageText);

    // Now we can send the mail
    SmtpClient smtp("smtp.yandex.ru", 465, SmtpClient::SslConnection);

    smtp.connectToHost();
    if (!smtp.waitForReadyConnected()) {
        qDebug() << "Failed to connect to host!";
        return -1;
    }

    smtp.login("ilia.zhuc@yandex.ru", "password");
    if (!smtp.waitForAuthenticated()) {
        qDebug() << "Failed to login!";
        return -2;
    }

    smtp.sendMail(message);
    if (!smtp.waitForMailSent()) {
        qDebug() << "Failed to send mail!";
        return -3;
    }

    emit messFinish();

    return 1;
}
