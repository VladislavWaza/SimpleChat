#include "server.h"

Server::Server()
{
}

Server::~Server()
{
    while (!_sockets.isEmpty())
    {
        //при отключении клиента срабатывает слот slotDisconnected, который удаляет его из массива
        _sockets[0]->disconnectFromHost();
    }
    deleteLater();
}

void Server::slotDisconnected()
{
    _socket = reinterpret_cast<QTcpSocket*>(sender());
    _sockets.removeOne(_socket);
    emit debugMsg("Client disconnected, clients now: " + QString::number(_sockets.size()));
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    _socket = new QTcpSocket;
    _socket->setSocketDescriptor(socketDescriptor);
    connect(_socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead); //объединям сигнал readyRead с соотвествующим слотом
    connect(_socket, &QTcpSocket::disconnected, this, &Server::slotDisconnected); //объединям сигналы disconnected с функцией deleteLater

    _sockets.push_back(_socket);
    emit debugMsg("Client " + QString::number(socketDescriptor) + " connected, clients now: " + QString::number(_sockets.size()));
}

void Server::slotReadyRead()
{
    _socket = reinterpret_cast<QTcpSocket*>(sender()); //записываем в сокет отправителя
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
            emit debugMsg(str);
            SendToClient(str);
        }
    }
    else
    {
        emit debugMsg("QDataStream error when reading");
    }
}

void Server::SendToClient(QString msg)
{
    _data.clear();
    QDataStream out(&_data, QIODevice::WriteOnly);//режим работы только для записи
    out.setVersion(QDataStream::Qt_6_4);
    if(out.status() == QDataStream::Ok)
    {
        out << msg;
        for (int i = 0; i < _sockets.size(); ++i)
            _sockets[i]->write(_data);
    }
    else
    {
        emit debugMsg("QDataStream error when sending");
    }
}
