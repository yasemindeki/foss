#ifndef QCATEGORYBUTTON_H
#define QCATEGORYBUTTON_H

#include <QToolButton>

class QCategoryButton : public QToolButton
{
    Q_OBJECT
public:
    explicit QCategoryButton(const QString &text, Qt::GlobalColor color, QWidget *parent = nullptr);

protected:
    bool event(QEvent *e) override;
    //void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void paintEvent(QPaintEvent *) override;

private:
    bool            isHoverIcon;
    QString         categoryText;
    Qt::GlobalColor categoryColor;
    QString         removeIconPath;

signals:

};

#endif // QCATEGORYBUTTON_H
