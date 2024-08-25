#ifndef QINBOX_H
#define QINBOX_H

#include <QWidget>
#include "mailer/engines.h"

QT_BEGIN_NAMESPACE
class QInboxToolBar;
class QInboxBrowser;
class QInboxTabPane;
QT_END_NAMESPACE

class QInbox : public QWidget
{
    Q_OBJECT
public:
    explicit QInbox(QWidget *parent = nullptr);
    ~QInbox();

    QInboxToolBar *getToolBar();

private:
    QInboxToolBar   *topBar;
    QInboxBrowser   *browser;
    QInboxTabPane   *tabs;

    Mail::CURLEngine *mailEngine;

signals:

public slots:
    void receiveMessages(bool);

};

#endif // QINBOX_H
