#ifndef QIMAGEBROWSER_H
#define QIMAGEBROWSER_H

#include <QFileDialog>

class QImageBrowser : public QFileDialog
{
    Q_OBJECT
public:
    explicit QImageBrowser(QWidget *parent = nullptr,
                         const QString &caption = QString(),
                         const QString &directory = QString(),
                         const QString &filter = QString());

    static void setOptions();
    static QStringList  imageTypeList;
    static QString      allImagesFilter;

private:

signals:

};

#endif // QIMAGEBROWSER_H
