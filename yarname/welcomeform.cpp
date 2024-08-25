#include "welcomeform.h"
#include "subwidgets/qlineframe.h"
#include "subwidgets/qtextbutton.h"
#include "subwidgets/qlineinput.h"
#include "net/restservice.h"

#include <QtWidgets>
#include <QTextEdit>

WelcomeForm::WelcomeForm(const QString &header, QWidget *parent) : LoginForm(header, parent)
{
    createWidgets();

    this->setLayout(mainLayout);
}

void WelcomeForm::createWidgets()
{
    googleSignButton = new QTextButton("Sign in with Google", ":/icons/google.png");
    otherSignButton = new QTextButton("Sign in with Other Platforms", ":/icons/down-arrow.png");
    orLine = new QLineFrame(this, QLineFrame::WithLabel);

    emailAddress = new QLineInput("Email", this);
    emailAddress->setPixmap(":/icons/email.png");
    emailAddress->setValidator(".+@.+", submitButton);

    mainLayout->addWidget(googleSignButton);
    mainLayout->addWidget(otherSignButton);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(orLine);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(emailAddress);
    mainLayout->addWidget(submitButton);
    mainLayout->addWidget(termsLabel);

    this->connect(submitButton, SIGNAL(pressed()), this, SLOT(onSubmitPressed()));

}

void WelcomeForm::setEnabled(bool isOn)
{
    //QWidget::setEnabled(isOn);

    googleSignButton->setEnabled(isOn);
    otherSignButton->setEnabled(isOn);
    emailAddress->setEnabled(isOn);
    orLine->setEnabled(isOn);
    submitButton->setEnabled(isOn);
}

QString WelcomeForm::getCustomerId()
{
    return emailAddress->text();
}

void WelcomeForm::onSubmitPressed()
{
    setEnabled(false);
    RESTService::instance()->sendLoginRequest(emailAddress->text());
}
