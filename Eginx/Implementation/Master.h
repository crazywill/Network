#ifndef MASTER_H
#define MASTER_H

#include "Interface/IExecutor.h"
#include "Interface/IComponent.h"
#include "Interface/IConfigReader.h"
#include "Utility/ScopedPtr.h"
#include "Utility/PackDef.h"
#include <string>
#include <map>
#include <vector>
class Master : public IEXecutor
{
public:
    Master(const std::string & cfgpath);
    ~Master();
    void initialize();
    ExecRet execute();
    void finalize();
    std::string name();
    std::vector<ListenSocket> ListenSockets();
private:
    void createListenfd();
    int forkWorkers();
private:
    ScopedPtr<IConfigReader> m_configReader;
    std::vector<ListenSocket> m_listenSockets;
    bool m_isQuit;
    int m_aliveWorkers;
    int m_maxWorkers;
    std::string m_name;
    std::string m_configPath;
};

#endif // MASTER_H
