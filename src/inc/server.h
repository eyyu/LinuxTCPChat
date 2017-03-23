/*------------------------------------------------------------------------------
-- SOURCE FILE: server.h - Server Class
--
-- PROGRAM: LinuxChat
--
-- FUNCTIONS:
-- int run ( void )
-- PRIVATE:
-- int runserver ( void )
-- int selectloop ( void )
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
#include "msg.h"

#ifndef SERVER_H
#define SERVER_H

#define STD_FD 3

class ChatServer
{
public:
    int run(); // called by the user

private:
    int listener; // the server socket
    Message clientMsg; // Message struct
    fd_set master; // master set of file descripts.
    int fd_max; // the max number of file desc.

    int runserver(); // private run function that is called by run
    int selectloop(); // select, read, write, and accept called here
};

#endif
