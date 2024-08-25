#include <QCoreApplication>
#include <QDebug>

#include "qrootserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QRootServer *rootServer = new QRootServer();
    if (rootServer->start())
        return 0;

    return a.exec();
}
