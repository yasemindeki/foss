#ifndef QINBOXTABPANE_H
#define QINBOXTABPANE_H

#include <QTabWidget>

QT_BEGIN_NAMESPACE
class QInboxTabBar;
QT_END_NAMESPACE

class QInboxTabPane : public QTabWidget
{
    Q_OBJECT
public:
    explicit QInboxTabPane(QWidget *parent = nullptr);
    int addTab(QWidget *widget, const QString &); // override
    //int addTab(QWidget *widget, const QIcon& icon, const QString &label);

private:
    QInboxTabBar *inboxTabBar;

protected:
    void tabInserted(int index) override;

signals:

};

#endif // QINBOXTABPANE_H
