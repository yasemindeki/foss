#ifndef MIMEMESSAGE_H
#define MIMEMESSAGE_H

#include <QObject>
#include "mailer/engines.h"

QT_BEGIN_NAMESPACE
class MimeEntity;
class MimeMultipart;
QT_END_NAMESPACE

class YNCORE_EXPORT MimeMessage : public QObject
{
    Q_OBJECT
public:
    MimeMessage(Mail::HeaderStore *headerFields, bool composite = false, QObject *parent = nullptr);

    int setContent(MimeEntity *entity);
    int addContent(MimeEntity *entity);
    int addContent(MimeMultipart *multipart);

    QString serialize() const;

    const QList<MimeEntity *> &entities() const;
    const QList<MimeMultipart *> &multipartEntities() const;

    int mimeType() const;

    friend class Mail::CURLEngine;

private:
    int mediaType;

    bool isCompositeMedia;
    bool headerError;

    Mail::HeaderStore *headerSection;

    QList<MimeEntity *> bodies;
    QList<MimeMultipart *> bodyParts;

    const Mail::HeaderStore &header();

};


#endif // MIMEMESSAGE_H
