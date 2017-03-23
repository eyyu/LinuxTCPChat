/*------------------------------------------------------------------------------
-- SOURCE FILE: client.cpp - Client Class
--
-- PROGRAM: LinuxChat
--
-- FUNCTIONS:
-- void Run();
-- void RecvFunc();
-- void SendMsg(QString& msg, QString& username);
-- void Connect(QString& serverIP, int port);
-- void Disconnect();
-- PRIVATE:
-- QString Receive();
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
-- This client class runs and creates a receive thread that keeps
-- reading messages from the server.
-- It connects the server with the server ip and port that user entered.
------------------------------------------------------------------------------*/

#include <arpa/inet.h>
#include <QObject>
#include <QDebug>
#include <QString>
#include <thread>
#include <stdio.h>
#include <unistd.h>

#include "inc/a3defs.h"
#include "inc/msg.h"
#include "inc/client.h"
#include "inc/wrapper.h"
#include "ui_mainwindow.h"

/*------------------------------------------------------------------------------
-- FUNCTION: Client
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
-- INTERFACE: Client::Client(const Ui::MainWindow* ui)
--
--
-- NOTES:
-- The constructor.
--------------------------------------------------------------------------*/
Client::Client(const Ui::MainWindow* ui)
{
    _socket = -1;
    this->ui = ui;
    connected = false;
    terminated = false;

    ui->connectBtn->setEnabled(true);
    ui->lineEdit_ip->setEnabled(true);
    ui->lineEdit_port->setEnabled(true);

    ui->disconnectBtn->setEnabled(false);
    ui->lineEdit_username->setEnabled(false);
    ui->lineEdit_msg->setEnabled(false);
}

/*------------------------------------------------------------------------------
-- FUNCTION: Run
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
-- INTERFACE: void Client::Run()
--
-- RETURNS:
-- void
--
-- NOTES:
-- This creates a receiving thread.
--------------------------------------------------------------------------*/
void Client::Run()
{
    std::thread recvThread(&Client::RecvFunc, this);
    recvThread.detach();
}

/*------------------------------------------------------------------------------
-- FUNCTION: Connect
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
-- INTERFACE: void Client::Connect(QString &serverIP, int port)
--
-- RETURNS:
-- void
--
-- NOTES:
-- This connects to the server with
-- the server ip and port that user entered.
--------------------------------------------------------------------------*/
void Client::Connect(QString &serverIP, int port)
{
    struct hostent* hp;
    struct sockaddr_in server;
    char** pptr;
    char str[16];

    // Create the socket
    if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Cannot create socket");
        exit(1);
    }
    bzero((char *)&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if ((hp = gethostbyname(serverIP.toStdString().c_str())) == NULL)
    {
        fprintf(stderr, "Unknown server address\n");
        exit(1);
    }
    bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

    // Connecting to the server
    if (connect(_socket, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        fprintf(stderr, "Can't connect to server\n");
        perror("connect");
        exit(1);
    }
    printf("Connected:    Server Name: %s\n", hp->h_name);
    pptr = hp->h_addr_list;
    printf("\t\tIP Address: %s\n", inet_ntop(hp->h_addrtype, *pptr, str, sizeof(str)));

    connected = true;

    ui->listWidget->addItem("Connected!");

    ui->connectBtn->setEnabled(false);
    ui->lineEdit_ip->setEnabled(false);
    ui->lineEdit_port->setEnabled(false);

    ui->disconnectBtn->setEnabled(true);
    ui->lineEdit_username->setEnabled(true);
    ui->lineEdit_msg->setEnabled(true);
}


/*------------------------------------------------------------------------------
-- FUNCTION: Disconnect
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
-- INTERFACE: void Client::Disconnect()
--
-- RETURNS:
-- void
--
-- NOTES:
-- This closes the socket to disconnect to the server.
--------------------------------------------------------------------------*/
void Client::Disconnect()
{
    connected = false;

    close(_socket);

    ui->listWidget->addItem("Disconnected");

    ui->connectBtn->setEnabled(true);
    ui->lineEdit_ip->setEnabled(true);
    ui->lineEdit_port->setEnabled(true);

    ui->disconnectBtn->setEnabled(false);
    ui->lineEdit_username->setEnabled(false);
    ui->lineEdit_msg->setEnabled(false);
}

/*------------------------------------------------------------------------------
-- FUNCTION: RecvFunc
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
-- INTERFACE: void Client::RecvFunc()
--
-- RETURNS:
-- void
--
-- NOTES:
-- This is for a receive thread, keeps reading a message from the server.
--------------------------------------------------------------------------*/
void Client::RecvFunc()
{
    while (!terminated)
    {
        while (connected)
        {
            QString result = Receive();
            ui->listWidget->addItem(result);
            //ui->listWidget->scrollToBottom();
        }
    }
}

/*------------------------------------------------------------------------------
-- FUNCTION: SendMsg
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
-- INTERFACE: void Client::SendMsg(QString& msg, QString& username)
--
-- RETURNS:
-- void
--
-- NOTES:
-- This sends a message to the server.
--------------------------------------------------------------------------*/
void Client::SendMsg(QString& msg, QString& username)
{
    Message sendMsg;
    strncpy(sendMsg.msg, msg.toStdString().c_str(), MAX_MSG_SIZE);
    strncpy(sendMsg.username, username.toStdString().c_str(), MAX_USERNAME_SIZE);
    int bytesToSend = sizeof(sendMsg);
    snd(_socket, reinterpret_cast<char *>(&sendMsg), bytesToSend);
}

/*------------------------------------------------------------------------------
-- FUNCTION: Receive
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
-- INTERFACE: QString Client::Receive()
--
-- RETURNS:
-- QString
--
-- NOTES:
-- This obtains a message from the server.
--------------------------------------------------------------------------*/
QString Client::Receive()
{
    int bytesRead, bytesToRead = MSG_SIZE;
    Message recvmsg;
    char * p =  reinterpret_cast<char *>(&recvmsg);
    while((bytesRead = rcv(_socket, p, MSG_SIZE)) < bytesToRead)
    {
        //printf("Recv\n");
        if(bytesRead < 0)
        {
            qDebug() << "recv() failed";
            return "";
        }
        if(bytesRead == 0)
        {
            printf("Server has closed the socket\n");
            return "";
        }
        p += bytesRead;
        bytesToRead -= bytesRead;
    }
    QString returnMessage(recvmsg.username);
    returnMessage.append(":\n");
    returnMessage.append(recvmsg.msg);
    returnMessage.append("\n\n");
    return returnMessage;
}
