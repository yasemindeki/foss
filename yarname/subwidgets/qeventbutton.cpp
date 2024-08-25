#include "qeventbutton.h"

#include <QDebug>

QEventButton::QEventButton(const QString &tooltip, QWidget *parent) : QToolButton(parent)
{
    this->setToolTip(tooltip);
    this->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->setStyleSheet("QToolButton { \
                            border: none; \
                            background-color: transparent; \
                        } \
                        QToolButton:hover { \
                            border: 1px solid rgb(36, 41, 46); \
                            border-radius: 15px; \
                            background-color: rgb(36, 41, 46); \
                        }");

}

void QEventButton::setIcons(const QString &filePath, const QString &hoverPath)
{
    iconPath = filePath;
    hoverIconPath = hoverPath;

    this->setIcon(QIcon(iconPath));
}

void QEventButton::enterEvent(QEvent *event)
{
    QToolButton::enterEvent(event);

    this->setFixedSize(30, 30);
    this->setIcon(QIcon(hoverIconPath));

    qDebug() << "QEventButton mouse enter";
}

void QEventButton::leaveEvent(QEvent *event)
{
    QToolButton::leaveEvent(event);

    this->setFixedSize(20, 20);
    this->setIcon(QIcon(iconPath));

    qDebug() << "QEventButton mouse leave";
}
