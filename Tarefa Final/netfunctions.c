#include "netfunctions.h"

int Getifaddrs(struct ifaddrs **_ifaddrs)
{
    int result = getifaddrs(_ifaddrs);
    if (result == -1)
    {
        perror("getifaddrs");
        exit(1);
    }
    return result;
}

int Socket(int domain, int type, int protocol)
{
    int socketfd = socket(domain, type, protocol);
    if (socketfd == -1)
    {
        perror("socket");
        exit(1);
    }
    return socketfd;
}

int Bind(int fd, const struct sockaddr *addr, socklen_t len)
{
    int result = bind(fd, addr, len);
    if (result == -1)
    {
        perror("bind");
        exit(1);
    }
    return result;
}

int Listen(int fd, int n)
{
    int result = listen(fd, n);
    if (result == -1)
    {
        perror("listen");
        exit(1);
    }
    return result;
}

int Getsockname(int fd, struct sockaddr *addr, socklen_t *len)
{
    int result = getsockname(fd, addr, len);
    if (result == -1)
        perror("getsockname");
    return result;
}

int Accept(int fd, struct sockaddr *addr, socklen_t *len)
{
    int connfd = accept(fd, addr, len);
    if (connfd == -1)
    {
        perror("accept");
        exit(1);
    }
    return connfd;
}

int Getpeername(int fd, struct sockaddr *addr, socklen_t *len)
{
    int result = getpeername(fd, addr, len);
    if (result == -1)
    {
        perror("getpeername");
        exit(1);
    }
    return result;
}

int Inet_pton(int af, const char *cp, void *buf)
{
    int result = inet_pton(af, cp, buf);
    if (result <= 0)
    {
        perror("inet_pton error");
        exit(1);
    }
    return result;
}

int Connect(int fd, const struct sockaddr *addr, socklen_t len)
{
    int result = connect(fd, addr, len);
    if (result < 0)
    {
        perror("connect error");
        exit(1);
    }
    return result;
}

// Pega o IP Local da interface definida como "default" -> Passa esse endereço para a variavel host
char *getLocalIPaddress()
{
    FILE *f;
    char line[100], *p, *c;

    f = fopen("/proc/net/route", "r");

    while (fgets(line, 100, f))
    {
        p = strtok(line, " \t");
        c = strtok(NULL, " \t");

        if (p != NULL && c != NULL)
        {
            if (strcmp(c, "00000000") == 0)
            {
                break;
            }
        }
    }

    struct ifaddrs *actualIP;
    Getifaddrs(&actualIP);

    int family;
    char host[NI_MAXHOST];
    for (struct ifaddrs *ifa = actualIP; ifa != NULL; ifa = ifa->ifa_next)
    {
        family = ifa->ifa_addr->sa_family;
        if (family == AF_INET && strcmp(ifa->ifa_name, p) == 0)
        {
            getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            break;
        }
    }
    char *ipAddress = (char *)malloc(sizeof(host));
    strcpy(ipAddress, host);
    return ipAddress;
}