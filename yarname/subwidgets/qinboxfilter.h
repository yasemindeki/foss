#ifndef QINBOXFILTER_H
#define QINBOXFILTER_H

#include <QTreeWidget>

QT_BEGIN_NAMESPACE
class QInboxFilterDelegate;
QT_END_NAMESPACE

class QInboxFilter : public QTreeWidget
{
    Q_OBJECT

public:
    explicit QInboxFilter(QWidget *parent = nullptr);
    void updateData(int messageCount);

private:
    QInboxFilterDelegate *itemDelegate;
    QMap<QString, QTreeWidgetItem *> itemMap;

signals:
    void channelSelected(bool);

public slots:
    void onItemToggled(const QModelIndex &index);
    void onItemSelectionChanged();

    void onItemExpanded(QTreeWidgetItem *);
    void onItemCollapsed(QTreeWidgetItem *);

};

#endif // QINBOXFILTER_H
