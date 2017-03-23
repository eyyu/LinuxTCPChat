/*------------------------------------------------------------------------------
-- SOURCE FILE: wrapper.h - Wrappers for Socket`
--
-- PROGRAM: Linux chat
--
-- FUNCTIONS:
-- int getAddrInfo(const char * addr, const char * port, addrinfo *results)
-- int getAddrInfoUDP(const char * addr, const char * port, addrinfo * results = nullptr)
-- int makeSocket(int type = SOCK_STREAM)
-- int bindSocket(int sock, const sockaddr_in & addr)
-- int listenSock(int sock)
-- int acceptSock(int listenSock)
-- inline int setResuseAddr(int sock)
-- int connectSocket(int sock, const char * port, const char * ipaddr)
-- int selectSock(int numfd, fd_set * readset)
-- int sndAllClients(int sock )
-- int snd(int sock, char * sbuff, int blen)
-- int rcv(int sock, char * rbuff, int blen)
-- inline int setSockOpt(int sock, int level, int cmd, void * req, const int reqlen)
--
--
-- DATE: Feb. 21, 2017
--
-- REVISIONS:
-- 1.5 , Date and Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- NOTES:
-- This is the header for all the wrapper functions used by the program,
-- retireved and used by previous assignment in comp4985 a2
------------------------------------------------------------------------------*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <netdb.h>
#include <string.h>
#include "inc/wrapper.h"

/*------------------------------------------------------------------------------
-- FUNCTION: getAddrInfo
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- 1.0  - EY - Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int getAddrInfo (const char * addr, const char * port, addrinfo *results)
-- addr -- the ipaddress, can be null
-- port -- the port number as a string
-- results -- pointer to the locatons to store the linked list of addr infos
--
-- RETURNS:
-- int  -- 0 represents success
--
-- NOTES:
-- results is written to and used in the future
--------------------------------------------------------------------------*/
int getAddrInfo(const char * addr, const char * port, addrinfo *results)
{
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(addr, port, &hints, &results);
    return 0;
}

/*------------------------------------------------------------------------------
-- FUNCTION: makeSocket
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- 1.0  - EY - Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int makeSocket (int type)
-- type -- socket type == can be SOCK_STREAM or SOCK_DGRAN; defaults to Stream
--
-- RETURNS:
-- int  -- >0 represents success, socket created
--
-- NOTES:
-- wrapper to create a socket, resuse addr set, and no flags
--------------------------------------------------------------------------*/
int makeSocket(int type) // defaults to TCP
{
    int sock;

    if ( (sock = socket(AF_INET, type, 0)) < 0 )
    {
        perror("socket() error");
        exit(1);
    }
    setResuseAddr(sock);
    return sock;
}

/*------------------------------------------------------------------------------
-- FUNCTION: listenSock
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- 1.0  - EY - Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int listenSock (int sock)
-- sock -- socket to listen on
--
-- RETURNS:
-- int  -- 0 represents success
--
-- NOTES:
-- wrapper to listen to a socket, with backlog of 5
--------------------------------------------------------------------------*/
int listenSock(int sock)
{
    if ( listen(sock, BACKLOG /* = 5 */) < 0 )
    {
        perror("listen() error");
        exit(1);
    }
    return 0;
}

/*------------------------------------------------------------------------------
-- FUNCTION: bindSocket
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- 1.0  - EY - Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int bindSocket (int sock, const sockaddr_in &addr)
-- sock -- socket to bind
-- &addr -- the relevant information of the address to help the bind
--
-- RETURNS:
-- int  -- 0 represents success
--
-- NOTES:
-- wrapper to bind socket
--------------------------------------------------------------------------*/
int bindSocket(int sock, const sockaddr_in &addr)
{
    if( bind(sock,(struct sockaddr *)&addr,sizeof(addr)) < 0 )
    {
        perror("bind() error");
        exit(-1);
    }
    return 0;
}

/*------------------------------------------------------------------------------
-- FUNCTION: connectSocket
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- 1.0  - EY - Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int connectSocket (int sock, const char * port, const char * ipaddr)
-- sock -- socket to bind
-- port -- the port number string
-- ipaddr -- the ip address string
--
-- RETURNS:
-- int  -- 0 represents success
--
-- NOTES:
-- wrapper to bind socket
--------------------------------------------------------------------------*/
int connectSocket(int sock, const char * port, const char * ipaddr)
{
    addrinfo * results = nullptr;
    getAddrInfo(ipaddr, port, results);
    if ( connect(sock, results->ai_addr, results->ai_addrlen) < 0)
    {
        perror("connect() error");
        freeaddrinfo(results);
        exit(1);
    }
    freeaddrinfo(results);
    return 0;
}

