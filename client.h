#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>

class Client : public QObject
{
    Q_OBJECT
public:
    Client();
    ~Client();

    void connectToServer();
    void disconnectFromServer();
    void sendToServer(const QString &msg);

signals:
    void readyRead(QString& msg);

public slots:
    void slotReadyRead();
    void slotErrorOccurred(QAbstractSocket::SocketError socketError);
    void slotHostFound();

private:
    QTcpSocket *_socket;
    QByteArray _data;
};

#endif // CLIENT_H
