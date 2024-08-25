#ifndef QSIDEGRIP_H
#define QSIDEGRIP_H

#include <QSizeGrip>

class QSideGrip : public QSizeGrip
{
    Q_OBJECT
public:
    explicit QSideGrip(QWidget *parent = nullptr, Qt::Edge edgeType = Qt::RightEdge);

private:
    Qt::Edge edge;
    QPoint mousePosition;

    void resizeLeft(QPoint delta);
    void resizeRight(QPoint delta);
    void resizeTop(QPoint delta);
    void resizeBottom(QPoint delta);

protected:
    void mousePressEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *mouseEvent) override;
    void paintEvent(QPaintEvent *) override;

signals:

};

#endif // QSIDEGRIP_H
