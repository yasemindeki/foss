#include <QCoreApplication>

#include <QDebug>
#include <QDateTime>
#include <QSettings>
#include <QTextCodec>

#include "sqlite3.h"

#include "openssl/evp.h"

#include "openssl/aes.h"
#include "openssl/crypto.h"
#include "openssl/rand.h"
#include "openssl/err.h"
#include "openssl/evperr.h"

#include <openssl/pem.h>
#include <openssl/core_names.h> // INCLUDE FOR OSSL_PKEY_PARAM_RSA_BITS
#include "openssl/bio.h"

#include <openssl/pkcs12.h>

#include "rsacrypto.h"

QString encryptionKey = "BAbsgkyarnameukdvhaneAyvyo436320"; // 32 bytes * 8 = 256 bits
QString initVector = "BAidvsdo436320yy";    // 16 bytes

/*
 * \brief Brief description
 *        Brief description continued.
 *
 * Detailed description follows after an empty line.
 */
static int getSha256Text()
{
    unsigned int outLen = 0;
    QByteArray dataBuffer;
    dataBuffer.resize(EVP_MAX_MD_SIZE);

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    int rc = EVP_DigestInit(ctx, EVP_sha256());

    qDebug() << "EVP:The function is returned " << rc;

    rc = EVP_DigestUpdate(ctx, encryptionKey.toStdString().c_str(), encryptionKey.size());
    qDebug() << "EVP:The function is returned " << rc;

    rc = EVP_DigestFinal(ctx, (unsigned char *)dataBuffer.data(), &outLen);

    dataBuffer.resize(outLen);

    qDebug() << "EVP:EVP_DigestFinal() is returned " << rc;
    qDebug() << "EVP:EVP_DigestFinal() is set to outLen " << outLen;
    qDebug() << "EVP:EVP_DigestFinal is set to dataBuffer " << dataBuffer.toHex();

    return 0;
}

// EVP_sha512()
// Key length 512 bits
// Block size 1024 bits
// Baska bir anahtari sifrelemek icin kullaniyorsak ona KEK diyoruz.
// Veriyi sifrelemek icin kullaniyorsak ona DEK diyoruz.

// KURALLAR
// 1. Sifresiz KEY, veritabaninda olsun dersek, AES ile sifreyerek depolamali.
// 2. Sifresiz KEY, dosyada olsun dersek, AES ile sifreli olarak dosyaya yazmali,
// dosya sistemi yetkilerini ayarlamali ve mumkunse dosya acimina parola koymali (msword gibi ya da db manager gibi).

// ALGORITMA
// Kullanici uye olurken, yarnamelib uzerinden bir PBKDF (Password Based Key Derivation Function) ile DEK uretelim (gecici bellekte, bu asamada)
// DEK'i KMS'den aldigimiz KEK ile sifreleyip (AES 256) settings.conf dosyasinda depolayalim.
// cloud.google.com/kms/pricing
// cloud.google.com/kms/docs/reference/libraries
// console.cloud.google.com/marketplace/product/google/cloudkms.googleapis.com?project=dashboard-projec-

// KEK depolama secenekleri
// Kodda mi gomulu, db'de mi, dosyada mi, sunucuda mi, KMS'de mi ?
// Hirsizin onune cikarabilecegimiz engeller:
// * Sunucu koduna erisilirse
// * Sunucu veritabanina erisilirse
// * Sunucu private.key dosyasina erisilirse
// * Kullanicinin sifresi ele gecirilirse
// * Sunucu ile istemci arasindaki haberlesme cozulurse
// * KMS hacklenirse

// Senaryo: Kullanici sifresini unuttu... sifresini degistirdi...

/* AWS piyasanin %60'ina hakimse, en az onun kadar iyisini yapabilmek icin
 * onu gecmek niyetiyle, onu kullanalim; ama mumkun olan en tez zamanda da
 * yerli ve milli CloudKMS hizmeti sunmayi Allah bizlere nasip eylesin...
 *
 * CREATE and CONTROL keys used to encrypt or digitally sign your data.
 * Biz sadece kendi olusturacagimiz key'in kontrolunu KMS'de yapmayi hedefliyoruz.
 *
 * Start with 20.000 free requests per month with AWS Free Tier.
 * Oturum acarken KEK'e erisim olsun dersek, 1.5 milyon kullanici icin
 * 1.5 milyon KMS request demek.
 *
 * AWS Encryption SDK
 *
 * https://aws.amazon.com/kms/
 * 1. CREATE an AWS Account
 *
 * SORU: Anladigim kadariyla KMS'de depolanan key'i API uzerinden cekmek yerine
 * o key'in id'sini KMS'nin SDK'sina verip sifrelemeyi dahi KMS uzerinden yaptirmak
 * gibi bir yol secmisler.
 * Tum KMS'ler mi key'leri API'leri uzerinden cekmeye izin vermiyorlar ???
 * CEVAP: Evet, acik erisime kapali KMS'lerde. Sadece key import ozellikleri var.s
 *
 */

