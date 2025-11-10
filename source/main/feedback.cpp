
#include "feedback.h"
#include <QDebug>
#include "../smtp/mimetext.h"
#include "../smtp/mimemessage.h"
#include "../smtp/smtpclient.h"


Feedback::Feedback(QObject *parent) :
    QObject(parent)
{

}

Feedback::~Feedback()
{

}

int
Feedback::sendFedback(QString text)
{
   MimeMessage message;
   MimeText Text;

   EmailAddress sender("your_email_address@host.com", "Your Name");
   message.setSender(sender);

   EmailAddress to("recipient@host.com", "Recipient's Name");
   message.addRecipient(to);

   message.setSubject("SmtpClient for Qt - Demo");

   // Now add some text to the email.
   // First we create a MimeText object.

   Text.setText(text);

   // Now add it to the mail

   message.addPart(&Text);

   // Now we can send the mail
   SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);


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
