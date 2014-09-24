#include <sys/epoll.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netdb.h>
#include <string.h>
 #include <arpa/inet.h>

#define PORT "10000"
static int socketCreateAndBind()
{
/*
   struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    //把socket设置为非阻塞方式

    //setnonblocking(listenfd);

    //设置与要处理的事件相关的文件描述符

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    char local_addr[]="127.0.0.1";
    inet_aton(local_addr,&(serveraddr.sin_addr));//htons(portnumber);

    serveraddr.sin_port=htons(10000);
    bind(listenfd,(sockaddr *)&serveraddr, sizeof(serveraddr));
	return listenfd;
*/

	struct addrinfo hints;
	struct addrinfo *result,*rp;
	int s,sfd = -1;
	
	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;

	s = getaddrinfo(NULL,PORT, &hints, &result);
	if (s != 0)  
    {  
        fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));  
        return -1;  
    }

	for (rp = result; rp != NULL; rp = rp->ai_next)  
    {  
        sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);  
        if (sfd == -1)  
        	continue;  
  
        s = bind (sfd, rp->ai_addr, rp->ai_addrlen);  
        if (s == 0)  
        {  

            /* We managed to bind successfully! _addr:[%s]",rp->ai_addr->sa_data);*/ 

	        break;  
        }  
  
        close (sfd);  
    }  
  
    if (rp == NULL)  
    {  
        fprintf (stderr, "Could not bind\n");  
        return -1;  
    }  
	
	freeaddrinfo (result);  

	return sfd;

}

static int makeNoBlock(int sfd)
{
	int flags,status = -1;
	
	flags = fcntl(sfd,F_GETFL,0);
	if (-1 == flags) {
		perror("fcntl get");
		return -1;
	}

	flags |= O_NONBLOCK;
	status = fcntl(sfd,F_SETFL,flags);
	if (-1 == status) {
		perror("fcntl set");
		return -1;
	}

	return 0;
}

int main(int argc, char* argv[])
{
	int sfd,status = -1;
	int efd = -1;
	struct epoll_event event;
	struct epoll_event *events;

	sfd = socketCreateAndBind();
	if (sfd < 0) {
		perror("socket create and bind");
		abort();
	}

	status = makeNoBlock(sfd);
	if (-1 == status) {
		perror("make no-block");
		abort();
	}

	status = listen(sfd,SOMAXCONN);
	if (-1 == status) {
		perror("listen");
		abort();
	}

    efd = epoll_create1(0);
	if (efd < 0) {
		perror("epoll create");
		abort();
	}

	event.data.fd = sfd;
	event.events = 	EPOLLIN | EPOLLET;
	status = epoll_ctl(efd,EPOLL_CTL_ADD,sfd,&event);
	if (status < 0) {
		perror("epoll ctl");
		abort();
	}	

	events = static_cast<epoll_event*>(calloc(64,sizeof event));

	while (true) {
		int eventNum,i = 0;
		eventNum = epoll_wait(efd,events,64,1000);
		for (;i < eventNum;++i) {
			printf("epoll notify\n");
			fflush(NULL);			
		}
	}

	free(events);
	events = NULL;
	return 1;
}
