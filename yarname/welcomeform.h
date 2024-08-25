#ifndef WELCOMEFORM_H
#define WELCOMEFORM_H

#include "loginform.h"

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QLabel;
class QPushButton;
class QLineInput;
class QTextButton;
class QLineFrame;
QT_END_NAMESPACE

class WelcomeForm : public LoginForm
{
    Q_OBJECT
public:
    explicit WelcomeForm(const QString &header, QWidget *parent = nullptr);
    void createWidgets() override;

    QString getCustomerId();

private:
    QLineInput  *emailAddress;
    QTextButton *googleSignButton;
    QTextButton *otherSignButton;
    QLineFrame  *orLine;

protected:

public slots:
    void onSubmitPressed();
    void setEnabled(bool);  // override

};

#endif // WELCOMEFORM_H
