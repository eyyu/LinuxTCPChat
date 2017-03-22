//A bunch of wrappers!

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

int getAddrInfo(const char * addr, const char * port, addrinfo *results)
{
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(addr, port, &hints, &results);
    return 0;
}

int makeSocket(int type)
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

int listenSock(int sock)
{
    if ( listen(sock, BACKLOG) < 0 )
    {
        perror("listen() error");
        exit(1);
    }
    return 0;
}

int bindSocket(int sock, const sockaddr_in &addr)
{
    if( bind(sock,(struct sockaddr *)&addr,sizeof(addr)) < 0 )
    {
        perror("bind() error");
        exit(-1);
    }
    return 0;
}

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

int selectSock(int numfd, fd_set * readset)
{
    if (select(numfd, readset, NULL, NULL, NULL) < 0)
    {
        perror("select() error");
        exit(1);
    }
    return 0;
}

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

inline int setResuseAddr(int sock)
{
    int arg = 1;
    return setsockopt (sock, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg));
}

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
