#ifndef NETFUNCTIONS_H
#define NETFUNCTIONS_H

#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#define LISTENQ 10
#define MAXCLIREAD 8192
#define MAXLINE 4096
#define MAXRESPONSE 8192

int Getifaddrs(struct ifaddrs **_ifaddrs);
int Socket(int domain, int type, int protocol);
int Bind(int fd, const struct sockaddr *addr, socklen_t len);
int Listen(int fd, int n);
int Getsockname(int fd, struct sockaddr *addr, socklen_t *len);
int Accept(int fd, struct sockaddr *addr, socklen_t *len);
int Getpeername(int fd, struct sockaddr *addr, socklen_t *len);
int Inet_pton(int af, const char *cp, void *buf);
int Connect(int fd, const struct sockaddr *addr, socklen_t len);
char *getLocalIPaddress();

#endif