#ifndef WORKER_H
#define WORKER_H

#include "Interface/IExecutor.h"
#include "Interface/IComponent.h"
#include "Interface/Egx_IProcMutex.h"
#include "Utility/ScopedPtr.h"
#include "Utility/PackDef.h"
#include <string>
#include <map>
#include <vector>
#include <sys/socket.h>
class Worker : public IExecutor
{
public:
    Worker(const std::vector<ListenSocket> & listenSockets,Egx_IProcMutex * mutex = NULL);
    ~Worker();
    void initialize();
    ExecRet execute();
    void finalize();

    std::string name();

private:
    struct ClientData{
        sockaddr_in clientAddr;
        socklen_t clientLen;
    };

    void addListenFD();
    void rmListenFD();
    void addClientFD(int fd);
    bool isListenFD(int fd);
    void closeFD();
private:
    std::map<std::string,ScopedPtr<IComponent> > m_components;
    std::string m_name;
    std::vector<ListenSocket> m_listenSockets;
    std::vector<ClientData> m_clients;
    Egx_IProcMutex *m_procMutex;
    int m_epollfd;
};

#endif // WORKER_H
