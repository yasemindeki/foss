#ifndef QROOTSERVER_H
#define QROOTSERVER_H

#include <QTcpServer>

namespace Protocol {
const quint8 TCP_HEADER        = 0xAB;
const quint8 MAX_STRING_LENGTH = 128;

const quint16 ACK_FAILED            = 0x0000;
const quint16 ACK_UNDEFINED_USER    = 0x0001;
const quint16 ACK_UNAUTHORISED_USER = 0x0002;

const quint8 COMMAND_LOGIN        = 0x01;
const quint8 KEEP_ALIVE           = 0x0D;
}


/**
 * @brief QRootServer TCP server that nodejs components of yarname architecture connects to.
 * @turkish QRootServer yarname mimarisinin nodejs bilesenlerinin baglanti kurdugu TCP sunucu.
 */
class QRootServer : public QTcpServer
{
    Q_OBJECT
public:
    /**
     * @brief QRootServer Constructor function.
     * @param[in] parent NOTUSED.
     * @param[in][out][in,out] param_name param_explanation
     * @return return_explanation
     */
    explicit QRootServer(QObject *parent = nullptr);

    /**
     * @brief ~QRootServer Deconstructor function.
     *
     * QAbstractSocket::disconnectFromHost() attempts to close the socket.
     * If there is pending data waiting to be written,
     * QAbstractSocket will enter ClosingState and wait until all data has been written.
     * Eventually, it will enter UnconnectedState and emit the disconnected() signal.
     * @see QAbstractSocket::disconnectFromHost
     */
    ~QRootServer();

    /**
     * @brief start The function connects the signal newConnection() to our slot and starts to listen TCP connection requests.
     * @return RSE_NONE0 if the server starts with no error, a value with prefix RSE_ if the server fails.
     *
     * @turkish start newConnection sinyalini bizim slota baglayip TCP baglanti taleplerini dinlemeyi baslatan fonksiyon.
     * @turkish return Sunucunun hatasiz baslamasi durumunda RSE_NONE(0), aksi durumda RSE_ onekli baska bir deger geri donuyor.
     */
    int start();

private:
    /**
     * @brief clientSocket The socket variable for incoming nodejs connection.
     * @turkish Gelen nodejs baglantisinin soket degiskeni.
     */
    QTcpSocket *mClientSocket;
    int         mListeningPort;
    int         mLastError;

    enum RootServerError {
        RSE_NONE = 0,
        RSE_INIPARSE,
        RSE_INTERNAL
    };

signals:

public slots:
    /**
     * @brief onNewConnectionAvailable meets incoming connection request to the root server.
     * @turkish Sunucuya gelen baglanti talebini karsilayan fonksiyon.
     *
     * QTcpServer::newConnection signal is emitted every time a new connection is available.
     */
    void onNewConnectionAvailable();

    /**
     * @brief onConnectionLost is called when the nodejs application disconnects from the root server for some reason.
     * @turkish nodejs uygulamasinin bir sebepten baglantisini kesmesi ile cagrilan fonksiyon.
     *
     * QAbstractSocket::disconnected signal is emitted when the socket has been disconnected.
     */
    void onConnectionLost();

    /**
     * @brief onConnectionError is called when an error has occured on nodejs connection.
     * @turkish nodejs baglantisi uzerinde hata olusursa cagrilan fonksiyon.
     * @param socketError describes the type of error that occurred.
     *
     * QAbstractSocket::error signal is emitted after an error occurred.
     */
    void onConnectionError(QAbstractSocket::SocketError socketError);

    /**
     * @brief onNewDataAvailable is called when nodejs application writes to the socket.
     * @turkish nodejs uygulamasi sokete veri yazdiginda cagrilan slot fonksiyon.
     *
     * QAbstractSocket::readyRead signal is emitted once every time new data is available for reading.
     *
     * LittleEndian: byte siralamasi/duzeni ilk yazilani ilk okuyacak sekilde.
     * QDataStream(socket) gecersiz paket ulasir ve beklenenden az byte gelmis ise, >> ile okuma exception uretmiyor.
     * Protokole uymayan istek ulasir ise, paketin geri kalanini degerlendirmeksizin okuyoruz ki bir sonraki paket ile karismasin.
     */
    void onNewDataAvailable();

    void readStringData(QString &sData, QDataStream &rxDataStream, int rxCount = -1);

};

#endif // QROOTSERVER_H
// LOC 62d
