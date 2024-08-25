#ifndef QINBOXMESSAGEBODY_H
#define QINBOXMESSAGEBODY_H

#include <QFrame>

QT_BEGIN_NAMESPACE
class QLabel;
class QFrame;
class QTextBrowser;
QT_END_NAMESPACE

class QInboxMessageBody : public QFrame
{
    Q_OBJECT

    friend class QInboxMessagePane;

public:
    explicit QInboxMessageBody(QWidget *parent = nullptr);

private:
    QFrame          *topBar;
    QLabel          *fromLabel;
    QLabel          *timeLabel;
    QLabel          *profileImage;
    QTextBrowser    *messageView;

    bool        fullView;

    void createTopBar();

protected:
    void paintEvent(QPaintEvent *) override;

signals:

};

#endif // QINBOXMESSAGEBODY_H
