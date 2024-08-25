#ifndef QPOSTEDITOR_H
#define QPOSTEDITOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPostEditBlock;
class MimeMessage;
class QVBoxLayout;
QT_END_NAMESPACE

class QPostEditor : public QWidget
{
    Q_OBJECT
public:
    explicit QPostEditor(QWidget *parent = nullptr);
    QString getHtmlData();

private:
    QVector<QPostEditBlock *> textBlocks;
    QVBoxLayout *innerLayout;

    QString getHtmlBody(const QString &);

signals:
    void aFileAttached(const QString &filePath);

public slots:
    void onAttachmentClicked();
    void onTextBlockClicked();
};

#endif // QPOSTEDITOR_H
