#ifndef PACKDEF_H
#define PACKDEF_H

#include <netinet/in.h>
#include <bits/sockaddr.h>
#include <string>
#include <bits/socket.h>
#define Debug true

struct ListenSocket
{
    ListenSocket()
        :ip("")
        ,port(-1)
        ,family(AF_INET)
        ,hostname("")
        ,fd(-1)
    {}
    std::string ip;
    in_port_t port;
    sa_family_t family;
    std::string hostname;
    int fd;
};

#endif // PACKDEF_H
