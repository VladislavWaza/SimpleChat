#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _server = nullptr;
    _client = nullptr;

    //запустить несколько серверов и посмотреть что будет
    //сделать вывод сообщений не в debug
}

MainWindow::~MainWindow()
{
    delete ui;
    if (_server)
        delete _server;
    if (_client)
    {
        disconnect(_client, &Client::readyRead, this, &MainWindow::slotReadyRead);
        _client->disconnectFromServer();
        delete _client;
    }
}


void MainWindow::on_server_clicked()
{
    if (_server)
    {
        delete _server;
        _server = nullptr;
        ui->server->setText("Открыть сервер");
    }
    else
    {
        _server = new Server;
        ui->server->setText("Закрыть сервер");
    }
}


void MainWindow::on_connect_clicked()
{
    if (_client)
    {
        disconnect(_client, &Client::readyRead, this, &MainWindow::slotReadyRead);
        _client->disconnectFromServer();
        delete _client;
        _client = nullptr;
        ui->connect->setText("Подключиться к серверу");
        ui->name->setEnabled(true);
        ui->lineEdit->setEnabled(false);
        ui->send->setEnabled(false);
    }
    else
    {
        _client = new Client;
        connect(_client, &Client::readyRead, this, &MainWindow::slotReadyRead);
        _clientName = ui->name->text();
        ui->name->setEnabled(false);
        _client->connectToServer();
        ui->connect->setText("Отключиться от сервера");
        ui->lineEdit->setEnabled(true);
        ui->send->setEnabled(true);
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

