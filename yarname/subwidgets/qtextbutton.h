#ifndef QTEXTBUTTON_H
#define QTEXTBUTTON_H

#include <QPushButton>

class QTextButton : public QPushButton
{
    Q_OBJECT
public:
    explicit QTextButton(const QString &text, QWidget *parent = nullptr);
    explicit QTextButton(const QString &text, const QString &iconFilePath, QWidget *parent = nullptr);

    void setHoverStyle(const QString &json);
    void setDefaultStyle(const QString &json);
    void clearStyles();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap pixmap;

    void initialize();

signals:

};

#endif // QTEXTBUTTON_H
