#include <iostream>
#include <bitset>
#include "Ping.h"
#include "Traceroute.h"

struct icmpT
{
  u_int8_t  icmp_type;	/* type of message, see below */
  u_int8_t  icmp_code;	/* type sub code */
  u_int16_t icmp_cksum;
};

int main(int args, char **argv)
{
    if(args<1){
        return 0;
    }

    Traceroute t("61.135.169.125");
    t.run();
//    printf("sizeof(timeval):%d\n",sizeof(timeval));
//    ping("www.baidu.com",5000,10);
//    printf("sizeof(char):%d\n",sizeof(char));
    return 0;
}

