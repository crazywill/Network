#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include "Interface/IConfigReader.h"

class ConfigReader : public IConfigReader
{
public:
    ConfigReader(const std::string & path);
    ~ConfigReader();
    void initialize();
    void finalize();
    virtual std::string name();
    std::vector<ListenSocket> listenSockets();
    int workers();
private:
    std::vector<ListenSocket> m_listenSockets;
    int m_workers;
    std::string m_name;
    std::string m_path;
};

#endif // CONFIGREADER_H
