#ifndef WRAPPER_H
#define WRAPPER_H

#include <sys/socket.h>
#include <netdb.h>

#define BACKLOG 5

int getAddrInfo(const char * addr, const char * port, addrinfo *results);
int getAddrInfoUDP(const char * addr, const char * port, addrinfo * results = nullptr);
int makeSocket(int type = SOCK_STREAM);
int bindSocket(int sock, const sockaddr_in & addr);
int listenSock(int sock);
int acceptSock(int listenSock);
inline int setResuseAddr(int sock);
inline int addToMcastGroup(int sock, ip_mreq & mreq);
inline int rmvFromMcastGroup(int sock, ip_mreq & mreq);
int connectSocket(int sock, const char * port, const char * ipaddr);
int selectSock(int numfd, fd_set * readset);
int sndAllClients(int sock /*, array of clients*/);
int snd(int sock, char * sbuff, int blen);
int rcv(int sock, char * rbuff, int blen);
inline int setSockOpt(int sock, int level, int cmd, void * req, const int reqlen);

#endif
