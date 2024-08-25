#include "qposteditor.h"
#include "subwidgets/qposteditblock.h"
#include "subwidgets/qpostedit.h"

#include <QtWidgets>

QPostEditor::QPostEditor(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground);
    this->setObjectName("postEditor");
    this->setStyleSheet("QWidget#postEditor { \
                            background-color: rgb(248, 248, 248); \
                        } \
                        QWidget#innerWindow { \
                                    background-color: rgb(255, 255, 255); \
                                    border: 1px solid rgb(220, 220, 220); \
                                    border-top: 5px solid rgb(91, 161, 123); \
                                }");

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QToolBar *postToolBar = new QToolBar();
    postToolBar->setFixedHeight(27);
    // enable color: rgb(32, 50, 78)
    // background-color: rgb(176, 182, 193);
    postToolBar->setStyleSheet("QToolBar { \
                                    margin: 0px; \
                                    background-color: rgb(32, 50, 78); \
                                    border-radius: 0px; \
                                } \
                                QToolButton { margin-left: 5px; border:none; background-color:transparent; } \
                                QToolButton:hover { border-bottom: 2px solid rgb(91, 161, 123); }  \
                                ");

    QAction *actionTextBlock = new QAction(QIcon(":/icons/addtext.png"), tr("Add Text Block"), this);
    QAction *actionImageBlock = new QAction(QIcon(":/icons/addimage.png"), tr("Add Image Block"), this);
    QAction *actionAttachment = new QAction(QIcon(":/icons/attach.png"), tr("Add an attachment"), this);
    postToolBar->addSeparator();
    postToolBar->addAction(actionTextBlock);
    postToolBar->addAction(actionImageBlock);
    postToolBar->addAction(actionAttachment);

    this->connect(actionAttachment, SIGNAL(triggered()), this, SLOT(onAttachmentClicked()));
    this->connect(actionTextBlock, SIGNAL(triggered()), this, SLOT(onTextBlockClicked()));

    QVBoxLayout *editorLayout = new QVBoxLayout();
    editorLayout->setContentsMargins(40, 20, 40, 20);

    // Daha icte yer alan editor penceresi islemleri
    QWidget *innerWindow = new QWidget();
    innerWindow->setObjectName("innerWindow");
    innerWindow->setAttribute(Qt::WA_StyledBackground);

    innerLayout = new QVBoxLayout();
    innerLayout->setContentsMargins(1, 5, 1, 1);    // border-top: 5px oldugundan ustten 5px, diger kenarlardan 1 px margin birakiyoruz.
    innerLayout->setSpacing(0);
    innerLayout->setAlignment(Qt::AlignTop);

    QFrame *topFrame = new QFrame();
    QLabel *postFooter = new QLabel("Powered by yarname");

    QPostEditBlock *textBlock = new QPostEditBlock();
    textBlocks.append(textBlock);

    // QSS ayarlari
    topFrame->setFixedHeight(27);
    topFrame->setStyleSheet("QFrame { border-bottom: 1px solid rgb(220, 220, 220); }");
    postFooter->setFixedHeight(47);     // padding-top 20px olmasi sebebiyle 27 + 20 = 47 px yuksekligi
    postFooter->setAlignment(Qt::AlignHCenter);
    postFooter->setStyleSheet("QLabel { font: 8pt 'Goldman'; border-top: 1px solid rgb(220, 220, 220); padding-top: 20px; }");

    innerLayout->addWidget(topFrame);
    innerLayout->addWidget(textBlock);
    innerLayout->addStretch();
    innerLayout->addWidget(postFooter);
    innerLayout->addWidget(postToolBar);

    innerWindow->setLayout(innerLayout);
    editorLayout->addWidget(innerWindow);

    mainLayout->addLayout(editorLayout);
    //mainLayout->addWidget(postToolBar);
    this->setLayout(mainLayout);

}

void QPostEditor::onTextBlockClicked()
{
    QPostEditBlock *textBlock = new QPostEditBlock();

    textBlocks.append(textBlock);
    innerLayout->insertWidget(textBlocks.count(), textBlock);
}

