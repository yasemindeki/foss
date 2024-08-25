#ifndef QLINEFRAME_H
#define QLINEFRAME_H

#include <QFrame>

QT_BEGIN_NAMESPACE
class QLabel;
class QLoginWidget;
QT_END_NAMESPACE

class QLineFrame : public QFrame
{
    Q_OBJECT
public:
    explicit QLineFrame(QWidget *parent, int type = OnlyLine);
    explicit QLineFrame(QWidget *parent, const QString &color, int type = OnlyLine);
    ~QLineFrame();

    enum FrameType {
      OnlyLine = 0,
      WithLabel = 1
    };

public slots:
    void setLabelPos();
    void setEnabled(bool);  // override

private:
    QLabel *orLabel = nullptr;
    FrameType type;

    void initialize(QWidget *parent);

signals:

};

#endif // QLINEFRAME_H
