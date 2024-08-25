#include "qinboxtabpane.h"
#include "subwidgets/qinboxtabbar.h"

#include <QVariant>

QInboxTabPane::QInboxTabPane(QWidget *parent) : QTabWidget(parent)
{
    inboxTabBar = new QInboxTabBar(this);

    this->setAttribute(Qt::WA_StyledBackground);
    this->setTabPosition(QTabWidget::South);
    this->setTabBar(inboxTabBar);

    /*margin-top: 0px;
    margin-bottom: 8px;
    font-weight: bold;
    border-top-left-radius: 0px;
    border-top-right-radius: 0px;
    border-bottom-left-radius: 5px;
    border-bottom-right-radius: 5px;
    border-left: none;
    border-right: none;
    border-top: none;
    */
    // QTabWidget::pane icin border-bottom 10px yaparsak, arada beyaz bosluk olusturabiliriz.
    this->setStyleSheet("QTabWidget::pane { border: none; border-bottom: 0px solid white; } \
                         QTabWidget { background-color: rgb(91, 161, 123); } \
                        }");

}

int QInboxTabPane::addTab(QWidget *widget, const QString &label)
{
    return QTabWidget::addTab(widget, label);
}

void QInboxTabPane::tabInserted(int index)
{
    QTabWidget::tabInserted(index);

    int tabType = this->widget(index)->property("tabType").toInt();

    QString iconPath;
    switch (tabType)
    {
    case 1:
        iconPath = ":/icons/channel-email-black.png";
        break;
    case 2:
        iconPath = ":/icons/subscriptions.png";
        break;
    default:
        iconPath = ":/icons/channel-email-black.png";
        break;
    }

    inboxTabBar->setTabData(index, QVariant(iconPath));
}

/*
int QInboxTabPane::addTab(QWidget *widget, const QIcon& icon, const QString &label)
{
    QTabWidget::addTab(widget, icon, label);
}
*/