static int generateKey(QByteArray *baKeyOut)
{
    //QString passphrase = "BAbsgkyarnameukhvdane"; // 32 bytes
    uint iterationCount = 100000; // 100.000 kez

    QByteArray baPassword;
    baPassword.resize(32);

    QByteArray baSalt;
    baSalt.resize(16);
    baKeyOut->resize(32); // 256 bits / 8 = 32 bytes

    int rc = RAND_status();
    if (!rc)
    {
        qCritical() << "EVP:RAND_status() is failed";
        return -1;
    }

    rc = RAND_bytes((unsigned char *)baPassword.data(), baPassword.size());
    if (!rc)
    {
        qCritical() << "EVP:RAND_bytes() is failed to generate passphrase";
        return -1;
    }

    rc = RAND_bytes((unsigned char *)baSalt.data(), baSalt.size());
    if (!rc)
    {
        qCritical() << "EVP:RAND_bytes() is failed to generate salt";
        return -1;
    }

    EVP_MD *md = (EVP_MD *)EVP_sha256();

    rc = PKCS5_PBKDF2_HMAC(baPassword.toStdString().c_str(), baPassword.size(),   // sifre DIGESTSIZE byte olmali
                      (const unsigned char *)baSalt.data(), baSalt.size(),        // salt MINIMIM 16 byte olmali
                      iterationCount, md,
                      baKeyOut->size(), (unsigned char *)baKeyOut->data());         // uretilen anahtar DIGESTSIZE byte
    if (!rc)
    {
        qCritical() << "EVP:PKCS5_PBKDF2_HMAC() is failed to generate an encryption key";
        return -1;
    }

    return 0;
}

