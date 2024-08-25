#include "qinbox.h"
#include "subwidgets/qinboxtoolbar.h"
#include "subwidgets/qinboxbrowser.h"
#include "subwidgets/qinboxtabpane.h"

#include <QVBoxLayout>
#include <QSplitter>
#include <QTabBar>
#include <QDebug>

QInboxToolBar *QInbox::getToolBar()
{
    return topBar;
}

QInbox::QInbox(QWidget *parent) : QWidget(parent)
{
    mailEngine = new Mail::CURLEngine();

    topBar = new QInboxToolBar();
    browser = new QInboxBrowser(this);
    tabs = new QInboxTabPane();

    this->connect(topBar, SIGNAL(startDownloadProcess(bool)), this, SLOT(receiveMessages(bool)));

    tabs->addTab(browser, "Primary");

    QWidget *subscriptions = new QWidget();
    subscriptions->setProperty("tabType", QVariant(2));
    tabs->addTab(subscriptions, tr("Subscriptions"));

    /* QInboxFilter, isminden sadece filtreleme yapmayi saglayan ozellikler anlasilabilir.
     * Ornegin Thunderbird uygulamasinda QuickFilter diye bir arac var; herhangi bir kullanicinin tanimladigi veri tipleri olmaksizin
     * belirli kistaslara gore filtrelemeyi sagliyor (okunmus olanlar, yildizli olanlar, ekli olanlar vb.)
     *
     * Bizde ise kullanicinin olusturdugu folders/tags/labels/bookmarks da orada listeleniyor olsun istiyoruz.
     * Gerci onlar da filtreleme kistaslarina giriyor. Son kullaniciya verdigimiz isim outlook gibi "Folder Pane" de olabilir
     * Ama arkaplanda filtreleme birimleri oluyor burada listelenenler.
    */

    //QSplitter *hsplit = new QSplitter();
/*
    hsplit->setContentsMargins(0, 0, 10, 10);
    hsplit->addWidget(filterPane);
    hsplit->addWidget(browser);
    hsplit->setStyleSheet("background-color: rgb(241, 248, 255);");

    hsplit->setStretchFactor(0, 1);
    hsplit->setStretchFactor(1, 9);
*/
    QVBoxLayout *inboxLayout = new QVBoxLayout();
    inboxLayout->setContentsMargins(0, 0, 0, 0);
    inboxLayout->setSpacing(0);
    inboxLayout->addWidget(topBar);
    inboxLayout->addWidget(tabs);

    this->setLayout(inboxLayout);
}

QInbox::~QInbox()
{
    delete mailEngine;
    mailEngine = nullptr;
}

void QInbox::receiveMessages(bool importDone)
{
    QInboxData *inboxData = new QInboxData();

    // TODO: GUI donmasinin onune gecmek icin thread icine alinmasi gerek
    int rc = 0;

    mailEngine->loadFromFile(inboxData);

    /*
    if (importDone)
        rc = mailEngine->getRecentMessage(inboxData);
    else
        rc = mailEngine->startBulkImport(inboxData);
    */
    browser->showMessage(inboxData, rc);

    if (!importDone && rc == 0)
        topBar->changeDownloadIcon(true);
}
