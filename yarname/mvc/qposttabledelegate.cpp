#include "qposttabledelegate.h"
#include "mainwidgets/qpostbrowser.h"

#include <QtWidgets>

QPostTableDelegate::QPostTableDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void QPostTableDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);

    QColor color = QColor(0, 0, 0);

    if (index.column() == 0)
    {
        color = QColor(56, 113, 193);
    }

    option->palette.setColor(QPalette::HighlightedText, color);
    option->palette.setBrush(QPalette::Text, color);
}

void QPostTableDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 1)
    {
        // Item secili ise arkaplan rengini atiyoruz.
        if (option.showDecorationSelected && (option.state & QStyle::State_Selected))
        {
            QBrush brush(QColor(234, 245, 251));
            painter->fillRect(option.rect, brush);
        }

        // Gonderinin yayinlanacagi kanallarin ikonlarini gosteriyoruz.
        int spacing = 0;
        foreach (const QString &iconPath, QPostBrowser::channelIconPaths())
        {
            int leftPosition = option.rect.left() + spacing;
            int topPosition  = option.rect.top() + 6;
            QRect rect = QRect(leftPosition, topPosition, 16, 16);

            QIcon icon(iconPath);
            icon.paint(painter, rect, Qt::AlignLeft|Qt::AlignVCenter);

            spacing += 20;
        }
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}