/*
 * \brief AES sifreleme yapilarinin ve fonksiyonlarinin testi
 *        OpenSSL'in bu amacla biz gelistiricilere sundugu yapilar:
 *        EVP_CIPHER_CTX,
 *        EVP_CIPHER
 *        OpenSSL'in bu amacla biz gelistiricilere sundugu fonksiyonlar:
 *        EVP_CIPHER_CTX_new,
 *        EVP_CIPHER_CTX_free,
 *        EVP_CIPHER_get_key_length,
 *        EVP_CIPHER_get_iv_length,
 *        EVP_CIPHER_get_type,
 *        EVP_CIPHER_get_nid,
 *        EVP_CIPHER_get_mode,
 *        EVP_CIPHER_fetch,
 *        EVP_CIPHER_free,
 *        EVP_EncryptInit_ex2,
 *        EVP_EncryptUpdate,
 *        EVP_EncryptFinal_ex,
 *        RAND_bytes
 *
 * EVP_CIPHER_CTX
 * Amacimiz sifreleme yapmak ise, bu struct turunden bir degisken, tum islemlerin merkezinde var.
 *
 * EVP_CIPHER_get_key_length
 * Return the key length of a cipher when passed an EVP_CIPHER. (bytes)
 *
 * EVP_CIPHER_get_nid
 * Objects in OpenSSL can have a short name, a long name and a numerical identifier (NID) associated with them.
 *
 * EVP_CIPHER_CTX_new
 * Allocates and returns a cipher context.
 *
 * EVP_CIPHER_CTX_free
 * Clears all information from a cipher context and frees any allocated memeory associated with it, including ctx itself.
 * This function should be called after all operations using a cipher are complete, so sensitive information does not remain in memory.
 *
 * EVP_EncryptInit_ex2
 * Sets up cipher context ctx for encryption with cipher type.
 * type is typically supplied by calling EVP_CIPHER_fetch().
 * type may also be set using legacy functions such as EVP_aes_256_cbc(),
 * but this is not recommended for new applications.
 *
 * EVP_CIPHER_fetch
 * Fetches the cipher implementation for the given algorithm.
 * The returned value must eventually be freed with EVP_CIPHER_free().
 *
 * EVP_CIPHER_free
 * Decrements the reference count for the fetched EVP_CIPHER structure.
 * If the reference count drops to 0 then the structure is freed.
 *
 * EVP_EncryptUpdate
 * This function encrypts inl bytes from the buffer in, and writes the encrypted version to out.
 * For most ciphers and modes, the amount of data written can be anything
 * from zero bytes to (inl + cipher_block_size - 1) bytes.
 *
 * EVP_EncryptFinal_ex
 * If padding is enabled (the default) then this function encrypts the "final" data,
 * that is any data that remains in a partial block.
 *
 * EVP_CIPHER_CTX_init
 * This function remains as an alias for EVP_CIPHER_CTX_reset().
 *
 * EVP_CIPHER_CTX_reset
 * This function should be called anytime ctx is reused
 * by another EVP_CipherInit() / EVP_CipherUpdate() / EVP_CipherFinal() series of calls.
 *
 * EVP_CIPHER_get_mode
 * evp.h dosyasinda tanimli asagidaki degerlerden birini geri donduruyor:
    # define         EVP_CIPH_STREAM_CIPHER          0x0
    # define         EVP_CIPH_ECB_MODE               0x1
    # define         EVP_CIPH_CBC_MODE               0x2
    # define         EVP_CIPH_CFB_MODE               0x3
    # define         EVP_CIPH_OFB_MODE               0x4
    # define         EVP_CIPH_CTR_MODE               0x5
    # define         EVP_CIPH_GCM_MODE               0x6
    # define         EVP_CIPH_CCM_MODE               0x7
    # define         EVP_CIPH_XTS_MODE               0x10001
    # define         EVP_CIPH_WRAP_MODE              0x10002
    # define         EVP_CIPH_OCB_MODE               0x10003
    # define         EVP_CIPH_SIV_MODE               0x10004
    # define         EVP_CIPH_MODE                   0xF0007
 *
 * RAND_bytes
 * This function puts num cryptographically strong pseudo-random bytes into buf.
 * An error occurs if the PRNG has not been seeded with enough randomness to ensure an unpredictable byte sequence.
 *
 * RAND_status
 * This function indicates whether or not the random generator has been sufficiently seeded. If not,
 * functions such as RAND_bytes() will fail.
 *
 * ASIMETRIK ANAHTAR VERI YAPILARI ve FONKSIYONLARI
 *
 * EVP_PKEY
 * to store a private key in an EVP_PKEY.
 * to describe an asymmetric key pair that can be an RSA key pair.
 *
 * EVP_PKEY_CTX
 * to store fundamental data elements (keys, padding, etc.)
 * used to process cryptographic transformations and deal with padding.
 *
 * EVP_PKEY_encrypt
 *
 * EVP_PKEY_decrypt
 *
 * EVP_PKEY_CTX_free
 *
 * PEM_read_PUBKEY
 *
 * PEM_read_PrivateKey
 *
 * BIO -- Basic I/O abstraction
 * Memory BIOs can be used immediately after valling BIO_new.
 * OpenSSL uses the a concept of BIOs which is an input/output abstraction allowing us to use data from a FILE,
 * MEMORY or NETWORK in a similar way.
 *
 * BIO_new
 *
 * BIO_s_mem
 *
 * PEM_write_bio_PUBKEY
 * The function writes a public key TO a bio using EVP_PKEY.
 *
 * PEM_read_bio_PUBKEY
 * The function reads a public key FROM a bio and returns EVP_PKEY.
 *
 * PEM_write_PUBKEY
 * The function writes a public key TO a file using EVP_PKEY.
 *
 * PEM_write_bio_PrivateKey
 * The function writes a private key TO a bio (for example file bio) using EVP_PKEY.
 * enc and u(kstr) parameter are sued for encrypting private key string.
 *
 *
 * KEKs - Key Encryption Keys - AES sifrelemede kullanilan key
 * AKs (MKs) - Authentication Keys - are encrypted using AES and stored in the server's database.
 * SUB2 is RSA encrypted and stored in the server's database.
 *
 * MIMARI
 * Veritabaninda key'ler plaintext olarak saklanmiyor. AES ile sifreleyerek saklamak tercih edilebilir.
 *
 * AK (MKs), sunucunun urettigi key - AES 256: Mesajlari sifrelemek icin kullanilan key
 * KEK1, sunucunun urettigi key - AES 256: AK'yi sifrelemek icin kullanilan key
 * KEK2, kullanicinin sifresiyle uretilen key - PBKDF2: KEK1'i sifrelemek icin kullanilan key
 *
 * KEK2 kullanilarak KEK1'in sifresi cozulsun; Elde edilen KEK1'i kullanarak AK'in sifresi cozulsun.
 * Nihayetinde elde edilen AK ile de mesajin sifresi cozulebilir olsun.
 *
 */

