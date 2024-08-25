#ifndef QPOSTBROWSER_H
#define QPOSTBROWSER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineInput;
class QTabWidget;
class QAccordionWidget;
class QStyledToolBar;
class QTableWidget;
QT_END_NAMESPACE

class QPostBrowser : public QWidget
{
    Q_OBJECT
public:
    explicit QPostBrowser(QWidget *parent = nullptr);
    static QStringList channelIconPaths();

private:
    QLineInput          *searchInput;
    QAccordionWidget    *sideBar;
    QStyledToolBar      *miniSideBar;
    QTabWidget          *browserTabs;
    QTableWidget        *postTable;

    void createPostTable();
    void createSideBar();
    void createMiniSideBar();
    void createTabs();
    void setAccordionContents();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void sizeUpdated();

public slots:
    void onExpandActionTriggered();
    void onShrinkButtonClicked();
    void onPlusActionTriggered();
    void onFilterActionTriggered();
    void onChannelActionTriggered();


};

#endif // QPOSTBROWSER_H
