#ifndef QSTYLEDTOOLBAR_H
#define QSTYLEDTOOLBAR_H

#include <QToolBar>

class QStyledToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit QStyledToolBar(QWidget *parent = nullptr);
    void addAction(QAction *action, const QString &name); // override

private:
    /* @setActionName
     * QToolBar üyesi QAction'a stylesheet uygulayabilmek icin gerekli isimlendirmeyi yapan fonksiyon.
     */
    void setActionName(QAction *action, const QString &name);

signals:

};

#endif // QSTYLEDTOOLBAR_H