static void redirectLogMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString logEntry;

    QString caller(context.function);
    if (!caller.isEmpty())
    {
        QString fpItem = caller.split(' ')[1];  // fp - function prototype
        caller = fpItem.mid(0, fpItem.indexOf('('));
    }

    QString category("EVP"); // SQL

    if (type > QtMsgType::QtWarningMsg && category == "EVP") // Error = Critical, Fatal
    {
        QByteArray baErrorString;
        baErrorString.resize(256);
        ERR_error_string_n(ERR_get_error(), baErrorString.data(), baErrorString.size());

        logEntry = QString("[%1][%2][%3][%4] %5.\n<%6>\n")
                    .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
                    .arg(type)
                    .arg(category)
                    .arg(caller)
                    .arg(msg)
                    .arg(baErrorString.constData());
    }
    else // Debug, Warning
    {
        logEntry = QString("[%1][%2][%3][%4] %5.\n")
                    .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
                    .arg(type)
                    .arg(category)
                    .arg(caller)
                    .arg(msg);
    }


    fprintf(stderr, logEntry.toStdString().c_str());
    fflush(stderr);
}

static int decryptMessage(const QByteArray &baKeyInput, const QByteArray &cipherText, QByteArray *baOutput)
{
    QByteArray baMixed = QByteArray::fromBase64(cipherText);

    // STEP INIT
    // CREATE an EVP context for the decrypt operation
    EVP_CIPHER_CTX *aesCTX = EVP_CIPHER_CTX_new();
    if (!aesCTX)
    {
        // Q_FUNC_INFO
        qCritical() << "EVP:EVP_CIPHER_CTX_new() is failed to create cipher context";
        return 0;
    }

    // STEP 2
    // FETCH the cipher implementation
    EVP_CIPHER *aesCipher = EVP_CIPHER_fetch(NULL, "AES-256-CBC", NULL);
    if (!aesCipher)
    {
        qCritical() << "EVP:EVP_CIPHER_fetch() is failed to create aesCipher";

        EVP_CIPHER_CTX_free(aesCTX);
        return 0;
    }

    int ivlen = EVP_CIPHER_get_iv_length(aesCipher);       // 16 bytes
    qDebug() << "EVP:The ivlen is " << ivlen;

    // STEP 3
    // GET key and iv, input
    QByteArray baInput = baMixed.mid(ivlen);
    QByteArray baInitVector = baMixed.mid(0, ivlen);

    qDebug() << "EVP:Encrypted input is " << baInput;


    // STEP 4
    int rc = EVP_DecryptInit_ex2(aesCTX, aesCipher, (unsigned char *)baKeyInput.constData(), (unsigned char *)baInitVector.constData(), NULL);
    if (!rc)
    {
        qCritical() << "EVP:EVP_DecryptInit_ex2() is failed to initialise cipher context";

        EVP_CIPHER_free(aesCipher);
        EVP_CIPHER_CTX_free(aesCTX);

        return 0;
    }

    int blockSize = EVP_CIPHER_get_block_size(aesCipher);
    //qDebug() << "EVP:The AES blockSize is " << blockSize;
    baOutput->resize(baInput.size() + blockSize);
    int outputSize, finalSize;

    // STEP 5
    rc = EVP_DecryptUpdate(aesCTX, (unsigned char *)baOutput->data(), &outputSize, (const unsigned char *)baInput.constData(), baInput.size());
    if (!rc)
    {
        qCritical() << "EVP:EVP_DecryptUpdate() is failed to decrypt data";

        EVP_CIPHER_free(aesCipher);
        EVP_CIPHER_CTX_free(aesCTX);

        return 0;
    }
    finalSize = outputSize;

    qDebug() << "EVP:Decrypted baOutput size is " << baOutput->size();
    qDebug() << "EVP:Decrypted output size is " << outputSize;

    // STEP 6
    rc = EVP_DecryptFinal_ex(aesCTX, ((unsigned char *)baOutput->data()) + finalSize, &outputSize);
    if (!rc)
    {
        qCritical() << "EVP:EVP_DecryptFinal_ex() is failed to finalize data";

        EVP_CIPHER_free(aesCipher);
        EVP_CIPHER_CTX_free(aesCTX);

        return 0;
    }

    finalSize += outputSize;
    baOutput->resize(finalSize);
    qDebug() << "EVP:Decrypted output is " << baOutput;

    // STEP END
    // MEMORY DEALLOCATION
    EVP_CIPHER_free(aesCipher);
    EVP_CIPHER_CTX_free(aesCTX);

    return 0;
}

