#ifndef QACCORDIONITEM_H
#define QACCORDIONITEM_H

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineFrame;
QT_END_NAMESPACE

class QAccordionItem : public QWidget {
    Q_OBJECT
private:
    QToolButton *toolButton;
    QScrollArea *contentArea;

    QString itemName;
    QStringList icons;

    QParallelAnimationGroup heightAnimations;
    int animationDuration{300};
    int contentHeight;

    bool isItemOpen;

    const int HEIGHT_COLLAPSED_STATE = 28;
    const QString TOOLBUTTON_STYLE = "QToolButton { \
            border: none; \
            margin-left: 10px; \
            text-align: left center; \
            font: 8pt 'Goldman'; \
            color: %1; \
          } \
          QToolButton:hover { \
              color: rgb(56, 113, 193); \
          }";

public:
    explicit QAccordionItem(const QString & title = "", bool open = false, QWidget *parent = 0);
    void setContentLayout(QLayout *contentLayout);
    void setIcons(QString activeIconPath, QString passiveIconPath);

    bool eventFilter(QObject *watched, QEvent *event) override;

    QString name();

public slots:
    void setOpened(bool checked, bool animation);
};

#endif // QACCORDIONITEM_H
