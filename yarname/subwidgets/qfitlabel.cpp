#include "qfitlabel.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>

QFitLabel::QFitLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f)
{
}

QFitLabel::QFitLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
}

void QFitLabel::paintEvent(QPaintEvent *)
{
    //QLabel::paintEvent(event);

    // Temel sinifin implementasyonundan kodlar
    QStyle *style = QWidget::style();

    QRect cr = contentsRect();
    cr.adjust(margin(), margin(), -margin(), -margin());

    QPainter painter(this);
    drawFrame(&painter);

    // Metni kisalttigimiz kodlar yeni eklenen kodlar
    QFontMetrics fm = painter.fontMetrics();
    QString fullText = this->text();
    QString shortenText = fm.elidedText(fullText, Qt::ElideMiddle, this->width());

    // Temel sinifin implementasyonundan kodlar
    QStyleOption opt;
    opt.initFrom(this);

    style->drawItemText(&painter, cr, alignment(), opt.palette, isEnabled(), shortenText, foregroundRole());

    //painter.drawText(QPoint(0, fm.ascent()), shortenText);
    //painter.drawText(this->rect(), shortenText);
    //painter.drawText(this->contentsRect(), shortenText);


/*
    qDebug() << "QFitLabel fit text is " << shortenText;
    qDebug() << "QFitLabel contentsRect.width is " << this->contentsRect().width();
    qDebug() << "QFitLabel font family is " << painter.font().family();
    qDebug() << "QFitLabel rect.width is " << this->rect().width();
*/

}
