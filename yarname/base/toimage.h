#ifndef TOIMAGE_H
#define TOIMAGE_H

#include <QImage>

class ToImage : public QObject, public QImage
{
    Q_OBJECT
public:
    explicit ToImage(QObject *parent = nullptr);
    ToImage(const QImage &);
    ToImage(const QString &addr);

    const QString &address() const;

private:
    typedef enum
    {
        DEFAULT = 0,
        HOVER = 1,
        PRESSED = 2
    }ViewType;

    int viewType;
    QString toAddress;

signals:

public slots:
    void onImageClicked(const QString &name, int vt);


};

#endif // TOIMAGE_H