static int encryptMessage(const QByteArray &baKeyInput, const QByteArray &baInput, QByteArray *cipherText)
{
    // STEP INIT
    // CREATE an EVP context for the encrypt operation
    EVP_CIPHER_CTX *aesCTX = EVP_CIPHER_CTX_new();
    if (!aesCTX)
    {
        qCritical() << "EVP:EVP_CIPHER_CTX_new() is failed to create cipher context";
        return 0;
    }


    // ERR_error_string
    // ERR_GET_LIB
    // ERR_GET_REASON

    // STEP 2
    // FETCH the cipher implementation
    EVP_CIPHER *aesCipher = EVP_CIPHER_fetch(NULL, "AES-256-CBC", NULL);
    if (!aesCipher)
    {
        qCritical() << "EVP:EVP_CIPHER_fetch() is failed to create aesCipher";

        EVP_CIPHER_CTX_free(aesCTX);
        return 0;
    }

    int blockSize = EVP_CIPHER_get_block_size(aesCipher);     // 16 bytes
    qDebug() << "EVP:The AES blockSize is " << blockSize;

    int keylen = EVP_CIPHER_get_key_length(aesCipher);     // 32 bytes
    qDebug() << "EVP:The AES keylen is " << keylen;

    int ivlen = EVP_CIPHER_get_iv_length(aesCipher);       // 16 bytes
    qDebug() << "EVP:The AES ivlen is " << ivlen;

    int mode = EVP_CIPHER_get_mode(aesCipher);     // 2
    qDebug() << "EVP:The AES mode is " << mode;

    int nid = EVP_CIPHER_get_nid(aesCipher);   // 427
    qDebug() << "EVP:The AES nid is " << nid;

    int type = EVP_CIPHER_get_type(aesCipher);     // 427
    qDebug() << "EVP:The AES type is " << type;


    // STEP 3
    // GENERATE key and iv
    QByteArray baInitVector;
    baInitVector.resize(ivlen);
    int rc = RAND_bytes((unsigned char *)baInitVector.data(), ivlen);
    if (!rc)
    {
        qCritical() << "EVP:RAND_bytes() is failed to generate random iv";

        EVP_CIPHER_CTX_free(aesCTX);
        return 0;
    }


    qDebug() << "EVP:bakey len is " << baKeyInput.size();
    qDebug() << "EVP:baInitVector len is " << baInitVector.size();


    // STEP 4
    // INITIALISE an encrypt operation with the cipher, key and IV.
    rc = EVP_EncryptInit_ex2(aesCTX, aesCipher, (unsigned char *)baKeyInput.constData(), (unsigned char *)baInitVector.constData(), NULL);
    if (!rc)
    {
        qCritical() << "EVP:EVP_EncryptInit_ex2() is failed to initialise cipher context";

        EVP_CIPHER_free(aesCipher);
        EVP_CIPHER_CTX_free(aesCTX);

        return 0;
    }

    QByteArray baOutput;
    baOutput.resize(baInput.size() + blockSize - 1);
    int outputSize, finalSize;

    // STEP 5
    rc = EVP_EncryptUpdate(aesCTX, (unsigned char *)baOutput.data(), &outputSize, (const unsigned char *)baInput.constData(), baInput.size());
    if (!rc)
    {
        qCritical() << "EVP:EVP_EncryptUpdate() is failed to encrypt data";

        EVP_CIPHER_free(aesCipher);
        EVP_CIPHER_CTX_free(aesCTX);

        return 0;
    }
    finalSize = outputSize;

    qDebug() << "EVP:Encrypted baOutput size is " << baOutput.size();
    qDebug() << "EVP:Encrypted output size is " << outputSize;

    // STEP 6
    rc = EVP_EncryptFinal_ex(aesCTX, ((unsigned char *)baOutput.data()) + finalSize, &outputSize);
    if (!rc)
    {
        qCritical() << "EVP:EVP_EncryptFinal_ex() is failed to finalize data";

        EVP_CIPHER_free(aesCipher);
        EVP_CIPHER_CTX_free(aesCTX);

        return 0;
    }

    finalSize += outputSize;
    baOutput.resize(finalSize);
    qDebug() << "EVP:Encrypted output is " << baOutput;
    qDebug() << "EVP:Encrypted output base64 is " << baOutput.toBase64();

    // STEP END
    // MEMORY DEALLOCATION
    EVP_CIPHER_free(aesCipher);
    EVP_CIPHER_CTX_free(aesCTX);

    baOutput.prepend(baInitVector);

    *cipherText = baOutput.toBase64();

    return 0;
}

