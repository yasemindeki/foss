#ifndef QINBOXMESSAGEPANE_H
#define QINBOXMESSAGEPANE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QInboxListItem;
class QInboxMessageHeader;
class QInboxMessageBody;
QT_END_NAMESPACE

class QInboxMessagePane : public QWidget
{
    Q_OBJECT
public:
    explicit QInboxMessagePane(QWidget *parent = nullptr);

    void show();
    void show(QInboxListItem *);

private:
    QInboxMessageHeader *messageHeader;
    QInboxMessageBody   *messageBody;

signals:

};

#endif // QINBOXMESSAGEPANE_H
