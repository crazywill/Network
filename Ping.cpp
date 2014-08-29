#include "Ping.h"
#include <bitset>
#include <memory.h>
#include <sys/time.h>
#include <sys/select.h>
#define PACKET_SIZE     1024

unsigned short cal_chksum(unsigned short *addr, int len)
{
    int nleft=len;
    int sum=0;
    unsigned short *w=addr;
    unsigned short answer=0;

    while(nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }

    if( nleft == 1)
    {
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;

    return answer;
}

unsigned short checksum(unsigned short *buf,int nword)
{
    unsigned long sum;
    for(sum=0;nword>0;nword--)
    {
    sum += *buf++;
    sum = (sum>>16) + (sum&0xffff);
    }
    return ~sum;
}


u_int16_t checkSum(unsigned short *ptr, int len)
{
    unsigned int sum = 0;
    while(len>1){
        sum+=*ptr;
        len-=2;
        ++ptr;
//        sum=(sum>>16)+(sum&0xFFFF);
    }
    if(len==1){
        sum+=*(unsigned char *)(ptr);
    }
    while((sum&0xFFFF0000)!=0){
//        printf("go >>16\n");
        sum=(sum>>16)+(sum&0xFFFF);
    }
    return ~sum;
}

void getAddrByName(char *name,std::list<std::string> &addrs)
{
    char **ptr = NULL;
    char str[INET_ADDRSTRLEN];
    struct hostent *res = gethostbyname(name);
//    printf("hostname is %s \n",res->h_name);
//    for(ptr = res->h_aliases; *ptr!=NULL;++ptr)
//    {
//        printf("aliase is %s\n",*ptr);
//    }
    addrs.clear();
    for(ptr = res->h_addr_list;*ptr!=NULL;++ptr)
    {
//        printf("address is %s\n",);
        inet_ntop(res->h_addrtype,*ptr,str,sizeof(str));
        addrs.push_back(str);
    }   
    return;
}


void ping(char *name,int timeout, int count)
{
    std::list<std::string> ipList;
    getAddrByName(name, ipList);
    if(!ipList.empty()){
        printf("PING %s[%s]\n",ipList.front().c_str(),name);
        for(int i=0;i<count;++i)
        {
            pingShot(ipList.front().c_str(),timeout);
            sleep(1);
        }
    }
}

void pingShot(const char *dstip, int timeout)
{
    static int seq = 0;
    int socketfd(-1);
    struct sockaddr_in dst;
    struct sockaddr_in src;
    struct timeval timeo;
    struct timeval * timevalPtr;
    struct timeval timevalNow;
    struct icmp *icmpPtr;
    struct ip *iph;
    fd_set readfds;
    int maxfd;
    int n;
    int sendBytes=0;
    int rcvBytes=0;
    char sendbuf[PACKET_SIZE];
    char recvbuf[PACKET_SIZE];
    int recvLen;
    int timems=0;
    pid_t pid = getpid();

    //initialize dst addr
    bzero(&dst,sizeof(dst));
    dst.sin_addr.s_addr = inet_addr(dstip);
    dst.sin_family = AF_INET;
    dst.sin_port = htons(30000);

    socketfd = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);

    if(socketfd!=-1){
        // set timeout
        timeo.tv_sec = timeout / 1000;
        timeo.tv_usec = timeout % 1000;

        // set socket opt
        if (setsockopt(socketfd, SOL_SOCKET, SO_SNDTIMEO, &timeo, sizeof(timeo)) == -1){
            printf("setsockopt error\n");
            return;
        }

        // send icmp
        memset(sendbuf,0,sizeof(sendbuf));
        icmpPtr = (struct icmp*)sendbuf;
        icmpPtr->icmp_type = ICMP_ECHO;
        icmpPtr->icmp_code = 0;
        icmpPtr->icmp_id = pid;
        icmpPtr->icmp_seq = seq++;
        timevalPtr = (struct timeval *)icmpPtr->icmp_data;
        gettimeofday(timevalPtr,NULL);
        icmpPtr->icmp_cksum = 0;
        icmpPtr->icmp_cksum = checkSum((unsigned short *)icmpPtr,sizeof(struct icmp));
        sendBytes = sendto(socketfd,sendbuf,sizeof(struct icmp),0,(struct sockaddr *)(&dst),sizeof(dst));
//        printf("sendto %d bytes\n",sizeof(struct icmp));
        if(sendBytes<1){
            printf("send error!\n");
            return;
        }

        while(true){
            FD_ZERO(&readfds);
            FD_SET(socketfd,&readfds);
            maxfd = socketfd+1;
            n = select(maxfd,&readfds,NULL,NULL,&timeo);
            if(n<1){
                printf("time out\n");
                close(socketfd);
                return;
            }
            if(!FD_ISSET(socketfd,&readfds)){
                printf("FD_ISSET failed!\n");
                continue;
            }
            memset(recvbuf, 0, sizeof(recvbuf));
            memset(&src,0,sizeof(src));
            rcvBytes = recvfrom(socketfd,recvbuf,sizeof(recvbuf),0,(struct sockaddr *)&src,(socklen_t *)&recvLen);
            if(rcvBytes<1){
                break;
            }

            iph = (struct ip *)recvbuf;
            char ipsrc[64] = {'\0'};
            char ipdst[64] = {'\0'};
            snprintf(ipsrc,64,"%s",inet_ntoa(iph->ip_src));
            snprintf(ipdst,64,"%s",inet_ntoa(iph->ip_dst));
            if(strcmp(ipsrc,dstip)!=0){
                printf("ip error! ip is %s\n",ipsrc);
                continue;
            }
//                printf("total len=[%d] headersiez=[%d]  destip=[%d.%d.%d.%d]\n",tl,sizeof(struct ip),(unsigned char)*(recvbuf+16),(unsigned char)*(recvbuf+17),(unsigned char)*(recvbuf+18),(unsigned char)*(recvbuf+19));
            //show ip header
//                printf("ip_hl<<2:%d ip_len:%u ip_ttl:%d src:%s dst:%s\n",(iph->ip_hl<<2)
//                                                                      ,iph->ip_len
//                                                                      ,iph->ip_ttl
//                                                                      ,ipsrc
//                                                                      ,ipdst);
            icmpPtr=(struct icmp *)(recvbuf + (iph->ip_hl<<2));
            timevalPtr = (struct timeval *)icmpPtr->icmp_data;
            gettimeofday(&timevalNow,NULL);
            timems = (timevalNow.tv_sec-timevalPtr->tv_sec)*1000+(timevalNow.tv_usec-timevalPtr->tv_usec)/1000;

//                printf("ip:%s,icmp->icmp_type:%d,icmp->icmp_id:%d\n",ipList.front().c_str(),icmpPtr->icmp_type,icmpPtr->icmp_id);
            // 判断Ping回复包的状态
            if (icmpPtr->icmp_type == ICMP_ECHOREPLY && icmpPtr->icmp_id == pid)   //ICMP_ECHOREPLY回显应答
            {
                // 正常就退出循环
                printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%d ms\n",rcvBytes,ipsrc,icmpPtr->icmp_seq,iph->ip_ttl,timems);
                break;
            }
            else
            {
                // 否则继续等
                continue;
            }
        }
    }
    else{
        printf("Error! socketfd is -1.\n");
        return;
    }



}
