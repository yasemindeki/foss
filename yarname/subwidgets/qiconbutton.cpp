#include "qiconbutton.h"

#include <QtWidgets>

QIconButton::QIconButton(const QString &tooltip, QWidget *parent) : QPushButton(parent)
{
    this->setToolTip(tooltip);
    this->setCursor(Qt::PointingHandCursor);
}

void QIconButton::setIcons(QString filePath, QString hoverFilePath)
{
    QString qss = QString("\
            QPushButton { \
              background-color: transparent; \
              background-image: url(%1); \
              background-repeat: none;    \
              border: none; } \
            QPushButton:hover { \
              background-color: transparent; \
              background-repeat: none;    \
              background-image: url(%2); \
              border: none; }")
                          .arg(filePath)
                          .arg(hoverFilePath);

    this->setStyleSheet(qss);

}



