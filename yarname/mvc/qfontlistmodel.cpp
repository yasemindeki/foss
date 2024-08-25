#include "qfontlistmodel.h"

#include <QDebug>
#include <QPainter>

FontListData::FontListData(const QString &fontTitle)
    : name(fontTitle)
{
    fallbackFonts = "";
}

FontListData::FontListData(const QString &fontFamily, const QString &fallbackFontList)
    : name(fontFamily), fallbackFonts(fallbackFontList)
{

}

QString FontListData::displayName()
{
    return name;
}

QString FontListData::fallbacks()
{
    return fallbackFonts;
}

QFontListModel::QFontListModel(QObject *parent) : QAbstractListModel(parent)
{
    flData.append(new FontListData("Designer Favorities"));
    flData.append(new FontListData("Calibri", "Arial"));
    flData.append(new FontListData("Helvetica", "Arial"));
    flData.append(new FontListData("Safe Fonts"));
    flData.append(new FontListData("Arial", "sans-serif, serif"));
    flData.append(new FontListData("Courier New", "Arial"));
    flData.append(new FontListData("All Fonts"));

    // PC'de yuklu tum fontlar
    QFontDatabase fb;
    foreach (QString fontfamily, fb.families())
    {
        // MingLiU font ailesinin QFontComboBox da dahil gösteriminde bug var.
        if (!fontfamily.startsWith("MingLiU"))
            flData.append(new FontListData(fontfamily, "Courier New"));
    }

}

int QFontListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return flData.count();
}

QVariant QFontListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    FontListData *listData = flData.at(index.row());
    bool isTitleRow = listData->fallbacks().isEmpty();

    switch (role) {
    case FontTitleRole:
        return isTitleRow;
        break;
    case FontFallbackRole:
        return listData->fallbacks();
        break;
    case Qt::DisplayRole:
        return listData->displayName();
        break;
    // ItemDelegate::paint reimplementasyonu varsa, islevi kalmiyor ki bizde oyle
    /*
    case Qt::ForegroundRole:
        return isTitleRow ? QVariant::fromValue(QColor(Qt::white)) : QVariant();
        break;
    case Qt::BackgroundRole:
        return isTitleRow ? QVariant::fromValue(QColor(91, 161, 123)) : QVariant();
        break;
    */
    default:
        //qDebug() << role;
        break;

    }

    return {};
}

