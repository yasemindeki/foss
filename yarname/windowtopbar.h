#ifndef WINDOWTOPBAR_H
#define WINDOWTOPBAR_H

#include <QWidget>

namespace YPY {
    const int TOPBAR_HEIGHT = 50;
    const int TOPBAR_TEXTBUTTON_WIDTH = 200;

    class WindowTopBar : public QWidget
    {
        Q_OBJECT
    public:
        explicit WindowTopBar(QWidget *parent = nullptr);

    private:

    signals:

    public slots:

    };
};
#endif // WINDOWTOPBAR_H