static int testKeyPair()
{
    // STEP 01
    // Public key, dosyadan alma

    FILE *keyFile = fopen("yarnamepub.key", "r");  // openssl rsa -in yarname.key -pubout -out yarnamepub.key
    if (keyFile == NULL)
    {
        return -1;
    }
    EVP_PKEY *publicKey = PEM_read_PUBKEY(keyFile, NULL, NULL, NULL);
    fclose(keyFile);
    if (publicKey == NULL)
    {
        qCritical() << "EVP:PEM_read_PUBKEY() is failed to get public key";
        return -1;
    }

    // STEP 02
    // Private key, dosyadan alma
    FILE *keyPairFile = fopen("yarname.key", "r");    // openssl genrsa -aes256 -out yarname.key 4096
    if (keyFile == NULL)
    {
        return -1;
    }
    EVP_PKEY *privateKey = PEM_read_PrivateKey(keyPairFile, NULL, NULL, (void *)"123456");
    fclose(keyFile);
    if (privateKey == NULL)
    {
        qCritical() << "EVP:PEM_read_PUBKEY() is failed to get public key";
        return -1;
    }

    // STEP 03
    // Public key ile metin sifreleme

    EVP_PKEY_CTX *publicKeyContext = EVP_PKEY_CTX_new(publicKey, NULL);
    int result = EVP_PKEY_encrypt_init(publicKeyContext);

    QByteArray baInput("deneme");
    QByteArray baEncrypted;
    size_t outputSize = 2048;
    baEncrypted.resize(2048);

    result = EVP_PKEY_encrypt(publicKeyContext, (unsigned char *)baEncrypted.data(), &outputSize, (const unsigned char *) baInput.constData(), baInput.size());
    baEncrypted.resize(outputSize);

    EVP_PKEY_free(publicKey);
    EVP_PKEY_CTX_free(publicKeyContext);

    // STEP 04
    // Private key ile metnin sifresini cozme

    EVP_PKEY_CTX *privateKeyContext = EVP_PKEY_CTX_new(privateKey, NULL);
    result = EVP_PKEY_decrypt_init(privateKeyContext);

    QByteArray baDecrypted;
    size_t decryptedSize;

    result = EVP_PKEY_decrypt(privateKeyContext, NULL, &decryptedSize, (const unsigned char *)baEncrypted.constData(), baEncrypted.size());
    if (result != 1)
    {
        qCritical() << "EVP:EVP_PKEY_decrypt() is failed to specify the outsize";
        return -1;
    }

    baDecrypted.resize(decryptedSize);
    result = EVP_PKEY_decrypt(privateKeyContext, (unsigned char *)baDecrypted.data(), &decryptedSize, (const unsigned char *)baEncrypted.constData(), baEncrypted.size());
    if (result != 1)
    {
        qCritical() << "EVP:EVP_PKEY_decrypt() is failed to decrypt";
        return -1;
    }

    baDecrypted.resize(decryptedSize);
    qDebug() << "The decrypted text is " << baDecrypted;

    EVP_PKEY_free(privateKey);
    EVP_PKEY_CTX_free(privateKeyContext);

    return result;
}

static int createKeyPairFiles()
{
    // STEP 05
    // Kod ile RSA keypair uretme -aes256 olmaksizin

    EVP_PKEY *keyPair = NULL;
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

    int result = EVP_PKEY_keygen_init(ctx);
    result = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 4096);
    //EVP_PKEY_CTX_set_rsa_pss_saltlen()
    //EVP_PKEY_CTX_set_rsa_pss_keygen_saltlen()
    result = EVP_PKEY_keygen(ctx, &keyPair);

    int         numValue;
    size_t      stringValueLen;
    QByteArray  baString;
    baString.resize(80);

    const OSSL_PARAM *keyPairParams = EVP_PKEY_gettable_params(keyPair);
    for (int i=0; keyPairParams[i].key != NULL; i++)
    {
        OSSL_PARAM param = keyPairParams[i];
        //qDebug() << param.key << " (of type " << param.data_type << ")\n";

        switch (param.data_type)
        {
        case OSSL_PARAM_INTEGER:
        case OSSL_PARAM_UNSIGNED_INTEGER:
            result = EVP_PKEY_get_int_param(keyPair, param.key, &numValue); // OSSL_PKEY_PARAM_BITS vb.
            qDebug() << param.key << " value is " << numValue;

            break;
        case OSSL_PARAM_UTF8_STRING:
            result = EVP_PKEY_get_utf8_string_param(keyPair, param.key, baString.data(), baString.size(), &stringValueLen);
            baString.resize(stringValueLen);
            qDebug() << param.key << " value is " << baString;

            break;
        default:
            qDebug() << "Unexpected data type for OSSL_PARAM";

            break;
        }
    }

    // STEP 06
    // Kod ile keypair'den public key elde etme ve dosyaya kayit
    BIO *bioPublicKey = BIO_new(BIO_s_mem()); // CREATE a memory BIO
    result = PEM_write_bio_PUBKEY(bioPublicKey, keyPair);

    EVP_PKEY *publicKey = PEM_read_bio_PUBKEY(bioPublicKey, NULL, NULL, NULL);
    FILE *keyFile = fopen("codepub.key", "w");
    if (keyFile == NULL)
    {
        return -1;
    }
    result = PEM_write_PUBKEY(keyFile, publicKey);

    BIO_free(bioPublicKey);
    fclose(keyFile);

    // STEP 07
    // Kod ile keypair'i dosyaya kayit
    FILE *keyPairFile = fopen("code.key", "w");
    if (keyPairFile == NULL)
    {
        return -1;
    }
    result = PEM_write_PrivateKey(keyPairFile, keyPair, NULL, NULL, 0, NULL, NULL);
    fclose(keyPairFile);

    // STEP 08
    // openssl rsa -pubout operasyonunu konsol uygulamasi ile yapip kiyaslama

    EVP_PKEY_free(keyPair);
    EVP_PKEY_CTX_free(ctx);

    return result;
}

