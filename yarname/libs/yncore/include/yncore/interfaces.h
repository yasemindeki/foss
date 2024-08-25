#ifndef INTERFACES_H
#define INTERFACES_H

#include <QObject>

QT_BEGIN_NAMESPACE
class MimeMessage;
QT_END_NAMESPACE

namespace Mail
{
    class EngineInterface
    {

    public:
        virtual int sendMail(MimeMessage *mail) = 0;

    signals:

    };


} // end NAMESPACE Mail

#endif // INTERFACES_H
