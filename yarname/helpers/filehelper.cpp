#include "filehelper.h"

#include <QtWidgets>

FileHelper::FileHelper(QObject *parent) : QObject(parent)
{

}

void FileHelper::loadStyleSheet(const QString &filePath, QWidget *widgetToSet)
{
    QFile qssFile(filePath);

    qssFile.open(QFile::ReadOnly);
    QString qss = QLatin1String(qssFile.readAll());

    widgetToSet->setStyleSheet(qss);
}


