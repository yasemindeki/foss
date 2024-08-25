#include "qposteditblock.h"
#include "mvc/qfontlistmodel.h"
#include "subwidgets/qpostedit.h"
#include "subwidgets/qimagebrowser.h"

#include <QtWidgets>

QPostEditBlock::QPostEditBlock(QWidget *parent) : QFrame(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    vmargin = 10;
    mainLayout->setContentsMargins(10, vmargin, 10, 0);
    mainLayout->setSpacing(0);

    //this->setStyleSheet("border: 1px solid red;");

    textEdit = new QPostEdit();
    tbEditBlock = new QToolBar();
    tbEditBlock->setMovable(false);

    fontSizeSelector = new QComboBox();
    fontSizeSelector->addItem("8");     // tiny
    fontSizeSelector->addItem("10");    // small
    fontSizeSelector->addItem("12");    // medium           yandex 16 font buyuklugu buna karsilik geliyor
    fontSizeSelector->addItem("14");    // large
    fontSizeSelector->addItem("16");    // extra large
    fontSizeSelector->addItem("20");    // huge
    fontSizeSelector->setCurrentIndex(1);
    textEdit->setFontPointSize(fontSizeSelector->currentText().toInt());

    /* fontSizeSelector lookandfeel ayarlari*/
    fontSizeSelector->setToolTip("Font size");
    fontSizeSelector->setFixedSize(50, 20);
    fontSizeSelector->setStyleSheet("QComboBox { \
                                        selection-color: black; \
                                        selection-background-color: rgb(242, 244, 252); \
                                        border: 1px solid rgb(242, 244, 252); border-top-right-radius: 5px; border-bottom-right-radius: 5px; \
                                    } \
                                    QComboBox::hover { border: none; color: black; background-color: rgb(242, 244, 252); } \
                                    QComboBox::drop-down { border: none; } \
                                    QComboBox::down-arrow { image: url(:/icons/down-arrow.png); width: 16px; height: 16px; } \
                                    QComboBox QAbstractItemView { \
                                        selection-color: black; \
                                        selection-background-color: rgb(242, 244, 252); \
                                    }");


    fontSelector = new QComboBox();
    fontSelector->setModel(new QFontListModel());
    fontSelector->setItemDelegate(new QFontItemDelegate(fontSelector));
    fontSelector->setCurrentIndex(5);
    textEdit->setFontFamily(fontSelector->currentText());

    /* fontSelector lookandfeel ayarlari*/
    fontSelector->setToolTip("Font family");
    fontSelector->setFixedSize(100, 20);
    fontSelector->setStyleSheet("QComboBox { \
                                    selection-color: black; \
                                    selection-background-color: rgb(242, 244, 252); \
                                    border: 1px solid rgb(242, 244, 252); border-top-left-radius: 5px; border-bottom-left-radius: 5px; \
                                } \
                                QComboBox::hover { border: none; color: black; background-color: rgb(242, 244, 252); } \
                                QComboBox::drop-down { border: none; } \
                                QComboBox::down-arrow { image: url(:/icons/down-arrow.png); width: 16px; height: 16px; } \
                                QComboBox QAbstractItemView { \
                                    min-width: 200px; \
                                    selection-color: black; \
                                    selection-background-color: rgb(242, 244, 252); \
                                }");

    tbEditBlock->addSeparator();
    tbEditBlock->addWidget(fontSelector);
    tbEditBlock->addWidget(fontSizeSelector);

    addToolButtons();

    this->connect(textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));
    this->connect(textEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)), this, SLOT(onCurrentCharFormatChanged(const QTextCharFormat &)));
    this->connect(fontSelector, SIGNAL(currentIndexChanged(const QString &)), textEdit, SLOT(onFontChanged(const QString &)));
    this->connect(fontSizeSelector, SIGNAL(currentIndexChanged(const QString &)), textEdit, SLOT(onFontSizeChanged(const QString &)));

    textEdit->installEventFilter(this);

    tbEditBlock->setFixedHeight(37);    // ustten alttan 5px margin sebebiyle 27 + 10 = 37 px yuksekligi
    //tbEditBlock->setContentsMargins(10, 0, 10, 0);
    tbEditBlock->setStyleSheet("QToolBar { \
                                    margin: 5px; \
                                    background-color: rgb(32, 50, 78); \
                                    border-radius: 5px; \
                                  } \
                                  QToolButton { margin-left: 0px; border:none; background-color:transparent; } \
                                  QToolButton:checked { border: none; background-color: rgb(91, 161, 123); }  \
                                  QToolButton:hover { border-bottom: 2px solid rgb(91, 161, 123); }  \
                                ");
    tbEditBlock->hide();

    mainLayout->addWidget(textEdit);
    mainLayout->addWidget(tbEditBlock);
    mainLayout->addStretch(1);

    this->setLayout(mainLayout);
}

