#include "qfinderitemdelegate.h"

#include <QStylePainter>
#include <QApplication>

QFinderItemDelegate::QFinderItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void QFinderItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    const QWidget *widget = option.widget;
    QStyle *style = nullptr;
    if (widget) // QListWidget geliyor
        style = widget->style();
    else
        QApplication::style();

    //style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, NULL);

    QStyleOptionButton *opt = new QStyleOptionButton;

    //opt->initFrom(widget);
    //initStyleOption()

    //painter->draw

    opt->rect = QRect(option.rect.x(), option.rect.y(), 16, 16);
    opt->state |= QStyle::State_Enabled;
    opt->text = "";
    opt->iconSize = QSize(16, 16);
    opt->icon = QIcon(":/icons/remove.png");

    style->drawControl(QStyle::CE_PushButton, opt, painter);

    painter->restore();

    //QStyledItemDelegate::paint(painter, option, index);

}
