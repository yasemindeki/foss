#include "qlineframe.h"

#include <QtWidgets>

QLineFrame::QLineFrame(QWidget *parent, int type) : QFrame(parent)
{
    this->type = (FrameType)type;
    initialize(parent);
}

QLineFrame::QLineFrame(QWidget *parent, const QString &color, int type) : QFrame(parent)
{
    this->type = (FrameType)type;
    initialize(parent);

    this->setStyleSheet(QString("color: %1;").arg(color));
}

QLineFrame::~QLineFrame()
{
}

void QLineFrame::initialize(QWidget *parent)
{
    this->setFrameShape(QFrame::HLine);
    this->setLineWidth(1);

    this->setFrameShadow(QFrame::Plain);
    this->setFixedHeight(1);

    this->setAttribute(Qt::WA_StyledBackground);
    this->setStyleSheet("color: rgb(192, 194, 204); ");

    if (type == WithLabel)
    {
        orLabel = new QLabel("veya", parent);
        orLabel->setAlignment(Qt::AlignCenter);
        orLabel->setFixedSize(40, 20);
        orLabel->setStyleSheet("QLabel { \
                                    background-color: white; \
                                    font: 8pt 'Goldman'; \
                                    color: rgb(36, 41, 46); \
                                } \
                                QLabel:disabled { \
                                        background-color: white; \
                                        font: 8pt 'Goldman'; \
                                        color: rgb(36, 41, 46, 125); \
                                    }");

        connect(parent, SIGNAL(sizeUpdated()), this, SLOT(setLabelPos()));
    }

}

void QLineFrame::setEnabled(bool isOn)
{
    QFrame::setEnabled(isOn);
    if (type == WithLabel)
    {
        orLabel->setEnabled(isOn);
    }
}

void QLineFrame::setLabelPos()
{
    QRect rect = this->geometry();
    QPoint point((rect.width() / 2) - (orLabel->width() / 2), rect.top() - (orLabel->height() / 2) + 1);

    orLabel->raise();
    orLabel->move(point);
/*
    QDialog about;
    about.setWindowTitle(QString::number(point.y()));
    about.exec();
*/
}
