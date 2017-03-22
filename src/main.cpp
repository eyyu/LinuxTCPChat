#include "mainwindow.h"
#include "inc/client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Client client(w.getUI());
    w.SetClient(&client);
    client.Run();

    return a.exec();
}
