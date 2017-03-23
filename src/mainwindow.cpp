/*------------------------------------------------------------------------------
-- SOURCE FILE: mainwindow.cpp- Main Window Class
--
-- PROGRAM: LinuxChat
--
-- FUNCTIONS:
-- explicit MainWindow(QWidget *parent = 0);
-- ~MainWindow();
-- inline const Ui::MainWindow* getUI() { return ui; }
-- void SetClient(Client* client);
--
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- 1.0 Mar.19.17 - Created Class
--
-- DESIGNER: Jamie Lee
--
-- PROGRAMMER: Jamie Lee
--
-- NOTES:
-- This class represents the main window for the client program.
------------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

/*------------------------------------------------------------------------------
-- FUNCTION: MainWindow
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- Version, Date and Description
--
-- DESIGNER: Jamie Lee
--
-- PROGRAMMER: Jamie Lee
--
-- INTERFACE: MainWindow::MainWindow(QWidget *parent)
--
--
-- NOTES:
-- The constructor. It sets up the UI and binds the UI components events
-- and this class's handlers.
--------------------------------------------------------------------------*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->lineEdit_msg, SIGNAL(returnPressed()), this, SLOT(EnterMsgHandler()));
    connect(ui->connectBtn, SIGNAL(clicked(bool)), this, SLOT(ClickConnectBtn()));
    connect(ui->disconnectBtn, SIGNAL(clicked(bool)), this, SLOT(ClickDisconnectBtn()));
    connect(ui->listWidget->model(), SIGNAL(rowsInserted(QModelIndex,int,int)), ui->listWidget, SLOT(scrollToBottom()));
}

/*------------------------------------------------------------------------------
-- FUNCTION: SetClient
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- Version, Date and Description
--
-- DESIGNER: Jamie Lee
--
-- PROGRAMMER: Jamie Lee
--
-- INTERFACE: void MainWindow::SetClient(Client *client)
--
-- RETURNS:
-- void
--
-- NOTES:
-- This holds the client object.
--------------------------------------------------------------------------*/
void MainWindow::SetClient(Client *client)
{
    this->client = client;
}

/*------------------------------------------------------------------------------
-- FUNCTION: EnterMsgHandler
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- Version, Date and Description
--
-- DESIGNER: Jamie Lee
--
-- PROGRAMMER: Jamie Lee
--
-- INTERFACE: void MainWindow::EnterMsgHandler()
--
-- RETURNS:
-- void
--
-- NOTES:
-- This function is called when a user enters a message.
-- It requests the client to send a message to the client.
--------------------------------------------------------------------------*/
void MainWindow::EnterMsgHandler()
{
    QString msg = ui->lineEdit_msg->text();
    if (msg.length() == 0)
        return;
    QString username = ui->lineEdit_username->text();
    if (username.length() == 0)
        return;
    ui->listWidget->addItem("Me:\n" + msg + "\n\n");
    client->SendMsg(msg, username);
    ui->lineEdit_msg->setText("");
    //ui->listWidget->scrollToBottom();
}

/*------------------------------------------------------------------------------
-- FUNCTION: ClickConnectBtn
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- Version, Date and Description
--
-- DESIGNER: Jamie Lee
--
-- PROGRAMMER: Jamie Lee
--
-- INTERFACE: void MainWindow::ClickConnectBtn()
--
-- RETURNS:
-- void
--
-- NOTES:
-- This function is called when a user clicks the connect button.
-- It requests the client to connect to the server.
--------------------------------------------------------------------------*/
void MainWindow::ClickConnectBtn()
{
    QString serverIP = ui->lineEdit_ip->text();
    int port = ui->lineEdit_port->text().toInt();

    client->Connect(serverIP, port);
}

/*------------------------------------------------------------------------------
-- FUNCTION: ClickDisconnectBtn
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- Version, Date and Description
--
-- DESIGNER: Jamie Lee
--
-- PROGRAMMER: Jamie Lee
--
-- INTERFACE: void MainWindow::ClickDisconnectBtn()
--
-- RETURNS:
-- void
--
-- NOTES:
-- This function is called when a user clicks the disconnect button.
-- It requests the client to disconnect to the server.
--------------------------------------------------------------------------*/
void MainWindow::ClickDisconnectBtn()
{
    client->Disconnect();
}

/*------------------------------------------------------------------------------
-- FUNCTION: ClickDisconnectBtn
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- Version, Date and Description
--
-- DESIGNER: Jamie Lee
--
-- PROGRAMMER: Jamie Lee
--
-- INTERFACE: MainWindow::~MainWindow()
--
-- RETURNS:
-- void
--
-- NOTES:
-- The destructor.
--------------------------------------------------------------------------*/
MainWindow::~MainWindow()
{
    client->terminated = true;
    client->connected = false;
    QString history = "";
    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        history += item->text();
        history += "\n";
    }

    std::ofstream logFile("log.txt");
    logFile << history.toStdString();;
    logFile.close();
    delete ui;
}
