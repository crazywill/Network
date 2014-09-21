#ifndef EGINX_H
#define EGINX_H

#include "Utility/ScopedPtr.h"
#include "Interface/IComponent.h"
#include "Interface/IExecutor.h"
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
    std::map<std::string,ScopedPtr<IComponent> > m_components;
    ScopedPtr<IEXecutor> m_executor;
};

#endif // EGINX_H
