#ifndef TOTEXTEDIT_H
#define TOTEXTEDIT_H

#include <QTextEdit>

#include "mailer/basetypes.h"

QT_BEGIN_NAMESPACE
class QTextDocument;
class Checker;
QT_END_NAMESPACE

class ToTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit ToTextEdit(QWidget *parent = nullptr);
    MailboxList *toList();

private:
    typedef enum
    {
        IMG_DEFAULT = 0,
        IMG_PRESSED = 1,
        IMG_INCORRECT = 2
    }ImageType;

    QTextDocument   *baseDoc;
    Checker         *checker;

    MailboxList     *mbList;

    int             currentImagePos;
    int             currentImageType;
    QString         currentImageName;

    //int             fontHeight;

    void toImage(int type, const QString &text);
    QTextImageFormat createTextImage(int type, const QString &text);
    void resetCurrentImage(QTextCursor *cursor);

    void iterateByCursor(QTextCursor *cursor);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *) override;

signals:
    void toImagePressed(const QString &name, int vt);

public slots:
    void onDocumentSizeChanged(const QSizeF &);

};

#endif // TOTEXTEDIT_H
