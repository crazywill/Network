#include <iostream>
#include <bitset>
#include "Ping.h"
#include "Traceroute.h"

int main(int args, char **argv)
{
    if(args<1){
        return 0;
    }
    if(args>=2){
        ping(argv[1],5000,10);
//        Traceroute t(argv[1]);
//        t.run();
    }
//    printf("sizeof(timeval):%d\n",sizeof(timeval));
//    ping("www.baidu.com",5000,10);
//    printf("sizeof(char):%d\n",sizeof(char));
    return 0;
}

