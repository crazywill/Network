#include "Traceroute.h"
#include <memory.h>
#include <sys/time.h>
#include <errno.h>
#include <netinet/udp.h>
struct rec {					/* format of outgoing UDP data */
  u_short	rec_seq;			/* sequence number */
  u_short	rec_ttl;			/* TTL packet left with */
  int       rec_pid;
  struct timeval	rec_tv;		/* time packet left */
};


#define SUCCESS 1
#define CONTINUE 0
#define FAILURE -1
Traceroute::Traceroute(char *name, int timeot, int maxt, int vrb)
    :targetIp("")
    ,maxttl(maxt)
    ,verbose(vrb)
    ,sendfd(-1)
    ,recvfd(-1)
    ,seq(0)
    ,ttl(0)
    ,pid(getpid())
    ,timeout(timeot)
{
    targetIp = getAddrByName(name);
    initialize();
}

Traceroute::~Traceroute()
{
    finialize();
}

void Traceroute::run()
{
    runImpl();
}

void Traceroute::initialize()
{
    printf("sizeof(rec):%d\n",sizeof(rec));
    //init sendfd
    sendfd = socket(AF_INET,SOCK_DGRAM, 0);
    memset(&dst,0,sizeof(dst));

    //init recvfd
    recvfd = socket(AF_INET,SOCK_RAW, IPPROTO_ICMP);
    memset(&src,0,sizeof(src));

    memset(&binder,0,sizeof(binder));

}

void Traceroute::finialize()
{
}

std::string Traceroute::getAddrByName(char *name)
{
    char **ptr = NULL;
    char str[INET_ADDRSTRLEN];
    struct hostent *res = gethostbyname(name);
    ptr = res->h_addr_list;
    if(*ptr!=NULL){
        inet_ntop(res->h_addrtype,*ptr,str,sizeof(str));
        return str;
    }
    return "";
}

void Traceroute::sendMessage()
{
    int re;
    rec * recPtr;

    srcPort = (getpid() & 0xffff) | 0x8000;
    binder.sin_family = AF_INET;
    binder.sin_port = htons(srcPort);
    binder.sin_addr.s_addr = htonl(INADDR_ANY);
    re=bind(sendfd,(sockaddr *)&binder,sizeof(binder));
    printf("bind res:%d srcPort:%d\n",re,srcPort);
    if(errno!=0){
        printf("ERRNO:%d\n",errno);
    }
    for(ttl = 1;ttl<=maxttl;++ttl)
    {
        re=setsockopt(sendfd,IPPROTO_IP,IP_TTL,&ttl,sizeof(ttl));
        if(re<0){
            printf("SETSOCKOPT ERROR!\n");
        }
        printf("%d  ",ttl);
//        printf("setsockopt %d\n",re);
        memset(sendbuf,0,sizeof(sendbuf));
        recPtr = (struct rec *) sendbuf;
        recPtr->rec_seq = seq++;
        recPtr->rec_ttl = ttl;
        recPtr->rec_pid = getpid();
        gettimeofday(&recPtr->rec_tv, NULL);

        for(int i=0;i<verbose;++i)
        {
            dstPort = 60000+seq*2+i;
            dst.sin_family = AF_INET;
            dst.sin_port = htons(dstPort);
            inet_pton(AF_INET,targetIp.c_str(),&dst.sin_addr);
            re=sendto(sendfd,sendbuf,sizeof(rec),0,(sockaddr *)&dst,sizeof(dst));
            if(re<0){
                printf("SENDTO ERROR!\n");
            }
            if(errno!=0){
                printf("ERRNO:%d\n",errno);
            }

            if(recvMessage()==SUCCESS){
                fflush(stdout);
                break;
            }
            fflush(stdout);
        }
        printf("\n");

    }
}

