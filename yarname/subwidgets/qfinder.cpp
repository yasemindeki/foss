#include "qfinder.h"
#include "subwidgets/qiconbutton.h"
#include "subwidgets/qfinderitem.h"
#include "mvc/qfinderitemdelegate.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QListWidget>
#include <QLabel>
#include <QFileInfo>
#include <QDebug>

#define FILE_SIZE_LIMIT (25 * 1024 * 1024)

QFinder::QFinder(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);

    indicator = new QFrame();
    editor = new QListWidget();
    editor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    editor->hide();

    //editor->setItemDelegate(new QFinderItemDelegate(this));
    editor->setFocusPolicy(Qt::NoFocus);

    //padding: 5px 1px 5px 1px;
    editor->setStyleSheet("QListWidget { border-top: 1px solid rgb(234, 238, 240); }");
    /* editor->setStyleSheet("QListWidget::item {\
                            background-color: white; \
                          } \
                          QListWidget::item:selected { background-color: rgb(212, 242, 235); } \
                          QListWidget::item:hover { background-color: rgb(246, 248, 250); } \
                          QListWidget::item:selected:hover { background-color: rgb(212, 242, 235); } \
                          ");
    */

    createIndicatorItems();

    mainLayout->addWidget(indicator);
    mainLayout->addWidget(editor);

    this->setLayout(mainLayout);

    totalSize = 0;
}

void QFinder::createIndicatorItems()
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(10, 0, 10, 0);

    countIndicator = new QLabel("0 file");
    sizeIndicator = new QLabel("0 bytes");

    displayButton = new QIconButton(tr("Show Attachments"));
    displayButton->setFixedSize(16, 16);
    displayButton->setIcons(":/icons/expand-bottomright.png", ":/icons/expand-bottomright-hover.png");

    layout->addWidget(displayButton);
    layout->addWidget(countIndicator);
    layout->addWidget(sizeIndicator);
    layout->addStretch();

    indicator->setLayout(layout);

    this->connect(displayButton, SIGNAL(pressed()), this, SLOT(onDisplayButtonClicked()));
}

void QFinder::onDisplayButtonClicked()
{
    if (editor->isHidden())
    {
        displayButton->setIcons(":/icons/expand-topleft.png", ":/icons/expand-topleft-hover.png");
        displayButton->setToolTip(tr("Hide Attachments"));
        editor->show();
    }
    else
    {
        hideEditor();
    }
}

bool QFinder::hasFiles()
{
    return itemList.count();
}

QVector<QFinderItem *> QFinder::files()
{
    return itemList;
}

bool QFinder::calculateFileSize(const QString &filePath)
{
    bool    largeFile = false;
    QFileInfo fileInfo(filePath);
    quint64 fileSize = fileInfo.size();

    if ((totalSize + fileSize) >= (FILE_SIZE_LIMIT))
    {
        largeFile = true;

        emit largeFileSendRequest();

        qDebug() << "Warning! You are about to send a message of size " << fileSize;
        qDebug() << "This is a large file. It might be better to use Filelink instead.";
        qDebug() << "The size of the message you are trying to send \
                     exceeds the global size limit of the server. \
                     The message was not sent; reduce the message size and try again.";
    }
    else
    {
        totalSize += fileSize;

        updateSize();

        addFinderItem(filePath, fileSize);
    }

    return largeFile;
}

QString QFinder::getSizeString(quint64 size)
{
    QString sizeText;

    const quint64 kb = 1024;
    const quint64 mb = 1024 * kb;
    //const quint64 gb = 1024 * mb;

    if (size >= mb)
        sizeText = QString("%1 MB").arg(size / mb);
    else if (size >= kb)
        sizeText = QString("%1 KB").arg(size / kb);
    else
        sizeText = QString("%1 bytes").arg(size);

    return sizeText;
}

void QFinder::onFileAttached(const QString &filePath)
{
    if (!calculateFileSize(filePath))
    {
        updateCount();
        setEditorHeight();

        this->show();
    }
}

void QFinder::setEditorHeight()
{
    int editorHeight = (editor->sizeHintForRow(0) * editor->count()) + (2 * editor->frameWidth());
    editor->setFixedHeight(editorHeight);
}

void QFinder::updateCount()
{
    QString fileText;
    int fileCount = itemList.count();

    if (fileCount == 1)
        fileText = tr(" file");
    else
        fileText = tr(" files");

    countIndicator->setText(QString::number(fileCount).append(fileText));
}

void QFinder::updateSize()
{
    QString totalSizeText = QFinder::getSizeString(totalSize);
    sizeIndicator->setText(totalSizeText);
}

void QFinder::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    qDebug() << "QFinder width is " << this->width();

    foreach (QFinderItem *item, itemList)
    {
        item->setMaximumWidth(this->width());
        item->resize(this->width(), item->height());
    }
}

void QFinder::addFinderItem(const QString &filePath, quint64 fileSize)
{
    QFinderItem *finderItem = new QFinderItem();
    finderItem->setData(filePath, fileSize);

    itemList.append(finderItem);

    QListWidgetItem *item = new QListWidgetItem(filePath);
    item->setSizeHint(finderItem->sizeHint());
    //qDebug() << "addFinderItem: " << item->flags();

    editor->addItem(item);
    editor->setItemWidget(item, finderItem);

    connect(finderItem, SIGNAL(itemDeletion(const QString &)), this, SLOT(onListItemRemoved(const QString &)));
    connect(editor, SIGNAL(currentTextChanged(const QString &)), finderItem, SLOT(onCurrentItemChanged(const QString &)));
}

void QFinder::onListItemRemoved(const QString &path)
{
    for (int i=0; i< editor->count(); i++)
    {
        QListWidgetItem *item = editor->item(i);
        QFinderItem *finderItem = dynamic_cast<QFinderItem *>(editor->itemWidget(item));

        if (finderItem->id() == path)
        {
            totalSize -= finderItem->bytes();
            updateSize();

            itemList.removeOne(finderItem);
            updateCount();

            setEditorHeight();

            delete item;
            break;
        }
    }

    if (itemList.isEmpty())
    {
        hideEditor();
        this->hide();
    }
}

void QFinder::hideEditor()
{
    displayButton->setIcons(":/icons/expand-bottomright.png", ":/icons/expand-bottomright-hover.png");
    displayButton->setToolTip(tr("Show Attachments"));
    editor->hide();
}
