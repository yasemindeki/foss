#include "qpostwindow.h"
#include "qposteditor.h"
#include "qiconbutton.h"
#include "subwidgets/totextedit.h"

#include "mailer/mimemessage.h"
#include "mailer/mimemessage.h"
#include "mailer/mimeentity.h"

#include "yncore/checker.h"
#include "subwidgets/qnotificationbox.h"
#include "subwidgets/qfinder.h"
#include "subwidgets/qfinderitem.h"

#include <QtWidgets>

QPostWindow::QPostWindow(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground);
    this->setObjectName("postWindow");
    this->setStyleSheet("QWidget{\
                            font: 8pt 'Ubuntu'; \
                        }\
                        QLineEdit { \
                                border: none; \
                                border-bottom: 1px solid rgb(223, 225, 229); \
                                color: rgb(53, 70, 98); \
                                selection-background-color: rgb(56, 113, 193); \
                                selection-color: rgb(246, 248, 250); \
                        } \
                        QToolButton { \
                            border: none; \
                            margin-left: 10px; \
                            text-align: left center; \
                            color: rgb(36, 41, 46); \
                        } \
                        QToolButton:hover { \
                            color: rgb(56, 113, 193); \
                        } \
                        QWidget#postWindow { \
                            background-color: rgb(255, 255, 255); \
                            border-left: 15px solid rgb(220, 220, 220); \
                            border-right: 15px solid rgb(220, 220, 220); \
                            border-top: 10px solid rgb(220, 220, 220); \
                            border-bottom: 30px solid rgb(220, 220, 220); \
                            border-left-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, \
                                                            stop: 0 rgb(255, 255, 255), \
                                                            stop: 0.9 rgb(245, 245, 245), \
                                                            stop: 1 rgb(220, 220, 220)); \
                            border-top-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, \
                                                            stop: 0 rgb(255, 255, 255), \
                                                            stop: 0.9 rgb(245, 245, 245), \
                                                            stop: 1 rgb(220, 220, 220)); \
                            border-right-color: qlineargradient(spread:pad, x1:1, y1:0, x2:0, y2:0, \
                                                            stop: 0 rgb(255, 255, 255), \
                                                            stop: 0.9 rgb(245, 245, 245), \
                                                            stop: 1 rgb(220, 220, 220)); \
                            border-bottom-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, \
                                                            stop: 0 rgb(255, 255, 255), \
                                                            stop: 0.9 rgb(245, 245, 245), \
                                                            stop: 1 rgb(220, 220, 220)); \
                        }");

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(15, 20, 15, 30);
    mainLayout->setAlignment(Qt::AlignTop);

    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setContentsMargins(40, 0, 40, 0);
    gridLayout->setAlignment(Qt::AlignTop);

    // QLabel & QLineEdit ciftleri
    QLabel *subject = new QLabel("Subject: ");
    subjectInput = new QLineEdit("New Subject");
    QLabel *to = new QLabel("To: ");
    toInput = new ToTextEdit(this);

    subjectInput->setStyleSheet("QLineEdit { \
                                    font: 9pt 'Ubuntu';\
                                    color: rgb(76, 76, 76); \
                                    border: none; \
                                }");

    // Send butonu
    sendButton = new QIconButton("Send this message now", this);
    sendButton->setIcons(":/icons/send.png", ":/icons/send-hover.png");


    // CC, From butonu
    QToolButton *ccFromButton = new QToolButton();
    ccFromButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ccFromButton->setLayoutDirection(Qt::RightToLeft);
    ccFromButton->setText("CC, From");
    ccFromButton->setIcon(QIcon(":/icons/down-arrow.png"));
    ccFromButton->setParent(parent);
    ccFromButton->installEventFilter(this);

    gridLayout->addWidget(subject, 0, 0);
    gridLayout->addWidget(subjectInput, 0, 1);
    gridLayout->addWidget(to, 1, 0);
    gridLayout->addWidget(toInput, 1, 1);
    gridLayout->addWidget(ccFromButton, 1, 2);

    editor = new QPostEditor();

    mailEngine = new Mail::CURLEngine();

    checker = new Checker();
    finder = new QFinder();
    finder->hide();

    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(finder);
    mainLayout->addWidget(editor, 9);

    this->setLayout(mainLayout);

    connect(sendButton, SIGNAL(clicked()), this, SLOT(onSendButtonClicked()));
    connect(editor, SIGNAL(aFileAttached(const QString &)), finder, SLOT(onFileAttached(const QString &)));
    connect(finder, SIGNAL(largeFileSendRequest()), this, SLOT(showMessageSizeLimit()));
}

