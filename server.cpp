#include "server.h"

Server::Server()
{
    //слушаем входящие сообщения по всем адресам, порт 4236
    if(this->listen(QHostAddress::Any, 4236))
    {
        qDebug() << "start";
    }
    else
    {
        qDebug() << "error";
    }
}

Server::~Server()
{
    qDebug() << "end";
    deleteLater();
}

void Server::slotDisconnected()
{
    _socket = reinterpret_cast<QTcpSocket*>(sender());
    _sockets.removeOne(_socket);
    qDebug() << "client disconnected";
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    _socket = new QTcpSocket;
    _socket->setSocketDescriptor(socketDescriptor);
    connect(_socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead); //объединям сигнал readyRead с соотвествующим слотом
    connect(_socket, &QTcpSocket::disconnected, this, &Server::slotDisconnected); //объединям сигналы disconnected с функцией deleteLater

    _sockets.push_back(_socket);
    qDebug() << "client connected" << socketDescriptor;
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
            qDebug() << str;
            SendToClient(str);
        }
    }
    else
    {
        qDebug() << "QDataStream error";
    }
}

void Server::SendToClient(QString msg)
{
    _data.clear();
    QDataStream out(&_data, QIODevice::WriteOnly);//режим работы только для записи
    out.setVersion(QDataStream::Qt_6_4);
    if(out.status() == QDataStream::Ok)
    {
        qDebug() << "send...";
        out << msg;
        for (int i = 0; i < _sockets.size(); ++i)
            _sockets[i]->write(_data);
    }
    else
    {
        qDebug() << "QDataStream error";
    }
}
