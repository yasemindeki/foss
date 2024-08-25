#include "qstyledtoolbar.h"
#include "subwidgets/qlineframe.h"

QStyledToolBar::QStyledToolBar(QWidget *parent) : QToolBar(parent)
{
    this->setOrientation(Qt::Vertical);

    this->setMovable(false);
    //this->setContentsMargins(0, 0, 0, 0);

    QAction *previousPageAction = new QAction(QIcon(":/icons/previous.png"), tr("Önceki Sayfa"), this);
    this->addAction(previousPageAction, "previousPage");

    QLineFrame *hline = new QLineFrame(this, "rgb(234, 238, 240)");
    this->addWidget(hline);

    this->setStyleSheet("QToolBar { \
                            border-right: 1px solid rgb(234, 238, 240); \
                            background-color: rgb(255, 255, 255); \
                            spacing: 0; \
                        } \
                        QToolButton:hover, QToolButton#previousPage:hover { \
                            background-color: rgb(243, 240, 235); \
                            border: none; \
                        } \
                        QToolButton#previousPage { \
                            height: 39px; \
                        }");

    //this->connect(previousPageAction, SIGNAL(triggered()), this, SLOT(onPreviousActionTriggered()));
}

void QStyledToolBar::setActionName(QAction *action, const QString &name)
{
    if (action->objectName().isEmpty())
    {
        action->setObjectName(name);
    }

    this->widgetForAction(action)->setObjectName(action->objectName());
}

void QStyledToolBar::addAction(QAction *action, const QString &name)
{
    QToolBar::addAction(action);

    setActionName(action, name);
}
