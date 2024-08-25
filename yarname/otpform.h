#ifndef OTPFORM_H
#define OTPFORM_H

#include "loginform.h"

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QLabel;
class QLineInput;
class QPushButton;
class QButtonLabel;
QT_END_NAMESPACE

class OtpForm : public LoginForm
{
    Q_OBJECT
public:
    explicit OtpForm(const QString &header, QWidget *parent = nullptr);

    void createWidgets() override;
    void show(const QString &input);

private:
    QLineInput      *emailAddress;
    QLineInput      *oneTimePassword;
    QLabel          *infoLabel;
    QButtonLabel    *goBackLink;

protected:

public slots:
    void onSubmitPressed();
    void setEnabled(bool);  // override

};

#endif // LOGINFORM_H
