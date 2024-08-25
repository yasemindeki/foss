#include "qpostbrowser.h"
#include "subwidgets/qtextbutton.h"
#include "subwidgets/qlineinput.h"
#include "subwidgets/qaccordionwidget.h"
#include "subwidgets/qstyledtoolbar.h"
#include "mvc/qposttabledelegate.h"
#include "helpers/filehelper.h"

#include <QtWidgets>

QPostBrowser::QPostBrowser(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Akordiyon kutusu islemleri
    createSideBar();

    // Mini arac kutusu islemleri
    createMiniSideBar();

    // Tab kutusu islemleri
    createTabs();

    mainLayout->addWidget(sideBar);
    mainLayout->addWidget(miniSideBar);
    mainLayout->addWidget(browserTabs, 10);

    this->setLayout(mainLayout);

    // Layout atamasi sonrası QAccordionWidget.setItemLayout cagrilarini yapiyoruz.
    setAccordionContents();

    //QTimer::singleShot(0, this, SLOT(printLocations()));
}

QStringList QPostBrowser::channelIconPaths()
{
    static const QStringList iconList = { ":/icons/email.png", ":/icons/twitter-black.png" };

    return iconList;
}

void QPostBrowser::createSideBar()
{
    QList<QStringList> sideBarItems = {
        QStringList{"Creations", ":/icons/plus-hover.png", ":/icons/plus.png"},
        QStringList{"Filters", ":/icons/filter-hover.png", ":/icons/filter.png"},
        QStringList{"Channels", ":/icons/channels-hover.png", ":/icons/channels.png"}
    };
    sideBar = new QAccordionWidget(sideBarItems, this);

    sideBar->hide();
}

void QPostBrowser::createTabs()
{
    browserTabs = new QTabWidget();
    browserTabs->setContentsMargins(0, 0, 0, 0);
    browserTabs->setAttribute(Qt::WA_StyledBackground);

    FileHelper::loadStyleSheet(":/qss/postTabs.qss", browserTabs);

    createPostTable();

    browserTabs->addTab(postTable, "Posts");
    browserTabs->addTab(new QWidget, "Preferences");
    browserTabs->addTab(new QWidget, "Categories");
}

void QPostBrowser::createPostTable()
{
    postTable = new QTableWidget(this);
    postTable->setColumnCount(4);
    postTable->setAttribute(Qt::WA_StyledBackground);

    QStringList labels;
    labels << tr("Post Name") << tr("Channels") << tr("Number of Sent") << tr("Last Sent Time");
    postTable->setHorizontalHeaderLabels(labels);

    postTable->insertRow(0);
    postTable->insertRow(0);

    QTableWidgetItem *firstItem = new QTableWidgetItem("Multiple Channels Post");
    firstItem->setIcon(QIcon(":/icons/check.png"));

    QTableWidgetItem *secondItem = new QTableWidgetItem("Untitled Post");
    secondItem->setIcon(QIcon(":/icons/check.png"));

    postTable->setItem(0, 0, firstItem);
    postTable->setItem(1, 0, secondItem);

    postTable->setItem(0, 1, new QTableWidgetItem("___"));
    postTable->setItem(1, 1, new QTableWidgetItem("___"));

    postTable->setSelectionBehavior(QAbstractItemView::SelectRows);     // Secimin hucre hucre degil, satir satir olmasini sagliyor.
    postTable->setSelectionMode(QAbstractItemView::SingleSelection);    // Tabloda birden fazla satirin secilmesine izin vermiyor.
    postTable->setEditTriggers(QAbstractItemView::NoEditTriggers);      // Hucrelerdeki metin degistirilemiyor.
    postTable->setFocusPolicy(Qt::NoFocus);                             // Hucrelerin fare tiklamasi ile odakta gorunmemesini sagliyor.
    postTable->verticalHeader()->hide();                                // Basliklardan dikey olani sakliyor.
    postTable->setShowGrid(false);                                      // Baslik kismi haric, tablo veri satirlarinin yatay ve dikey cizgilerini sakliyor.
    postTable->horizontalHeader()->setStyleSheet("QHeaderView { \
                                                        font: 8pt 'Ubuntu'; \
                                                        color: rgb(53, 70, 98); \
                                                        border: none; \
                                                  } \
                                                  QHeaderView::section { \
                                                        background-color: rgb(255, 255, 255); \
                                                        border: none; \
                                                        padding-top: 10px; \
                                                        padding-left: 10px; \
                                                  }");

    postTable->setStyleSheet("QTableWidget { \
                                    font: 8pt 'Ubuntu'; \
                                    border: none; \
                                    background-color: rgb(249, 250, 251); \
                            } \
                            QTableWidget::item { \
                            } \
                            QTableWidget::item:selected { \
                                    background-color: rgb(234, 245, 251); \
                            }");

    QHeaderView *headerView = postTable->horizontalHeader();
    headerView->setSectionResizeMode(0, QHeaderView::Stretch);      // Belirli basliklarin stretch olabilmesini sagliyor.
    headerView->setSectionResizeMode(1, QHeaderView::Stretch);
    headerView->setHighlightSections(false);                        // Secili basliklarin font stili bold olmuyor.
    headerView->setDefaultAlignment(Qt::AlignLeft);                 // Metin hizalamasini sola yapiyor.
    //postTable->horizontalHeader()->setStretchLastSection(true);


    postTable->setItemDelegate(new QPostTableDelegate(this));

    postTable->selectRow(0);
}

