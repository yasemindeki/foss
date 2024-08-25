#ifndef QINBOXTABBAR_H
#define QINBOXTABBAR_H

#include <QTabBar>

QT_BEGIN_NAMESPACE
class QIconButton;
QT_END_NAMESPACE

class QInboxTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit QInboxTabBar(QWidget *parent = nullptr);

    QSize sizeHint() const override;

protected:
    void resizeEvent(QResizeEvent *) override;
    void paintEvent(QPaintEvent *) override;
    QSize tabSizeHint(int index) const override;

private:
    QIconButton *leftSideBarButton;
    QIconButton *trashButton;

    void setAbsolutePositions();

signals:

};

#endif // QINBOXTABBAR_H