Qt::ItemFlags QFontListModel::flags(const QModelIndex &index) const
{
    bool isTitleRow = index.data(QFontListModel::FontTitleRole).toBool();
    if (isTitleRow)
        return Qt::ItemIsEnabled;
    else
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

static QFontDatabase::WritingSystem writingSystemFromScript(QLocale::Script script)
{
    switch (script) {
    case QLocale::ArabicScript:
        return QFontDatabase::Arabic;
    case QLocale::CyrillicScript:
        return QFontDatabase::Cyrillic;
    case QLocale::GurmukhiScript:
        return QFontDatabase::Gurmukhi;
    case QLocale::SimplifiedHanScript:
        return QFontDatabase::SimplifiedChinese;
    case QLocale::TraditionalHanScript:
        return QFontDatabase::TraditionalChinese;
    case QLocale::LatinScript:
        return QFontDatabase::Latin;
    case QLocale::ArmenianScript:
        return QFontDatabase::Armenian;
    case QLocale::BengaliScript:
        return QFontDatabase::Bengali;
    case QLocale::DevanagariScript:
        return QFontDatabase::Devanagari;
    case QLocale::GeorgianScript:
        return QFontDatabase::Georgian;
    case QLocale::GreekScript:
        return QFontDatabase::Greek;
    case QLocale::GujaratiScript:
        return QFontDatabase::Gujarati;
    case QLocale::HebrewScript:
        return QFontDatabase::Hebrew;
    case QLocale::JapaneseScript:
        return QFontDatabase::Japanese;
    case QLocale::KhmerScript:
        return QFontDatabase::Khmer;
    case QLocale::KannadaScript:
        return QFontDatabase::Kannada;
    case QLocale::KoreanScript:
        return QFontDatabase::Korean;
    case QLocale::LaoScript:
        return QFontDatabase::Lao;
    case QLocale::MalayalamScript:
        return QFontDatabase::Malayalam;
    case QLocale::MyanmarScript:
        return QFontDatabase::Myanmar;
    case QLocale::TamilScript:
        return QFontDatabase::Tamil;
    case QLocale::TeluguScript:
        return QFontDatabase::Telugu;
    case QLocale::ThaanaScript:
        return QFontDatabase::Thaana;
    case QLocale::ThaiScript:
        return QFontDatabase::Thai;
    case QLocale::TibetanScript:
        return QFontDatabase::Tibetan;
    case QLocale::SinhalaScript:
        return QFontDatabase::Sinhala;
    case QLocale::SyriacScript:
        return QFontDatabase::Syriac;
    case QLocale::OriyaScript:
        return QFontDatabase::Oriya;
    case QLocale::OghamScript:
        return QFontDatabase::Ogham;
    case QLocale::RunicScript:
        return QFontDatabase::Runic;
    case QLocale::NkoScript:
        return QFontDatabase::Nko;
    default:
        return QFontDatabase::Any;
    }
}

static QFontDatabase::WritingSystem writingSystemFromLocale()
{
    QStringList uiLanguages = QLocale::system().uiLanguages();
    QLocale::Script script;
    if (!uiLanguages.isEmpty())
        script = QLocale(uiLanguages.at(0)).script();
    else
        script = QLocale::system().script();

    return writingSystemFromScript(script);
}

static QFontDatabase::WritingSystem writingSystemForFont(const QFont &font, bool *hasLatin)
{
    QList<QFontDatabase::WritingSystem> writingSystems = QFontDatabase().writingSystems(font.family());
//     qDebug() << font.family() << writingSystems;

    // this just confuses the algorithm below. Vietnamese is Latin with lots of special chars
    writingSystems.removeOne(QFontDatabase::Vietnamese);
    *hasLatin = writingSystems.removeOne(QFontDatabase::Latin);

    if (writingSystems.isEmpty())
        return QFontDatabase::Any;

    QFontDatabase::WritingSystem system = writingSystemFromLocale();

    if (writingSystems.contains(system))
        return system;

    if (system == QFontDatabase::TraditionalChinese
            && writingSystems.contains(QFontDatabase::SimplifiedChinese)) {
        return QFontDatabase::SimplifiedChinese;
    }

    if (system == QFontDatabase::SimplifiedChinese
            && writingSystems.contains(QFontDatabase::TraditionalChinese)) {
        return QFontDatabase::TraditionalChinese;
    }

    system = writingSystems.last();

    if (!*hasLatin) {
        // we need to show something
        return system;
    }

    if (writingSystems.count() == 1 && system > QFontDatabase::Cyrillic)
        return system;

    if (writingSystems.count() <= 2 && system > QFontDatabase::Armenian && system < QFontDatabase::Vietnamese)
        return system;

    if (writingSystems.count() <= 5 && system >= QFontDatabase::SimplifiedChinese && system <= QFontDatabase::Korean)
        return system;

    return QFontDatabase::Any;
}

QFontItemDelegate::QFontItemDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{
    truetype = QIcon(QLatin1String(":/qt-project.org/styles/commonstyle/images/fonttruetype-16.png"));
    bitmap = QIcon(QLatin1String(":/qt-project.org/styles/commonstyle/images/fontbitmap-16.png"));
    writingSystem = QFontDatabase::Any;
}

void QFontItemDelegate::paint(QPainter *painter,
                                const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    /*+*/bool isTitleRow = index.data(QFontListModel::FontTitleRole).toBool();

    QString text = index.data(Qt::DisplayRole).toString();
    QFont font(option.font);
    /*-*/
    //font.setPointSize(QFontInfo(font).pointSize() * 3 / 2);
    QFont font2 = font;

    /*+*/if (!isTitleRow)
    font2.setFamily(text);

    bool hasLatin;
    QFontDatabase::WritingSystem system = writingSystemForFont(font2, &hasLatin);
    if (hasLatin)
        font = font2;

    /*r -> itemRect*/
    QRect itemRect = option.rect;

    if (option.state & QStyle::State_Selected) {
        painter->save();
        painter->setBrush(option.palette.highlight());
        painter->setPen(Qt::NoPen);
        painter->drawRect(option.rect);
        painter->setPen(QPen(option.palette.highlightedText(), 0));
    }
    /*+*/else
    {
        if (isTitleRow)
        {
            painter->setBrush(QColor(91, 161, 123));
            painter->drawRect(option.rect);
            painter->setPen(QPen(Qt::white));
        }
        else
            painter->setPen(QPen(Qt::black));
    }

    // Sola hizali olarak font ikonu yerlestiriliyor; sistemin tanidiklari icin truetype, digerleri icin bitmap
    const QIcon *icon = &bitmap;
    if (QFontDatabase().isSmoothlyScalable(text)) {
        icon = &truetype;
    }
    /*+*/if (!isTitleRow)
        icon->paint(painter, itemRect, Qt::AlignLeft|Qt::AlignVCenter);

    // Ikonun genisligi kadar <sol baslangic> ilerletiliyor, 4 pixel de bosluk icin yer birakiliyor.
    QSize actualSize = icon->actualSize(itemRect.size());
    if (option.direction == Qt::RightToLeft)
        itemRect.setRight(itemRect.right() - actualSize.width() - 4);
    else
        itemRect.setLeft(itemRect.left() + actualSize.width() + 4);

    QFont old = painter->font();
    painter->setFont(font);

    // If the ascent of the font is larger than the height of the rect,
    // we will clip the text, so it's better to align the tight bounding rect in this case
    // This is specifically for fonts where the ascent is very large compared to
    // the descent, like certain of the Stix family.
    QFontMetricsF fontMetrics(font);
    if (fontMetrics.ascent() > itemRect.height()) {
        QRectF tbr = fontMetrics.tightBoundingRect(text);
        painter->drawText(itemRect.x(), itemRect.y() + (itemRect.height() + tbr.height()) / 2.0, text);
    } else {
        painter->drawText(itemRect, Qt::AlignVCenter|Qt::AlignLeading|Qt::TextSingleLine, text);
    }

    // BEGIN <O font ile (Arapca, Japonca vb diller icin) ornek metin yazimi>
    /*-*/
    /*if (writingSystem != QFontDatabase::Any)
        system = writingSystem;

    if (system != QFontDatabase::Any) {
        int w = painter->fontMetrics().width(text + QLatin1String("  "));
        painter->setFont(font2);
        QString sample = QFontDatabase().writingSystemSample(system);
        if (option.direction == Qt::RightToLeft)
            itemRect.setRight(itemRect.right() - w);
        else
            itemRect.setLeft(itemRect.left() + w);
        painter->drawText(itemRect, Qt::AlignVCenter|Qt::AlignLeading|Qt::TextSingleLine, sample);
    }*/
    // END

    painter->setFont(old);

    if (option.state & QStyle::State_Selected)
        painter->restore();
}

QSize QFontItemDelegate::sizeHint(const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const
{
    QString text = index.data(Qt::DisplayRole).toString();
    QFont font(option.font);
//     font.setFamily(text);
    font.setPointSize(QFontInfo(font).pointSize() * 3/2);
    QFontMetrics fontMetrics(font);
    return QSize(fontMetrics.width(text), fontMetrics.height());
}
