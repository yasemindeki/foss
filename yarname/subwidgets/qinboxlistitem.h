#ifndef QINBOXLISTITEM_H
#define QINBOXLISTITEM_H

#include <QWidget>
#include <QDateTime>

QT_BEGIN_NAMESPACE
class QLabel;
class QInboxData;
QT_END_NAMESPACE

class QInboxListItem : public QWidget
{
    Q_OBJECT
public:
    explicit QInboxListItem(QWidget *parent = nullptr);
    QInboxListItem(QInboxData *data, QWidget *parent = nullptr);

    QString subject();
    QString from();
    QString time();
    QString profile();

private:
    QInboxData *rxPacket;

    QLabel      *subjectLabel;
    QLabel      *fromLabel;
    QLabel      *timeLabel;
    QLabel      *body;

    void initialize();
    QString formatDateTime(const QDateTime &input, bool fullView = false);

protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void clicked(QInboxListItem *);

public slots:


};

#endif // QINBOXLISTITEM_H
