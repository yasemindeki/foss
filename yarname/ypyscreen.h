#ifndef YPYSCREEN_H
#define YPYSCREEN_H

#include <QWidget>
#include "mainwidget.h"

QT_BEGIN_NAMESPACE
class LoginScreen;
class QCornerGrip;
class QSideGrip;
class QTopBarButton;
class QPostBrowser;
class QStackedWidget;
class QInbox;
QT_END_NAMESPACE

namespace YPY {
    const int GRIP_SIZE = 2;
};

class YPYScreen : public QWidget
{
    Q_OBJECT
public:
    explicit YPYScreen(QWidget *parent = nullptr);
    void setLoginWidget(LoginScreen *);

private:
    LoginScreen *loginWidget;
    QList<QCornerGrip *> cornerGrips;
    QList<QSideGrip *> sideGrips;

    QPoint dragPosition;

    YPY::MainWidget *mainWidget;
    QTopBarButton *workplaceButton;
    QTopBarButton *mailboxButton;

    QStackedWidget  *postDesignerStack;
    QPostBrowser    *postBrowser;
    QInbox          *inbox;

    void createWidgets();
    void createTopBarButtons();

protected:
    void resizeEvent(QResizeEvent *) override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public slots:
    void showOnStartup();
    void onNewPostClicked();

signals:

};

#endif // YPYSCREEN_H
