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
    void EnterMsgHandler();

private:
    Ui::MainWindow *ui;
    Client* client;
};

#endif // MAINWINDOW_H
