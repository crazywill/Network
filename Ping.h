#ifndef PING_H
#define PING_H

#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <list>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>


void getAddrByName(char *name,std::list<std::string> &addrs);
void pingShot(const char *dstip , int timeout);
void ping(char *name,int timeout, int count);


#endif // PING_H
