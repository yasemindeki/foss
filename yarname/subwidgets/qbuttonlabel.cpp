#include "qbuttonlabel.h"

QButtonLabel::QButtonLabel(const QString &text, QWidget *parent, Qt::WindowFlags f) : QLabel(text, parent, f)
{
    this->setStyleSheet("QLabel { \
                            font: 8pt 'Goldman'; \
                            color: rgb(51, 109, 191); \
                            text-decoration: underline; \
                        } \
                        QLabel:disabled { \
                            font: 8pt 'Goldman'; \
                            color: rgb(51, 109, 191, 125); \
                            text-decoration: underline; \
                        }");
    this->setCursor(Qt::PointingHandCursor);

}

void QButtonLabel::mousePressEvent(QMouseEvent* event)
{
    QLabel::mousePressEvent(event);

    emit clicked();
}
