#ifndef QICONBUTTON_H
#define QICONBUTTON_H

#include <QPushButton>

class QIconButton : public QPushButton
{
    Q_OBJECT
public:
    QIconButton(const QString &tooltip, QWidget *parent = nullptr);

    void setIcons(QString filePath, QString hoverFilePath);

protected:

private:
};

#endif // QICONBUTTON_H
