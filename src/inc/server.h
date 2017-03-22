#include <stdlib.h>
#include "msg.h"

#ifndef SERVER_H
#define SERVER_H

#define STD_FD 3

class ChatServer
{
public:
    void show_usage();
    int run();

private:
    int listener;
    Message clientMsg;
    fd_set master;
    int fd_max;

    int runserver();
    int selectloop();
};

#endif
