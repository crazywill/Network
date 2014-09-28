#ifndef EGINX_H
#define EGINX_H

#include "Utility/ScopedPtr.h"
#include "Interface/IExecutor.h"
#include "Interface/Egx_IProcMutex.h"
#include <string>
#include <map>
#include <netinet/in.h>

class Eginx
{
public:
    Eginx(const std::string &cfg);
    void initialize();
    void finalize();
    void run();

private:
    ScopedPtr<IExecutor> m_executor;
    std::string m_configPath;
    Egx_IProcMutex* m_procMutex;
};

#endif // EGINX_H
