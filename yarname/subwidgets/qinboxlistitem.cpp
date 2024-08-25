#include "qinboxlistitem.h"
#include "mailer/engines.h"

#include <QGridLayout>
#include <QStyleOption>
#include <QLabel>
#include <QPainter>
#include <QDataStream>
#include <QTextCodec>
#include <QDebug>

QInboxListItem::QInboxListItem(QWidget *parent) : QWidget(parent)
{
    initialize();
}

QString QInboxListItem::formatDateTime(const QDateTime &input, bool fullView)
{
    /* Ayni gun ise,        hh:mm       Today hh:mm
     * Dun ise,             Yesterday   Yesterday hh:mm
     * Ayni yil icinde ise  26 Sep      26 September, hh:mm
     * Digerlerinde         26.09.22    26 September 2022, hh:mm
     */

    QString timeString = input.toString("hh:mm"); // "dd/MM/yy"
    QString dateString;
    if (fullView)
        dateString = QString("Today %1").arg(timeString);
    else
        dateString = timeString;

    return dateString;
}

QInboxListItem::QInboxListItem(QInboxData *data, QWidget *parent)
    : QWidget(parent), rxPacket(data)
{
    initialize();

    if (data->fromParts.count() > 0)
        data->decodeFrom();

    if (data->from->name().isEmpty())
        fromLabel->setText(data->from->addr());
    else
        fromLabel->setText(data->from->name());

    timeLabel->setText(formatDateTime(data->dateTime));
    qDebug() << "timeLabel is " << timeLabel->text();

    if (data->subjectParts.count() > 0)
    {
        qDebug() << "decoded subject";
        data->decodeSubject();
    }
    subjectLabel->setText(data->subject);

    if (data->isContentEncoded)
    {
        qDebug() << "decoded content";
        data->decodeContent();
    }
    body->setText(data->content);
}

void QInboxListItem::initialize()
{
    this->setAttribute(Qt::WA_StyledBackground);
    this->setAttribute(Qt::WA_Hover);

    QGridLayout *mainlayout = new QGridLayout();
    mainlayout->setContentsMargins(36, 10, 5, 10);
    mainlayout->setSpacing(0);

    subjectLabel = new QLabel("");
    fromLabel = new QLabel("");
    body = new QLabel("");
    timeLabel = new QLabel("");

    mainlayout->addWidget(fromLabel, 0, 0, Qt::AlignLeft);
    mainlayout->addWidget(subjectLabel, 1, 0, Qt::AlignLeft);
    mainlayout->addWidget(body, 2, 0, Qt::AlignLeft);
    mainlayout->addWidget(timeLabel, 0, 1, Qt::AlignRight);

    this->setLayout(mainlayout);

    this->setObjectName("QInboxListItem");
    this->setStyleSheet("QWidget#QInboxListItem { background-color: white; color: black; } \
                         ");
    fromLabel->setStyleSheet("font: 9pt 'Segoe UI'; font-weight: bold; color: rgb(39, 39, 39);");
    subjectLabel->setStyleSheet("font: 9pt 'Segoe UI'; color: rgb(39, 39, 39);");
    body->setStyleSheet("font: 8pt 'Segoe UI'; color: rgb(128, 128, 128);");
    timeLabel->setStyleSheet("font: 8pt 'Segoe UI'; color: rgb(128, 128, 128);");

    //QWidget#QInboxListItem:hover { background-color: rgb(36, 41, 46); color: white; border-top-left-radius: 10px; border-top-right-radius: 10px; border-bottom: 1px solid rgb(136, 182, 235); }

    this->setMinimumHeight(60);
}

QString QInboxListItem::subject()
{
    return subjectLabel->text();
}

QString QInboxListItem::from()
{
    return fromLabel->text();
}

QString QInboxListItem::time()
{
    return formatDateTime(rxPacket->dateTime, true);
}

QString QInboxListItem::profile()
{
    QString profileName;

    if (rxPacket->from->name().isEmpty())
        profileName = rxPacket->from->addr().at(0);
    else
    {
        QStringList list = rxPacket->from->name().split(' ', QString::SkipEmptyParts);
        profileName = list.at(0).at(0);
        if (list.count() > 1)
            profileName += list.at(1).at(0);
    }

    return profileName;
}

void QInboxListItem::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
/*
    from->setStyleSheet("font: 9pt 'Segoe UI'; font-weight: bold; color: white;");
    subject->setStyleSheet("font: 9pt 'Segoe UI'; color: white;");
    body->setStyleSheet("font: 8pt 'Segoe UI'; color: rgb(136, 182, 235);");
    sendTime->setStyleSheet("font: 8pt 'Segoe UI'; color: white;");
*/
}

void QInboxListItem::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
/*
    from->setStyleSheet("font: 9pt 'Segoe UI'; font-weight: bold; color: rgb(39, 39, 39);");
    subject->setStyleSheet("font: 9pt 'Segoe UI'; color: rgb(39, 39, 39);");
    body->setStyleSheet("font: 8pt 'Segoe UI'; color: rgb(128, 128, 128);");
    sendTime->setStyleSheet("font: 8pt 'Segoe UI'; color: rgb(128, 128, 128);");
*/
}

void QInboxListItem::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    QStyleOption option;
    option.initFrom(this);

    // En altina boydan boya cizgi olsun
    QPoint p1(36, this->height());
    QPoint p2(this->width(), this->height());
    QPen pen(QColor(229, 229, 229));
    pen.setWidth(2);

    // Solunda okundu bilgisi ikonu olsun
    QRect iconRect(QPoint(12, 10), QSize(10, 10));
    QPixmap pixmap(":/icons/readflag-10px.png");

    painter.save();
    painter.drawPixmap(iconRect, pixmap);
    if (option.state & QStyle::State_MouseOver)
        pen.setColor(QColor(241, 70, 89));
    painter.setPen(pen);
    painter.drawLine(p1, p2);
    painter.restore();
}

void QInboxListItem::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);

    emit clicked(this);
}
