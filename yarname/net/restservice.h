#ifndef RESTSERVICE_H
#define RESTSERVICE_H

#include <QObject>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
QT_END_NAMESPACE

class RESTService : public QObject
{
    Q_OBJECT
public:
    explicit RESTService(QObject *parent = nullptr);
    static RESTService* instance();

    void sendLoginRequest(const QString &id);
    void sendLoginRequest(const QString &id, const QString &otp);

private:
    QNetworkAccessManager manager;

signals:
    void networkError(QNetworkReply::NetworkError);
    void loginError(const QString &);
    void ok();
    void getStarted();

public slots:
    void loginAttemptReplied();
    void otpValidationReplied();

};

#endif // RESTSERVICE_H
