#include "qinboxbrowser.h"
#include "qinboxfilter.h"
#include "qinboxlistview.h"
#include "mainwidgets/qinbox.h"
#include "subwidgets/qinboxtoolbar.h"
#include "mainwidgets/qinboxmessagepane.h"

#include "mailer/engines.h"
#include "subwidgets/qnotificationbox.h"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QVariant>

QInboxBrowser::QInboxBrowser(QWidget *parent, Qt::WindowFlags f) : QFrame(parent, f)
{
    displayFormat = LAYOUT_HORIZONTAL;

    // rgb(241, 248, 255);
    //border:1px solid rgb(225, 228, 232);
    //border-radius: 10px;

    this->setProperty("tabType", QVariant(1));
    this->setObjectName("inboxbrowser");
    this->setStyleSheet(" \
                         QFrame#inboxbrowser { \
                            background-color: white; \
                         } \
                        ");

    filterPane = new QInboxFilter();
    listView = new QInboxListView(this);

    QVBoxLayout *filterLayout = new QVBoxLayout();
    filterLayout->setContentsMargins(0, 0, 0, 0);
    filterLayout->setSpacing(0);
    QWidget *filterTopBar = new QWidget();
    filterTopBar->setFixedHeight(22);
    filterTopBar->setStyleSheet("background-color: rgb(249, 250, 251); border-left: 5px solid rgb(91, 161, 123); border-right: 1px solid rgb(234, 238, 240);");
    filterLayout->addWidget(filterTopBar);
    filterLayout->addWidget(filterPane);

    mpHorizontalLayout = new QHBoxLayout();
    mpHorizontalLayout->setContentsMargins(0, 0, 0, 0);
    mpHorizontalLayout->setSpacing(0);

    mpVerticalLayout = new QVBoxLayout();
    mpVerticalLayout->setContentsMargins(0, 0, 0, 0);
    mpVerticalLayout->setSpacing(0);

    QVBoxLayout *browserLayout = new QVBoxLayout();
    browserLayout->setContentsMargins(0, 0, 0, 0);
    browserLayout->setSpacing(0);
    //mainLayout->addWidget(adbox);
    //mainLayout->addWidget(quickFilter);

    messagePane = new QInboxMessagePane(this);
    messagePane->hide();

    if (displayFormat == LAYOUT_VERTICAL)
    {
        mpVerticalLayout->addWidget(listView);
        mpVerticalLayout->addWidget(messagePane);

        browserLayout->addLayout(mpVerticalLayout);
    }
    else if (displayFormat == LAYOUT_HORIZONTAL)
    {
        mpHorizontalLayout->addWidget(listView);
        mpHorizontalLayout->addWidget(messagePane);

        browserLayout->addLayout(mpHorizontalLayout);
    }

    mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addLayout(filterLayout);
    mainLayout->addLayout(browserLayout);

    this->setLayout(mainLayout);

    QInbox *inbox = static_cast<QInbox *>(parent);
    this->connect(filterPane, SIGNAL(channelSelected(bool)), inbox->getToolBar(), SLOT(changeDownloadIcon(bool)));
}

void QInboxBrowser::showMessage(QInboxData *rxData, int status)
{
    QString userMessage("Recent Message received");

    if (status == 0)
    {
        int messageCount = listView->append(rxData);
        filterPane->updateData(messageCount);
    }
    else
    {
        if (status == -2)
            userMessage = QString("No messages to download");
        else
            userMessage = QString("Message not received (%1)").arg(status);
    }

    QNotificationBox *notifyBox = new QNotificationBox(userMessage, this);
    notifyBox->show();

    notifyBox->adjustSize();
    notifyBox->raise();

    int leftOffset = filterPane->width();
    int leftPos = leftOffset + (this->width() - leftOffset - notifyBox->width()) / 2;
    notifyBox->move(leftPos, 100);
}

void QInboxBrowser::showMessageDetails(QInboxListItem *itemData)
{
        messagePane->show(itemData);
}

