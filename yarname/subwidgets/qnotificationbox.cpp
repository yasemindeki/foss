#include "qnotificationbox.h"
#include "subwidgets/qiconbutton.h"

#include <QtWidgets>

QNotificationBox::QNotificationBox(const QString &text, QWidget *parent) : QFrame(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    layout->setContentsMargins(10, 5, 0, 5);
    layout->setSpacing(20);

    QLabel *infoIcon = new QLabel();
    infoIcon->setPixmap(QPixmap(":/icons/info-red.png"));

    QLabel *notifyText = new QLabel(text);

    closeButton = new QIconButton("Bildirimi kapat");
    closeButton->setIcons(":/icons/close-red.png", ":/icons/close-red-hover.png");

    layout->addWidget(infoIcon, 1);
    layout->addWidget(notifyText, 9);
    layout->addStretch();
    layout->addWidget(closeButton);

    this->setFont(QFont("Goldman", 8));
    this->setLayout(layout);

    this->setStyleSheet("QFrame { \
                                    border-radius: 3px; \
                                    border: none; \
                                    background-color: rgb(231, 29, 52); \
                                    color: white; \
                                }");
/*
    QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(1); //Adjust accordingly
    effect->setOffset(3,3); //Adjust accordingly
*/
/*
    QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect;
    effect->setColor(QColor(122, 193, 66));
    effect->setStrength(10);
    effect->setEnabled(true);

    this->setGraphicsEffect(effect);
*/

    this->connect(closeButton, SIGNAL(pressed()), this, SLOT(removeNotification()));
}

QNotificationBox::~QNotificationBox()
{
    qDebug() << "Deleted this notification";
}

void QNotificationBox::removeNotification()
{
    this->hide();
    this->deleteLater();
}
