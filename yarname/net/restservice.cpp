#include "restservice.h"

#include <QDebug>
#include <QNetworkAccessManager>
#include "welcomeform.h"

RESTService::RESTService(QObject *parent) : QObject(parent)
{

    //connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getResponse(QNetworkReply*)));

}

RESTService* RESTService::instance()
{
    static RESTService *_instance = 0;
    if ( _instance == 0 )
    {
        _instance = new RESTService();
    }
    return _instance;
}

void RESTService::loginAttemptReplied()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    /* nodejs express uzerinden HTTP Status kodunu
     * 200 gonderdiginde Qt NetworkReply::NoError olarak set ediyor
     * 500 gonderdiginde Qt QNetworkReply::InternalServerError olarak set ediyor
     */

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << reply->error();

        int bytes = reply->bytesAvailable();
        if (bytes == 0)
            emit networkError(reply->error());
        else
        {
            QString errorMessage = QString::fromUtf8(reply->readAll());
            emit loginError(errorMessage);
        }
    }
    else
    {
        emit ok();
    }
    qDebug() << reply->readAll();
}

void RESTService::otpValidationReplied()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << reply->error();
        emit networkError(reply->error());
    }
    else
    {
        bool ok;                    // indicates success or failure
        int  validationResult = reply->readAll().toInt(&ok);
        qDebug() << validationResult;

        if (ok)
        {
            if (validationResult == 1)
            {
                // Ana ekrana geçiş
                emit getStarted();
            }
            else
            {
                // Doğrulama sonucunu bildirme
                emit loginError("Yanlış kod girişi");
            }
        }
        else
        {

        }
    }
}

void RESTService::sendLoginRequest(const QString &id)
{
    qDebug() << id;

    QUrl url = QUrl(QString("http://localhost:3000/login/%1").arg(id));
    QNetworkRequest request(url);

    QNetworkReply *reply = manager.get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(loginAttemptReplied()));
}

void RESTService::sendLoginRequest(const QString &id, const QString &otp)
{
    qDebug() << id;
    qDebug() << otp;

    QUrl url = QUrl(QString("http://localhost:3000/login/%1/%2").arg(id).arg(otp));
    QNetworkRequest request(url);

    QNetworkReply *reply = manager.get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(otpValidationReplied()));
}
