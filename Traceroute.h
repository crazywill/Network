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
#define TIMEOUT 2
#define SUCCESS 1
#define CONTINUE 0
#define FAILURE -1

class Traceroute
{
public:
    Traceroute(char * name = NULL,int tmout = 3000, int maxt = 30, int vrb = 3);
    ~Traceroute();
    void run();
private:
    void initialize();
    void finialize();
    /**
     * @brief Get IP address from domain.Just return the first IP, if there are serveral candidates.
     * @param [IN]:domain
     * @return IP address.
     */
    std::string getAddrByName(char *name);
    /**
     * @brief Send udp packets. TTL:1 to MaxTTL
     */
    void sendMessage();
    /**
     * @brief Receive icmp packets;Check the packets and show route if valid.
     * @return :
     *          TIMEOUT: received nothing.
     *          SUCCESS: udp packets have arrived at remote host.
     *          CONTINUE: udp packets time exceed.(ttl=0)
     *          FAILURE: some errors have occured.
     */
    int recvMessage();
    /**
     * @brief The implement of public function run().
     */
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
