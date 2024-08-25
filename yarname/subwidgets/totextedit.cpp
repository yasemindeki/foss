#include "totextedit.h"
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QTextDocument>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>

#include "yncore/checker.h"

#include "subwidgets/qpostwindow.h"

ToTextEdit::ToTextEdit(QWidget *parent) : QTextEdit(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->connect(this, SIGNAL(toImagePressed(const QString &, int)), this, SLOT(onImageClicked(const QString &, int)));

    mbList = new MailboxList();
    baseDoc = this->document();
    checker = new Checker();

    currentImageType = -1;
    currentImagePos = -1;
    currentImageName = "";

    //QFontMetrics fm(this->font());
    //fontHeight = fm.lineSpacing();

    /*
    qDebug() << "doc font height is " << fm.height();
    qDebug() << "doc font leading is " << fm.leading();
    qDebug() << "doc font line spacing is " << fm.lineSpacing(); // This value is always equal to leading()+height().
    qDebug() << "doc margin is " << baseDoc->documentMargin();
    */

    QAbstractTextDocumentLayout *tdLayout = this->document()->documentLayout();
    connect(tdLayout, SIGNAL(documentSizeChanged(const QSizeF &)), this, SLOT(onDocumentSizeChanged(const QSizeF &)));

}

void ToTextEdit::onDocumentSizeChanged(const QSizeF &)
{
    //qDebug() << "ToTextEdit::onDocumentSizeChanged";

    //int nh = (fontHeight * this->document()->blockCount()) + 15;
    int contentHeight = baseDoc->size().height() + baseDoc->documentMargin();

    this->setFixedHeight(contentHeight);
}

void ToTextEdit::resizeEvent(QResizeEvent *event)
{
    this->updateGeometry();
    QTextEdit::resizeEvent(event);

    //qDebug() << "ToTextEdit width = " << this->width();
}

void ToTextEdit::toImage(int type, const QString &text)
{
    this->textCursor().beginEditBlock();

    QTextCursor cursor(baseDoc);

    // DIKKAT: QTextDocument::find fonksiyonu, selection durumunu degistiriyor !
    cursor = baseDoc->find(text, cursor);
    if (!cursor.isNull())
    {
        QTextImageFormat tif = createTextImage(type, text);
        cursor.insertImage(tif);
    }

    this->textCursor().endEditBlock();
}

void ToTextEdit::keyPressEvent(QKeyEvent *event)
{
    int keyPressed = event->key();
    if (keyPressed == Qt::Key_Space)
    {
        qDebug() << "keyPress: " << this->toPlainText();
        QStringList textList = this->toPlainText().split(keyPressed, QString::SkipEmptyParts);

        foreach (QString text, textList)
        {
            if (text.startsWith('\n'))
                text = text.mid(1);
            if (text.length() == 1)
                continue;

            if (checker->isMail(text))
                toImage(IMG_DEFAULT, text);
            else
                toImage(IMG_INCORRECT, text);
        }
    }

    QTextEdit::keyPressEvent(event);
}

void ToTextEdit::focusOutEvent(QFocusEvent *event)
{
    if (currentImagePos != -1)
    {
        QTextCursor cursor = this->textCursor();

        resetCurrentImage(&cursor);
        currentImagePos = -1;
    }

    QTextEdit::focusOutEvent(event);
}

void ToTextEdit::mousePressEvent(QMouseEvent *event)
{
    QTextCursor textCursor = this->cursorForPosition(event->pos());
    textCursor.select(QTextCursor::WordUnderCursor);

    QTextFormat format = textCursor.charFormat();
    if (format.isImageFormat())
    {
        QTextImageFormat    tif = format.toImageFormat();
        QString             toAddress = tif.name();
        int                 viewType = tif.property(QTextFormat::UserProperty + 1).toInt();
        qDebug() << "format.isImageFormat:" << toAddress << ":" << viewType;

        if (viewType != IMG_PRESSED)
        {
            // Basili gorunum verilecek olani silip ekliyoruz
            textCursor.select(QTextCursor::WordUnderCursor);
            textCursor.removeSelectedText();

            QTextImageFormat tif = createTextImage(IMG_PRESSED, toAddress);
            textCursor.insertImage(tif);

            int newPosition = textCursor.position();

            // Onceki secili olana default/incorrect gorunum veriyoruz
            if (currentImagePos != -1)
            {
                resetCurrentImage(&textCursor);
            }

            currentImagePos = newPosition;
            currentImageName = toAddress;
            currentImageType = viewType;

            //emit toImagePressed(tif.name(), tif.properties()[1].toInt());
            this->setCursorWidth(0);
        } // viewType

    } // isImageFormat
    else
    {
        this->setCursorWidth(1);
        QTextEdit::mousePressEvent(event);

        // Onceki secili olana default/incorrect gorunum veriyoruz
        if (currentImagePos != -1)
        {
            resetCurrentImage(&textCursor);
            currentImagePos = -1;
        }
    } // isImageFormat

}

