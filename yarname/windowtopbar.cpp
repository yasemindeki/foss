#include "windowtopbar.h"
#include "subwidgets/qiconbutton.h"
#include <QtWidgets>

using namespace YPY;

WindowTopBar::WindowTopBar(QWidget *parent) : QWidget(parent)
{
    this->setFixedHeight(YPY::TOPBAR_HEIGHT);
    this->setContentsMargins(0, 0, 0, 0);

    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("QWidget { \
                            background-color: rgb(36, 41, 46); \
                            border-bottom: 1px solid rgb(192, 194, 204); \
                            color: rgb(255, 255, 255); \
                        }");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 10, 0);
    layout->setSpacing(15);
    layout->setAlignment(Qt::AlignRight);

    QIconButton *langButton = new QIconButton("Daha fazla dil göster");
    langButton->setFixedSize(16, 16);
    langButton->setIcons(":/icons/topbar-lang.png", ":/icons/topbar-lang-hover.png");

    QIconButton *accountButton = new QIconButton("Hesap İşlemleri");
    accountButton->setFixedSize(16, 16);
    accountButton->setIcons(":/icons/topbar-account.png", ":/icons/topbar-account.png");

    QIconButton *beaconButton = new QIconButton("Henüz Bildirim Yok");
    beaconButton->setFixedSize(16, 16);
    beaconButton->setIcons(":/icons/topbar-bell.png", ":/icons/topbar-bell.png");

    QIconButton *settingsButton = new QIconButton("Ayarlar");
    settingsButton->setFixedSize(16, 16);
    settingsButton->setIcons(":/icons/topbar-settings.png", ":/icons/topbar-settings.png");

    QIconButton *helpButton = new QIconButton("Destek");
    helpButton->setFixedSize(16, 16);
    helpButton->setIcons(":/icons/topbar-help.png", ":/icons/topbar-help.png");

    layout->addWidget(helpButton);
    layout->addWidget(settingsButton);
    layout->addWidget(beaconButton);
    layout->addWidget(langButton);
    layout->addWidget(accountButton);

    this->setLayout(layout);
}
