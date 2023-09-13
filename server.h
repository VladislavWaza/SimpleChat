#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    ~Server();

signals:
    void debugMsg(QString msg);

private:
    QTcpSocket *_socket;
    QVector <QTcpSocket*> _sockets;
    QByteArray _data;

    void SendToClient(QString msg);

public slots:
    void incomingConnection(qintptr socketDescriptor) override; // обработчик подлючений
    void slotReadyRead();
    void slotDisconnected();
    void slotListen();

};

#endif // SERVER_H