void QPostEditor::onAttachmentClicked()
{
    QFileDialog fileDialog(this, tr("Select A File"));
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);

    if (fileDialog.exec() == QDialog::Accepted)
    {
        QString filePath = fileDialog.selectedFiles().first();
        qDebug() << filePath;

        emit aFileAttached(filePath);
    }

}

QString QPostEditor::getHtmlBody(const QString &htmlString)
{
    QString bodyContent;

    qDebug() << "getHtmlBody() " << htmlString;

    QRegularExpression regex("<body\\s+style=.*>([\\s\\S]*)</body>");
    QRegularExpressionMatchIterator regexi = regex.globalMatch(htmlString);

    while (regexi.hasNext())
    {
        QRegularExpressionMatch match = regexi.next();
        bodyContent = match.captured(1);
    }
    return bodyContent;
}

QString QPostEditor::getHtmlData()
{
    QString htmlOutput;

    // Metin bloklarindan body icini alip <br/> ekleyerek join etme
    foreach (QPostEditBlock *textBlock, textBlocks)
    {
        QString body = getHtmlBody(textBlock->textEdit->toHtml());
        body.append("<br/>");
        htmlOutput.append(body);
    }

    htmlOutput.prepend("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body>");
    htmlOutput.append("</body></html>");

    return htmlOutput;
}

/*
 *
 * PROGRESS
 * <TASK>
 * ToggleButton icin tbEditBlock islevselligini kodlama
 * + Button ekleme
 * + Buton hover ile border-bottom rengini yeşil yapma
 * + Tıklama ile arkaplan rengini yeşil yapma
 * + ToggleButton olması için tekrar tıklama ile arkaplan rengini eskisine döndürme
 * + Metnin kalin olmasi icin slot fonksiyonu baglama
 * + Italik icin tbEditBlock islevselligini kodlama
 * + Underline icin tbEditBlock islevselligini kodlama
 * Font rengi icin tbEditBlock islevselligini kodlama
 * - Renk secim kutusu ornegi Qt Examples icinde var mi arastirmasi
 * - Renk secim kutusu tasarimi
 * - Renk secim QAction butonu ekleme
 * - Renk secim QAction butonu islevselligini tamamlama
 *
 *  TODO
 *
 * <TASK>
 * Font rengini secili metin icin degistirebilme
 * <TASK>
 * İstenilen kadar TEXTBLOCK eklenebilmesi
 * <TASK>
 * Her TEXTBLOCK icin bir tbEditBlock olması
 *
 *
 * DONE
 *
 * <TASK>
 * BOLD-ITALIC-UNDERLINE metin islevselligini kazandirma
 * <TASK>
 * QPostEditor, postToolBar ve tbEditBlock ayrimini kodlama
 * <TASK>
 * TEXTBLOCK secildigi vakit cerceve renginin mavi olmasi
 * <TASK>
 * TEXTBLOCK secildigi vakit tbEditBlock aracinin gorunur olmasi
 * <TASK>
 * TEXTBLOCK secili olmadigi vakit cercevesiz olmasi
 * <TASK>
 * "You can write here..." focus yok iken ve bos metin ise yazmasi, focus var iken ilk harfi girdikten sonra yazilani korumasi.
 *
 *
 * SORU: TEXTBLOCK icinde secilen bir metnin sadece bold-underline yapilmasi mumkun mu?
 * CEVAP: Evet, QTextEdit sinifinda bu amaca hizmet eden uc fonksiyon var:
 *          setFontUnderline(true);
 *          setFontItalic(true);
 *          setFontWeight(QFont::DemiBold);
 *
 * HEDEFLENEN: mail kutusundaki Kaspersky mesajlarindan birinin tasarimini
 *             QPostEditor ile kolaylikla yapabilme
 *
 *             Logo Kaspersky
 *             Dikkat! Lisansınızın süresi doldu!
 *             Lisansınızın süresi doldu. Kaspersky Internet Security'yi hemen yenileyin ve güvende kalın.
 *             BUTTON Şimdi yenileyin
 *             %50'a kadar indirim*
 *
 *             Sayın Yasemin YILMAZ,
 *
 */

