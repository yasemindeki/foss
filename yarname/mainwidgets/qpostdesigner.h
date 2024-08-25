#ifndef QPOSTDESIGNER_H
#define QPOSTDESIGNER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTabWidget;
class QAccordionWidget;
class QStyledToolBar;
QT_END_NAMESPACE


class QPostDesigner : public QWidget
{
    Q_OBJECT
public:
    explicit QPostDesigner(QWidget *parent = nullptr);

private:
    QAccordionWidget    *sideBar;
    QStyledToolBar      *miniSideBar;
    QTabWidget          *designerTabs;

    void createSideBar();
    void createMiniSideBar();
    void createTabs();

signals:

public slots:
    void onExpandActionTriggered();
    void onShrinkButtonClicked();

};

#endif // QPOSTDESIGNER_H
