#ifndef QFONTLISTMODEL_H
#define QFONTLISTMODEL_H

#include <QAbstractListModel>
#include <QAbstractItemDelegate>
#include <QFontDatabase>

class QFontItemDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    explicit QFontItemDelegate(QObject *parent);

    // painting
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

    QIcon truetype;
    QIcon bitmap;
    QFontDatabase::WritingSystem writingSystem;
};

class FontListData
{
public:
    FontListData(const QString &fontFamily, const QString &fallbackFontList);
    FontListData(const QString &fontTitle);

    QString displayName();
    QString fallbacks();

private:
    QString name; // ya font basliginin ya da font ailesinin ismi
    QString fallbackFonts;

};

class QFontListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit QFontListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    enum FontListRoles
    {
        FontTitleRole = Qt::UserRole + 1,
        FontFallbackRole
    };

private:
    QVector<FontListData *> flData;
signals:

};

#endif // QFONTLISTMODEL_H
