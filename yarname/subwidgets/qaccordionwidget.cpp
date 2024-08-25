#include "qaccordionwidget.h"
#include "qaccordionitem.h"
#include "subwidgets/qiconbutton.h"

#include <QtWidgets>
#include <QtCore/QMap>

QAccordionWidget::QAccordionWidget(const QList<QStringList> &itemTitles, QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground);
    this->setContentsMargins(0, 0, 0, 0);
    this->setFixedWidth(200);

    this->setStyleSheet("QWidget { \
                                background-color: rgb(255, 255, 255); \
                                font: 8pt 'Goldman'; \
                                color: rgb(37, 51, 74); \
                                border-right: 1px solid rgb(234, 238, 240); \
                            }");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignTop);

    // Akordiyonlarin ust kismini olusturma
    QFrame *topBar = new QFrame();
    topBar->setAttribute(Qt::WA_StyledBackground);
    topBar->setStyleSheet("QFrame { \
                                border-bottom: 1px solid rgb(234, 238, 240); \
                            }");
    topBar->setFixedHeight(50);

    // Ust kismin basligi
    QLabel *topBarHeader = new QLabel("Posts", this);
    topBarHeader->setAlignment(Qt::AlignCenter);
    topBarHeader->setStyleSheet("QLabel { \
                                        font: 12pt 'Goldman'; \
                                        border: none; \
                                   }");

    // Ust kismin dinamik icerikli metni
    topBarLabel = new QLabel("2 post listed", this);
    topBarLabel->setAlignment(Qt::AlignCenter);
    topBarLabel->setStyleSheet("QLabel { \
                                        color: rgb(114, 107, 98); \
                                        border: none; \
                                   }");

    // Ust kismin butonu
    QIconButton *shrinkButton = new QIconButton("Paneli Gizle");
    shrinkButton->setFixedSize(16, 16);
    shrinkButton->setIcons(":/icons/shrink-gray.png", ":/icons/shrink-hover.png");

    // Ust kismin ikinci butonu
    QIconButton *prevPageButton = new QIconButton("Önceki sayfa");
    prevPageButton->setFixedSize(16, 16);
    prevPageButton->setIcons(":/icons/previous.png", ":/icons/previous-hover.png");

    // Ust kismin layout islemleri
    QHBoxLayout *topBarlayout = new QHBoxLayout();
    topBarlayout->setSpacing(0);
    topBarlayout->setContentsMargins(10, 0, 10, 1);

    QVBoxLayout *labelsLayout = new QVBoxLayout();
    labelsLayout->setSpacing(0);
    labelsLayout->setContentsMargins(0, 10, 0, 10);
    labelsLayout->addWidget(topBarHeader);
    labelsLayout->addWidget(topBarLabel);

    topBarlayout->addWidget(prevPageButton);
    topBarlayout->addLayout(labelsLayout);
    topBarlayout->addWidget(shrinkButton);

    topBar->setLayout(topBarlayout);

    // Ana layout islemleri
    layout->addWidget(topBar);

    // Akordiyon birimlerini ekleme
    QListIterator<QStringList> li(itemTitles);
    while (li.hasNext())
    {
        QStringList stringList = li.next();
        QString key = stringList[0];

        QAccordionItem *accordionItem = new QAccordionItem(key, false, this);
        accordionItem->setIcons(stringList[1], stringList[2]);

        items.insert(key, accordionItem);

        layout->addWidget(accordionItem);
    }

    this->setLayout(layout);

    this->connect(shrinkButton, SIGNAL(pressed()), parent, SLOT(onShrinkButtonClicked()));
}

void QAccordionWidget::setItemLayout(const QString &title, QLayout *contentLayout)
{
    items[title]->setContentLayout(contentLayout);
}

void QAccordionWidget::setHeaderText(const QString &text)
{
    topBarLabel->setText(text);
}

void QAccordionWidget::onItemPressed()
{
    QToolButton *buttonPressed = qobject_cast<QToolButton *>(sender());
    QAccordionItem *itemPressed = qobject_cast<QAccordionItem *>(buttonPressed->parentWidget());

    updateAccordions(itemPressed, false);
}

void QAccordionWidget::expandItem(const QString &title)
{
    QAccordionItem *itemExpanding = items[title];
    updateAccordions(itemExpanding, false);
}

void QAccordionWidget::updateAccordions(QAccordionItem *itemExpanding, bool animation)
{
    QMapIterator<QString, QAccordionItem *> mi(items);
    while (mi.hasNext())
    {
        mi.next();
        QAccordionItem *nextItem = mi.value();
        /* Eger siradaki akordiyonun ismi, basili olaninki ise
         *      siradaki akordiyon acik ise oldugu gibi kalmali
         *      siradaki akordiyon kapali ise acmali
         * Eger siradaki akordiyonun ismi, basili olaninki degil ise
         *      siradaki akordiyonun durumu kapali olmali
         */
        if (mi.key() == itemExpanding->name())
        {
            nextItem->setOpened(true, animation);
        }
        else
        {
            nextItem->setOpened(false, animation);
        }

    }
}