static int createPrivateKeyEncrypted(const QByteArray &baKeyString)
{
    // STEP 09
    // RSA key'i AES 256 ile sifreleme - wrapping

    EVP_PKEY *keyPair = NULL;
    EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

    int result = EVP_PKEY_keygen_init(ctx);
    result = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 4096);
    result = EVP_PKEY_keygen(ctx, &keyPair);

    BIO *bioFile = BIO_new_file("codeKeyPairEncrypted.key", "w"); // CREATE a file BIO
    EVP_CIPHER *aesCipher = EVP_CIPHER_fetch(NULL, "AES-256-CBC", NULL);

    result = PEM_write_bio_PrivateKey(bioFile, keyPair, aesCipher, NULL, 0, NULL, (void *)baKeyString.constData());

    BIO_free(bioFile);
    EVP_CIPHER_free(aesCipher);
    EVP_PKEY_free(keyPair);
    EVP_PKEY_CTX_free(ctx);
}

static int writeEncryptedDek()
{
    // STEP 00. DEK, ayni zamanda sunucuda saklanmaz ise, musterinin bilgisayarinda bu dosya bir sebeple bozulur ise
    //          DEK hicbir zaman elde edilemez olur, mesajlarin da sifresi cozulemez.
    //          Ya da, mesajlarin bir kopyasi sunucuda saklanir ise CLOUDSTORE kullanilirsa, yine mesajlarin
    //          DEK ile sifreli network uzerinden gitmesi gerekir.
    //
    //          DEK, sunucuda generate edilse
    //          Musteriye DEK'ini network uzerinden KEK ile sifreli sekilde gondersek
    //          Sunucu, db'ye KEK ile sifreli halini yazsa musteri de config dosyasina sifreli halini yazsa
    //          Iki taraftan birinden saglama -repair- mumkun olabilir bu sayede.
    // STEP 01. DEK, sunucudan gelen KEK ile sifrelenerek config dosyasina yazilsin.
    QByteArray baDEK;
    generateKey(&baDEK);

    QByteArray baKEK("123456");
    QByteArray baCipherKEK;
    encryptMessage(baKEK, baDEK, &baCipherKEK);

    QSettings configList("yarname.ini", QSettings::IniFormat);
    configList.beginGroup("Security");
    QString kekEncrypted = configList.value("KEK").toString();
    configList.setValue("KEK", baCipherKEK.constData());
    configList.endGroup();
    configList.sync();

    return 0;
}

static int callback(void *unused, int count, char **data, char **columns)
{
    for (int i = 0; i < count; i++)
    {
        qDebug() << "SQL:" << QString("%1 = %2\n").arg(columns[i]).arg(data[i] ? data[i] : "NULL");
    }

    return 0;
}

