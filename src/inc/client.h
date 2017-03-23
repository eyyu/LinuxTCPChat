/*------------------------------------------------------------------------------
-- SOURCE FILE: client.h - Client Class
--
-- PROGRAM: LinuxChat
--
-- FUNCTIONS:
-- Client(const Ui::MainWindow* ui);
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
-- It connects to the server with the server ip and port that user entered.
------------------------------------------------------------------------------*/
#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

namespace Ui {
class MainWindow;
}

class Client
{
public:
    Client(const Ui::MainWindow* ui);
    void Run(); // create receive thread.
    void RecvFunc(); // keeps reading messages.
    void SendMsg(QString& msg, QString& username); // send a message to the server.
    void Connect(QString& serverIP, int port); // connect to the server.
    void Disconnect(); // disconnect to the server.

    bool terminated;
    bool connected;

private:
    QString Receive(); // receive a message.

    const Ui::MainWindow* ui;
    int _socket;
};

#endif // CLIENT_H
