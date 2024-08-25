#ifndef MIMEENTITY_H
#define MIMEENTITY_H

#include <QObject>
#include "basetypes.h"

class YNCORE_EXPORT MimeEntity : public QObject
{
  Q_OBJECT

public:
    MimeEntity(QObject *parent = nullptr);
    MimeEntity(const QString &contentType, QObject *parent = nullptr);

    QString header() const;
    QString header(Mail::HeaderFieldName type) const;
    QString contentType() const;

    void setBody(const QVariant &body);

    const QVariant &body() const;
    int mimeType() const;
    const QString &mimeTypeString() const;

protected:
    Mail::HeaderStore   *headerFields;
    QVariant            data;
    int                 mediaType;
    QString             mediaTypeString;

signals:

};

#define MimeEntityList QList<MimeEntity *>

class YNCORE_EXPORT MimeAttachment : public MimeEntity
{
    Q_OBJECT

public:
    MimeAttachment(QString fileName, bool inlineBodyPart = false, MimeEntity *parent = nullptr);

    const QString &file();
    bool inlineType();

private:
    QString fileName;
    bool inlinePresentation;

};

class YNCORE_EXPORT MimeMultipart: public MimeEntity
{
    Q_OBJECT

public:
    MimeMultipart(QString contentType, MimeEntity *parent = nullptr);

    void addEntity(MimeEntity *entity);

    const MimeEntityList &getEntities();

private:
    MimeEntityList entities;

};

#define MimeMultipartList QList<MimeMultipart *>

#endif // MIMEENTITY_H
