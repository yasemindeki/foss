#include "qcategorybutton.h"

#include <QPainter>
#include <QStyleOptionToolButton>
#include <QDebug>
#include <QMouseEvent>

QCategoryButton::QCategoryButton(const QString &text, Qt::GlobalColor color, QWidget *parent)
    : QToolButton(parent), categoryText(text), categoryColor(color)
{
    isHoverIcon = false;

    this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->setLayoutDirection(Qt::RightToLeft);
    this->setFocusPolicy(Qt::ClickFocus); // from Qt::TabFocus to Qt::ClickFocus

    QFont font("Goldman", 8);
    QFontMetrics fm(font);
    int textWidth = fm.horizontalAdvance(categoryText);

    this->setFixedWidth(textWidth + 20 + 20);
    this->setFixedHeight(20);

    QString penColor, backgroundColor;
    switch (categoryColor) {
    case Qt::green:
        penColor = "rgb(63, 132, 68)";
        backgroundColor = "rgb(196, 241, 201)";

        removeIconPath = ":/icons/close-mini.png";
        break;

    case Qt::blue:
        penColor = "rgb(111, 163, 170)";
        backgroundColor = "rgb(209, 243, 247)";

        removeIconPath = ":/icons/close-mini-blue.png";
        break;

    case Qt::red:
        penColor = "rgb(213, 43, 47)";
        backgroundColor = "rgb(251, 212, 212)";

        removeIconPath = ":/icons/close-mini-red.png";
        break;

    default:
        break;

    }

    this->setStyleSheet(QString("QToolButton { \
                            font: 8pt 'Goldman'; \
                            color: %1; \
                            border: 1px solid %2; \
                            border-radius: 10px; \
                            background-color: %3; \
                        } \
                        QToolButton:hover { \
                        }").arg(penColor).arg(backgroundColor).arg(backgroundColor));

}

bool QCategoryButton::event(QEvent *event)
{
    //qDebug() << "QCategoryButton event = " << event->type();

    if (event->type() == QEvent::HoverMove)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        QRect tbRect = this->rect();
        int verticalAlignTop = tbRect.y() + ((tbRect.height() - 6) / 2);
        int buttonRectLeft = (tbRect.right() - 6 - 8);
        QRect buttonRect = QRect(QPoint(buttonRectLeft, verticalAlignTop), QSize(7, 6));

        //qDebug() << "QCategoryButton mouse pos = " << (buttonRect.contains(mouseEvent->pos()));

        if (buttonRect.contains(mouseEvent->pos()))
        {
            if (!isHoverIcon)
            {
                this->setToolTip("Remove category");
                this->update();

                qDebug() << "QCategoryButton event = " << event->type();
                isHoverIcon = true;

                //emit signalFunc();
            }

        }
        else
        {
            if (isHoverIcon)
            {
                isHoverIcon = false;

                this->setToolTip("Search for all messages with the category " + categoryText);
                this->update();
            }
        }
    }

    return QToolButton::event(event);
}

void QCategoryButton::mouseReleaseEvent(QMouseEvent *event)
{
    QToolButton::mouseReleaseEvent(event);
}

void QCategoryButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    QPainter painter(this);

    // Temel sinifin implementasyonundan kodlar
    QStyleOptionToolButton opt;
    opt.initFrom(this);

    qDebug() << "QCategoryButton rect = " << opt.rect;

    QRect tbRect = opt.rect;
    tbRect.setLeft(tbRect.left() + 10);

    painter.drawText(tbRect, Qt::AlignLeft | Qt::TextSingleLine, categoryText);

    QRect iconRect = tbRect;

    if (opt.state & QStyle::State_MouseOver)
    {
        if (isHoverIcon)
        {
            QRect circleRect(QPoint(tbRect.right() - 14 - 4, 3), QSize(13, 14));

            painter.setPen(QColor(250, 250, 250));
            painter.setBrush(QColor(250, 250, 250));
            painter.drawRoundRect(circleRect, 75, 75);
        }
    }

    QIcon removeIcon(removeIconPath);
    iconRect.setRight(iconRect.right() - 8);
    removeIcon.paint(&painter, iconRect, Qt::AlignRight | Qt::AlignVCenter);

}

