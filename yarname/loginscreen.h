#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QWidget>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
class QFrame;
class QLabel;
class QHBoxLayout;
class WelcomeForm;
class OtpForm;
class QNotificationBox;
class QIconButton;
class QLineFrame;
QT_END_NAMESPACE


class LoginScreen : public QWidget
{
    Q_OBJECT

public:
    LoginScreen(QWidget *parent = nullptr);
    ~LoginScreen();

    void adjustSize(); // override

private:
    WelcomeForm *welcomeForm;
    OtpForm *otpForm;
    QHBoxLayout *formLayout;

    QNotificationBox *notifyBox;
    QIconButton *maximizeButton;
    QFrame *rightPane;

    QLineFrame *hline;
    int leftPaneWidth;

    void createWidgets();
    QFrame *createRightPane();
    QHBoxLayout *createTopBar();

protected:
    void resizeEvent(QResizeEvent *) override;

public slots:
    void goNextPage();
    void goPreviousPage();
    void showNotification(QNetworkReply::NetworkError);
    void showNotification(const QString &);

    void showMaximized();  // override

signals:
    void sizeUpdated();
};
#endif // LOGINSCREEN_H