void ToTextEdit::resetCurrentImage(QTextCursor *cursor)
{
    cursor->setPosition(currentImagePos);

    cursor->select(QTextCursor::WordUnderCursor);
    cursor->removeSelectedText();

    QTextImageFormat tif = createTextImage(currentImageType, currentImageName);
    cursor->insertImage(tif);

}

void ToTextEdit::iterateByCursor(QTextCursor *cursor)
{
    //cursor.beginEditBlock();

    bool moved = cursor->movePosition(QTextCursor::Start);

    while (moved && !cursor->atEnd())
    {
        cursor->select(QTextCursor::WordUnderCursor);
        //qDebug() << "selectedText = " << cursor->selectedText();

        QTextFormat format = cursor->charFormat();
        //qDebug() << "textformat type = " << format.type();

        if (format.isValid() && format.isImageFormat())
        {
            QTextImageFormat tif = format.toImageFormat();
            QString toAddress = tif.name();

            qDebug() << "name: " << toAddress;
        }

        moved = cursor->movePosition(QTextCursor::Right);
        moved = cursor->movePosition(QTextCursor::Right);
    }

    //cursor.endEditBlock();
}

MailboxList *ToTextEdit::toList()
{
    QTextBlock block = this->document()->begin();
    while (block.isValid())
    {
        QTextBlock::iterator tbi;

        for (tbi = block.begin(); !(tbi.atEnd()); tbi++)
        {
            QTextFragment currentFragment = tbi.fragment();
            if (currentFragment.isValid())
            {
                QTextCharFormat charFormat = currentFragment.charFormat();
                if (charFormat.isImageFormat())
                {
                    QTextImageFormat tif = charFormat.toImageFormat();
                    int viewType = tif.property(QTextFormat::UserProperty + 1).toInt();

                    if (viewType == IMG_DEFAULT)
                    {
                        mbList->append(new Mail::Mailbox(tif.name()));
                        //qDebug() << "name: " << tif.name();
                    }

                } // isImageFormat

            } // isValid

        } // for


        block = block.next();
    }

    return mbList;
}

QTextImageFormat ToTextEdit::createTextImage(int type, const QString &text)
{
    QString path;
    QColor penColor;

    switch (type) {
    case IMG_INCORRECT:
        path = ":/icons/toimage-error.png";
        penColor.setRgb(164, 0, 15);
        break;
    case IMG_PRESSED:
        path = ":/icons/toimage-pressed.png";
        penColor.setRgb(255, 255, 255);
        break;
    default:
        path = ":/icons/toimage.png";
        penColor.setRgb(0, 0, 0);
        break;
    }

    QTextImageFormat tif;
    tif.setProperty(QTextFormat::UserProperty + 1, type);
    tif.setName(text);
    tif.setVerticalAlignment(QTextImageFormat::AlignMiddle);

    // Metin genisligini olcuyoruz
    QFont font("Ubuntu", 8);
    QFontMetrics fm(font);
    int width = fm.horizontalAdvance(text);

    //qDebug() << "fontmetrics: " << width << " for " << text;

    QPixmap pixmap(path);
    QPixmap newPixmap = pixmap.scaled(width + 20, 19);//, Qt::KeepAspectRatio);

    QPainter painter(&newPixmap);
    painter.setPen(penColor);
    painter.setFont(font);
    painter.drawText(QPoint(10, 12), text);

    QImage image = newPixmap.toImage();
    baseDoc->addResource(QTextDocument::ImageResource, QUrl(text), image);

    return tif;
}
