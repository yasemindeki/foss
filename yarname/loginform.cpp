#include "loginform.h"

#include <QtWidgets>

LoginForm::LoginForm(const QString &header, QWidget *parent) :
    QWidget(parent)
{
    this->setFont(QFont("Goldman", 9));
    this->setFixedWidth(320);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(10);

    topLabel = new QLabel(header);
    topLabel->setStyleSheet("font: 12pt 'Goldman';");
    topLabel->setAlignment(Qt::AlignHCenter);
    topLabel->setMargin(20);

    mainLayout->addWidget(topLabel);

    //"By signing in to yarname, you aggre to our terms of service.";
    termsLabel = new QLabel("By continuing, you aggre to the Terms of Service.");
    termsLabel->setStyleSheet("font: 8pt 'Goldman';");

    submitButton = new QPushButton("Continue");
    submitButton->setEnabled(false);
    submitButton->setFixedHeight(32);
    submitButton->setStyleSheet("QPushButton { \
                                        border-radius: 5px; \
                                        border: none; \
                                        font: 9pt 'Goldman'; \
                                        background-color: rgb(51, 109, 191); \
                                        color: white; \
                                    } \
                                    QPushButton:hover {\
                                        background-color: rgb(72, 128, 207); \
                                    } \
                                    QPushButton:disabled { \
                                        background-color: rgb(229, 234, 239); \
                                        color: rgb(115, 129, 155); \
                                    }");
}

void LoginForm::adjustSize()
{
    QWidget::adjustSize();
    emit sizeUpdated();
}
