#include "client.h"

Client::Client()
{
    _socket = new QTcpSocket(this);
    connect(_socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
    connect(_socket, &QTcpSocket::disconnected, this, &Client::deleteLater);
}

Client::~Client()
{
    delete _socket;
}

void Client::slotReadyRead()
{
    QDataStream in(_socket);
    in.setVersion(QDataStream::Qt_6_4);
    if(in.status() == QDataStream::Ok)
    {
        while (_socket->bytesAvailable())
        {
            in.startTransaction();
            QString str;
            in >> str;

            if (in.commitTransaction() == false)
            {
                emit debugMsg("Transaction error");
                return;
            }
            emit readyRead(str);
        }
    }
    else
    {
        emit debugMsg("QDataStream error when reading");
    }
}

bool Client::connectToServer()
{
    _socket->connectToHost("127.0.0.1", 4236); // только комп
    if (_socket->waitForConnected(1000))
        emit debugMsg("Connected!");
    else
    {
        emit debugMsg("Error: " + _socket->errorString());
        return false;
    }
    return true;
}

bool Client::disconnectFromServer()
{
    _socket->disconnectFromHost();
    if (_socket->state() == QAbstractSocket::UnconnectedState || _socket->waitForDisconnected(1000))
        emit debugMsg("Disconnected!");
    else
    {
        emit debugMsg("Error: " + _socket->errorString());
        return false;
    }
    return true;
}

void Client::sendToServer(const QString& msg)
{
    _data.clear();
    QDataStream out(&_data, QIODevice::WriteOnly);//режим работы только для записи
    out.setVersion(QDataStream::Qt_6_4);
    if(out.status() == QDataStream::Ok)
    {
        out << msg;
        _socket->write(_data);
    }
    else
    {
        emit debugMsg("QDataStream error when sending");
    }
}
