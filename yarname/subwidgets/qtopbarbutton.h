#ifndef QTOPBARBUTTON_H
#define QTOPBARBUTTON_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTextButton;
QT_END_NAMESPACE

class QTopBarButton : public QWidget
{
    Q_OBJECT
public:
    explicit QTopBarButton(const QString &title, const QString &leftIcon, QWidget *parent = nullptr);
    void resize(int menuPaneHeight);

    void setDepends(QTopBarButton *to);
    void onTwinChecked();

private:
    QTextButton *pushButton;
    QWidget *menuPane;
    QTopBarButton *twin;

signals:

public slots:
    void onButtonClicked(bool);


};

#endif // QTOPBARBUTTON_H
