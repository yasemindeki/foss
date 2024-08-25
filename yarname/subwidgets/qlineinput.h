#ifndef QLINEINPUT_H
#define QLINEINPUT_H

#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
QT_END_NAMESPACE

class QLineInput : public QLineEdit
{
    Q_OBJECT
public:
    explicit QLineInput(const QString &title, QWidget *parent = nullptr);
    explicit QLineInput(QWidget *parent = nullptr);
    void setPixmap(const QString &iconFilePath);

    void setValidator(const QString &regex, QPushButton *submitButton); // override

private:
    QLabel *titleLabel = nullptr;
    QPixmap pixmap;
    QPushButton *continueButton = nullptr;

    void initialize();

protected:
    void paintEvent(QPaintEvent* event) override;

public slots:
    void setLabelPos();
    void changeSubmitButton(const QString &);

    void setEnabled(bool);  // override

signals:

};

#endif // QLINEINPUT_H
