/*------------------------------------------------------------------------------
-- SOURCE FILE: mainwindow.h- Main Window Class
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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "inc/client.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    inline const Ui::MainWindow* getUI() { return ui; }
    void SetClient(Client* client);

private slots:
    void EnterMsgHandler(); // when a user enters a message.
    void ClickConnectBtn(); // when a user clicks the connect button.
    void ClickDisconnectBtn(); // when a user clicks the disconnect button.

private:
    Ui::MainWindow *ui;
    Client* client;
};

#endif // MAINWINDOW_H
