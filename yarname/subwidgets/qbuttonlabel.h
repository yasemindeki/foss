#ifndef QBUTTONLABEL_H
#define QBUTTONLABEL_H

#include <QObject>
#include <QLabel>

class QButtonLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QButtonLabel(const QString &text, QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);

};

#endif // QBUTTONLABEL_H