void QPostEditBlock::onInlineImageClicked()
{
    QImageBrowser fileDialog(this, tr("Select Image File"));

    if (fileDialog.exec() == QDialog::Accepted)
    {
        QString imageFilePath = fileDialog.selectedFiles().first();
        qDebug() << imageFilePath;

        QString htmlString = textEdit->toHtml();
        htmlString.append("<br />");
        htmlString.append(QString("<img src=\"%1\" alt=\"\"/>").arg(imageFilePath));
        textEdit->setHtml(htmlString);
    }
}

void QPostEditBlock::addToolButtons()
{
    actionTextBold = new QAction(QIcon(":/icons/tobold-14px.png"), tr("Format text to &bold"), this);
    actionTextBold->setCheckable(true);

    QToolButton *tbBold = new QToolButton();
    tbBold->setDefaultAction(actionTextBold);
    tbBold->setFocusPolicy(Qt::ClickFocus); // from Qt::TabFocus to Qt::ClickFocus
    tbBold->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));

    actionTextItalic = new QAction(QIcon(":/icons/toitalic-14px.png"), tr("Format text to &italic"), this);
    actionTextItalic->setCheckable(true);

    QToolButton *tbItalic = new QToolButton();
    tbItalic->setDefaultAction(actionTextItalic);
    tbItalic->setFocusPolicy(Qt::ClickFocus); // from Qt::TabFocus to Qt::ClickFocus
    tbItalic->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));

    actionTextUnderline = new QAction(QIcon(":/icons/tounderscore-14px.png"), tr("Format text to &underline"), this);
    actionTextUnderline->setCheckable(true);

    QToolButton *tbUnderline = new QToolButton();
    tbUnderline->setDefaultAction(actionTextUnderline);
    tbUnderline->setFocusPolicy(Qt::ClickFocus); // from Qt::TabFocus to Qt::ClickFocus
    tbUnderline->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));

    actionTextColor = new QAction(createTextColorIcon(QColor(Qt::black)), tr("Change text color"), this);

    QToolButton *tbColor = new QToolButton();
    tbColor->setDefaultAction(actionTextColor);
    tbColor->setFocusPolicy(Qt::ClickFocus); // from Qt::TabFocus to Qt::ClickFocus

    actionInlineImage = new QAction(QIcon(":/icons/addimage.png"), tr("Add an image"), this);

    QToolButton *tbImage = new QToolButton();
    tbImage->setDefaultAction(actionInlineImage);
    tbImage->setFocusPolicy(Qt::ClickFocus);

    this->connect(actionInlineImage, SIGNAL(triggered()), this, SLOT(onInlineImageClicked()));
    this->connect(actionTextBold, SIGNAL(triggered()), this, SLOT(onTextBoldToggled()));
    this->connect(actionTextItalic, SIGNAL(toggled(bool)), this, SLOT(onTextItalicToggled(bool)));
    this->connect(actionTextUnderline, SIGNAL(toggled(bool)), this, SLOT(onTextUnderlineToggled(bool)));
    this->connect(actionTextColor, SIGNAL(triggered()), this, SLOT(onTextColorClicked()));

    tbEditBlock->addWidget(tbBold);
    tbEditBlock->addWidget(tbItalic);
    tbEditBlock->addWidget(tbUnderline);
    tbEditBlock->addWidget(tbColor);
    tbEditBlock->addSeparator();
    tbEditBlock->addWidget(tbImage);
}

bool QPostEditBlock::hasToolButtonFocus()
{
    bool hasFocus = false;

    foreach (QToolButton *toolButton, tbEditBlock->findChildren<QToolButton *>())
    {
        if (toolButton->hasFocus())
        {
            hasFocus = true;
            break;
        }
    }

    //qDebug() << "hasToolButtonFocus() is return by " << hasFocus;

    return hasFocus;
}

void QPostEditBlock::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);

    //qDebug() << "QPostEditBlock width = " << this->width();

    // Ustten alttan toplamda 10 piksel az yer kaplamali
    // Sagdan soldan 10 piksel toplamda 20 piksel az yer kaplamali
    textEdit->resize(this->size().width() - 20, this->size().height() - vmargin);
    textEdit->viewport()->resize(this->size().width() - 20, this->size().height() - vmargin);

    //qDebug() << "fs w is " << fontSelector->width();
    //qDebug() << "fs h is " << fontSelector->height();
}

void QPostEditBlock::clearFormatActions()
{
    actionTextBold->setChecked(false);
    actionTextItalic->setChecked(false);
    actionTextUnderline->setChecked(false);
}

QIcon QPostEditBlock::createTextColorIcon(const QColor &color)
{
    QPixmap pixmap(":/icons/textcolor-18px.png");
    QPainter painter(&pixmap);

    QRect rect(2, 15, 14, 2);
    QPen pen(Qt::white);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawRect(rect);
    painter.fillRect(rect, QBrush(color, Qt::SolidPattern));

    QIcon tcIcon(pixmap);

    return tcIcon;

}

