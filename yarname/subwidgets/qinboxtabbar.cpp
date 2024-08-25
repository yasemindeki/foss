#include "qinboxtabbar.h"
#include "subwidgets/qiconbutton.h"

#include <QDebug>
#include <QStylePainter>
#include <QStyleOptionTab>
#include <QVector>

QInboxTabBar::QInboxTabBar(QWidget *parent) : QTabBar(parent)
{
    QFont tabFont;
    tabFont.setFamily("Goldman");
    tabFont.setPointSize(8);
    this->setFont(tabFont);

    //this->setTabsClosable(true);
    //QTabBar::close-button { subcontrol-position: left; image: url(:/icons/closetab-selected-hover.png); }

    this->setStyleSheet("QTabWidget::pane { border: none; border-bottom: 10px solid white; } \
                         QTabWidget { background-color: rgb(91, 161, 123); } \
                        QTabBar::tab { \
                            border-bottom: 2px solid rgb(91, 161, 123); \
                            padding: 7px; \
                            background-color: rgb(91, 161, 123); \
                            color: white; \
                        } \
                        QTabBar::tab:hover { \
                            background-color: rgb(101, 171, 132); \
                            border-bottom: 2px solid rgb(91, 161, 123); \
                        } \
                        QTabBar::tab:selected { \
                            background-color: white; \
                            color: black; \
                            border-bottom: 2px solid rgb(36, 41, 46); \
                        }");
    // guzel kirmizi
    // 241, 70, 89

    leftSideBarButton = new QIconButton("Hide left Sidebar", this);
    leftSideBarButton->setFixedSize(16, 16);
    leftSideBarButton->setIcons(":/icons/leftsidebar-toggle.png", ":/icons/leftsidebar-toggle.png");

    trashButton = new QIconButton("Closed Tabs", this);
    trashButton->setFixedSize(16, 16);
    trashButton->setIcons(":/icons/closedtabs.png", ":/icons/closedtabs.png");

    setAbsolutePositions();

    //connect(leftSideBarButton, SIGNAL(clicked()), this, SLOT(onLeftSideBarClicked()));

    this->setExpanding(false);
}

QSize QInboxTabBar::sizeHint() const
{
    QSize barSize = QTabBar::sizeHint();

    return QSize(barSize.width() + 100 + 48, barSize.height());
}

QSize QInboxTabBar::tabSizeHint(int index) const
{
    QSize s = QTabBar::tabSizeHint(index);
    //qDebug() << "QInboxTabBar::tabSizeHint " << index << " " << s.width();

    return s;
}

void QInboxTabBar::setAbsolutePositions()
{
/*
    int tabsWidth = 0;
    for (int i = 0; i < this->count(); ++i)
        tabsWidth += this->tabRect(i).width();

    int top = this->geometry().top();
    int width = this->width();
*/

    int y = (this->geometry().height() - 16) / 2;


    //qDebug() << "QInboxtabbar height is " << this->height();
    //qDebug() << "QInboxtabbar top is " << top;


    leftSideBarButton->move(10, y);
    trashButton->move(10 + 16 + 10, y);
}

void QInboxTabBar::resizeEvent(QResizeEvent *event)
{
    QTabBar::resizeEvent(event);

    setAbsolutePositions();
}

void QInboxTabBar::paintEvent(QPaintEvent *)
{
    QStylePainter painter(this);

    int offset = 100;

    //int i = 0;
    for(int i = 0; i < this->count(); i++)
    {
        QStyleOptionTab opt;

        // QTabBar temel siniftan gelen tab gorunumunu kaybetmemek icin bu fonksiyonu cagriyoruz.
        this->initStyleOption(&opt,i);

        QSize tabSizeHint = this->tabSizeHint(i);
        QRect tabRect = opt.rect;
        QSize newSize(tabSizeHint.width() + 24, tabSizeHint.height());

        QRect moveLeftRect(QPoint(offset, tabRect.y()), newSize);
        opt.rect = moveLeftRect;

        //qDebug() << i << " tab x is " << opt.rect.x();
        //qDebug() << i << " tab width is " << tabSizeHint.width();

        // Tabi etiketi ve sekliyle cizimini sagliyoruz.
        //painter.drawControl(QStyle::CE_TabBarTab, opt);

        // Tab dikdortgeninin cizimini sagliyoruz.
        painter.drawControl(QStyle::CE_TabBarTabShape, opt);

        // Sol ikonun x konumunu set ediyoruz.
        opt.rect.setX(offset + 8);

        QIcon *icon = new QIcon(this->tabData(i).toString());
        icon->paint((QPainter *)&painter, opt.rect, Qt::AlignLeft|Qt::AlignVCenter);

        // Etiketin yeni konumunu set ediyoruz.
        opt.rect.setX(offset + 8 + 16);
        opt.rect.setY(tabRect.y() + 6);

        // Tab etiketinin cizimini sagliyoruz.
        painter.drawControl(QStyle::CE_TabBarTabLabel, opt);

        offset += tabSizeHint.width() + 24;
    }
}

