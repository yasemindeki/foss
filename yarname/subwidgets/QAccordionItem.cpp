#include <QPropertyAnimation>
#include <QEvent>

#include "subwidgets/qaccordionitem.h"
#include "subwidgets/qlineframe.h"

QAccordionItem::QAccordionItem(const QString & title, bool open, QWidget *parent) : QWidget(parent) {
    isItemOpen = open;
    itemName = title;

    // Bu yeni QWidget turevi icin qss islemleri
    this->setAttribute(Qt::WA_StyledBackground);
    this->setStyleSheet("QWidget { border-bottom: 1px solid rgb(234, 238, 240); }");

    // QToolButton islemleri
    toolButton = new QToolButton();
    toolButton->setStyleSheet(QString(TOOLBUTTON_STYLE).arg((isItemOpen ? "rgb(56, 113, 193)" : "rgb(36, 41, 46)")));
    toolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolButton->setText("  " + title);
    toolButton->setCheckable(true);
    toolButton->setChecked(false);
    toolButton->setParent(parent);
    toolButton->installEventFilter(this);
    //toolButton.setUserData()

    // QScrollArea islemleri
    contentArea = new QScrollArea();
    contentArea->setStyleSheet("QScrollArea { \
                                    background-color: rgb(255, 255, 255); \
                                    border: none; \
                                    margin-right: 1px; \
                                    margin-bottom: 1px; \
                                }");
    contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Eger durumu kapali ise, yukseklik degerlerini set ederek akordiyon ogesine ilk gorunumunu veriyoruz.
    if (!isItemOpen)
    {
        contentArea->setMaximumHeight(0);
        contentArea->setMinimumHeight(0);
        this->setFixedHeight(HEIGHT_COLLAPSED_STATE);
    }

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(toolButton);
    mainLayout->addWidget(contentArea);

    this->setLayout(mainLayout);

    // Hem akordiyon ogesinin kendisinin hem de icerik kisminin genisleyip kuculebilmesi icin gerekli yukseklik animasyonlarini ekliyoruz.
    heightAnimations.addAnimation(new QPropertyAnimation(this, "minimumHeight"));
    heightAnimations.addAnimation(new QPropertyAnimation(this, "maximumHeight"));
    heightAnimations.addAnimation(new QPropertyAnimation(contentArea, "maximumHeight"));

    // Sinyal slot baglantilarinin kurulumu
    this->connect(toolButton, SIGNAL(pressed()), parent, SLOT(onItemPressed()));
}

void QAccordionItem::setIcons(QString activeIconPath, QString passiveIconPath)
{
    icons.append(activeIconPath);
    icons.append(passiveIconPath);

    toolButton->setIcon(QIcon((isItemOpen ? activeIconPath : passiveIconPath)));
}

void QAccordionItem::setOpened(bool checked, bool animation)
{
    toolButton->setIcon(QIcon(checked ? icons[0] : icons[1]));
    toolButton->setStyleSheet(QString(TOOLBUTTON_STYLE).arg((checked ? "rgb(56, 113, 193)" : "rgb(36, 41, 46)")));

    if (isItemOpen != checked)
    {
        if (animation)
        {
            heightAnimations.setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
            heightAnimations.start();
        }
        else
        {
            if (checked)
            {
                this->setFixedHeight(HEIGHT_COLLAPSED_STATE + contentHeight);
                contentArea->setFixedHeight(contentHeight);
            }
            else
            {
                this->setFixedHeight(HEIGHT_COLLAPSED_STATE);
                contentArea->setFixedHeight(0);
            }

        }
    }

    isItemOpen = checked;
}

bool QAccordionItem::eventFilter(QObject *watched, QEvent *event)
{
    QToolButton *button = qobject_cast<QToolButton *>(watched);

    if (!button)
        return false;

    // Hover sirasinda aktif gorunumu veriyoruz.
    if (event->type() == QEvent::Enter)
    {
        button->setIcon(QIcon(icons[0]));
        return true;
    }

    // Hover'dan cikista, acik olup olmamasina gore ikon set ediyoruz.
    if (event->type() == QEvent::Leave)
    {
        button->setIcon(QIcon(isItemOpen ? icons[0] : icons[1]));
        return true;
    }

    return false;
}

QString QAccordionItem::name()
{
    return itemName;
}

void QAccordionItem::setContentLayout(QLayout *contentLayout) {
    //delete contentArea->layout();

    // Icindekileri simdi - bir kez - atiyoruz.
    // Fonksiyon cagrisi birden fazla olsaydi, layout silme islemine gerek olurdu.
    contentArea->setLayout(contentLayout);

    // Akordiyonun genislemesi - Forward yonu - animasyonlarini ayarliyoruz.
    contentHeight = contentLayout->sizeHint().height();
    for (int i = 0; i < heightAnimations.animationCount() - 1; i++)
    {
        QPropertyAnimation * itemAnimation = static_cast<QPropertyAnimation *>(heightAnimations.animationAt(i));
        itemAnimation->setDuration(animationDuration);
        itemAnimation->setStartValue(HEIGHT_COLLAPSED_STATE);
        itemAnimation->setEndValue(HEIGHT_COLLAPSED_STATE + contentHeight);
    }

    QPropertyAnimation * contentAnimation = static_cast<QPropertyAnimation *>(heightAnimations.animationAt(heightAnimations.animationCount() - 1));
    contentAnimation->setDuration(animationDuration);
    contentAnimation->setStartValue(0);
    contentAnimation->setEndValue(contentHeight);

    // Eger akordiyon ogesinin durumu acik ise, icindekilerin gorunebilmesi icin yukseklik atamasi yapiyoruz.
    if (isItemOpen)
    {
        this->setFixedHeight(HEIGHT_COLLAPSED_STATE + contentHeight);
    }
}
