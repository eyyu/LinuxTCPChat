#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(EnterMsgHandler()));
}

void MainWindow::SetClient(Client *client)
{
    this->client = client;
}

void MainWindow::EnterMsgHandler()
{
    QString msg = ui->lineEdit->text();
    QString username = ui->lineEdit_2->text();
    client->SendMsg(msg, username);
}

MainWindow::~MainWindow()
{
    client->connected = false;
    delete ui;
}
