#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _server = nullptr;
    _client = nullptr;

    //запустить несколько серверов и посмотреть что будет, новый просто не открывается?
    //сообщение у клинета при отрубе сервера и отруб
}

MainWindow::~MainWindow()
{
    delete ui;
    if (_server)
        delete _server;
    if (_client)
    {
        _client->disconnectFromServer();
        delete _client;
    }
}


void MainWindow::on_server_clicked()
{
    if (_server)
    {
        disconnect(_server, &Server::debugMsg, this, &MainWindow::slotServerDebug);
        delete _server;
        _server = nullptr;
        slotServerDebug("Server end");
        ui->port->setEnabled(true);
        ui->server->setText("Open the server");
    }
    else
    {
        int port = ui->port->text().toInt();
        if ((port < 1024) || (port > 65535))
            slotServerDebug("Invalid port");
        else //если порт корректный
        {
            _server = new Server;
            connect(_server, &Server::debugMsg, this, &MainWindow::slotServerDebug);
            if(_server->listen(QHostAddress::Any, port)) //если сервер слушает
            {
                slotServerDebug("Server start");
                ui->port->setEnabled(false);
                ui->server->setText("Close the server");
            }
            else
            {
                slotServerDebug("Listen error: " + _server->errorString());
                disconnect(_server, &Server::debugMsg, this, &MainWindow::slotServerDebug);
                delete _server;
                _server = nullptr;
            }
        }
    }
}


void MainWindow::on_connect_clicked()
{
    if (_client)
    {
        if (_client->disconnectFromServer())
        {
            _client = nullptr; //само удаление _client происходит после отключения c помощью deleteLater
            ui->connect->setText("Connect to the server");
            ui->name->setEnabled(true);
            ui->hostAddress->setEnabled(true);
            ui->portClient->setEnabled(true);
            ui->lineEdit->setEnabled(false);
            ui->send->setEnabled(false);
        }
    }
    else
    {
        _client = new Client;
        connect(_client, &Client::debugMsg, this, &MainWindow::slotClientDebug);
        connect(_client, &Client::readyRead, this, &MainWindow::slotReadyRead);
        connect(_client, &Client::disconnected, this, &MainWindow::slotClientDisconnected);
        //пытаемся подключиться
        if (_client->connectToServer(ui->hostAddress->text(), ui->portClient->text().toInt()))
        {
            _clientName = ui->name->text();
            ui->name->setEnabled(false);
            ui->hostAddress->setEnabled(false);
            ui->portClient->setEnabled(false);
            ui->connect->setText("Disconnect from the server");
            ui->lineEdit->setEnabled(true);
            ui->send->setEnabled(true);
        }
        else //если не получилось, то удаляем клиента
        {
            disconnect(_client, &Client::debugMsg, this, &MainWindow::slotClientDebug);
            disconnect(_client, &Client::readyRead, this, &MainWindow::slotReadyRead);
            disconnect(_client, &Client::disconnected, this, &MainWindow::slotClientDisconnected);
            delete _client;
            _client = nullptr;
        }
    }
}


void MainWindow::on_send_clicked()
{
    _client->sendToServer(_clientName + ": " + ui->lineEdit->text());
    ui->lineEdit->clear();
}


void MainWindow::on_lineEdit_returnPressed()
{
    _client->sendToServer(_clientName + ": " + ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::slotReadyRead(QString &msg)
{
    ui->textBrowser->append(msg);
}

void MainWindow::slotClientDebug(QString msg)
{
    ui->label_connect->setText(msg);
}

void MainWindow::slotClientDisconnected()
{
    _client = nullptr; //само удаление _client происходит после отключения c помощью deleteLater
    ui->connect->setText("Connect to the server");
    ui->name->setEnabled(true);
    ui->hostAddress->setEnabled(true);
    ui->portClient->setEnabled(true);
    ui->lineEdit->setEnabled(false);
    ui->send->setEnabled(false);
}

void MainWindow::slotServerDebug(QString msg)
{
    ui->label_server->setText(msg);
}

