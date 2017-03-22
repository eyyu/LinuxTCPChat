#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->lineEdit_msg, SIGNAL(returnPressed()), this, SLOT(EnterMsgHandler()));
    connect(ui->connectBtn, SIGNAL(clicked(bool)), this, SLOT(ClickConnectBtn()));
    connect(ui->disconnectBtn, SIGNAL(clicked(bool)), this, SLOT(ClickDisconnectBtn()));
}

void MainWindow::SetClient(Client *client)
{
    this->client = client;
}

void MainWindow::EnterMsgHandler()
{
    QString msg = ui->lineEdit_msg->text();
    QString username = ui->lineEdit_username->text();
    client->SendMsg(msg, username);
}

void MainWindow::ClickConnectBtn()
{
    QString serverIP = ui->lineEdit_ip->text();
    int port = ui->lineEdit_port->text().toInt();

    client->Connect(serverIP, port);
}

void MainWindow::ClickDisconnectBtn()
{

}

MainWindow::~MainWindow()
{
    client->terminated = true;
    client->connected = false;
    delete ui;
}
