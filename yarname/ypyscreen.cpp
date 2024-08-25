#include "ypyscreen.h"
#include "net/restservice.h"

#include "loginscreen.h"
#include "subwidgets/qsidegrip.h"
#include "subwidgets/qcornergrip.h"
#include "windowtitlebar.h"
#include "windowtopbar.h"
#include "subwidgets/qtopbarbutton.h"
#include "mainwidgets/qpostbrowser.h"
#include "mainwidgets/qpostdesigner.h"
#include "mainwidgets/qinbox.h"


#include <QSizeGrip>
#include <QtWidgets>

YPYScreen::YPYScreen(QWidget *parent) : QWidget(parent)
{
    this->connect(RESTService::instance(), SIGNAL(getStarted()), this, SLOT(showOnStartup()));

    QPalette palette = this->palette();
    QColor backColor(36, 41, 46);
    palette.setColor(QPalette::Window, backColor);          // window background-color

    this->setPalette(palette);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMinimumSize(720, 600);
    this->setContentsMargins(YPY::GRIP_SIZE, YPY::GRIP_SIZE, YPY::GRIP_SIZE, YPY::GRIP_SIZE);

    sideGrips.append(new QSideGrip(this, Qt::LeftEdge));
    sideGrips.append(new QSideGrip(this, Qt::RightEdge));
    sideGrips.append(new QSideGrip(this, Qt::TopEdge));
    sideGrips.append(new QSideGrip(this, Qt::BottomEdge));

    for (int i = 0; i < 4; i++){
        QCornerGrip *cornerGrip = new QCornerGrip(this);
        cornerGrip->resize(YPY::GRIP_SIZE, YPY::GRIP_SIZE);
        sideGrips[i]->resize(YPY::GRIP_SIZE, YPY::GRIP_SIZE);
        cornerGrips.append(cornerGrip);
    }

    createWidgets();
    createTopBarButtons();
}

void YPYScreen::onNewPostClicked()
{
    postDesignerStack->setCurrentIndex(1);
}

void YPYScreen::createWidgets()
{
    YPY::WindowTitleBar *titleBar = new YPY::WindowTitleBar(this);
    YPY::WindowTopBar *topBar = new YPY::WindowTopBar(this);

    mainWidget = new YPY::MainWidget(this);

    // Designer tabinda,
    // ic ice gecmis ve bir anda sadece biri gorunen widget'lar olmasi icin
    // QStackedWidget kullanildi.
    postDesignerStack = new QStackedWidget(mainWidget);

    postBrowser = new QPostBrowser(this);
    QPostDesigner *postDesigner = new QPostDesigner();

    postDesignerStack->addWidget(postBrowser);
    postDesignerStack->addWidget(postDesigner);
    postDesignerStack->setCurrentIndex(0);

    inbox = new QInbox(this);

    mainWidget->addTab(inbox, "Inbox");
    mainWidget->addTab(postDesignerStack, "Designer");
    mainWidget->addTab(new QWidget, "Dashboard");
    mainWidget->addTab(new QWidget, "Brand Kit");
    mainWidget->addTab(new QWidget, "Stats");
    mainWidget->addTab(new QWidget, "Logs");
    mainWidget->addTab(new QWidget, "+");

    mainWidget->setCurrentIndex(1);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setAlignment(Qt::AlignTop);

    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(topBar);
    mainLayout->addWidget(mainWidget);

    this->setLayout(mainLayout);
}

void YPYScreen::createTopBarButtons()
{
    workplaceButton = new QTopBarButton("Current Workplace", ":/icons/minimize-sign-11px.png", this);
    mailboxButton = new QTopBarButton("Current Mailbox", ":/icons/minimize-sign-11px.png", this);

    workplaceButton->move(YPY::GRIP_SIZE, YPY::TITLEBAR_HEIGHT + 2);
    mailboxButton->move(YPY::GRIP_SIZE + YPY::TOPBAR_TEXTBUTTON_WIDTH, YPY::TITLEBAR_HEIGHT + 2);

    workplaceButton->setDepends(mailboxButton);
    mailboxButton->setDepends(workplaceButton);
}

void YPYScreen::setLoginWidget(LoginScreen *src)
{
    loginWidget = src;
}

void YPYScreen::showOnStartup()
{
    loginWidget->close();
    loginWidget->deleteLater();
    loginWidget = nullptr;

    this->showMaximized();
}

void YPYScreen::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    //qDebug() << "YPYScreen width = " << this->width();

    if (workplaceButton != nullptr)
    {
        workplaceButton->resize(mainWidget->height());
        mailboxButton->resize(mainWidget->height());
    }

    QRect outRect = this->rect();
    QRect inRect = outRect.adjusted(YPY::GRIP_SIZE, YPY::GRIP_SIZE, -YPY::GRIP_SIZE, -YPY::GRIP_SIZE);
/*
    // move(x,y)
    sizeGrips[0]->move(0, 0);
    sizeGrips[1]->move(outRect.right() - YPY::GRIP_SIZE, 0);
    sizeGrips[2]->move(outRect.right() - YPY::GRIP_SIZE, outRect.bottom() - YPY::GRIP_SIZE);
    sizeGrips[3]->move(0, outRect.bottom() - YPY::GRIP_SIZE);
*/

    cornerGrips[0]->setGeometry(QRect(outRect.topLeft(), inRect.topLeft()));
    cornerGrips[1]->setGeometry(QRect(outRect.topRight(), inRect.topRight()).normalized());
    cornerGrips[2]->setGeometry(QRect(inRect.bottomRight(), outRect.bottomRight()));
    cornerGrips[3]->setGeometry(QRect(outRect.bottomLeft(), inRect.bottomLeft()).normalized());

    sideGrips[0]->setGeometry(0, inRect.top(), YPY::GRIP_SIZE, inRect.height());
    sideGrips[1]->setGeometry(inRect.left() + inRect.width(), inRect.top(), YPY::GRIP_SIZE, inRect.height());
    sideGrips[2]->setGeometry(inRect.left(), 0, inRect.width(), YPY::GRIP_SIZE);
    sideGrips[3]->setGeometry(YPY::GRIP_SIZE, inRect.top() + inRect.height(), inRect.width(), YPY::GRIP_SIZE);
}


void YPYScreen::mousePressEvent(QMouseEvent *event)
{
    if (event->pos().y() < YPY::TITLEBAR_HEIGHT)
    {
        if (event->button() == Qt::LeftButton) {
            dragPosition = event->globalPos() - this->frameGeometry().topLeft();
            event->accept();
        }
    }
}

void YPYScreen::mouseMoveEvent(QMouseEvent *event)
{
    if (event->pos().y() < YPY::TITLEBAR_HEIGHT)
    {
        if (event->buttons() & Qt::LeftButton) {
            this->move(event->globalPos() - dragPosition);

            event->accept();
        }
    }
}

