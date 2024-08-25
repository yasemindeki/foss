#ifndef QPOSTTABLEDELEGATE_H
#define QPOSTTABLEDELEGATE_H

#include <QStyledItemDelegate>

class QPostTableDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit QPostTableDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;


protected:
    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;


signals:

};

#endif // QPOSTTABLEDELEGATE_H
