#include "mainwidget.h"
#include "subwidgets/qsidetabbar.h"
#include <QtWidgets>

using namespace YPY;

MainWidget::MainWidget(QWidget *parent) : QTabWidget(parent)
{
    this->setTabBar(new QSideTabBar);
    this->setTabPosition(QTabWidget::East);

    this->setAttribute(Qt::WA_StyledBackground);
    this->setStyleSheet("QTabWidget { \
                            background-color: rgb(242, 244, 252); \
                        } \
                        QTabWidget::pane { \
                             border-right: 1px solid rgb(192, 194, 204); \
                        }");

}
