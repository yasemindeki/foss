#include "qinboxlistview.h"
#include "qinboxlistitem.h"
#include "mailer/engines.h"
#include "subwidgets/qinboxbrowser.h"

#include <QVBoxLayout>

QInboxListView::QInboxListView(QWidget *parent) : QScrollArea(parent)
{
    owner = (QInboxBrowser *)parent;

    listWidget = new QWidget();

    //this->setWidgetResizable(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
/*
    for (int i= 0; i < 5; i++)
    {
        QInboxListItem *finderItem = new QInboxListItem();
        mainLayout->addWidget(finderItem);
    }
*/
    listWidget->setLayout(mainLayout);

    this->setWidget(listWidget);
    this->setStyleSheet("QScrollArea { border: none; background-color: transparent; }");
}

int QInboxListView::append(QInboxData *rxData)
{
    QInboxListItem *inboxItem = new QInboxListItem(rxData, this);

    this->connect(inboxItem, SIGNAL(clicked(QInboxListItem *)), owner, SLOT(showMessageDetails(QInboxListItem *)));

    int itemIndex = mainLayout->count();
    int totalHeight = (itemIndex + 1) * inboxItem->height();
    mainLayout->addWidget(inboxItem, itemIndex);

    listWidget->setFixedHeight(totalHeight);

    listWidget->update();
    this->update();

    return mainLayout->count();
}

void QInboxListView::resizeEvent(QResizeEvent *event)
{
    QScrollArea::resizeEvent(event);

    listWidget->setFixedWidth(this->width());
}
