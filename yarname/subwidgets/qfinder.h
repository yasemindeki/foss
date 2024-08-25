#ifndef QFINDER_H
#define QFINDER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QFrame;
class QListWidget;
class QLabel;
class QIconButton;
class QFinderItem;
QT_END_NAMESPACE

/* Dosya ekleri yoneticisi
 * Ismi macos dosya yoneticisinden geliyor.
 */
class QFinder : public QWidget
{
    Q_OBJECT
public:
    explicit QFinder(QWidget *parent = nullptr);

    static QString getSizeString(quint64 size);

    bool hasFiles();
    QVector<QFinderItem *> files();

    void hideEditor();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QFrame      *           indicator;
    QListWidget *           editor;    // Yakin gelecekte, QScrollArea donusumu gerek
    QVector<QFinderItem *>  itemList;

    QLabel      *countIndicator;
    QLabel      *sizeIndicator;
    QIconButton *displayButton;
    quint64     totalSize;

    void createIndicatorItems();
    bool calculateFileSize(const QString &filePath);
    void addFinderItem(const QString &filePath, quint64 fileSize);
    void updateCount();
    void updateSize();
    void setEditorHeight();

public slots:
    void onFileAttached(const QString &filePath);
    void onDisplayButtonClicked();
    void onListItemRemoved(const QString &path);

signals:
    void largeFileSendRequest();

};

#endif // QFINDER_H
