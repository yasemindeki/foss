#ifndef QACCORDIONWIDGET_H
#define QACCORDIONWIDGET_H

#include <QWidget>
#include <QtCore/QList>
#include <QtCore/QMap>

QT_BEGIN_NAMESPACE
class QAccordionItem;
class QLabel;
QT_END_NAMESPACE

class QAccordionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QAccordionWidget(const QList<QStringList> &itemTitles, QWidget *parent = nullptr);
    void setItemLayout(const QString &title, QLayout *contentLayout);
    void expandItem(const QString &title);

    void setHeaderText(const QString &text);

private:
    QLabel *topBarLabel;

    QMap<QString, QAccordionItem *> items;
    void updateAccordions(QAccordionItem *itemExpanding, bool animation);

public slots:
    void onItemPressed();

signals:

};

#endif // QACCORDIONWIDGET_H