void QPostWindow::onSendButtonClicked()
{
    // To, CC, From e-mail adresi dogrulama islemi
    // mail kontrolu coktan yapildi
    // sirada
    // MailboxList olusturup gondermek var

    Mail::HeaderStore   *headerSection = new Mail::HeaderStore();
    MimeMessage         *email = new MimeMessage(headerSection);

    MailboxList *toList = toInput->toList();
    if (toList->isEmpty())
    {
        // gecerli bir email adresi ihtiyacini bildirme
    }
    else
    {
        // Elektronik postanin ustbilgisini Mime arayuzuyle tanimlama

        Mail::Mailbox *sender = new Mail::Mailbox("Yasemin YILMAZ", "yasemin.yilmaz.embedded@gmail.com");

        headerSection->addField(Mail::FROM, QVariant::fromValue(sender));
        headerSection->addField(Mail::TO, QVariant::fromValue(*toList));
        headerSection->addField(Mail::SUBJECT, subjectInput->text());

        composeMimeMessage(email);
    }
}

void QPostWindow::composeMimeMessage(MimeMessage *mimeMessage)
{
    QString htmlString = editor->getHtmlData();

    MimeEntity *mimeEntity = new MimeEntity("text/html; charset=\"utf8\"");

    // Eger ek varsa
    bool hasAttachments = finder->hasFiles();
    foreach(QFinderItem *finderItem, finder->files())
    {
        QString filePath = finderItem->id();
        qDebug() << filePath << " will be send";

        MimeAttachment *fileEntity = new MimeAttachment(filePath);
        mimeMessage->addContent(fileEntity);
    }

    // Eger inline image var ise cid replace
    bool hasInlineImage = false;
    MimeMultipart *multipart = NULL;

    QRegularExpression regex("<img\\s+src=\"(.*)\"");
    QRegularExpressionMatchIterator regexi = regex.globalMatch(htmlString);

    while (regexi.hasNext())
    {
        if (!hasInlineImage)
        {
            multipart = new MimeMultipart("multipart/related");
            hasInlineImage = true;
        }

        QRegularExpressionMatch match = regexi.next();
        //QString result = match.captured();
        QString imageFilePath = match.captured(1);

        MimeAttachment *fileEntity = new MimeAttachment(imageFilePath, true);
        multipart->addEntity(fileEntity);

    }

    if (hasInlineImage)
    {
        htmlString = htmlString.replace(QRegularExpression("<img\\s+src=\""), "<img src=\"cid:");

        mimeEntity->setBody(htmlString);
        multipart->addEntity(mimeEntity); // data in body of the message

        mimeMessage->addContent(multipart);
    }
    else
    {
        mimeEntity->setBody(htmlString);
        if (hasAttachments)
            mimeMessage->addContent(mimeEntity);
        else
            mimeMessage->setContent(mimeEntity);
    }

    qDebug() << htmlString;

    sendMail(mimeMessage);
}

void QPostWindow::sendMail(MimeMessage *email)
{
    // Mail gonderme kuyrugunu devreye alsak
    // Kuyruk bos ise - sinyal alana kadar -
    // Sinyal ile tetiklendiginde kuyruk dolu ise - kuyruk bos olana kadar -
    // Kuyruk beklemeden buradan gelecek sinyal ile hemen cikabilsin

    // nodejs'den email ayarlarini ya da diger kanal ayarlarini alan bir Activity Monitor
    // benzeri bir GUI elamani olsa
    /*
    QMailConfig emailSettings = restservice->getMailSettings(); // email_user_pwd DEK ile sifreli gelsin
    mailEngine->setAccount(emailSettings);
    */
    /*
    packet = restService->getCredentials(customerID, connectionID);
    int errorCode = mailEngine->updateAccount(packet); // cagrisi sonrasi packet bellekten silinsin
    QByteArray customerKey = cryptoEngine->resolvePacket(packet); // yncore updateAccount fonksiyonu icinde yapilsin
    */

    int errorCode = mailEngine->sendMail(email);
    QString userMessage("Message sent");

    if (errorCode != 0)
        userMessage = QString("Message not sent (%1)").arg(errorCode);

    QNotificationBox *notifyBox = new QNotificationBox(userMessage, this);
    notifyBox->show();

    notifyBox->adjustSize();
    notifyBox->raise();

    int leftPos = (this->width() - notifyBox->width()) / 2;
    notifyBox->move(leftPos, 100);
}

void QPostWindow::showMessageSizeLimit()
{
    QString userMessage = tr("This is a large file. It might be better to use Filelink instead.");
    QNotificationBox *notifyBox = new QNotificationBox(userMessage, this);
    notifyBox->show();

    notifyBox->adjustSize();
    notifyBox->raise();

    int leftPos = (this->width() - notifyBox->width()) / 2;
    notifyBox->move(leftPos, 100);
}

void QPostWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    //qDebug() << "QPostWindow width = " << this->width();

    sendButton->move((this->width() - 20), 0); // setter funtion for property 'pos'
}

bool QPostWindow::eventFilter(QObject *watched, QEvent *event)
{
    QToolButton *button = qobject_cast<QToolButton *>(watched);

    if (!button)
        return false;

    // Hover sirasinda aktif gorunumu veriyoruz.
    if (event->type() == QEvent::Enter)
    {
        button->setIcon(QIcon(":/icons/down-arrow-hover.png"));
        return true;
    }

    // Hover'dan cikista, pasif gorunumu veriyoruz.
    if (event->type() == QEvent::Leave)
    {
        button->setIcon(QIcon(":/icons/down-arrow.png"));
        return true;
    }

    return false;
}
