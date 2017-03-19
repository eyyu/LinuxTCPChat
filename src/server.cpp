#include <stdlib.h>
#include <stdio.h>

#include "inc/server.h"
#include "inc/wrapper.h"

const char * ChatServer::PORT = "10954";

int ChatServer::run()
{
    runserver();
    return 0;
}

int ChatServer::runserver()
{
    listenSock = makeSocket();
    return 0;
}
