#include "qsidegrip.h"

#include <QMouseEvent>
#include <QtGlobal>         // in order to use qMax
#include <QtWidgets>

QSideGrip::QSideGrip(QWidget *parent, Qt::Edge edgeType) : QSizeGrip(parent)
{
    edge = edgeType;
    mousePosition = QPoint(0, 0);

    this->setStyleSheet("QWidget { background-color: rgb(36, 41, 46); }");
}

void QSideGrip::resizeLeft(QPoint delta)
{
    QWidget *window = this->window();
    int width = qMax(window->minimumWidth(), window->width() - delta.x());
    QRect geo = window->geometry();
    geo.setLeft(geo.right() - width);

    window->setGeometry(geo);
}

void QSideGrip::resizeRight(QPoint delta)
{
    QWidget *window = this->window();
    int width = qMax(window->minimumWidth(), window->width() + delta.x());
    window->resize(width, window->height());
}

void QSideGrip::resizeTop(QPoint delta)
{
    QWidget *window = this->window();
    int height = qMax(window->minimumHeight(), window->height() - delta.y());
    QRect geo = window->geometry();
    geo.setTop(geo.bottom() - height);
    window->setGeometry(geo);
}

void QSideGrip::resizeBottom(QPoint delta)
{
    QWidget *window = this->window();
    int height = qMax(window->minimumHeight(), window->height() + delta.y());
    window->resize(window->width(), height);
}

void QSideGrip::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousePosition = event->pos();
    }
}

void QSideGrip::mouseMoveEvent(QMouseEvent *event)
{
    if (!mousePosition.isNull())
    {
        QPoint delta = event->pos() - mousePosition;
        switch (edge)
        {
        case Qt::LeftEdge:
            resizeLeft(delta);
            break;
        case Qt::TopEdge:
            resizeTop(delta);
            break;
        case Qt::BottomEdge:
            resizeBottom(delta);
            break;
        default:
            resizeRight(delta);
            break;
        }

    }
}

void QSideGrip::mouseReleaseEvent(QMouseEvent *)
{
    mousePosition = QPoint(0, 0);
}

void QSideGrip::paintEvent(QPaintEvent *)
{
    if (edge == Qt::LeftEdge || edge == Qt::RightEdge)
    {
        this->setCursor(Qt::SizeHorCursor);
    }
    else
    {
        this->setCursor(Qt::SizeVerCursor);
    }

    QStyleOption option;
    option.init(this);
    QPainter painter(this);

    this->style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);

    /*
     * QSizeGrip varsayilan gorunumunu degistirdik.
     * Temel sinifin implementasyonu soyleydi:
     *
        QPainter painter(this);
        QStyleOptionSizeGrip opt;
        opt.init(this);
        opt.corner = d->m_corner; // Qt::Corner
        style()->drawControl(QStyle::CE_SizeGrip, &opt, &painter, this);
     *
     */

}
