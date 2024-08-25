#ifndef QFINDERITEM_H
#define QFINDERITEM_H

#include <QFrame>

QT_BEGIN_NAMESPACE
class QLabel;
class QFitLabel;
class QIconButton;
QT_END_NAMESPACE

class QFinderItem : public QFrame
{
    Q_OBJECT
public:
    explicit QFinderItem(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~QFinderItem();
    //QSize sizeHint() const override;
    void setMaximumWidth(int maxw); // override

    void setData(const QString &filePath, quint64 fileSize);
    QString id();
    quint64 bytes();

private:
    QFitLabel   *pathLabel;
    QLabel      *sizeLabel;
    QIconButton *removeButton;
    quint64     size;

protected:
    //void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void itemDeletion(const QString &path);

public slots:
    void onRemoveButtonClicked();
    void onCurrentItemChanged(const QString &);

};

#endif // QFINDERITEM_H
