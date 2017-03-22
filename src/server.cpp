#define STD_FD 3

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

int main()
{
    ChatServer server;
    server.run();
    return 0;
}

int ChatServer::run()
{
    runserver();
    return 0;
}

int ChatServer::runserver()
{
    sockaddr_in server;
    listener = makeSocket();

    memset(&server, 0 , sizeof(sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    bindSocket(listener, server);
    cout << "Socket Bound";

    listenSock(listener);

    FD_SET(listener, &master);
    fd_max = listener;

    selectloop();
    return 0;
}

int ChatServer::selectloop()
{
    int nBytes, acptSock;
    fd_set read_fds;
    while(true)
    {
        read_fds = master;

        selectSock(fd_max+1, &read_fds);
        for (int i = 0; i < fd_max; ++i)
        {
            if(FD_ISSET(i, &read_fds) )
            {

                if(i == listener)
                {
                    acptSock = acceptSock(listener);
                    FD_SET(acptSock, &master);
                    fd_max = MAX(fd_max, acptSock);
                    cout << "\nNew connection made." << endl;
                }
                else
                {
                    nBytes = rcv(listener, clientMsg, BUFFSIZE);
                    if(nBytes == 0)
                    {
                        close(i);
                        FD_CLR(i, &master); // remove from set
                    }
                    else if (nBytes > 0) // send to everyone else
                    {
                        for(int j = STD_FD; j <= fd_max; j++)
                        {
                            if(FD_ISSET(j, &master))
                            {
                                if( j != listener && j != i)
                                {
                                    snd( j, clientMsg, nBytes);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
