#ifndef CLIENT_H
#define CLIENT_H

#include <QString>

namespace Ui {
class MainWindow;
}

class Client
{
public:
    static const std::string SERVER_IP;

    Client(const Ui::MainWindow* ui);
    void Run();
    void RecvFunc();
    void SendMsg(QString& msg, QString& username);

    bool connected;

private:
    QString Receive();

    const Ui::MainWindow* ui;
    int _socket;
};

#endif // CLIENT_H
