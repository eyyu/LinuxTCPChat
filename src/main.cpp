#include "mainwindow.h"
#include "inc/client.h"
#include <QApplication>

/*------------------------------------------------------------------------------
-- FUNCTION: main
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
-- INTERFACE: int main(int argc, char *argv[])
--
-- RETURNS:
-- int
--
-- NOTES:
-- The main function of a client. It initiates the main window and client.
--------------------------------------------------------------------------*/
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
