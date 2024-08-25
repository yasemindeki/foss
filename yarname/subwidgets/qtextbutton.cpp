#include "qtextbutton.h"
#include "ypyscreen.h"

#include <QtWidgets>

QTextButton::QTextButton(const QString &text, QWidget *parent) : QPushButton(text, parent)
{
    initialize();
}

QTextButton::QTextButton(const QString &text, const QString &iconFilePath, QWidget *parent) : QPushButton(text, parent)
{
    pixmap = QPixmap(iconFilePath);
    initialize();
}

void QTextButton::initialize()
{
    this->setFixedHeight(32);

    this->clearStyles();
}

void QTextButton::setDefaultStyle(const QString &json)
{
    QString defaultStyle = QString("%1 QPushButton %2").arg(this->styleSheet()).arg(json);
    this->setStyleSheet(defaultStyle);
}

void QTextButton::setHoverStyle(const QString &json)
{
    QString hoverStyle = QString("%1 QPushButton:hover %2").arg(this->styleSheet()).arg(json);
    this->setStyleSheet(hoverStyle);
}

void QTextButton::clearStyles()
{
    this->setStyleSheet("QPushButton { \
                            border-radius: 5px; \
                            border: 1px solid rgb(218, 220, 224); \
                            font: 8pt 'Goldman'; \
                        } \
                        QPushButton:disabled { \
                            border-radius: 5px; \
                            border: 1px solid rgb(218, 220, 224, 125); \
                            font: 8pt 'Goldman'; \
                        } \
                        QPushButton:hover {\
                            border: 1px solid rgb(210, 227, 252); \
                            background-color: rgb(247, 250, 255); \
                        \
                        }");
}

void QTextButton::paintEvent(QPaintEvent* event)
{
    QPushButton::paintEvent(event);
    QPainter painter(this);

    if (!this->isEnabled())
        painter.setOpacity(0.5);

    if (!pixmap.isNull())
    {
        const int y = (this->height() - pixmap.height()) / 2;
        const int x = (this->width() - pixmap.width() - 16);

        painter.drawPixmap(x, y, pixmap);
    }

    if (this->isCheckable())
    {
        if (this->isChecked())
        {
            painter.setPen(QColor(0, 0, 0));
        }
        else
        {
            painter.setPen(QColor(255, 255, 255));
        }
        painter.drawText(26, 32, "yarname");
    }
}
