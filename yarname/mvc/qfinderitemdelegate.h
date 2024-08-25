#ifndef QFINDERITEMDELEGATE_H
#define QFINDERITEMDELEGATE_H

#include <QStyledItemDelegate>

class QFinderItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit QFinderItemDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

signals:

};

#endif // QFINDERITEMDELEGATE_H
