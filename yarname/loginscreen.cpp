#include "loginscreen.h"
#include "welcomeform.h"
#include "otpform.h"
#include "subwidgets/qiconbutton.h"
#include "subwidgets/qlineframe.h"
#include "subwidgets/qnotificationbox.h"
#include "net/restservice.h"

#include <QtWidgets>
#include <QNetworkReply>
#include <QScreen>

LoginScreen::LoginScreen(QWidget *parent)
    : QWidget(parent)
{
    QPalette palette = this->palette();
    QColor textColor(36, 41, 46);
    palette.setColor(QPalette::Text, textColor);
    palette.setColor(QPalette::ButtonText, textColor);      // QPushButton, QTextEdit.. color
    palette.setColor(QPalette::WindowText, textColor);      // QLabel.. color
    palette.setColor(QPalette::Window, Qt::white);          // window background-color
    this->setPalette(palette);

    createWidgets();

    this->connect(RESTService::instance(), SIGNAL(networkError(QNetworkReply::NetworkError)), this, SLOT(showNotification(QNetworkReply::NetworkError)));
    this->connect(RESTService::instance(), SIGNAL(ok()), this, SLOT(goNextPage()));
    this->connect(RESTService::instance(), SIGNAL(loginError(const QString &)), this, SLOT(showNotification(const QString &)));

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800, 600);

    this->move(QGuiApplication::primaryScreen()->geometry().center() - this->frameGeometry().center());
}

LoginScreen::~LoginScreen()
{
}

void LoginScreen::showNotification(QNetworkReply::NetworkError error)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<QNetworkReply::NetworkError>();
    QString errorMessage = QString("Network hatası: %1").arg(metaEnum.valueToKey(error));

    this->showNotification(errorMessage);
}

void LoginScreen::showNotification(const QString &errorMessage)
{
    notifyBox = new QNotificationBox(errorMessage, this);
    notifyBox->show();

    notifyBox->adjustSize();
    notifyBox->raise();

    int leftPos = (leftPaneWidth - notifyBox->width()) / 2;
    notifyBox->move(leftPos, 100);

    welcomeForm->setEnabled(true);
    otpForm->setEnabled(true);
}


void LoginScreen::createWidgets()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    rightPane = this->createRightPane();
    QHBoxLayout *topBarLayout = this->createTopBar();

    welcomeForm = new WelcomeForm("yarname Uygulamamıza\nHoş Geldiniz", this);
    otpForm = new OtpForm("Güvenli Giriş", this);
    otpForm->hide();

    formLayout = new QHBoxLayout();
    formLayout->addStretch();
    formLayout->addWidget(welcomeForm);
    formLayout->addWidget(otpForm);
    formLayout->addStretch();

    QLabel *copyrightLabel = new QLabel("2022 Yarname, Inc. All rights reserved.");
    copyrightLabel->setAlignment(Qt::AlignHCenter);

    hline = new QLineFrame(this);

    QVBoxLayout *leftPaneLayout = new QVBoxLayout();
    leftPaneLayout->setContentsMargins(0, 0, 0, 10);
    leftPaneLayout->addLayout(topBarLayout);
    leftPaneLayout->addWidget(hline);
    leftPaneLayout->addStretch();
    leftPaneLayout->addLayout(formLayout);
    leftPaneLayout->addStretch();
    leftPaneLayout->addWidget(copyrightLabel);

    mainLayout->addLayout(leftPaneLayout, 6);
    mainLayout->addWidget(rightPane, 4);

    this->setFont(QFont("Goldman", 8));
    this->setLayout(mainLayout);

    this->adjustSize();
    welcomeForm->adjustSize();
}

QHBoxLayout *LoginScreen::createTopBar()
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->setContentsMargins(10, 5, 10, 5);
    layout->setSpacing(0);

    QLabel *logoText = new QLabel();
    logoText->setPixmap(QPixmap(":/images/logo-with-text.PNG"));

    QIconButton *langButton = new QIconButton("Daha fazla dil göster");
    langButton->setFixedSize(26, 25);
    langButton->setIcons(":/icons/globe.png", ":/icons/globe-hover.png");

    //QLabel *signupLabel = new QLabel("Don't have an account?", this);
    //QPushButton *signupButton = new QPushButton("Sign up", this);
    //signupButton->setFlat(true);

    layout->addWidget(logoText);
    layout->addStretch();
    layout->addWidget(langButton);
    //topBarLayout->addWidget(signupLabel);
    //topBarLayout->addWidget(signupButton);

    return layout;
}

QFrame *LoginScreen::createRightPane()
{
    QFrame *frame = new QFrame(this);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setAlignment(Qt::AlignTop);

    frame->setFrameStyle(QFrame::StyledPanel);
    frame->setStyleSheet("QFrame { \
                            border-image: url(:/images/right-pane-320px.png); \
                            background-repeat: none; \
                         }");

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

    this->connect(minimizeButton, SIGNAL(pressed()), this, SLOT(showMinimized()));
    this->connect(closeButton, SIGNAL(pressed()), this, SLOT(close()));
    this->connect(maximizeButton, SIGNAL(pressed()), this, SLOT(showMaximized()));

    layout->setSpacing(10);
    layout->addStretch();
    layout->addWidget(minimizeButton);
    layout->addWidget(maximizeButton);
    layout->addWidget(closeButton);

    frame->setLayout(layout);

    return frame;
}

void LoginScreen::adjustSize()
{
    QWidget::adjustSize();
    emit sizeUpdated();
}

void LoginScreen::showMaximized()
{
    if (maximizeButton->isChecked())
    {
        QWidget::showNormal();

        maximizeButton->setToolTip("Tam ekrana geçiş");
        maximizeButton->setChecked(false);

        rightPane->setStyleSheet("QFrame { \
                                border-image: url(:/images/right-pane-320px.png); \
                                background-repeat: none; \
                             }");
    }
    else
    {
        QWidget::showMaximized();

        maximizeButton->setToolTip("Tam ekrandan çıkış");
        maximizeButton->setChecked(true);

        rightPane->setStyleSheet("QFrame { \
                                border-image: url(:/images/right-pane-545px.png); \
                                background-repeat: none; \
                             }");
    }
    //maximizeButton->setIcons(":/icons/exit-fullscreen.png", ":/icons/exit-fullscreen-hover.png");

}

void LoginScreen::resizeEvent(QResizeEvent *event)
{
    static int counter = 0;
    QWidget::resizeEvent(event);
    counter++;

    leftPaneWidth = hline->width();
/*
    QDialog about;
    about.setWindowTitle(QString::number(this->width()));
    about.exec();
*/
}

void LoginScreen::goNextPage()
{
    QString email = welcomeForm->getCustomerId();

    welcomeForm->setEnabled(true);
    welcomeForm->hide();
    otpForm->show(email);

    this->adjustSize();
    otpForm->adjustSize();

    this->resize(800, 600);

    //Timer başlatma
}

void LoginScreen::goPreviousPage()
{
    otpForm->hide();
    welcomeForm->show();

    this->adjustSize();
    welcomeForm->adjustSize();
}