void QPostBrowser::createMiniSideBar()
{
    miniSideBar = new QStyledToolBar(this);

    QAction *expandAction = new QAction(QIcon(":/icons/expand.png"), tr("Paneli Göster"), this);
    miniSideBar->addAction(expandAction, "expandPanel");

    QAction *plusAction = new QAction(QIcon(":/icons/plus.png"), tr("Yeni Posta"), this);
    miniSideBar->addAction(plusAction, "newPost");

    QAction *filterAction = new QAction(QIcon(":/icons/filter.png"), tr("Yeni Filtre"), this);
    miniSideBar->addAction(filterAction, "newFilter");

    QAction *channelAction = new QAction(QIcon(":/icons/channels.png"), tr("Kanal Ayarları"), this);
    miniSideBar->addAction(channelAction, "channelSettings");

    this->connect(expandAction, SIGNAL(triggered()), this, SLOT(onExpandActionTriggered()));
    this->connect(plusAction, SIGNAL(triggered()), this->parentWidget(), SLOT(onNewPostClicked()));
    this->connect(filterAction, SIGNAL(triggered()), this, SLOT(onFilterActionTriggered()));
    this->connect(channelAction, SIGNAL(triggered()), this, SLOT(onChannelActionTriggered()));
}

void QPostBrowser::onExpandActionTriggered()
{
    miniSideBar->hide();
    sideBar->show();
    sideBar->expandItem("Creations");
}

void QPostBrowser::onShrinkButtonClicked()
{
    sideBar->hide();
    miniSideBar->show();
}

void QPostBrowser::onPlusActionTriggered()
{
    miniSideBar->hide();
    sideBar->show();
    sideBar->expandItem("Creations");
}

void QPostBrowser::onFilterActionTriggered()
{
    miniSideBar->hide();
    sideBar->show();
    sideBar->expandItem("Filters");
}

void QPostBrowser::onChannelActionTriggered()
{
    miniSideBar->hide();
    sideBar->show();
    sideBar->expandItem("Channels");
}

void QPostBrowser::setAccordionContents()
{
    QTextButton *createPostButton = new QTextButton("Create Post", ":/icons/google.png", this);
    QVBoxLayout *firstAccordionLayout = new QVBoxLayout();
    firstAccordionLayout->addWidget(createPostButton);

    searchInput = new QLineInput(this);
    searchInput->setPixmap(":/icons/email.png");
    QVBoxLayout *filterAccordionLayout = new QVBoxLayout();
    filterAccordionLayout->addWidget(searchInput);

    sideBar->setItemLayout("Creations", firstAccordionLayout);
    sideBar->setItemLayout("Filters", filterAccordionLayout);

    this->connect(createPostButton, SIGNAL(pressed()), this->parentWidget(), SLOT(onNewPostClicked()));
}

void QPostBrowser::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    QWidget::adjustSize();
    emit sizeUpdated();
}