void QPostEditBlock::onCursorPositionChanged()
{
    const QTextCharFormat format = textEdit->currentCharFormat();

    if (format.fontWeight() >= QFont::DemiBold)
    {
        actionTextBold->setChecked(true);
    }
    else
    {
        actionTextBold->setChecked(false);
    }

    actionTextItalic->setChecked(format.fontItalic());
    actionTextUnderline->setChecked(format.fontUnderline());

    // Renk kontrolu
    QBrush brush = format.foreground();
    QColor color = brush.color();

    actionTextColor->setIcon(createTextColorIcon(color));

    fontSelector->setCurrentText(format.fontFamily());
    fontSizeSelector->setCurrentText(QString::number(format.fontPointSize()));
}

void QPostEditBlock::onTextColorClicked()
{
    QColor colorPicked = QColorDialog::getColor(textEdit->textColor(), this);
    if (!colorPicked.isValid())
        return;

    setColorOnSelection(colorPicked);

    actionTextColor->setIcon(createTextColorIcon(colorPicked));

    textEdit->setFocus();
}

void QPostEditBlock::setColorOnSelection(const QColor &colorPicked)
{
    // Secili metin yok ise cursor'un o anda uzerinde oldugu kelimeyi seciyoruz.
    QTextCursor cursor = textEdit->textCursor();
    if (!cursor.hasSelection())
    {
        cursor.select(QTextCursor::WordUnderCursor);
    }

    // Yeni metin rengine sahip bir CharFormat olusturuyoruz.
    QTextCharFormat charFormat;
    charFormat.setForeground(colorPicked);

    // Cursor'a bu yeni CharFormat'i ekliyoruz.
    cursor.mergeCharFormat(charFormat);
    // Metin kutusuna bu yeni CharFormat'i ekliyoruz.
    textEdit->mergeCurrentCharFormat(charFormat);

    qDebug() << textEdit->toHtml();
}

void QPostEditBlock::onTextUnderlineToggled(bool checked)
{
    if (checked)
    {
        textEdit->setFontUnderline(true);
    }
    else
    {
        textEdit->setFontUnderline(false);
    }

    textEdit->setFocus();
}

void QPostEditBlock::onTextItalicToggled(bool checked)
{
    if (checked)
    {
        textEdit->setFontItalic(true);
    }
    else
    {
        textEdit->setFontItalic(false);
    }

    textEdit->setFocus();
}

void QPostEditBlock::onTextBoldToggled()
{
    if (actionTextBold->isChecked())
    {
        textEdit->setFontWeight(QFont::Bold);
    }
    else
    {
        textEdit->setFontWeight(QFont::Normal);
    }

    textEdit->setFocus();
}

void QPostEditBlock::focusInEvent(QFocusEvent *event)
{
    QWidget::focusInEvent(event);

    qDebug() << "QPostEditBlock::focusInEvent";
}

void QPostEditBlock::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);

    qDebug() << "QPostEditBlock::focusOutEvent";
}

bool QPostEditBlock::eventFilter(QObject *watched, QEvent *event)
{
    bool rc = true;
    QTextEdit *textBlock = qobject_cast<QTextEdit *>(watched);

    if (!textBlock)
        return false;

    //qDebug() << "QPostEditBlock::event type is " << event->type();

    switch (event->type())
    {
    case QEvent::FocusIn:   // Klavye odagini aldigi vakit, postToolBar goruntuleniyor.
        tbEditBlock->show();

        rc = false;

        break;
    case QEvent::FocusOut:  // Klavye odagindan ciktigi vakit, postToolBar saklaniyor.
        if (!fontSelector->hasFocus() && !fontSizeSelector->hasFocus() && !hasToolButtonFocus())
        {
            tbEditBlock->hide();
            this->adjustSize();
        }

        rc = false;

        break;

    case QEvent::KeyPress:
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //Qt::Key key = static_cast<Qt::Key>(keyEvent->key());
        //qDebug() << "key is " << key;

        if (keyEvent->matches(QKeySequence::SelectAll))
        {
            clearFormatActions();
        }
    }
        rc = false; // to allow them to be sent on the target widget (QTextEdit'in varsayilan implementasyonu icra edilecekse false)

        break;

    default:
        break;
    }

    return rc;
}

void QPostEditBlock::onCurrentCharFormatChanged(const QTextCharFormat &)
{
    qDebug() << "onCurrentCharFormatChanged";

    if (textEdit->toPlainText().isEmpty())
    {
        textEdit->setFontFamily(fontSelector->currentText());
        textEdit->setFontPointSize(fontSizeSelector->currentText().toInt());
    }
}
