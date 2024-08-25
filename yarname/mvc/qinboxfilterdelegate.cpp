#include "qinboxfilterdelegate.h"
#include "base/channeltypes.h"
#include "subwidgets/qinboxfilter.h"

#include <QStylePainter>
#include <QDebug>
#include <QMouseEvent>
#include <QTreeWidgetItem>

QInboxFilterDelegate::QInboxFilterDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    inboxFilter = static_cast<QInboxFilter *>(parent);
}

void QInboxFilterDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);

    // painter      POINTER
    // option       READONLY
    // modelIndex   READONLY
    if (index.column() == 0)
    {
        QString iconFilePath;
        QRect itemRect = option.rect;
        itemRect.setLeft(itemRect.left() + 8);

        if (!index.parent().isValid())
        {
            bool isSelected = (option.showDecorationSelected && (option.state & QStyle::State_Selected));

            //qDebug() << "QInboxFilterDelegate: x = " << option.rect.x();
            //qDebug() << "QInboxFilterDelegate: y = " << option.rect.y();
            //qDebug() << "QInboxFilterDelegate: size = " << option.rect.size();

            // Kanalin logosunu ciziyoruz.
            int channelType = index.data(Qt::UserRole).toInt();
            switch (channelType)
            {
            case CHANNEL_MAIL:
                iconFilePath = (isSelected ? ":/icons/channel-email.png" : ":/icons/channel-email-black.png");
                break;
            case CHANNEL_TWITTER:
                iconFilePath = (isSelected ? ":/icons/twitter-white.png" : ":/icons/twitter-black.png");
                break;
            default:
                break;
            }

            QIcon icon(iconFilePath);
            icon.paint(painter, itemRect, Qt::AlignLeft|Qt::AlignVCenter);

            // Sol baslangici, ikonun genisligi (16px) ve 4 pixel kadar ileri tasiyoruz.
            itemRect.setLeft(itemRect.left() + 16 + 4);
            // Canli veriyi yazdiktan sonra  bu QRect'i kullanmak icin sakliyoruz.
            QRect titleRect = itemRect;

            bool hasChildren = index.model()->hasChildren(index);
            int liveDataRightOffset = 0;

            // En saga expand/collapse ikonu yerlestirelim
            if (hasChildren && (option.state & QStyle::State_MouseOver))
            {
                bool isExpanded = inboxFilter->isExpanded(index);
                if (isSelected)
                {
                    if (isExpanded)
                        iconFilePath = ":/icons/down-arrow-white.png";
                    else
                        iconFilePath = ":/icons/right-arrow-white.png";
                }
                else
                {
                    if (isExpanded)
                        iconFilePath = ":/icons/down-arrow.png";
                    else
                        iconFilePath = ":/icons/right-arrow.png";
                }

                QIcon buttonIcon(iconFilePath);

                itemRect.setRight(itemRect.right() - 7);
                buttonIcon.paint(painter, itemRect, Qt::AlignRight|Qt::AlignVCenter);

                liveDataRightOffset = 16;
            } // has children on mouse over

            int liveData = index.data(Qt::UserRole + 2).toInt();
            if (liveData > 0)
            {
                int liveDataRectLeft = this->drawLiveData(painter, &itemRect, liveDataRightOffset, liveData);
                titleRect.setRight(liveDataRectLeft);
            }
            else
            {
                titleRect.setRight(titleRect.right() - liveDataRightOffset);
            }

            QString channelAccountName = index.data(Qt::UserRole + 1).toString();
            this->drawInboxTitle(painter, &titleRect, isSelected, channelAccountName);

        } // top level node
    } // first column
}

void QInboxFilterDelegate::drawInboxTitle(QPainter *painter, QRect *titleRect, bool hasSelection, const QString &accountName) const
{
    painter->save();

    // Kalem rengini atiyoruz.
    if (hasSelection)
        painter->setPen(QColor("white"));
    else
        painter->setPen(QColor("black"));

    QFont inboxFont("Goldman", 9);
    painter->setFont(inboxFont);

    titleRect->setTop(titleRect->y() + 4);
    painter->drawText(*titleRect, Qt::AlignLeft|Qt::TextSingleLine, tr("Inbox"));

    QFont accountFont("Ubuntu", 7);
    painter->setFont(accountFont);

    titleRect->setTop(titleRect->y() + 17);
    QFontMetrics fm(accountFont);
    QString shortenText = fm.elidedText(accountName, Qt::ElideRight, titleRect->width());
    painter->drawText(*titleRect, Qt::AlignLeft|Qt::TextSingleLine, shortenText);

    painter->restore();
}

int QInboxFilterDelegate::drawLiveData(QPainter *painter, QRect *itemRect, int rightOffset, int data) const
{
    QString liveDataText = QString::number(data);

    if (rightOffset == 0)
        rightOffset = 14;
    else
        rightOffset += 4;
    itemRect->setRight(itemRect->right() - rightOffset);

    QFont liveDataFont("Goldman", 7);
    QFontMetrics fm(liveDataFont);
    int width = fm.horizontalAdvance(liveDataText);

    itemRect->setLeft(itemRect->right() - width - 8);
    itemRect->setTop(itemRect->y() + ((itemRect->height() - 14) / 2));
    itemRect->setHeight(14);

    painter->save();

    painter->setFont(liveDataFont);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QColor(249, 250, 251)); // 91, 161, 123
    painter->setBrush(QColor(249, 250, 251)); // 101, 171, 132
    painter->drawRoundRect(*itemRect, 75, 75);

    painter->setPen(QColor(36, 41, 46));
    painter->drawText(*itemRect, Qt::AlignCenter|Qt::TextSingleLine, liveDataText);

    painter->restore();

    return itemRect->left();
}

bool QInboxFilterDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                       const QStyleOptionViewItem &option, const QModelIndex &index)
{
    bool hasChildren = model->hasChildren(index);
    if (hasChildren && event->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        int verticalAlignTop = option.rect.y() + ((option.rect.height() - 16) / 2);
        QRect buttonRect = QRect(QPoint(option.rect.right() - 7 - 16, verticalAlignTop), QSize(16, 16));
        qDebug() << "QInboxFilterDelegate : mouse pos = " << (buttonRect.contains(mouseEvent->pos()));

        if (buttonRect.contains(mouseEvent->pos()))
        {
            emit toggleChildrens(index);
        }

        //qDebug() << "QInboxFilterDelegate : mouse pos = " << mouseEvent->pos();
        //qDebug() << "QInboxFilterDelegate : option rect x y = " << option.rect.x() << " " << option.rect.y();
        //qDebug() << "QInboxFilterDelegate : option rect size = " << option.rect.size();

    }
    return false;
}

void QInboxFilterDelegate::onCurrentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)
{
}

void QInboxFilterDelegate::onItemSelectionChanged()
{
}
