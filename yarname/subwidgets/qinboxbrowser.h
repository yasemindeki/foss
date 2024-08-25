#ifndef QINBOXBROWSER_H
#define QINBOXBROWSER_H

#include <QScrollArea>

QT_BEGIN_NAMESPACE
class QInboxFilter;
class QInboxListView;
class QInboxData;
class QHBoxLayout;
class QVBoxLayout;
class QInboxMessagePane;
class QInboxListItem;
QT_END_NAMESPACE

typedef enum
{
    LAYOUT_NOSPLIT = 0,
    LAYOUT_VERTICAL = 1,
    LAYOUT_HORIZONTAL = 2
}QInboxDisplayFormat;

class QInboxBrowser : public QFrame
{
    Q_OBJECT
public:
    explicit QInboxBrowser(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    void showMessage(QInboxData *rxData, int status);

private:
    QHBoxLayout         *mainLayout;
    QHBoxLayout         *mpHorizontalLayout; // horizontal layout of the messagePane
    QVBoxLayout         *mpVerticalLayout;

    QInboxFilter        *filterPane;
    QInboxListView      *listView;

    QInboxDisplayFormat  displayFormat;
    QInboxMessagePane   *messagePane;

signals:

public slots:
    void showMessageDetails(QInboxListItem *);

};

#endif // QINBOXBROWSER_H
