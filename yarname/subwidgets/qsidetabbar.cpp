#include "qsidetabbar.h"

//#include <QDebug>

QSideTabBar::QSideTabBar(QWidget *parent) : QTabBar(parent)
{
    this->setStyleSheet("QTabBar { \
                            background-color: rgb(242, 244, 252); \
                            border: none; \
                            font: 8pt 'Goldman'; \
                            color: rgb(36, 41, 46); \
                        } \
                        QTabBar::tab { \
                            border: none; \
                            padding: 7px; \
                            margin-top: 0px; \
                            margin-bottom: 8px; \
                        } \
                        QTabBar::tab:hover { \
                            background-color: rgb(246, 248, 250); \
                            border-left: 2px solid rgb(56, 113, 193); \
                        } \
                        QTabBar::tab:selected { \
                            background-color: rgb(246, 248, 250); \
                            border-left: 2px solid rgb(56, 113, 193); \
                            color: rgb(56, 113, 193); \
                        }");

}

QSize QSideTabBar::tabSizeHint(int index) const
{
    QSize s = QTabBar::tabSizeHint(index);
    s.transpose();
    return s;

}

void QSideTabBar::paintEvent(QPaintEvent * /*event*/)
{
    QStylePainter painter(this);
    QStyleOptionTab opt;

    for(int i = 0; i < this->count(); i++)
    {
        // QTabBar temel siniftan gelen tab gorunumunu kaybetmemek icin bu fonksiyonu cagriyoruz.
        this->initStyleOption(&opt,i);

        // Tab dikdortgeninin cizimini sagliyoruz.
        painter.drawControl(QStyle::CE_TabBarTabShape, opt);
        painter.save();

        // Tab metnini cevreleyen dikdortgenin
        // genislik yuksekligini tersine ceviriyoruz (transpose)
        QSize s = opt.rect.size();
        s.transpose();
        QRect r(QPoint(), s);
        r.moveCenter(opt.rect.center());
        opt.rect = r;

        QPoint c = tabRect(i).center();
        painter.translate(c);   // Boyaciyi o konuma (x,y) ilerletiyoruz
        painter.rotate(270);    // 270 derece dondurup yatay hale geliyor
        painter.translate(-c);  // Boyaciyi eski konumuna (-x,-y) geri aliyoruz ki tab metni oraya cizilsin
        painter.drawControl(QStyle::CE_TabBarTabLabel,opt);
        painter.restore();

        //qDebug() << "tab height is " << tabRect(i).height();

    }
}
