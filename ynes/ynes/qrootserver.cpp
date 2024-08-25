#include "qrootserver.h"

#include <QDebug>
#include <QSettings>
#include <QTcpSocket>
#include <QDataStream>

#define CONFIG_FILE_NAME    "ynes.ini"

QRootServer::QRootServer(QObject *parent) : QTcpServer(parent)
{
    QSettings configMap(CONFIG_FILE_NAME, QSettings::IniFormat);
    int maxConnections  = configMap.value("Root/maxConnections", 0).toInt();
    mListeningPort      = configMap.value("Root/port", 0).toInt();
    mClientSocket       = NULL;
    mLastError          = RSE_NONE;

    this->setMaxPendingConnections(maxConnections);

    if (maxConnections == 0 || mListeningPort == 0)
        mLastError = RSE_INIPARSE;
}

int QRootServer::start()
{
    switch (mLastError)
    {
    case RSE_NONE:
        connect(this,       SIGNAL(newConnection()),
                this,       SLOT  (onNewConnectionAvailable()));

        if (this->listen(QHostAddress("127.0.0.1"), mListeningPort))
        {
            qDebug() <<"ynes is listening on port " << mListeningPort;
        }
        else
        {
            qDebug() << "ynes could not be started because of the internal error";
            mLastError = RSE_INTERNAL;
        }

        break;
    case RSE_INIPARSE:
        qDebug() << "ynes could not be started because of incompatible config file.";
        break;
    default:
        break;
    }


    return mLastError;
}

QRootServer::~QRootServer()
{
    if (mClientSocket)
    {
        mClientSocket->disconnectFromHost();
        if (mClientSocket->waitForDisconnected())
            qDebug() << "Disconnected from nodejs application.";
        else
            qDebug() << "Failed the disconnection from nodejs application. " << this->errorString();
    }

    this->close();
}

void QRootServer::onNewConnectionAvailable()
{
    if (mClientSocket) /*< nodejs ile aktif tek baglantiya izin veriyoruz. */
    {
        QTcpSocket *unexpectedSocket = this->nextPendingConnection();
        unexpectedSocket->abort();

        qDebug() << "Error: unexpected incoming connection!";
    }
    else
    {
        mClientSocket = this->nextPendingConnection();

        if (!mClientSocket)
        {
            qDebug() << "Error: attempt to get invalid pending connection!";
        }
        else
        {
            mClientSocket->setSocketOption(QAbstractSocket::KeepAliveOption, true);

            connect(mClientSocket,  SIGNAL(readyRead()),
                    this,           SLOT(onNewDataAvailable()));
            connect(mClientSocket,  SIGNAL(disconnected()),
                    this,           SLOT(onConnectionLost()));
            connect(mClientSocket,  SIGNAL(error(QAbstractSocket::SocketError)),
                    this,           SLOT(onConnectionError(QAbstractSocket::SocketError)));

            qDebug() << "nodejs has connected to the root server.";
        }
    }
}

void QRootServer::onConnectionLost()
{
    qDebug() << "nodejs application has disconnected.";

    mClientSocket->deleteLater();
    mClientSocket = NULL;
}

void QRootServer::onConnectionError(QAbstractSocket::SocketError socketError)
{
    qDebug() << QString("nodejs application has been failed with error code '%1'. Message: %2 !").
                arg(socketError).
                arg(mClientSocket->errorString());

}

void QRootServer::onNewDataAvailable()
{
    QDataStream rxDataStream(mClientSocket);
    quint8      rxHeader;
    quint16     rxDataLength;
    quint8      rxCommandNo;
    QString     rxStringData;
    quint64     rxUInt64;
    qint64      ba; /* ba: The inital letters of 'bytes available' */

    rxDataStream.setByteOrder(QDataStream::LittleEndian);
    rxDataStream.setVersion(QDataStream::Qt_5_10);

    // ADIM 1. Paket basligini alalim.
    rxDataStream >> rxHeader;
    // ADIM 2. Veri uzunlugu bilgisini alalim.
    rxDataStream >> rxDataLength;
    // ADIM 3. Paketin protokole uygunlugunun kontrolu
    if (rxHeader != Protocol::TCP_HEADER)
    {
        qDebug() << tr("Invalid TCP packet header. Discarded.");

        mClientSocket->readAll();
        return;
    }

    if (mClientSocket->bytesAvailable() < rxDataLength || rxDataLength == 0)
    {
        qDebug() << tr("Invalid TCP packet data length. Discarded.");

        mClientSocket->readAll();
        return;
    }

    // ADIM 4. Talebi degerlendirelim.
    rxDataStream >> rxCommandNo;

    qDebug() << "bytesAvailable() = " << mClientSocket->bytesAvailable();

    switch(rxCommandNo)
    {
    case Protocol::COMMAND_LOGIN:
        readStringData(rxStringData, rxDataStream, 5);
        readStringData(rxStringData, rxDataStream);
    default:
        break;
    }

}

void QRootServer::readStringData(QString &sData, QDataStream &rxDataStream, int rxCount)
{
    char *rxString = new char[Protocol::MAX_STRING_LENGTH];
    uint strlen = (rxCount == -1 ? mClientSocket->bytesAvailable() : rxCount);

    rxDataStream.readRawData(rxString, strlen);
    rxString[strlen] = '\0';
    sData = QString::fromUtf8(rxString);

    delete[] rxString;
}



// LOC 63
