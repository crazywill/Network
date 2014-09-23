#include "Master.h"
#include "Implementation/Configreader.h"
#include "Utility/PubDefine.h"
#include "Utility/Log.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>
#include <unistd.h>

Master::Master(const std::string & cfgpath)
    :m_isQuit(false)
    ,m_aliveWorkers(0)
    ,m_name(MASTER)
    ,m_configPath(cfgpath)
{
}

Master::~Master()
{

}

void Master::initialize()
{
    m_listenSockets.clear();
    m_configReader.reset(new ConfigReader(m_configPath));
    m_configReader->initialize();
    m_listenSockets = m_configReader->listenSockets();
    m_maxWorkers = m_configReader->workers();
}

IEXecutor::ExecRet Master::execute()
{
    int ret=0;
    createListenfd();
    if(m_configReader.get()){
        if(m_aliveWorkers<m_configReader->workers()){
            ret=forkWorkers();
            if(ret==0){
                return WorkerStart;
            }
        }
    }
    //Wait for signal or other commond
    return NormalReturn;
}

void Master::finalize()
{
    m_configReader->finalize();
    m_listenSockets.clear();
}

std::string Master::name()
{
    return m_name;
}

void Master::createListenfd()
{
    std::vector<ListenSocket>::iterator it;
    for(it = m_listenSockets.begin(); it != m_listenSockets.end(); ++it)
    {
        ListenSocket &lsocket = (*it);
        if(lsocket.family == AF_INET){
            struct sockaddr_in servaddr;
            lsocket.fd = socket(lsocket.family,SOCK_STREAM,0);
            memset(&servaddr,0,sizeof(servaddr));
            servaddr.sin_family = lsocket.family;
            servaddr.sin_addr.s_addr = htonl(inet_addr(lsocket.ip.c_str()));
            servaddr.sin_port = lsocket.port;
            if(bind(lsocket.fd,(const sockaddr*)&servaddr,sizeof(servaddr))==-1){
                Log(LogError,"Bind Error!");
            }
            if(listen(lsocket.fd,LISTENQ)==-1){
                Log(LogError,"Listen Error!");
            }
        }
        else if(lsocket.family == AF_INET6)
        {
            //IPv6
        }

    }
}

int Master::forkWorkers()
{
    int pid = 0;
    for(; m_aliveWorkers<m_maxWorkers; ++m_aliveWorkers)
    {
        pid = fork();
        if(pid == 0){
            return pid;
        }
    }
    return 1;
}
