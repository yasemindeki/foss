#ifndef QINBOXTOOLBAR_H
#define QINBOXTOOLBAR_H

#include <QToolBar>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

class QInboxToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit QInboxToolBar(QWidget *parent = nullptr);

protected:
    void showEvent(QShowEvent *event) override;

private:
    QAction *actionDownload;
    //QPushButton *getButton;

    bool importCompleted;

public slots:
    void onDownloadClicked();
    void changeDownloadIcon(bool);

signals:
    void startDownloadProcess(bool);
};

#endif // QINBOXTOOLBAR_H
