#ifndef MAILBOX_H
#define MAILBOX_H

#include <QObject>
#include "yncore/yncore_global.h"

namespace Mail
{
    class YNCORE_EXPORT Mailbox : public QObject
    {
        Q_OBJECT

    public:
        Mailbox(const QString &name, const QString &addr, QObject *parent = nullptr);
        Mailbox(const QString &addr, QObject *parent = nullptr);

        QString addr() const;
        QString name() const;
        QString toString() const;

    private:
        QString displayName;
        QString addrSpec;
    };
}

#define MailboxList QList<Mail::Mailbox *>

#endif // MAILBOX_H
