#include "qcornergrip.h"

#include <QtWidgets>

QCornerGrip::QCornerGrip(QWidget *parent) : QSizeGrip(parent)
{
    this->setStyleSheet("QWidget { background-color: rgb(36, 41, 46); }");
}

void QCornerGrip::paintEvent(QPaintEvent *event)
{
    QSizeGrip::paintEvent(event);

    QStyleOption option;
    option.init(this);
    QPainter painter(this);

    this->style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);

}
