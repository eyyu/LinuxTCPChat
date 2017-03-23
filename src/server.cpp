/*------------------------------------------------------------------------------
-- SOURCE FILE: server.cpp - Server Class
--
-- PROGRAM: LinuxChat
--
-- FUNCTIONS:
-- list all functions here!
-- returnType funcName (funcParams)
--
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- 1.0 Mar.18.17 - Created Class
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- NOTES:
-- This is the server object and the main function sits at the beginning
-- of the file.
-- The main function simply excutes a function called "run" that will start up
-- the server on port 14396
------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/param.h>

#include "inc/a3defs.h"
#include "inc/msg.h"
#include "inc/server.h"
#include "inc/wrapper.h"

using namespace std;

/*------------------------------------------------------------------------------
-- FUNCTION: Main
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- Version, Date and Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int main ()
--
-- RETURNS:
-- int to represent state ( not zero means error )
--
-- NOTES:
-- runs the server program
--------------------------------------------------------------------------*/
int main()
{
    ChatServer server;
    server.run();
    return 0;
}

/*------------------------------------------------------------------------------
-- FUNCTION: run
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- Version, Date and Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int ChatServer::run ()
--
-- RETURNS:
-- int representing state ( not zero means error )
--
-- NOTES:
-- this runs the private run functions for encapsulation purposes
--------------------------------------------------------------------------*/
int ChatServer::run()
{
    runserver();
    return 0;
}

/*------------------------------------------------------------------------------
-- FUNCTION: runserver
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- Version, Date and Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int ChatServer::runserver ()
--
-- RETURNS:
-- int representing state ( not zero means error )
--
-- NOTES:
-- server makes socket, binds and listens to connect ( all wrapper functions)
-- found in wrapper.cpp
-- server then goes into the select loop
--------------------------------------------------------------------------*/
int ChatServer::runserver()
{
    sockaddr_in server;
    listener = makeSocket();

    memset(&server, 0 , sizeof(sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    bindSocket(listener, server);

    listenSock(listener);

    FD_SET(listener, &master);
    fd_max = listener;

    selectloop();
    return 0;
}

/*------------------------------------------------------------------------------
-- FUNCTION: selectloop
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- Version, Date and Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int ChatServer::selectloop ()
--
-- RETURNS:
-- int representing state ( not zero means error )
--
-- NOTES:
-- selectloop is where server will accept sockets & read and send messages
-- to clients
--------------------------------------------------------------------------*/
int ChatServer::selectloop()
{
    int nBytes, acptSock;
    fd_set read_fds;
    while(true)
    {
        read_fds = master; // master is a private data member
        selectSock(fd_max+1, &read_fds); // wrapper function

        for (int i = 0; i <= fd_max; ++i)
        {
            if( FD_ISSET(i, &read_fds) ) // check for the one that is set
            {

                if(i == listener) // if we got a listener notification
                {
                    acptSock = acceptSock(listener);
                    FD_SET(acptSock, &master);
                    fd_max = MAX(fd_max, acptSock);
                    cout << "\nNew connection made to socket: " << acptSock << endl;
                }
                else // otheriwse must be a socket to rcv from
                {
                    memset(&clientMsg, 0, MSG_SIZE);
                    nBytes = rcv(i, (char *)&clientMsg, MSG_SIZE);
                    if(nBytes == 0)
                    {
                       cout << "\nClient disconnected from socket: " << i << endl;
                        close(i);
                        FD_CLR(i, &master); // remove from set
                    }
                    else if (nBytes > 0) // send to everyone else
                    {

                        cout << "\nMessage recieved from: " << clientMsg.username << endl;
                        cout << "\n\tMessage: " << clientMsg.msg << endl;

                        for(int j = STD_FD; j <= fd_max; j++) // start from after stdin, stcout, stcerr
                        {
                            if(FD_ISSET(j, &master))
                            {
                                if( j != listener && j != i) // all the other clients
                                {
                                    snd(j, (char *)&clientMsg, MSG_SIZE);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
