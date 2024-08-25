#ifndef WINDOWTITLEBAR_H
#define WINDOWTITLEBAR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QIconButton;
QT_END_NAMESPACE

namespace YPY {
    const int TITLEBAR_HEIGHT = 27;

    class WindowTitleBar : public QWidget
    {
        Q_OBJECT
    public:
        explicit WindowTitleBar(QWidget *parent = nullptr);

    protected:

    private:
        QIconButton *maximizeButton;

    signals:

    public slots:
        void onMaximizeClicked();

    };

};

#endif // WINDOWTITLEBAR_H
