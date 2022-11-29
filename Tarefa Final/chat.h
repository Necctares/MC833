#ifndef CHAT_H
#define CHAT_H

#include <ifaddrs.h>

typedef struct User
{
    int id;
    struct ifaddrs *ifaddrs;
    struct User* connection;
    char *nickname;
} User;

User* createNewUser(int id, char *nickname, struct ifaddrs *_ifaddrs);
void deleteUser(User* u);
void startPeerConnection(User* u1, User* u2);
void endPeerConnection(User* u1, User* u2);

#endif