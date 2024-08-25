#ifndef QNOTIFICATIONBOX_H
#define QNOTIFICATIONBOX_H

#include <QFrame>

QT_BEGIN_NAMESPACE
class QIconButton;
QT_END_NAMESPACE

class QNotificationBox : public QFrame
{
    Q_OBJECT
public:
    explicit QNotificationBox(const QString &text, QWidget *parent = nullptr);
    ~QNotificationBox();
private:
    QIconButton *closeButton;

signals:

public slots:
    void removeNotification();
};

#endif // QNOTIFICATIONBOX_H
