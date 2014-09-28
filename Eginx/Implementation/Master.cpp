#include "Master.h"
#include "Implementation/Configreader.h"
#include "Utility/PubDefine.h"
#include "Utility/Egx_Log.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>
#include <unistd.h>
#include <errno.h>

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
    LogDebug("Master::%s\n",__FUNCTION__);
    m_listenSockets.clear();
    m_configReader.reset(new ConfigReader(m_configPath));
    m_configReader->initialize();
    m_listenSockets = m_configReader->listenSockets();
    m_maxWorkers = m_configReader->workers();
    createListenfd();
}

IExecutor::ExecRet Master::execute()
{
    LogDebug("Master::%s\n",__FUNCTION__);
    int ret=0;
    if(m_configReader.get()){
        if(m_aliveWorkers<m_configReader->workers()){
            ret=forkWorkers();
            if(ret==0){
                return WorkerStart;
            }
        }
    }
    sleep(1);
    //Wait for signal or other commond
    return NormalReturn;
}

void Master::finalize()
{
    LogDebug("Master::%s\n",__FUNCTION__);
    m_configReader->finalize();
    m_listenSockets.clear();
}

std::string Master::name()
{
    LogDebug("Master::%s\n",__FUNCTION__);
    return m_name;
}

std::vector<ListenSocket> Master::ListenSockets()
{
    LogDebug("Master::%s\n",__FUNCTION__);
    return m_listenSockets;
}

void Master::createListenfd()
{
    LogDebug("Master::%s\n",__FUNCTION__);
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
                LogError("Bind Error errno:%d!\n",errno);
                continue;
            }
            if(listen(lsocket.fd,LISTENQ)==-1){
                LogError("Listen Error errno:%d!\n",errno);
                continue;
            }
            LogDebug("CreateListenFD Successfully!\n");
        }
        else if(lsocket.family == AF_INET6)
        {
            //IPv6
        }

    }
}

int Master::forkWorkers()
{
    LogDebug("Master::%s\n",__FUNCTION__);
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
