#ifndef QEVENTBUTTON_H
#define QEVENTBUTTON_H

#include <QToolButton>

/* QEventButton, mouse enter/leave fonksiyonlariyla butonun ui'ini degistirmek icin var.
 */
class QEventButton : public QToolButton
{
    Q_OBJECT
public:
    explicit QEventButton(const QString &tooltip, QWidget *parent = nullptr);
    void setIcons(const QString &, const QString &);

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QString iconPath;
    QString hoverIconPath;
signals:

};

#endif // QEVENTBUTTON_H
