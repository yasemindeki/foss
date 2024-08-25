#ifndef QINBOXFILTERDELEGATE_H
#define QINBOXFILTERDELEGATE_H

#include <QStyledItemDelegate>

QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
class QInboxFilter;
QT_END_NAMESPACE

class QInboxFilterDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit QInboxFilterDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;

private:
    QInboxFilter *inboxFilter;

    int drawLiveData(QPainter *painter, QRect *itemRect, int rightOffset, int data) const;
    void drawInboxTitle(QPainter *painter, QRect *titleRect, bool hasSelection, const QString &accountName) const;

signals:
    void toggleChildrens(const QModelIndex &index);

public slots:
    void onCurrentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *);
    void onItemSelectionChanged();
};

#endif // QINBOXFILTERDELEGATE_H
