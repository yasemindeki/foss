#ifndef QINBOXLISTVIEW_H
#define QINBOXLISTVIEW_H

#include <QScrollArea>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QInboxData;
class QInboxBrowser;
QT_END_NAMESPACE

class QInboxListView : public QScrollArea
{
    Q_OBJECT
public:
    explicit QInboxListView(QWidget *parent = nullptr);

    int append(QInboxData *);

protected:
    void resizeEvent(QResizeEvent *) override;

private:
    QWidget         *listWidget;
    QVBoxLayout     *mainLayout;
    QInboxBrowser   *owner;

signals:

};

#endif // QINBOXLISTVIEW_H
