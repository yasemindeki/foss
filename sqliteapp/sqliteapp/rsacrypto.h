#ifndef RSACRYPTO_H
#define RSACRYPTO_H

#include <QObject>
#include "openssl/evp.h"

class RSACrypto : public QObject
{
    Q_OBJECT
public:
    explicit RSACrypto(const QString &filename, QObject *parent = nullptr);
    ~RSACrypto();

    bool encrypt(const QString &, QByteArray *baOutput);
    bool decrypt(QByteArray baInput, QByteArray *baOutput);

private:
    EVP_PKEY *publicKey;
    EVP_PKEY *privateKey;

    bool getPublicKey(const QString &filename);
    bool getPrivateKey();

    void deleteKey(EVP_PKEY **);

signals:

};

#endif // RSACRYPTO_H