int Traceroute::recvMessage()
{
    fd_set readfds;
    struct timeval timeo;
    int maxfd=0,n=0;
    int recvlen=0;
    int icmpLen = 0;
    struct icmp *icmpPtr;
    struct ip *ipPtr;
    struct ip *iipPtr;
    struct udphdr *udpPtr;
    int result = CONTINUE;
    while(true){
        FD_ZERO(&readfds);
        FD_SET(recvfd,&readfds);
        maxfd = recvfd+1;
        timeo.tv_sec = timeout / 1000;
        timeo.tv_usec = timeout % 1000;
        n=select(maxfd,&readfds,NULL,NULL,&timeo);
        if(n<1){
            printf("*  ");
            break;
        }
        if(!FD_ISSET(recvfd,&readfds)){
            printf("FDSET ERROR!  ");
            continue;
        }
        memset(recvbuf,0,sizeof(recvbuf));
        n=recvfrom(recvfd,recvbuf,sizeof(recvbuf),0,(sockaddr *)&src,(socklen_t *)&recvlen);
//        printf("recv %d line.\n",n);
        ipPtr = (struct ip*)recvbuf;
        icmpPtr = (struct icmp*)(recvbuf+(ipPtr->ip_hl<<2));
        char ipsrc[64] = {'\0'};
        char ipdst[64] = {'\0'};
        snprintf(ipsrc,64,"%s",inet_ntoa(ipPtr->ip_src));
        snprintf(ipdst,64,"%s",inet_ntoa(ipPtr->ip_dst));
//        printf("dst:%s src:%s type:%d code:%d \n",ipdst,ipsrc,icmpPtr->icmp_type,icmpPtr->icmp_code);
        icmpLen = n-(ipPtr->ip_hl<<2);
        if(icmpLen<8){
            printf("icmp length is not enough![%d]  ",icmpLen);
            continue;
        }
        if((icmpLen-8)<sizeof(ip)){
            printf("icmp inner data is not enough for inner ip header![%d]  ",icmpLen-8);
            continue;
        }
        if((icmpLen-8-sizeof(ip))<sizeof(udphdr)){
            printf("icmp inner data is not enough for inner udp header![%d]  ",icmpLen-8-sizeof(ip));
            continue;
        }
        iipPtr = (struct ip*)(recvbuf+(ipPtr->ip_hl<<2)+8);
        udpPtr = (struct udphdr *)(recvbuf+(ipPtr->ip_hl<<2)+8+(iipPtr->ip_hl<<2));
//        printf("udp dst:%d[%d] src:%d[%d]\n",ntohs(udpPtr->dest),dstPort,ntohs(udpPtr->source),srcPort);
        if(icmpPtr->icmp_type == ICMP_TIMXCEED &&
           icmpPtr->icmp_code == ICMP_TIMXCEED_INTRANS){
            if(iipPtr->ip_p == IPPROTO_UDP &&
                ntohs(udpPtr->dest) == dstPort &&
                ntohs(udpPtr->source) == srcPort){
                printf("%s  ",ipsrc);
            }
            else{
//                printf("CHECK ERROR! Src:%s Dst:%s Type:%d Code:%d Protocol:%d DstPort:%d[%d] SrcPort:%d[%d]  \n",
//                       ipsrc,
//                       ipdst,
//                       icmpPtr->icmp_type,
//                       icmpPtr->icmp_code,
//                       iipPtr->ip_p,
//                       ntohs(udpPtr->dest),
//                       dstPort,
//                       ntohs(udpPtr->source),
//                       srcPort);
                continue;
            }

        }
        else if(icmpPtr->icmp_type == ICMP_UNREACH &&
                icmpPtr->icmp_code == ICMP_UNREACH_PORT){
            if(iipPtr->ip_p == IPPROTO_UDP &&
                ntohs(udpPtr->dest) == dstPort &&
                ntohs(udpPtr->source) == srcPort){
                printf("%s  ",ipsrc);
                result = SUCCESS;
            }
            else{
//                printf("CHECK ERROR! Src:%s Dst:%s Type:%d Code:%d Protocol:%d DstPort:%d[%d] SrcPort:%d[%d]  \n",
//                       ipsrc,
//                       ipdst,
//                       icmpPtr->icmp_type,
//                       icmpPtr->icmp_code,
//                       iipPtr->ip_p,
//                       ntohs(udpPtr->dest),
//                       dstPort,
//                       ntohs(udpPtr->source),
//                       srcPort);
                continue;
            }
        }
        else{
            printf("ICMP DATA USELESS! TYPE:%d CODE:%d  ",icmpPtr->icmp_type,icmpPtr->icmp_code);
            continue;
        }
        break;
    }
}

void Traceroute::runImpl()
{
    printf("traceroute to %s %d hops max\n",targetIp.c_str(),maxttl);
    sendMessage();
}
