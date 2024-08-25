#ifndef QPOSTEDIT_H
#define QPOSTEDIT_H

#include <QTextEdit>

class QPostEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit QPostEdit(QWidget *parent = nullptr);
    //void mouseMoveEvent(QMouseEvent *e) override;

    //void enterEvent(QEvent *event) override;
    //void leaveEvent(QEvent *event) override;
    //bool event(QEvent *e) override;

private:
    QTextDocument *baseDoc;

protected:
    //void paintEvent(QPaintEvent *e) override;
    //void showEvent(QShowEvent *) override;
    void resizeEvent(QResizeEvent *e) override;

signals:

public slots:
    void onDocumentSizeChanged(const QSizeF &);
    void onFontSizeChanged(const QString &fontSize);
    void onFontChanged(const QString &fontFamily);

};

#endif // QPOSTEDIT_H
