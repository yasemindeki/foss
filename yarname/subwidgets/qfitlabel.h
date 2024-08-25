#ifndef QFITLABEL_H
#define QFITLABEL_H

#include <QLabel>

class QFitLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QFitLabel(QWidget *parent = nullptr, Qt::WindowFlags f=Qt::WindowFlags());
    explicit QFitLabel(const QString &text, QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags());

protected:
    void paintEvent(QPaintEvent *) override;

signals:

};

#endif // QFITLABEL_H
