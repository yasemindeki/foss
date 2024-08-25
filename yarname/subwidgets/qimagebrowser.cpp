#include "qimagebrowser.h"

#include <QMimeDatabase>
#include <QImageReader>
#include <QDebug>

QStringList QImageBrowser::imageTypeList;
QString QImageBrowser::allImagesFilter;

QImageBrowser::QImageBrowser(QWidget *parent, const QString &caption, const QString &directory, const QString &filter)
    : QFileDialog(parent, caption, directory, filter)
{
    this->setAcceptMode(QFileDialog::AcceptOpen);
    this->setFileMode(QFileDialog::ExistingFile);

    this->setMimeTypeFilters(QImageBrowser::imageTypeList);

    QStringList filters = this->nameFilters();
    filters.append(QImageBrowser::allImagesFilter);
    this->setNameFilters(filters);
    this->selectNameFilter(QImageBrowser::allImagesFilter);
}

void QImageBrowser::setOptions()
{
    // Herhangi bir destekleneni secebilmesi
    QMimeDatabase mimeDB;
    QStringList filterList;

    // Desteklenen resim dosyasi turlerinin acilir listede TEK TEK gorunmesi
    const QByteArrayList imageMimeTypes = QImageReader::supportedMimeTypes();
    foreach(const QByteArray &mimeTypeName, imageMimeTypes)
    {
        imageTypeList.append(mimeTypeName);

        QMimeType mimeType = mimeDB.mimeTypeForName(mimeTypeName);
        if (mimeType.isValid())
        {
            filterList.append(mimeType.globPatterns());
            qDebug() << mimeType.globPatterns();
        }
    }

    allImagesFilter = QString("All supported formats (%1)").arg(filterList.join(' '));
}
