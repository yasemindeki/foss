#ifndef ENGINES_H
#define ENGINES_H

#include "yncore/yncore_global.h"
#include "yncore/interfaces.h"
#include "mailer/basetypes.h"
#include "curl/curl.h"

#include <QDateTime>
#include <QVector>

QT_BEGIN_NAMESPACE
class QFile;
class MimeMessage;
class MimeEntity;
QT_END_NAMESPACE

class YNCORE_EXPORT QHeaderPart : public QObject
{
    Q_OBJECT

    friend class QInboxData;
public:
    explicit QHeaderPart(QObject *parent = nullptr);

private:
    QString charset;
    QChar   encoding;
    QString encodedText;
    QString afterText;

};

class YNCORE_EXPORT QInboxData : public QObject
{
    Q_OBJECT
public:
    explicit QInboxData(QObject *parent = nullptr);
    void decodeContent();
    void decodeSubject();
    void decodeFrom();
    void setSubject(const QString &rawText);
    void setFrom(const QString &rawText);

    Mail::Mailbox  *from;
    QString         subject;
    QString         content;
    QDateTime       dateTime;
    QString         id;

    QVector<QHeaderPart *> subjectParts;
    QVector<QHeaderPart *> fromParts;

    bool        isContentEncoded;

private:
};

namespace Mail
{
    class YNCORE_EXPORT CURLEngine : public QObject, public EngineInterface
    {
        Q_OBJECT

    public:
        explicit CURLEngine(QObject *parent = nullptr);
        ~CURLEngine();

        virtual int sendMail(MimeMessage *email);

        // RX YONLU FONKSIYONLAR
        int getRecentMessage(QInboxData *);
        int startBulkImport(QInboxData *outData);
        void loadFromFile(QInboxData *);

    private:
        QDateTime       lastCheckTime;
        uint            uidNext;

        CURL            *curlHandle;
        curl_slist      *recipients;
        curl_slist      *headers;
        curl_mime       *root;

        curl_mime *alt;

        FILE            *logFilePointer;
        static QFile    *logger;

        static int      mailSentPos;

        void initialize();
        void startCurlSession();
        void endCurlSession();
        void setHeaderOptions(int mediaType, const HeaderStore &headerSection);
        void setUploadOptions();
        void setCompositeMedia(MimeMessage *email);
        void setDiscreteMedia(MimeMessage *email);

        static size_t prepareMailMessage(char *buffer, size_t size, size_t nitems, void *userdata);
        static int onDebugDataReady(CURL *handle, curl_infotype infotype, char *data, size_t size, void *userptr);

        void composeMultipartBody(curl_mime *parent, const QList<MimeEntity *> &entities);

        // RX YONLU DEGISKENLER
        QStringList     mailIndexList;
        QString         rxData;
        QFile          *rxFile;

        // RX YONLU FONKSIYONLAR
        void setDownloadOptions();
        int getCurrentMailIndexes();
        static size_t rxDataAvailable(char *ptr, size_t size, size_t nmemb, void *userdata);
        void setMailIndexes();
        void setInboxData(QInboxData *);
        void openInboxFile();
        void setDownloadData(char *ptr, size_t nmemb);
        void examineInbox();

    signals:

    };

    class DIYEngine : public QObject, public EngineInterface
    {
        Q_OBJECT

    public:
        explicit DIYEngine(QObject *parent = nullptr);
    private:

    signals:

    };

} // end NAMESPACE Mail

#endif // ENGINES_H
