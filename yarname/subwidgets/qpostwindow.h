#ifndef QPOSTWINDOW_H
#define QPOSTWINDOW_H

#include <QWidget>

#include "mailer/basetypes.h"
#include "mailer/engines.h"

QT_BEGIN_NAMESPACE
class ToTextEdit;
class QIconButton;
class QPostEditor;
class MimeMessage;
class Checker;
class QLineEdit;
class QFinder;
QT_END_NAMESPACE

class QPostWindow : public QWidget
{
    Q_OBJECT
public:
    explicit QPostWindow(QWidget *parent = nullptr);

    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    void resizeEvent(QResizeEvent *) override;

private:
    ToTextEdit      *toInput;
    QLineEdit       *subjectInput;
    QIconButton     *sendButton;
    QPostEditor     *editor;
    QFinder         *finder;

    Mail::CURLEngine    *mailEngine;
    Checker             *checker;

    void composeMimeMessage(MimeMessage *);

signals:

public slots:
    void onSendButtonClicked();
    void sendMail(MimeMessage *);
    void showMessageSizeLimit();
};

#endif // QPOSTWINDOW_H
