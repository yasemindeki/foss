#include "qpostdesigner.h"
#include "subwidgets/qaccordionwidget.h"
#include "subwidgets/qstyledtoolbar.h"
#include "helpers/filehelper.h"
#include "subwidgets/qpostwindow.h"

#include <QtWidgets>

QPostDesigner::QPostDesigner(QWidget *parent) : QWidget(parent)
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
    mainLayout->addWidget(designerTabs, 10);

    this->setLayout(mainLayout);

    // Layout atamasi sonrası QAccordionWidget.setItemLayout cagrilarini yapiyoruz.
    //setAccordionContents();
}

void QPostDesigner::createSideBar()
{
    QList<QStringList> sideBarItems = {
        QStringList{"Channels", ":/icons/channels-hover.png", ":/icons/channels.png"},
        QStringList{"Undo", ":/icons/plus-hover.png", ":/icons/plus.png"},
        QStringList{"Redo", ":/icons/filter-hover.png", ":/icons/filter.png"},
        QStringList{"Save", ":/icons/filter-hover.png", ":/icons/filter.png"}
    };
    sideBar = new QAccordionWidget(sideBarItems, this);
    sideBar->setHeaderText("1 post created");

    sideBar->hide();
}

void QPostDesigner::createMiniSideBar()
{
    miniSideBar = new QStyledToolBar(this);

    QAction *expandAction = new QAction(QIcon(":/icons/expand.png"), tr("Paneli Göster 2"), this);
    miniSideBar->addAction(expandAction, "expandPanel");

    QAction *channelAction = new QAction(QIcon(":/icons/channels.png"), tr("Kanal Ayarları"), this);
    miniSideBar->addAction(channelAction, "channelSettings");

    QAction *undoAction = new QAction(QIcon(":/icons/plus.png"), tr("Geri Al"), this);
    miniSideBar->addAction(undoAction, "newPost");

    QAction *redoAction = new QAction(QIcon(":/icons/plus.png"), tr("İleri Al"), this);
    miniSideBar->addAction(redoAction, "newPost");

    QAction *saveAction = new QAction(QIcon(":/icons/filter.png"), tr("Kaydet"), this);
    miniSideBar->addAction(saveAction, "newFilter");


    this->connect(expandAction, SIGNAL(triggered()), this, SLOT(onExpandActionTriggered()));
/*
    this->connect(plusAction, SIGNAL(triggered()), this->parentWidget(), SLOT(onNewPostClicked()));
    this->connect(filterAction, SIGNAL(triggered()), this, SLOT(onFilterActionTriggered()));
    this->connect(channelAction, SIGNAL(triggered()), this, SLOT(onChannelActionTriggered()));
    */
}

void QPostDesigner::onExpandActionTriggered()
{
    miniSideBar->hide();
    sideBar->show();
    //sideBar->expandItem("Creations");
}

void QPostDesigner::onShrinkButtonClicked()
{
    sideBar->hide();
    miniSideBar->show();
}

void QPostDesigner::createTabs()
{
    designerTabs = new QTabWidget();
    designerTabs->setContentsMargins(0, 0, 0, 0);
    designerTabs->setAttribute(Qt::WA_StyledBackground);

    FileHelper::loadStyleSheet(":/qss/postTabs.qss", designerTabs);

    QWidget *designTab = new QWidget();
    designTab->setAttribute(Qt::WA_StyledBackground);
    designTab->setStyleSheet("background-color: rgb(255, 255, 255);");

    QPostWindow *postWindow = new QPostWindow(this);
    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->setContentsMargins(0, 0, 0, 0);

    vlayout->addWidget(postWindow, 8);
    vlayout->addStretch(2);

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setContentsMargins(0, 30, 0, 0);   // 30 piksel yukaridan bosluk birakiyoruz.
    hlayout->addStretch(1);
    hlayout->addLayout(vlayout, 8);
    hlayout->addStretch(1);

    designTab->setLayout(hlayout);

    QScrollArea *contentArea = new QScrollArea();
    contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    contentArea->setWidgetResizable(true);
    contentArea->setWidget(designTab);
    designerTabs->addTab(contentArea, "Design");

    //designerTabs->addTab(designTab, "Design");
    designerTabs->addTab(new QWidget, "Preview");
}
