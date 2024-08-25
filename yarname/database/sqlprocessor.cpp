#include "sqlprocessor.h"

#include <QtSql>
#include <QDebug>
#include <QDialog>

SQLProcessor::SQLProcessor() : QObject()
{
    supportedDrivers();
}

void SQLProcessor::supportedDrivers()
{
    QStringList databases = QSqlDatabase::drivers();

    QListIterator<QString> li(databases);
    while (li.hasNext())
    {
        QString databaseName = li.next();
        qDebug() << "YPY: " << databaseName;
    }

}

bool SQLProcessor::exec(const QString &connectionName, const QString &queryText)
{
    QSqlDatabase database = QSqlDatabase::database(connectionName);
    database.setHostName("localhost");
    database.setUserName("postgres");
    database.setPassword("BAigys1744");
    database.setDatabaseName("yarmail");
    database.open();

    QSqlQuery query(database);

    query.prepare(queryText);
    bool rc = query.exec();
    if (!rc) {
        QDialog about;
        about.setWindowTitle(database.lastError().text());
        about.exec();
    }

    return rc;
}
