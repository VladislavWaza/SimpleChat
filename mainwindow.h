#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_server_clicked();

    void on_connect_clicked();

    void on_send_clicked();

    void on_lineEdit_returnPressed();

    void slotReadyRead(QString &msg);

private:
    Ui::MainWindow *ui;
    Server *_server;
    Client *_client;
    QString _clientName;
};
#endif // MAINWINDOW_H
