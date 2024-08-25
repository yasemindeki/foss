#include "qinboxmessagepane.h"
#include "subwidgets/qinboxmessageheader.h"
#include "subwidgets/qinboxmessagebody.h"
#include "subwidgets/qinboxlistitem.h"
#include "mailer/engines.h"

#include <QVBoxLayout>
#include <QLabel>

QInboxMessagePane::QInboxMessagePane(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(10);

    messageHeader = new QInboxMessageHeader(this);
    messageBody = new QInboxMessageBody(this);

    mainLayout->addWidget(messageHeader);
    mainLayout->addWidget(messageBody);
    mainLayout->addStretch();

    this->setObjectName("QInboxMessagePane");
    this->setAttribute(Qt::WA_StyledBackground);
    this->setStyleSheet("QWidget#QInboxMessagePane { \
                            background-color: rgb(250, 250, 250); \
                            border-left: 1px solid rgb(229, 229, 229); \
                        }");

    this->setLayout(mainLayout);
}

void QInboxMessagePane::show()
{
    QWidget::show();
}

void QInboxMessagePane::show(QInboxListItem *itemData)
{
    messageHeader->subjectLabel->setText(itemData->subject());
    messageBody->fromLabel->setText(itemData->from());
    messageBody->timeLabel->setText(itemData->time());
    messageBody->profileImage->setText(itemData->profile());

    if (this->isHidden())
        this->show();
}
