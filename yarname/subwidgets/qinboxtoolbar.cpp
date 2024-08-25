#include "qinboxtoolbar.h"

#include <QAction>
#include <QDebug>

QInboxToolBar::QInboxToolBar(QWidget *parent) : QToolBar(parent)
{
    this->setStyleSheet("QToolBar { \
                            background-color: rgb(249, 250, 251); \
                            border-bottom: 1px solid rgb(234, 238, 240); \
                        } \
    QToolButton { margin-left: 0px; border:none; background-color:transparent; } \
    QToolButton:hover { border-bottom: 2px solid rgb(91, 161, 123); }  \
                         }");
    this->setFixedHeight(26);

    actionDownload = new QAction(QIcon(":/icons/bulkimport.png"), tr("Import Messages"), this);
    this->addAction(actionDownload);

    this->connect(actionDownload, SIGNAL(triggered()), this, SLOT(onDownloadClicked()));

/*
    getButton = new QPushButton(QIcon(":/icons/refresh.png"), "Get Messages", this);
    getButton->setStyleSheet("QPushButton { margin-left: 10px; font: 8pt 'Goldman'; padding-top: 0px; padding-bottom: 0px; padding-right: 10px; padding-left: 10px; border-radius: 2px; border: 1px solid rgb(56, 113, 193); background-color: rgb(56, 113, 193); color: white; }");

    this->addWidget(getButton);
    this->addSeparator();
*/
}

void QInboxToolBar::onDownloadClicked()
{
    emit startDownloadProcess(importCompleted);
}

void QInboxToolBar::showEvent(QShowEvent *event)
{
    QToolBar::showEvent(event);

    //qDebug() << "getButton font is " << actionDownload->font().family();
}

void QInboxToolBar::changeDownloadIcon(bool importDone)
{
    importCompleted = importDone;

    if (importDone)
    {
        actionDownload->setIcon(QIcon(":/icons/refresh.png"));
        actionDownload->setToolTip(tr("Get Messages"));
    }
    else
    {
        actionDownload->setIcon(QIcon(":/icons/bulkimport.png"));
        actionDownload->setToolTip(tr("Import Messages"));
    }
}
