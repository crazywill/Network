#include "Worker.h"
#include "Utility/PubDefine.h"
#include "Utility/Egx_Log.h"
#include <sys/epoll.h>
Worker::Worker(const std::vector<ListenSocket> &listenSockets,Egx_IProcMutex * mutex)
    :m_name(WORKER)
    ,m_listenSockets(listenSockets)
    ,m_procMutex(mutex)
    ,m_epollfd(-1)
{
}

Worker::~Worker()
{

}

void Worker::initialize()
{
    m_epollfd = epoll_create(EPOLLSIZE);
    m_clients.clear();
}

IExecutor::ExecRet Worker::execute()
{
    bool lockhold;
    epoll_event events[EPOLLSIZE];
    int eventnum;
    for(;;)
    {
        if(m_procMutex){
            if(lockhold=m_procMutex->tryLock()){
                addListenFD();
            }
            if(lockhold){
                eventnum=epoll_wait(m_epollfd,events,EPOLLSIZE,LOCKTIME);
            }
            else{
                eventnum=epoll_wait(m_epollfd,events,EPOLLSIZE,UNLOCKTIME);
            }
            for(int i=0; i<eventnum; ++i)
            {
                if(lockhold && isListenFD(events[i].data.fd)){
                    ClientData client;
                    int acceptFD = accept(events[i].data.fd,(sockaddr *)&client.clientAddr,&client.clientLen);
                    if(acceptFD){
                        m_clients.push_back(client);
                        addClientFD(acceptFD);
                    }
                }
                else if(events[i].events&EPOLLIN){
                    // data read
                    if(lockhold){
//                        postTask();
                        LogDebug("postTask()\n");
                    }
                    else{
//                        doTask();
                        LogDebug("doTask()\n");
                    }
                }
                else if(events[i].events&EPOLLOUT){
                    // data write
                    if(lockhold){
//                        postTask();
                        LogDebug("postTask()\n");
                    }
                    else{
//                        doTask();
                        LogDebug("doTask()\n");
                    }
                }
            }
            if(lockhold){
                m_procMutex->unlock();
            }
            //do task
        }
        else{
            eventnum=epoll_wait(m_epollfd,events,EPOLLSIZE,UNLOCKTIME);

            for(int i=0; i<eventnum; ++i)
            {
                if(events[i].events&EPOLLIN){
                    // data read
                    if(lockhold){
//                        postTask();
                        LogDebug("postTask()\n");
                    }
                    else{
//                        doTask();
                        LogDebug("doTask()\n");
                    }
                }
                else if(events[i].events&EPOLLOUT){
                    // data write
                    if(lockhold){
//                        postTask();
                        LogDebug("postTask()\n");
                    }
                    else{
//                        doTask();
                        LogDebug("doTask()\n");
                    }
                }
            }
            //do task
        }
    }
    return NormalReturn;
}

void Worker::finalize()
{
    closeFD();
    m_clients.clear();
}

std::string Worker::name()
{
    return m_name;
}

void Worker::addListenFD()
{
    std::vector<ListenSocket>::iterator it;
    for(it = m_listenSockets.begin(); it != m_listenSockets.end(); ++it)
    {
        epoll_event event;
        ListenSocket & lsocket = *it;
        event.data.fd = lsocket.fd;
        event.events = EPOLLIN|EPOLLET;
        epoll_ctl(m_epollfd,EPOLL_CTL_ADD,lsocket.fd,&event);
    }
}

void Worker::addClientFD(int fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN|EPOLLET;
    epoll_ctl(m_epollfd,EPOLL_CTL_ADD,fd,&event);
}

bool Worker::isListenFD(int fd)
{
    std::vector<ListenSocket>::iterator it;
    for(it = m_listenSockets.begin(); it != m_listenSockets.end(); ++it)
    {
        ListenSocket & lsocket = *it;
        if(lsocket.fd == fd){
            return true;
        }
    }
    return false;
}

void Worker::closeFD()
{
}
