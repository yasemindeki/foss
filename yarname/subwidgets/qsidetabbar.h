#ifndef QSIDETABBAR_H
#define QSIDETABBAR_H

#include <QTabBar>
#include <QStyleOptionTab>
#include <QStylePainter>

class QSideTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit QSideTabBar(QWidget *parent = nullptr);

protected:
    QSize tabSizeHint(int index) const override;
    void paintEvent(QPaintEvent *) override;

signals:

};

#endif // QSIDETABBAR_H
