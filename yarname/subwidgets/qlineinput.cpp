#include "qlineinput.h"

#include <QtWidgets>

QLineInput::QLineInput(QWidget *parent) : QLineEdit(parent)
{
    initialize();
}

QLineInput::QLineInput(const QString &title, QWidget *parent) : QLineEdit(parent)
{
    initialize();

    titleLabel = new QLabel(title, parent);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedSize(50, 14);
    titleLabel->setMinimumSize(50, 14);
    titleLabel->setStyleSheet("QLabel { \
                                background-color: white; \
                                font: 8pt 'Goldman'; \
                                color: rgb(36, 41, 46); \
                                border-radius: 7px; \
                                border: 1px solid white; \
                              } \
                                QLabel:disabled { \
                                background-color: white; \
                                font: 8pt 'Goldman'; \
                                color: rgb(36, 41, 46, 125); \
                                border-radius: 7px; \
                                border: 1px solid white; \
                              }");

    this->connect(parent, SIGNAL(sizeUpdated()), this, SLOT(setLabelPos()));
}

void QLineInput::initialize()
{
    this->setTextMargins(8, 8, 30, 8);
    this->setFixedHeight(32);
    this->setStyleSheet("QLineEdit { \
                            border: 1px solid rgb(218, 220, 224); \
                            font: 8pt 'Goldman'; \
                            border-radius: 5px; \
                            background-color: palette(base); \
                            color: rgb(169, 169, 169); \
                            padding-left: 10px; \
                        } \
                        QLineEdit:hover { \
                            border: 1px solid rgb(169, 169, 169); \
                        } \
                        QLineEdit:focus { \
                            border: 1px solid rgb(51, 109, 191); \
                            color: rgb(51, 109, 191); \
                        } \
                        QLineEdit:disabled { \
                            background-color: rgb(238, 244, 254, 125); \
                            color: rgb(64, 64, 64); \
                        }");
}

void QLineInput::setEnabled(bool isOn)
{
    QLineEdit::setEnabled(isOn);
    titleLabel->setEnabled(isOn);
}

void QLineInput::setLabelPos()
{
    QRect rect = this->geometry();
    QPoint point(8, rect.top() - (titleLabel->height() / 2));

    titleLabel->raise();
    titleLabel->move(point);
}

void QLineInput::setPixmap(const QString &iconFilePath)
{
    pixmap = QPixmap(iconFilePath);
}

void QLineInput::paintEvent(QPaintEvent* event)
{
    QLineEdit::paintEvent(event);
    QPainter painter(this);

    if (!this->isEnabled())
        painter.setOpacity(0.5);

    if (!pixmap.isNull())
    {
        const int y = (this->height() - pixmap.height()) / 2;   // Dikey ortaliyoruz
        const int x = (this->width() - pixmap.width() - 16);    // Saga dayiyoruz

        painter.drawPixmap(x, y, pixmap);
    }
}

void QLineInput::setValidator(const QString &regex, QPushButton *submitButton)
{
    QValidator *validator = new QRegularExpressionValidator(QRegularExpression(regex), this);
    QLineEdit::setValidator(validator);

    continueButton = submitButton;
    this->connect(this, SIGNAL(textEdited(QString)), this, SLOT(changeSubmitButton(const QString &)));
}

void QLineInput::changeSubmitButton(const QString &)
{
    if (continueButton)
    {
        if (this->hasAcceptableInput())
            continueButton->setEnabled(true);
        else
            continueButton->setDisabled(true);
    }
}

