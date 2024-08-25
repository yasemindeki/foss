#include "rsacrypto.h"
#include <openssl/pem.h>
#include <openssl/pkcs12.h>

#include <QDebug>


RSACrypto::RSACrypto(const QString &filename, QObject *parent) : QObject(parent)
{
    this->publicKey= NULL;
    this->privateKey= NULL;

    this->getPublicKey(filename);
}

RSACrypto::~RSACrypto()
{
    this->deleteKey(&this->publicKey);
    this->deleteKey(&this->privateKey);
}

void RSACrypto::deleteKey(EVP_PKEY **pkey)
{
    if (*pkey != NULL)
    {
        EVP_PKEY_free(*pkey);
        *pkey = NULL;
    }
}

bool RSACrypto::encrypt(const QString &dataToEncrypt, QByteArray *baOutput)
{
    QByteArray baEncrypted;
    size_t outputSize = 4096;
    baEncrypted.resize(4096);

    int result = false;

    if (this->publicKey)
    {
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(this->publicKey, NULL);
        if (ctx)
        {
            result = EVP_PKEY_encrypt_init(ctx);
            if (result)
            {
                result = EVP_PKEY_CTX_set_rsa_padding(ctx, EVP_PADDING_PKCS7);
                if (result)
                {
                    result = EVP_PKEY_encrypt(ctx, (unsigned char *)baEncrypted.data(), &outputSize, (const unsigned char *)dataToEncrypt.toStdString().c_str(), dataToEncrypt.size());
                    if (result)
                    {
                        baEncrypted.resize(outputSize);
                        *baOutput = baEncrypted.toBase64();
                        qDebug() << "EVP:Encrypted output base64 is " << baEncrypted.toBase64();
                    }
                } // rsa params
            } // init

            EVP_PKEY_CTX_free(ctx);
        } // ctx
    } //getPublicKey

    return result;
}

bool RSACrypto::decrypt(QByteArray baInput, QByteArray *baOutput)
{
    QByteArray baEncrypted = QByteArray::fromBase64(baInput);
    size_t decryptedSize;

    int result = false;

    this->getPrivateKey();
    if (this->privateKey)
    {
        EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(this->privateKey, NULL);
        if (ctx)
        {
            result = EVP_PKEY_decrypt_init(ctx);
            if (result)
            {
                result = EVP_PKEY_decrypt(ctx, NULL, &decryptedSize, (const unsigned char *)baEncrypted.constData(), baEncrypted.size());
                if (result)
                {
                    baOutput->resize(decryptedSize);
                    result = EVP_PKEY_decrypt(ctx, (unsigned char *)baOutput->data(), &decryptedSize, (const unsigned char *)baEncrypted.constData(), baEncrypted.size());
                    if (result)
                    {
                        baOutput->resize(decryptedSize);
                        qDebug() << "EVP:Decrypted output is " << *baOutput;
                    }
                }
            } // init

            EVP_PKEY_CTX_free(ctx);
        } // ctx

        this->deleteKey(&this->privateKey);
    }

    return result;
}

bool RSACrypto::getPublicKey(const QString &filename)
{
    bool hasPublicKey = false;

    FILE *keyFile = fopen(filename.toStdString().c_str(), "r");
    if (keyFile)
    {
        this->publicKey = PEM_read_PUBKEY(keyFile, NULL, NULL, NULL);
        if (this->publicKey)
            hasPublicKey = true;
        else
            qCritical() << "EVP:PEM_read_PUBKEY() is failed to get public key";

        fclose(keyFile);
    }

    return hasPublicKey;
}

bool RSACrypto::getPrivateKey()
{
    int rc = false;
    OpenSSL_add_all_algorithms();

    FILE *p12File = fopen("yarname.p12", "rb");
    if (p12File)
    {
        PKCS12 *p12 = d2i_PKCS12_fp(p12File, NULL);
        fclose(p12File);

        if (p12)
        {
            QString passin = "BOKçditskmeyçşkytsi0)8013";

            this->privateKey = NULL;
            X509 *cert = NULL;
            STACK_OF(X509) *chain = NULL;

            int result = PKCS12_parse(p12, passin.toLocal8Bit(), &this->privateKey, &cert, &chain);
            if (result)
            {
                /*
                FILE *keyFile = fopen("test.key", "w");
                if (keyFile)
                {
                    result = PEM_write_PrivateKey(keyFile, this->privateKey, NULL, NULL, 0, NULL, NULL);
                    fclose(keyFile);
                }
                */

                rc = true;
            }

            PKCS12_free(p12);
        }
    }


    return rc;

}
