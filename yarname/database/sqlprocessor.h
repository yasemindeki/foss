#ifndef SQLPROCESSOR_H
#define SQLPROCESSOR_H

#include <QObject>

class SQLProcessor : public QObject
{
    Q_OBJECT
public:
    SQLProcessor();
    bool exec(const QString &connectionName, const QString &queryText);

private:
    void supportedDrivers();
};

#endif // SQLPROCESSOR_H