/*------------------------------------------------------------------------------
-- FUNCTION: selectSock
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- 1.0  - EY - Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int selectSock (int numfd, fd_set * readset)
-- sock -- socket to bind
-- &addr -- the relevant information of the address to help the bind
--
-- RETURNS:
-- int  -- 0 represents success
--
-- NOTES:
-- wrapper that selects the read set with all other parameters set to null
--------------------------------------------------------------------------*/
int selectSock(int numfd, fd_set * readset)
{
    if (select(numfd, readset, NULL, NULL, NULL) < 0)
    {
        perror("select() error");
        exit(1);
    }
    return 0;
}

/*------------------------------------------------------------------------------
-- FUNCTION: acceptSock
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- 1.0  - EY - Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int acceptSock(int listenSock)
-- listenSock -- the listen socket
--
-- RETURNS:
-- int  -- >0 represents success; returns the socket created
--
-- NOTES:
-- wrapper that accpets a socket as new client
--------------------------------------------------------------------------*/
int acceptSock(int listenSock)
{
    int acceptsock;
    if ( (acceptsock = accept(listenSock,NULL,NULL)) < 0)
    {
        perror("accept() error");
        exit(1);
    }
    return acceptsock;
}

/*------------------------------------------------------------------------------
-- FUNCTION: snd
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- 1.0  - EY - Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int snd(int sock, char * sbuff, int blen)
-- sock  -- server socket
-- sbuff -- the send buffer
-- blen -- the num of bytes to send
--
-- RETURNS:
-- int  -- >0 represents success; returns the number of bytes sent
--
-- NOTES:
-- wrapper that wrpas the send
-- will send repearted to handle partial sends
--------------------------------------------------------------------------*/
int snd(int sock, char * sbuff, int blen)
{
    int bsent, bleft = blen, ttl = 0 ;
    while (ttl < blen)
    {
        if((bsent = send(sock, sbuff + ttl, blen, 0)) < 0 )
        {
            perror("sendto() error");
            exit(1);
        }
        bleft -= bsent;
        ttl += bsent;
    }
    return ttl;
}

/*------------------------------------------------------------------------------
-- FUNCTION: rcv
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- 1.0  - EY - Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: int rcv(int sock, char * rbuff, int blen)
-- sock  -- server socket
-- sbuff -- the buffer to store data read
-- blen -- the num of bytes recieved
--
-- RETURNS:
-- int  -- >0 represents success; returns number of bytes read
--
-- NOTES:
-- wrapper that wrpas the send
--------------------------------------------------------------------------*/
int rcv(int sock, char * rbuff, int blen)
{
    int bRead;
    if((bRead = recv(sock, rbuff, blen, 0)) < 0 )
    {
        perror("recvfrom() error");
        return bRead;
    }
    return bRead;
}

/*------------------------------------------------------------------------------
-- FUNCTION: rcv
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- 1.0  - EY - Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: inline int setResuseAddr(int sock)
-- sock  -- the socket to set the resuse on
--
-- RETURNS:
-- int  -- >0 represents success; returns number of bytes read
--
-- NOTES:
-- inline function that sets a socket to be resuable instantle
--------------------------------------------------------------------------*/
inline int setResuseAddr(int sock)
{
    int arg = 1;
    return setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg));
}

/*------------------------------------------------------------------------------
-- FUNCTION: setSockOpt
--
-- DATE: Mar. 22, 2017
--
-- REVISIONS:
-- 1.0  - EY - Description
--
-- DESIGNER: Eva Yu
--
-- PROGRAMMER: Eva Yu
--
-- INTERFACE: inline int setSockOpt(int sock, int level, int cmd, void * req, const int reqlen)
-- sock  -- the socket to set the resuse on
-- level -- socket level
-- cmd -- command to put on the socket
-- req -- the request structure
-- the request struct length
--
-- RETURNS:
-- int  -- >0 represents success; returns number of bytes read
--
-- NOTES:
-- sets the socket options
--------------------------------------------------------------------------*/
inline int setSockOpt(int sock, int level, int cmd, void * req, const int reqlen)
{
    if( setSockOpt(sock, level, cmd, req, reqlen) < 0)
    {
        perror("setsockopt() error");
        exit(1);
    }
    return 0;
}

// inline int addToMcastGroup(int sock, ip_mreq & mreq)
// {
//     return setSockOpt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(ip_mreq));
// }
//
// inline int rmvFromMcastGroup(int sock, ip_mreq & mreq)
// {
//     return setSockOpt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(ip_mreq));
// }
