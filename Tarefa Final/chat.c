#include "chat.h"
#include <stdlib.h>
#include <string.h>

User* createNewUser(int id, char *nickname, struct ifaddrs *_ifaddrs) {
    User* user = malloc(sizeof(User));
    strcpy(user->nickname, nickname);
    user->connection = NULL;
    user->ifaddrs = _ifaddrs;
    user->id = id;
    return user;
}

void deleteUser(User* u) {
    freeifaddrs(u->ifaddrs);
    free(u->nickname);
    free(u);
}

void startPeerConnection(User* u1, User* u2) {
    u1->connection = u2;
    u2->connection = u1;
}

void endPeerConnection(User* u1, User* u2) {
    u1->connection = NULL;
    u2->connection = NULL;
}