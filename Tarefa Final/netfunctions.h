#ifndef NETFUNCTIONS_H
#define NETFUNCTIONS_H

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