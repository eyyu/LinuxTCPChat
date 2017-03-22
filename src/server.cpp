#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/param.h>

#include "inc/a3defs.h"
//#include "inc/msg.h"
#include "inc/server.h"
#include "inc/wrapper.h"

using namespace std;
int server();

int main()
{
    server();
    return 0;
}

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

int server()
{
    static const int BUFFSIZE = 1024;
    static const int STD_FD = 3;
    char buffer[BUFFSIZE];
    fd_set master, read_fds;
    int fdmax;
    sockaddr_in server;
    int listener;
    int acptSock;
    int nBytes;
    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    listener = makeSocket();

    memset(&server, 0 , sizeof(sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    bindSocket(listener, server);
    cout << "Socket Bound";

    listenSock(listener);
    FD_SET(listener, &master);
    fdmax = listener;

    while(true)
    {
        read_fds = master;
        selectSock(fdmax+1, &read_fds);
        for (int i = 0; i < fdmax; ++i)
        {
            if(FD_ISSET(i, &read_fds) )
            {

                if(i == listener)
                {
                    acptSock = acceptSock(listener);
                    FD_SET(acptSock, &master);
                    fdmax = MAX(fdmax, acptSock);
                    cout << "\nNew connection made." << endl;
                }
                else
                {
                    nBytes = rcv(listener, buffer, BUFFSIZE);
                    if(nBytes == 0)
                    {
                        close(i);
                        FD_CLR(i, &master); // remove from set
                    }
                    else if (nBytes > 0) // send to everyone else
                    {
                        for(int j = STD_FD; j <= fdmax; j++)
                        {
                            if(FD_ISSET(j, &master))
                            {
                                if( j != listener && j != i)
                                {
                                    snd(j,buffer, nBytes);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
