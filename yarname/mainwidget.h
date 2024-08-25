#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QTabWidget>

namespace YPY {
    class MainWidget : public QTabWidget
    {
        Q_OBJECT
    public:
        explicit MainWidget(QWidget *parent = nullptr);

    signals:

    };
};
#endif // MAINWIDGET_H
