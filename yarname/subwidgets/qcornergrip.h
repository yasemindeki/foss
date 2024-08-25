#ifndef QCORNERGRIP_H
#define QCORNERGRIP_H

#include <QSizeGrip>

class QCornerGrip : public QSizeGrip
{
    Q_OBJECT
public:
    explicit QCornerGrip(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;

signals:

};

#endif // QCORNERGRIP_H
