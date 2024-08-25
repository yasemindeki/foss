#include "windowtitlebar.h"
#include "subwidgets/qiconbutton.h"

#include <QtWidgets>

using namespace YPY;


WindowTitleBar::WindowTitleBar(QWidget *parent) : QWidget(parent)
{
    this->setFixedHeight(YPY::TITLEBAR_HEIGHT);
    this->setContentsMargins(0, 0, 0, 0);

    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("QWidget { \
                            background-color: rgb(36, 41, 46); \
                            border-bottom: 1px solid rgb(0, 0, 0); \
                        }");

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 5, 5, 0);

    QIconButton *minimizeButton = new QIconButton("Simge durumuna küçült");
    maximizeButton = new QIconButton("Tam ekrana geçiş");
    QIconButton *closeButton = new QIconButton("Kapat");

    minimizeButton->setFixedSize(16, 16);
    minimizeButton->setIcons(":/icons/minimize-sign-11px.png", ":/icons/minimize-sign-11px-hover.png");

    maximizeButton->setFixedSize(18, 15);
    maximizeButton->setCheckable(true);
    maximizeButton->setIcons(":/icons/exit-fullscreen.png", ":/icons/exit-fullscreen-hover.png");

    closeButton->setFixedSize(16, 16);
    closeButton->setIcons(":/icons/close.png", ":/icons/close-hover.png");

    this->connect(minimizeButton, SIGNAL(pressed()), this->parent(), SLOT(showMinimized()));
    this->connect(closeButton, SIGNAL(pressed()), this->parent(), SLOT(close()));
    this->connect(maximizeButton, SIGNAL(pressed()), this, SLOT(onMaximizeClicked()));

    layout->setSpacing(10);
    layout->addStretch();
    layout->addWidget(minimizeButton);
    layout->addWidget(maximizeButton);
    layout->addWidget(closeButton);

    this->setLayout(layout);

}

void WindowTitleBar::onMaximizeClicked()
{
    if (maximizeButton->isChecked())
    {
        this->parentWidget()->showNormal();

        maximizeButton->setToolTip("Tam ekrana geçiş");
        maximizeButton->setChecked(false);

    }
    else
    {
        this->parentWidget()->showMaximized();

        maximizeButton->setToolTip("Tam ekrandan çıkış");
        maximizeButton->setChecked(true);

    }
    //maximizeButton->setIcons(":/icons/exit-fullscreen.png", ":/icons/exit-fullscreen-hover.png");

}

