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

    bool connectToServer();
    bool disconnectFromServer();
    void sendToServer(const QString &msg);

signals:
    void readyRead(QString& msg);
    void debugMsg(QString msg);

public slots:
    void slotReadyRead();

private:
    QTcpSocket *_socket;
    QByteArray _data;
};

#endif // CLIENT_H
