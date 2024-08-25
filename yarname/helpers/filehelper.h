#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QObject>

class FileHelper : public QObject
{
    Q_OBJECT
public:
    explicit FileHelper(QObject *parent = nullptr);

    static void loadStyleSheet(const QString &filePath, QWidget *widgetToSet = nullptr);

private:

signals:

};

#endif // FILEHELPER_H
