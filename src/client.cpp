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

const std::string Client::SERVER_IP = "142.232.153.216";

Client::Client(const Ui::MainWindow* ui)
{
    this->ui = ui;
    connected = false;
    terminated = false;
}

void Client::Run()
{
    /*_socket = makeSocket();

    sockaddr_in server;
    memset(&server, 0 , sizeof(sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(Client::SERVER_IP.c_str());

    char strPort[32];
    sprintf(strPort, "%d", PORT);
    connectSocket(_socket, strPort, SERVER_IP.c_str());
    */

    std::thread recvThread(&Client::RecvFunc, this);
    recvThread.detach();
}

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
}

void Client::Disconnect()
{
    connected = false;
    close(_socket);
}

void Client::RecvFunc()
{
    while (!terminated)
    {
        while (connected)
        {
            QString result = Receive();
            ui->listWidget->addItem(result);
        }
    }
}

void Client::SendMsg(QString& msg, QString& username)
{
    Message sendMsg;
    strncpy(sendMsg.msg, msg.toStdString().c_str(), MAX_MSG_SIZE);
    strncpy(sendMsg.username, username.toStdString().c_str(), MAX_USERNAME_SIZE);
    int bytesToSend = sizeof(Message);
    char sendBuff[bytesToSend];
    memcpy(sendBuff, &sendMsg.header, sizeof(int));
    memcpy(sendBuff + sizeof(int), sendMsg.username, sizeof(MAX_USERNAME_SIZE));
    memcpy(sendBuff + sizeof(int) + sizeof(MAX_USERNAME_SIZE), sendMsg.msg, sizeof(MAX_MSG_SIZE));
    snd(_socket, sendBuff, bytesToSend);
}

QString Client::Receive()
{
    int bytesRead;
    int bytesToRead = sizeof(Message);
    char *message = (char *) malloc(bytesToRead);
    while((bytesRead = rcv(_socket, message, bytesToRead)) < bytesToRead)
    {
        //printf("Recv\n");
        if(bytesRead < 0)
        {
            qDebug() << "recv() failed";
            free(message);
            return "";
        }
        if(bytesRead == 0)
        {
            printf("Server has closed the socket\n");
            free(message);
            return "";
        }
        message += bytesRead;
        bytesToRead -= bytesRead;
    }
    QString returnMessage(message);
    free(message);
    return returnMessage;
}
