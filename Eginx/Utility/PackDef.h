#ifndef PACKDEF_H
#define PACKDEF_H

#include <netinet/in.h>
#include <bits/sockaddr.h>
#include <string>

struct ListenSocket
{
    char * ip;
    in_port_t port;
    sa_family_t family;
    std::string hostname;
};

#endif // PACKDEF_H
