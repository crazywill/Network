#ifndef WORKER_H
#define WORKER_H

#include "Interface/IExecutor.h"
#include "Interface/IComponent.h"
#include "Utility/ScopedPtr.h"
#include <string>
#include <map>
class Worker : public IExecutor
{
public:
    Worker();
    ~Worker();
    void initialize();
    ExecRet execute();
    void finalize();
    std::string name();
private:
    std::map<std::string,ScopedPtr<IComponent> > m_components;
    std::string m_name;
};

#endif // WORKER_H
