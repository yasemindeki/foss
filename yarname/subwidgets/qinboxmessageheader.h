#ifndef QINBOXMESSAGEHEADER_H
#define QINBOXMESSAGEHEADER_H

#include <QFrame>

QT_BEGIN_NAMESPACE
class QLabel;
class QToolBar;
QT_END_NAMESPACE

class QInboxMessageHeader : public QFrame
{
    Q_OBJECT

    friend class QInboxMessagePane;
public:
    explicit QInboxMessageHeader(QWidget *parent = nullptr);

private:
    QLabel *subjectLabel;
    QLabel *numBodies;

    QToolBar *tbCategories;

    void createCategories();

signals:

};

#endif // QINBOXMESSAGEHEADER_H
