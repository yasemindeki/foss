#ifndef CHECKER_H
#define CHECKER_H

#include <QObject>
#include "yncore_global.h"

class YNCORE_EXPORT Checker : public QObject
{
    Q_OBJECT
public:
    explicit Checker(QObject *parent = nullptr);

    QString mimeContentType(const QString &input);

    bool isMail(const QString &addr);

signals:

};

#endif // CHECKER_H
