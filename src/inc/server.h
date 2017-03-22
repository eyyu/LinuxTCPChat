#include <stdlib.h>

#ifndef SERVER_H
#define SERVER_H
class ChatServer
{
public:
    void show_usage();
    int run();

private:
    const static int BUFFSIZE = 1024;

    int listener;
    char clientMsg[BUFFSIZE];
    fd_set master;
    int fd_max;

    int runserver();
    int selectloop();
};

#endif
