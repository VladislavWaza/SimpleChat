#include "client.h"

Client::Client()
{
    _socket = new QTcpSocket(this);
    connect(_socket, &QTcpSocket::readyRead, this, &Client::slotReadyRead);
    connect(_socket, &QTcpSocket::disconnected, this, &Client::deleteLater);
    connect(_socket, &QTcpSocket::errorOccurred, this, &Client::slotErrorOccurred);
    connect(_socket, &QTcpSocket::hostFound, this, &Client::slotHostFound);
}

Client::~Client()
{
    //delete _socket;
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

            /*
            // Если не докачалось, то ждём новую порцию данных.
            if(data.isEmpty() == true) {
                in.rollbackTransaction();
                qDebug() << "Empty error";
                return;
            }
            */

            if (in.commitTransaction() == false)
            {
                qDebug() << "Transaction error";
                return;
            }
            emit readyRead(str);
        }
    }
    else
    {
        qDebug() << "QDataStream error";
    }
}

void Client::slotErrorOccurred(QAbstractSocket::SocketError socketError)
{
    qDebug() << "socketError: " << socketError;
}

void Client::slotHostFound()
{
    qDebug() << "host found";

}

void Client::connectToServer()
{
    _socket->connectToHost("127.0.0.1", 4236); // только комп
}

void Client::disconnectFromServer()
{
    _socket->disconnectFromHost();
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
        qDebug() << "QDataStream error";
    }
}
