#include "toimage.h"
#include <QPixmap>
#include <QPainter>

ToImage::ToImage(QObject *parent) : QObject(parent)
{
    viewType = DEFAULT;
}

ToImage::ToImage(const QImage &input) : QImage(input)
{
    viewType = DEFAULT;
}

ToImage::ToImage(const QString &addr)
{
    QPixmap pixmap(":/icons/toimage.png");
    QPixmap newPixmap = pixmap.scaled(100, 15);//, Qt::KeepAspectRatio);

    QPainter painter(&newPixmap);
    painter.setFont(QFont("Arial"));
    painter.drawText(QPoint(50, 10), addr);

    QImage image = newPixmap.toImage();

    //QImage::QImage(image);
    image.copy().swap(*this);

    viewType = DEFAULT;
    toAddress = addr;
}

QString const &ToImage::address() const
{
    return toAddress;
}

void ToImage::onImageClicked(const QString &name, int vt)
{
    if (toAddress == name && viewType != vt)
    {
        // icon degisiyor
        QPixmap pixmap(":/icons/logo.png");
        QPixmap newPixmap = pixmap.scaled(100, 15);//, Qt::KeepAspectRatio);

        QPainter painter(&newPixmap);
        painter.setFont(QFont("Arial"));
        painter.drawText(QPoint(50, 10), toAddress);

        QImage image = newPixmap.toImage();

        //QImage::QImage(image);
        image.copy().swap(*this);

        viewType = PRESSED;
    }

    if (toAddress != name && viewType != DEFAULT)
    {
        // icon degisiyor
    }

}
