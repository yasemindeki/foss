#ifndef QPOSTEDITBLOCK_H
#define QPOSTEDITBLOCK_H

#include <QFrame>
#include "subwidgets/qposteditor.h"

QT_BEGIN_NAMESPACE
class QPostEdit;
class QToolBar;
class QColorDialog;
class QTextCharFormat;
class QComboBox;
QT_END_NAMESPACE

class QPostEditBlock : public QFrame
{
    Q_OBJECT

public:
    explicit QPostEditBlock(QWidget *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QPostEdit   *textEdit;
    QToolBar    *tbEditBlock;
    QAction     *actionTextBold;
    QAction     *actionTextItalic;
    QAction     *actionTextUnderline;
    QAction     *actionTextColor;
    QAction     *actionInlineImage;

    QComboBox   *fontSelector;
    QComboBox   *fontSizeSelector;

    int         vmargin;

    void addToolButtons();
    bool hasToolButtonFocus();

    void setColorOnSelection(const QColor &colorPicked);
    void clearFormatActions();
    QIcon createTextColorIcon(const QColor &color);

    friend class QPostEditor;

protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void resizeEvent(QResizeEvent *) override;

public slots:
    void onTextBoldToggled();
    void onTextItalicToggled(bool checked);
    void onTextUnderlineToggled(bool checked);
    void onTextColorClicked();
    void onCursorPositionChanged();
    void onCurrentCharFormatChanged(const QTextCharFormat &);
    void onInlineImageClicked();

signals:

};

#endif // QPOSTEDITBLOCK_H
