#include "qinboxfilter.h"
#include "base/channeltypes.h"
#include "mvc/qinboxfilterdelegate.h"

#include <QTreeWidget>
#include <QHeaderView>

QInboxFilter::QInboxFilter(QWidget *parent) : QTreeWidget(parent)
{
    // Birden fazla satirin secilmesine izin vermiyor.
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    //padding-left: 5px;
    this->setStyleSheet("QTreeView { \
                            background-color: rgb(249, 250, 251); \
                            font: 8pt 'Goldman'; \
                            color: white; \
                            border: none; \
                            border-right: 1px solid rgb(234, 238, 240); \
                            border-left: 5px solid rgb(91, 161, 123); \
                            outline: 0; \
                        } QTreeView::item:hover { \
                              background-color: rgb(234, 245, 251); \
                              border: 1px solid rgb(234, 245, 251);  \
                              border-top-right-radius: 10px; \
                              border-bottom-right-radius: 10px; \
                              border-top-left-radius: 0px; \
                              border-bottom-left-radius: 0px; \
                        } QTreeView::item { \
                            color: black; \
                            height: 40px; \
                        } QTreeView::item:selected { \
                            background-color: rgb(91, 161, 123); \
                            color: white; \
                            border: 1px solid rgb(91, 161, 123); \
                            border-top-right-radius: 10px; \
                            border-bottom-right-radius: 10px; \
                        }");
    this->setFixedWidth(200);
    this->setColumnCount(1);

    // Kolonun genisliginin var olan alani kaplamasinin onune geciyoruz.
    this->header()->setStretchLastSection(false);
    this->setColumnWidth(0, 180);

    // Ustbasliklari yok
    this->setHeaderHidden(true);

    // Her bir tree item solundaki bosluk sifirlaniyor.
    //this->setIndentation(0);
    this->setRootIsDecorated(false);

    QTreeWidgetItem *firstFilterItem = new QTreeWidgetItem(this);
    firstFilterItem->setData(0, Qt::UserRole, CHANNEL_MAIL);
    firstFilterItem->setData(0, Qt::UserRole + 1, tr("accounts@yarname.com"));
    firstFilterItem->setData(0, Qt::UserRole + 2, 0);

    QTreeWidgetItem *inboxFilterItem = new QTreeWidgetItem(this);
    inboxFilterItem->setData(0, Qt::UserRole, CHANNEL_MAIL);
    inboxFilterItem->setData(0, Qt::UserRole + 1, tr("yasemin.yilmaz.embedded@gmail.com"));
    inboxFilterItem->setData(0, Qt::UserRole + 2, 0);
    //inboxFilterItem->setSizeHint(0, QSize(130, 20));
    //inboxFilterItem->setFlags(inboxFilterItem->flags() | Qt::ItemIsAutoTristate);

    QStringList mailItems{"Categories", "Spam", "Trash"};
    foreach (QString name, mailItems) {
        QTreeWidgetItem *item = new QTreeWidgetItem(inboxFilterItem);
        item->setText(0, name);
    }

    QTreeWidgetItem *twFilterItem = new QTreeWidgetItem(this);
    twFilterItem->setData(0, Qt::UserRole, CHANNEL_TWITTER);
    twFilterItem->setData(0, Qt::UserRole + 1, tr("yasemindeki"));
    twFilterItem->setData(0, Qt::UserRole + 2, 0);

    QStringList twItems{"Direct Messages", "Tweets"};
    foreach (QString name, twItems) {
        QTreeWidgetItem *item = new QTreeWidgetItem(twFilterItem);
        item->setText(0, name);
    }

    itemMap.insert("accounts@yarname.com", firstFilterItem);
    itemMap.insert("yasemin.yilmaz.embedded@gmail.com", inboxFilterItem);
    itemMap.insert("yasemindeki", twFilterItem);

    // ilk siradaki seciliyor
    this->setCurrentItem(this->topLevelItem(0));

    // sadece metin ve ikon saglayan gorunumu ozellestirmek istiyoruz.
    itemDelegate = new QInboxFilterDelegate(this);
    this->setItemDelegateForColumn(0, itemDelegate);

    connect(itemDelegate, SIGNAL(toggleChildrens(const QModelIndex &)), this, SLOT(onItemToggled(const QModelIndex &)));

    //connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), itemDelegate, SLOT(onCurrentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(onItemSelectionChanged()));

    //connect(this, SIGNAL(itemExpanded(QTreeWidgetItem *)), this, SLOT(onItemExpanded(QTreeWidgetItem *)));
    //connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem *)), this, SLOT(onItemCollapsed(QTreeWidgetItem *)));
}

void QInboxFilter::updateData(int messageCount)
{
    QTreeWidgetItem *topLevelItem = this->itemMap["yasemin.yilmaz.embedded@gmail.com"];
    topLevelItem->setData(0, Qt::UserRole + 2, messageCount);

    this->update();
}

void QInboxFilter::onItemToggled(const QModelIndex &index)
{
    if (this->isExpanded(index))
        this->setExpanded(index, false);
    else
        this->setExpanded(index, true);
}

void QInboxFilter::onItemSelectionChanged()
{
    bool importDone = false;

    QTreeWidgetItem *itemSelected = this->selectedItems().first();
    int messageCount = itemSelected->data(0, Qt::UserRole + 2).toInt();
    if (messageCount > 0)
        importDone = true;

    emit channelSelected(importDone);
}

void QInboxFilter::onItemExpanded(QTreeWidgetItem *)
{
}

void QInboxFilter::onItemCollapsed(QTreeWidgetItem *)
{
}


