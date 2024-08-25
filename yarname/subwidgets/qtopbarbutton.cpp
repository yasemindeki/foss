#include "qtopbarbutton.h"
#include "windowtopbar.h"
#include "subwidgets/qtextbutton.h"

#include <QtWidgets>

using namespace YPY;

QTopBarButton::QTopBarButton(const QString &title, const QString &leftIcon, QWidget *parent) : QWidget(parent)
{
    this->setFixedWidth(YPY::TOPBAR_TEXTBUTTON_WIDTH);
    this->setMinimumHeight(400);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setAlignment(Qt::AlignTop);

    pushButton = new QTextButton(title, ":/icons/down-arrow.png", this);
    pushButton->setFixedWidth(YPY::TOPBAR_TEXTBUTTON_WIDTH);
    pushButton->setFixedHeight(YPY::TOPBAR_HEIGHT - 1);
    pushButton->setIcon(QIcon(leftIcon));
    pushButton->setCheckable(true);

    pushButton->setDefaultStyle("{ \
                                    border: none; \
                                    border-right: 1px solid rgb(0, 0, 0); \
                                    border-radius: 0; \
                                    text-align: left top; \
                                    padding-top: 8px; \
                                    padding-left: 10px; \
                                    color: rgb(209, 213, 218); \
                                    background-color: rgb(36, 41, 46); \
                                 }");
    pushButton->setHoverStyle("{ \
                                    border: none; \
                                    border-right: 1px solid rgb(0, 0, 0); \
                                    border-radius: 0; \
                                    background-color: rgb(47, 54, 61); \
                                }");

    menuPane = new QWidget(this);
    menuPane->setAttribute(Qt::WA_StyledBackground, true);
    menuPane->setStyleSheet("background-color: rgb(255, 255, 255);");
    menuPane->setFixedHeight(300);
    menuPane->hide();

    mainLayout->addWidget(pushButton);
    mainLayout->addWidget(menuPane);
    mainLayout->addStretch();

    this->setLayout(mainLayout);

    this->connect(pushButton, SIGNAL(clicked(bool)), this, SLOT(onButtonClicked(bool)));
}

void QTopBarButton::resize(int menuPaneHeight)
{
    menuPane->setFixedHeight(menuPaneHeight);
    if (menuPane->isHidden())
    {
        this->setFixedHeight(YPY::TOPBAR_HEIGHT);
    }
    else
    {
        this->setFixedHeight(YPY::TOPBAR_HEIGHT + menuPaneHeight);
    }
}

void QTopBarButton::setDepends(QTopBarButton *to)
{
    twin = to;
}

void QTopBarButton::onButtonClicked(bool checked)
{
    pushButton->clearStyles();

    if (!checked)
    {
        pushButton->setDefaultStyle("{ \
                                        border: none; \
                                        border-right: 1px solid rgb(0, 0, 0); \
                                        border-radius: 0; \
                                        text-align: left top; \
                                        padding-top: 8px; \
                                        padding-left: 10px; \
                                        color: rgb(209, 213, 218); \
                                        background-color: rgb(36, 41, 46); \
                                     }");
        pushButton->setHoverStyle("{ \
                                        border: none; \
                                        border-right: 1px solid rgb(0, 0, 0); \
                                        border-radius: 0; \
                                        background-color: rgb(47, 54, 61); \
                                    }");
        menuPane->hide();
        this->setFixedHeight(YPY::TOPBAR_HEIGHT);
    }
    else
    {
        twin->onTwinChecked();
        pushButton->setDefaultStyle("{ \
                                        border: none; \
                                        border-radius: 0; \
                                        text-align: left top; \
                                        padding-top: 8px; \
                                        padding-left: 10px; \
                                        color: rgb(36, 41, 46); \
                                        background-color: rgb(255, 255, 255); \
                                         }");
        pushButton->setHoverStyle("{ \
                                       border: none; \
                                       border-radius: 0; \
                                       background-color: rgb(255, 255, 255); \
                                   }");
        menuPane->show();
        this->setFixedHeight(YPY::TOPBAR_HEIGHT + menuPane->height());
    }

}

void QTopBarButton::onTwinChecked()
{
    if (!menuPane->isHidden())
    {
        pushButton->setChecked(false);
        onButtonClicked(false);
    }
}

