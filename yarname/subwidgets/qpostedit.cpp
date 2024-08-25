#include "qpostedit.h"

#include <QStyleOptionFrame>
#include <QPainter>
#include <QDebug>
#include <QEvent>
#include <QAbstractTextDocumentLayout>

QPostEdit::QPostEdit(QWidget *parent) : QTextEdit(parent)
{
    //this->setMouseTracking(true);

    baseDoc = this->document();
    this->setPlaceholderText("You can write here...");

    this->setStyleSheet("QTextEdit { \
                            border: none; \
                            background-color: white; \
                        } \
                        QTextEdit:hover { border: 1px solid rgb(204, 213, 224); background-color: rgb(241, 250, 255); } \
                        QTextEdit:focus { \
                            border: 1px solid rgb(56, 113, 193); \
                            background-color: white; \
                        }");

    QAbstractTextDocumentLayout *tdLayout = this->document()->documentLayout();
    connect(tdLayout, SIGNAL(documentSizeChanged(const QSizeF &)), this, SLOT(onDocumentSizeChanged(const QSizeF &)));
}

void QPostEdit::resizeEvent(QResizeEvent *e)
{
    this->updateGeometry();
    QTextEdit::resizeEvent(e);

    //qDebug() << "QPostEdit width = " << this->width();

    //qDebug() << "resizeEvent(): document width is " << this->document()->size().width();
    //qDebug() << "resizeEvent(): viewport width is " << this->viewport()->width();
    //qDebug() << "resizeEvent(): width is " << this->width();
}

void QPostEdit::onDocumentSizeChanged(const QSizeF &)
{
    //qDebug() << "QPostEdit::onDocumentSizeChanged";

    int contentHeight = baseDoc->size().height() + baseDoc->documentMargin();
    this->setFixedHeight(contentHeight);

    //qDebug() << "QPostEdit::onDocumentSizeChanged(): document width is " << this->document()->size().width();
    //qDebug() << "QPostEdit::onDocumentSizeChanged(): viewport width is " << this->viewport()->width();
    //qDebug() << "QPostEdit::onDocumentSizeChanged(): width is " << this->width();
}

void QPostEdit::onFontSizeChanged(const QString &fontSize)
{
    this->setFontPointSize(fontSize.toInt());

    this->setFocus();
}

void QPostEdit::onFontChanged(const QString &fontFamily)
{
    this->setFontFamily(fontFamily);

    this->setFocus();
}



/*
void QPostEdit::paintEvent(QPaintEvent *event)
{
    QStyleOptionFrame *option = new QStyleOptionFrame();
    option->initFrom(this);

    QPainter painter(this->viewport());
    painter.setPen("red");
    painter.setBrush(QColor("red"));
    painter.drawRect(viewport()->rect());

    //qDebug() << "deger is " << option->state;

    // DIKKAT: setMouseTracking cagrilmadi ise MouseOver gelmiyor
    if (option->state & QStyle::State_MouseOver)
    {
        //qDebug() << "mouse over";
    }
    else
    {
        //qDebug() << "mouse over not";
    }

    QTextEdit::paintEvent(event);

    //qDebug() << "QPostEdit::paintEvent viewport width is " << this->viewport()->width();
    //qDebug() << "QPostEdit::paintEvent width is " << this->width();
}
void QPostEdit::showEvent(QShowEvent *event)
{
    QTextEdit::showEvent(event);

    //qDebug() << "showEvent(): document width is " << this->document()->size().width();
    //qDebug() << "showEvent(): viewport width is " << this->viewport()->width();
    //qDebug() << "showEvent(): width is " << this->width();

}

void QPostEdit::enterEvent(QEvent *event)
{
    QTextEdit::enterEvent(event);
    qDebug() << "mouse enter";
}

void QPostEdit::leaveEvent(QEvent *event)
{
    QTextEdit::leaveEvent(event);
    qDebug() << "mouse leave";
}

bool QPostEdit::event(QEvent *e)
{
    qDebug() << e->type();

    return QTextEdit::event(e);
}


void QPostEdit::mouseMoveEvent(QMouseEvent *event)
{
    QTextEdit::mouseMoveEvent(event);

    QPalette palette;
    palette.setColor(QPalette::Window, Qt::red);
    this->setPalette(palette);

    qDebug() << "mouseMoveEvent";

}
*/
