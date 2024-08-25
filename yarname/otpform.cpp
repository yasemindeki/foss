#include "otpform.h"
#include "subwidgets/qlineframe.h"
#include "subwidgets/qtextbutton.h"
#include "subwidgets/qlineinput.h"
#include "subwidgets/qbuttonlabel.h"
#include "net/restservice.h"

#include <QtWidgets>
#include <QTextEdit>

OtpForm::OtpForm(const QString &header, QWidget *parent) : LoginForm(header, parent)
{
    createWidgets();

    this->setLayout(mainLayout);
}

void OtpForm::createWidgets()
{
    infoLabel = new QLabel(this);
    infoLabel->setAlignment(Qt::AlignLeft);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("QLabel { \
                                font: 8pt 'Goldman'; \
                                color: rgb(0, 0, 0); \
                            } \
                            QLabel:disabled { \
                                font: 8pt 'Goldman'; \
                                color: rgb(0, 0, 0, 125); \
                             }");
    infoLabel->setText("We sent a code to your email address.Please check your email and enter the code below.");

    goBackLink = new QButtonLabel("Go Back");
    this->connect(goBackLink, SIGNAL(clicked()), (LoginScreen *)this->parent(), SLOT(goPreviousPage()));

    // "<a href=\"www.google.com\">Go Back</a>"
    /*goBackLink->setTextFormat(Qt::RichText);
    goBackLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    goBackLink->setOpenExternalLinks(true);*/

    emailAddress = new QLineInput("Email", this);
    emailAddress->setPixmap(":/icons/email.png");
    emailAddress->setDisabled(true);

    oneTimePassword = new QLineInput("Code", this);
    oneTimePassword->setPixmap(":/icons/loginkey.png");
    oneTimePassword->setFocus();
    oneTimePassword->setValidator("[0-9]{6}", submitButton);

    mainLayout->addWidget(topLabel);
    mainLayout->addWidget(emailAddress);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(infoLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(oneTimePassword);
    mainLayout->addWidget(submitButton);
    mainLayout->addWidget(termsLabel);
    mainLayout->addWidget(goBackLink);

    this->connect(submitButton, SIGNAL(pressed()), this, SLOT(onSubmitPressed()));
}

void OtpForm::setEnabled(bool isOn)
{
    //QWidget::setEnabled(isOn);

    infoLabel->setEnabled(isOn);
    oneTimePassword->setEnabled(isOn);
    submitButton->setEnabled(isOn);
    goBackLink->setEnabled(isOn);
}


void OtpForm::show(const QString &input)
{
    emailAddress->setText(input);
    QWidget::show();
}

void OtpForm::onSubmitPressed()
{
    setEnabled(false);
    RESTService::instance()->sendLoginRequest(emailAddress->text(), oneTimePassword->text());
}

