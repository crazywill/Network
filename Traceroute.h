#ifndef TRACEROUTE_H
#define TRACEROUTE_H

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <string>
#include <list>

#define MAXLENGTH 1200

class Traceroute
{
public:
    Traceroute(char * name = NULL,int timeot = 3000, int maxt = 30, int vrb = 3);
    ~Traceroute();
    void run();
private:
    void initialize();
    void finialize();
    std::string getAddrByName(char *name);
    void sendMessage();
    int recvMessage();
    void runImpl();
private:
    std::string targetIp;
    int maxttl;
    int verbose;
    int sendfd;
    int recvfd;
    int seq;
    int ttl;
    int pid;
    int timeout;
    u_short dstPort;
    u_short srcPort;
    char sendbuf[MAXLENGTH];
    char recvbuf[MAXLENGTH];
    struct sockaddr_in dst;
    struct sockaddr_in src;
    struct sockaddr_in binder;

};

#endif // TRACEROUTE_H