static bool getPrivateKeyFromPkcs12()
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

            EVP_PKEY *pkey = NULL;
            X509 *cert = NULL;
            STACK_OF(X509) *chain = NULL;

            int result = PKCS12_parse(p12, passin.toLocal8Bit(), &pkey, &cert, &chain);
            if (result)
            {
                FILE *keyFile = fopen("test.key", "w");
                if (keyFile)
                {
                    result = PEM_write_PrivateKey(keyFile, pkey, NULL, NULL, 0, NULL, NULL);
                    fclose(keyFile);
                }

                rc = true;
            }
            else
            {
                qCritical() << "PKCS12_parse() is failed to parse p12 object";
            }

            PKCS12_free(p12);
        }
        else
        {
            qDebug() << "Error reading PKCS12 object";
        } // p12
    }
    else
    {
        qDebug() << "Error reading PKCS12 file";
    } // p12File


    return rc;
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(redirectLogMessage);

    QCoreApplication a(argc, argv);

    QByteArray baDecrypted;
    QByteArray baEncrypted;
    RSACrypto *rsa = new RSACrypto("yarname.pub.pem");

    rsa->encrypt("deneme", &baEncrypted);
    qDebug() << "encrypted with rsa public key is: " << baEncrypted;

    rsa->decrypt(baEncrypted, &baDecrypted);
    qDebug() << "decrypted with rsa private key is: " << baDecrypted;

    delete rsa;

    getPrivateKeyFromPkcs12();

    // Allah'in yardimiyla insaAllah
    writeEncryptedDek();

    // STEP 02. Kullanici, uye olurken bir RSA anahtar cifti olussun.
    // STEP 03. Anahtarin private olani, kullanicinin sifresi ile AES sifrelensin ve username.key dosyasina yazilsin.
    // STEP 04. Anahtarin public olani, network uzerinden node.js'e ondan da veritabanina yazilsin.
    //          Sunucu yazmayi onayladiginda musterinin cihazindan public anahtar silinsin.
    // STEP 04.1 yarnamelib projesine OPENSSL operasyonlarini dahil etme
    // STEP 04.2 yarname projesinde lib uzerinden OPENSSL operasyon testi
    // STEP 04.3 yarname network uzerinden node.js baglanti testi
    // STEP 04.4 yarname network uzerinden public key gonderimi
    // STEP 04.5 postgresql veritabani testleri (Yonetim Konsolu uzerinden)
    // STEP 04.6 npm install pg ve postgresql baglantisi ve public anahtarin tabloya yazimi
    // STEP 04.7 basarildi mesajinin musteriye iletimi ve yarname uygulamasindan public anahtarin silinmesi




    // ASIMETRIK SIFRELEME CALISMASI
    testKeyPair();
    createKeyPairFiles();
    createPrivateKeyEncrypted(QByteArray("123456"));

    // STEP 10
    // PBKDF ile keystring olusturma
    QByteArray baKek;
    int result = generateKey(&baKek);

    result = createPrivateKeyEncrypted(baKek);





    // STEP 05. Sunucudan KEK nasil geliyor ?
    // STEP 05.1. Tek kullanimlik AES anahtari olusturulup KEK onunla sifreleniyor;
    // STEP 05.2. Veritabanindan kullanicinin public anahtari aliniyor ve
    //      tek kullanimlik anahtar, public anahtar ile AES sifreleniyor.
    // STEP 05.2. Ikisi beraber sunucudan yola cikiyor.
    // STEP 05.3. Sunucudan gelen paketin ikinci kismi private key ile cozuluyor ele gecen AES ile de paketin birinci
    // kismi aciliyor boylece KEK elde ediliyor. O KEK ile de sifreli DEK cozuluyor.





/********************************************************************************************************************/


    // ARALIK: PKCS12 sureclerini implementasyon
    // ARALIK: Base64 kodlama ve .conf dosyasina yazim, dosyadan okuma
    // ARALIK: Dosyadan wrapped DEK'i okuma, KMS'den KEK alma ve sifresini cozme
    // ARALIK: SQLite alanina mesaji sifrelemede DEK'i kullanma
    // ARALIK: yarnamelib guncelleme
    // ARALIK: yarname guncelleme


    QByteArray baKeyIn = encryptionKey.toLocal8Bit();
    QByteArray baDataIn = "Bismillahirrahmanirrahim";
    QByteArray baCipherText, baText;
    encryptMessage(baKeyIn, baDataIn, &baCipherText);
    decryptMessage(baKeyIn, baCipherText, &baText);
    QByteArray baCipherText2;
    encryptMessage(baKeyIn, baDataIn, &baCipherText2);

    getSha256Text();

    qDebug() << "SQL:SQLite version is " << sqlite3_libversion();

    sqlite3 *db;
    char *errMsg;
    int rc = sqlite3_open("inbox.sqlite3", &db);

    qDebug() << "SQL:sqlite3_open() is returned " << rc;

    // 1
    //rc = sqlite3_exec(db, "SELECT * FROM Messages", callback, 0, &errMsg);

    // 2
    rc = sqlite3_exec(db, "SELECT json_extract(Message, '$.from') AS fromAddress FROM Messages", callback, 0, &errMsg);

    // 3
    //rc = sqlite3_exec(db, "SELECT json_extract(Message, '$.subject') AS sbj FROM Messages", callback, 0, &errMsg);

    qDebug() << "exec return code is " << rc;

    if (rc)
    {
        qCritical() << "SQL:sqlite3_exec() error message is " << errMsg;
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);

    /*
     * PKCS12_parse()
     * KONSOL UYGULAMASINDAN: openssl pkcs12
     *
     */

    return a.exec();
}
