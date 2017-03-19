#ifndef WRAPPER_H
#define WRAPPER_H

#include <sys/socket.h>
#include <netdb.h>

inline int getAddrInfo(const char * addr, const char * port, addrinfo *results);
inline int getAddrInfoUDP(const char * addr, const char * port, addrinfo *results);
int makeSocket(int type = SOCK_STREAM);
int bindSocket(int sock, int port, char * ipaddr);
int listenSocket(int sock, const char * port, const char * ipaddr);
int connectSocket(int sock, const char * port, const char * ipaddr);
inline int setResuseAddr(int sock);
inline int addToMcastGroup(int sock, ip_mreq & mreq);
inline int rmvFromMcastGroup(int sock, ip_mreq & mreq);
int sndAllClients(int sock /*, array of clients*/);
int snd(int sock, char * sbuff, int blen);
int rcv(int sock, char * rbuff, int blen);
int sndto(int sock, char * sbuff, int blen, const sockaddr &sa);
int rcvfm(int sock, char * rbuff, int blen, sockaddr &sa, socklen_t &addrsize);
inline int setSockOpt(int sock, int level, int cmd, void * req, const int reqlen);

#endif
