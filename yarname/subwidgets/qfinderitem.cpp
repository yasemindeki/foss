#include "qfinderitem.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

#include "subwidgets/qiconbutton.h"
#include "subwidgets/qfinder.h"
#include "subwidgets/qfitlabel.h"

QFinderItem::QFinderItem(QWidget* parent, Qt::WindowFlags f) : QFrame(parent, f)
{
    pathLabel = new QFitLabel("");
    sizeLabel = new QLabel("");

    removeButton = new QIconButton(tr("Remove attachment"));
    removeButton->setFixedSize(21, 14);
    removeButton->setIcons(":/icons/remove.png", ":/icons/remove-hover.png");

    this->connect(removeButton, SIGNAL(pressed()), this, SLOT(onRemoveButtonClicked()));

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(pathLabel);
    mainLayout->addWidget(sizeLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(removeButton);
    this->setLayout(mainLayout);

    //this->setAttribute(Qt::WA_StyledBackground);
    this->setStyleSheet(" \
                        QFrame { \
                           background-color: white; \
                           border-bottom: 1px solid rgb(234, 238, 240); \
                        } \
                        QFrame:hover { background-color: rgb(246, 248, 250); } \
                        QFrame[selected=\"true\"] { background-color: rgb(212, 242, 235); } \
                        QLabel { \
                           background-color: transparent; \
                           border: none; \
                        } \
                        QLabel:hover { background-color: transparent; } \
                        ");
}

void QFinderItem::setMaximumWidth(int maxw)
{
    QFrame::setMaximumWidth(maxw);

    int availableWidth = (maxw - 16);
    int pathWidth = availableWidth * 0.8;
    int sizeWidth = availableWidth * 0.2;

    qDebug() << "pathWidth is " << pathWidth;

    this->pathLabel->setMaximumWidth(pathWidth);
    this->sizeLabel->setMaximumWidth(sizeWidth);
}

QFinderItem::~QFinderItem()
{

}

QString QFinderItem::id()
{
    return pathLabel->toolTip();
}

quint64 QFinderItem::bytes()
{
    return size;
}

void QFinderItem::setData(const QString &filePath, quint64 fileSize)
{
    pathLabel->setText(filePath);
    pathLabel->setToolTip(filePath);

    size = fileSize;

    QString sizeText = QFinder::getSizeString(size);
    sizeLabel->setText(sizeText);
}

void QFinderItem::onRemoveButtonClicked()
{
    emit itemDeletion(id());
}

void QFinderItem::mousePressEvent(QMouseEvent *event)
{
    QFrame::mousePressEvent(event);

    this->setProperty("selected", true);
    this->style()->polish(this);
}

void QFinderItem::onCurrentItemChanged(const QString &currentText)
{
    if (currentText == id())
    {
        this->setProperty("selected", true);
    }
    else
    {
        this->setProperty("selected", false);
    }

    this->style()->polish(this);
}

/*
void QFinderItem::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    // Font, stylesheet ile atanmis ise
    // paintEvent fonksiyonunda gecerli oldugunu goruyoruz, setStyleSheet cagrisi sonrasi degil.
    //QFont font = pathLabel->font();
    //qDebug() << "Attach font is " << font.family();

    QStyleOption option;
    option.init(this);

    QPainter painter(this);
    //qDebug () << "QFinderItem::state is " << option.state;
}*/
    //painter.draw
    /*
     * Eger burada silme ikonu drawImage, pesinden dosya ismi ve buyuklugu drawText etsek
     * QFinderItem'a komple tiklama ile silme yapabiliriz
     * Sadece ikon icin tiklama sinyali diye birsey yok
     * painter'la pushbutton, toolbutton cizdir diye de birsey yok
    */


