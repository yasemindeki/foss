#ifndef BASETYPES_H
#define BASETYPES_H

#include "mailbox.h"
#include <QVariant>

#define CURLSTR(qtString) qtString.toStdString().c_str()

namespace Mail
{
    Q_NAMESPACE

    class MediaType : public QObject
    {
        Q_OBJECT
    public:
        MediaType(QObject *parent = nullptr);

        typedef enum
        {
            TEXT_PLAIN = 1,
            TEXT_HTML = 2,
            MULTIPART_MIXED = 3,
            MULTIPART_RELATED = 4,
            FILE = 5
        }IANAToken;

        // key is the name of media type
        static int id(QString key);
        static MediaType *mt;

    private:
        QMap<QString, IANAToken> tokenMap;

    };


    typedef enum
    {
        FROM = 1,
        TO = 2,
        SUBJECT = 3,
        CONTENT_TYPE = 4,
        CONTENT_DISPOSITION = 5,
        CONTENT_ID = 6
    }HeaderFieldName;

    class HeaderField : public QObject
    {
        Q_OBJECT
    public:
        HeaderField(HeaderFieldName type, const QVariant &data, QObject *parent = nullptr);

        HeaderFieldName type() const;
        const QVariant &data() const;

    private:
        HeaderFieldName     name;
        QVariant            body;
    };

    class YNCORE_EXPORT HeaderStore : public QObject
    {
        Q_OBJECT
    public:
        explicit HeaderStore(QObject *parent = nullptr);
        ~HeaderStore();

        void addField(HeaderFieldName fieldName, const QVariant &fieldBody);

        QString contentType() const;
        const QVariant &to() const;
        const Mailbox &from() const;

        QString toString(HeaderFieldName type) const;
        QString toString(const MailboxList &mailboxList) const;

        QString serialize() const;

    private:
        QMap<HeaderFieldName, HeaderField *> fieldMap;

    signals:

    };

} // end NAMESPACE Mail

//Q_DECLARE_METATYPE(Mail::Mailbox)

#endif // BASETYPES_H
