#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include "loginscreen.h"

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class LoginForm : public QWidget
{
    Q_OBJECT
public:
    explicit LoginForm(const QString &header, QWidget *parent = nullptr);

    void adjustSize();  // override
    virtual void createWidgets() = 0;

protected:
    QVBoxLayout *mainLayout;
    QLabel *topLabel;
    QLabel *termsLabel;
    QPushButton *submitButton;

private:

signals:
    void sizeUpdated();

};

#endif // LOGINFORM_H
