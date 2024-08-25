#include "qinboxmessagebody.h"
#include "subwidgets/qiconbutton.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QTextBrowser>

QInboxMessageBody::QInboxMessageBody(QWidget *parent) : QFrame(parent)
{
    fullView = true;

    this->setObjectName("MessageBody");
    this->setStyleSheet("QFrame#MessageBody { \
                            border: 1px solid rgb(235, 235, 235); border-radius: 10px; \
                            background-color: white; \
                            margin-left: 10px; margin-right: 10px; \
                        } \
                        QFrame#MessageBody:hover { \
                            border: 3px double rgb(208, 214, 222); \
                        } \
                        QLabel { font: 8pt 'Segoe UI'; color: black; }");

    if (!fullView)
        this->setFixedHeight(80);
    else
        this->setMinimumHeight(120);

    createTopBar();

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(3, 0, 3, 10); // border 3px, border-radius 10 px
    mainLayout->setSpacing(0);

    messageView = new QTextBrowser();
    messageView->setStyleSheet("QTextBrowser { border: none; padding-left: 58px; padding-top: 20px; }");
    messageView->setHtml("<div> </div><div> </div><div>-------- Beginning of forwarded message --------</div><div>26.08.2023, 12:22, \"YILMAZ Yasemin\" &lt;yasemin-yilmaz@yandex.com&gt;:</div><div> </div><div><div><div><span style=\"background-color:#ffffff;color:#1a1a1a;float:none;font-family:'ys text' , 'arial' , sans-serif;font-size:16px;font-style:normal;font-weight:400;text-decoration-color:initial;text-decoration-style:initial;text-indent:0px;text-transform:none;white-space:normal;word-spacing:0px\">\"Çift Tırnak Kontolü\" &amp; and karakteri &lt; küçük &gt; büyük</span></div></div></div><div> </div><div>-------- End of forwarded message --------</div>");

    mainLayout->addWidget(topBar);
    //mainLayout->addWidget(finder);
    mainLayout->addWidget(messageView);
    mainLayout->addStretch();

    this->setLayout(mainLayout);
}

void QInboxMessageBody::createTopBar()
{
    topBar = new QFrame(this);
    topBar->setObjectName("MessageTopBar");
    topBar->setFixedHeight(80);
    topBar->setStyleSheet("QFrame#MessageTopBar { \
                                border-top: none; \
                                border-left: none; \
                                border-right: none; \
                                border-bottom: 1px solid rgb(235, 235, 235); \
                            }");

    QHBoxLayout *topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(10, 0, 10, 0);
    topBarLayout->setSpacing(10);

    QVBoxLayout *fromtoLayout = new QVBoxLayout();
    fromtoLayout->setContentsMargins(0, 0, 0, 0);
    fromtoLayout->setSpacing(5);

    fromLabel = new QLabel("153 Çözüm Merkezi");
    QLabel *toLabel = new QLabel("Me");

    fromLabel->setStyleSheet("QLabel { font: 10pt 'Segoe UI'; font-weight: bold; }");

    fromtoLayout->addStretch();
    fromtoLayout->addWidget(fromLabel);
    fromtoLayout->addWidget(toLabel);
    fromtoLayout->addStretch();

    profileImage = new QLabel("");
    profileImage->setAlignment(Qt::AlignCenter);
    profileImage->setFixedSize(36, 36);
    profileImage->setStyleSheet("QLabel { \
                                    font: 10pt 'Goldman'; \
                                    color: rgb(63, 132, 68); \
                                    background-color: rgb(196, 241, 201); \
                                    border: 1px solid rgb(196, 241, 201); \
                                    border-radius: 18px; \
                                }");

    timeLabel = new QLabel("Today at 11:30");
    timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    timeLabel->setStyleSheet("QLabel { color: rgb(173, 173, 173); font-weight: bold; }");

    QVBoxLayout *rightPaneLayout = new QVBoxLayout();
    rightPaneLayout->setContentsMargins(0, 0, 0, 0);
    rightPaneLayout->setSpacing(10);

    QHBoxLayout *indicatorLayout = new QHBoxLayout();
    indicatorLayout->setContentsMargins(0, 0, 0, 0);
    indicatorLayout->setSpacing(5);

    QIconButton *flagImportantButton = new QIconButton("Important", this);
    flagImportantButton->setFixedSize(10, 12);
    flagImportantButton->setIcons(":/icons/flag_important.png", ":/icons/flag_important.png");

    QIconButton *flagReadButton = new QIconButton("Unread", this);
    flagReadButton->setFixedSize(12, 12);
    flagReadButton->setIcons(":/icons/flag_unread.png", ":/icons/flag_unread.png");

    QIconButton *flagStarButton = new QIconButton("Starred", this);
    flagStarButton->setFixedSize(13, 12);
    flagStarButton->setIcons(":/icons/flag_star.png", ":/icons/flag_star.png");

    QIconButton *flagSubscriptionButton = new QIconButton("Subscribed", this);
    flagSubscriptionButton->setFixedSize(16, 12);
    flagSubscriptionButton->setIcons(":/icons/flag_subscribed.png", ":/icons/flag_subscribed.png");

    QIconButton *flagAttachmentButton = new QIconButton("Has attachments", this);
    flagAttachmentButton->setFixedSize(12, 12);
    flagAttachmentButton->setIcons(":/icons/flag_attachment.png", ":/icons/flag_attachment.png");

    QIconButton *flagAlternativeButton = new QIconButton("Has alternative messages", this);
    flagAlternativeButton->setFixedSize(16, 12);
    flagAlternativeButton->setIcons(":/icons/flag_alternative.png", ":/icons/flag_alternative.png");

    QIconButton *flagRelatedButton = new QIconButton("Has related messages", this);
    flagRelatedButton->setFixedSize(16, 12);
    flagRelatedButton->setIcons(":/icons/flag_related.png", ":/icons/flag_related.png");

    indicatorLayout->addStretch();
    indicatorLayout->addWidget(flagRelatedButton);
    indicatorLayout->addWidget(flagAlternativeButton);
    indicatorLayout->addWidget(flagAttachmentButton);
    indicatorLayout->addWidget(flagSubscriptionButton);
    indicatorLayout->addWidget(flagReadButton);
    indicatorLayout->addWidget(flagStarButton);
    indicatorLayout->addWidget(flagImportantButton);

    rightPaneLayout->addStretch();
    rightPaneLayout->addWidget(timeLabel);
    rightPaneLayout->addLayout(indicatorLayout);
    rightPaneLayout->addStretch();

    topBarLayout->addWidget(profileImage);
    topBarLayout->addLayout(fromtoLayout);
    topBarLayout->addStretch();
    topBarLayout->addLayout(rightPaneLayout);

    topBar->setLayout(topBarLayout);
}

void QInboxMessageBody::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    qDebug() << "timeLabel font is " << timeLabel->font().pointSize();
}
