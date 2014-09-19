#ifndef EGINX_H
#define EGINX_H

#include "Utility/ScopedPtr.h"
#include <string>
#include <list>
#include <netinet/in.h>
class IConfigReader;
class IWorker;
class IParser;
class IMemcached;
class IIOController;
class IProcessConnection;
class Eginx
{
public:
    Eginx(const std::string &cfg);
    void start();
    void stop();

private:
    void initialize();
    void finalize();
    void run();

private:
    ScopedPtr<IConfigReader> m_cfgReader;
    std::list<IWorker> m_workerList;
    ScopedPtr<IParser> m_parser;
    ScopedPtr<IMemcached> m_memcached;
    ScopedPtr<IIOController> m_ioController;
};

#endif // EGINX_H
