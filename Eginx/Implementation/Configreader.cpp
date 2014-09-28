#include "Configreader.h"
#include "Utility/PubDefine.h"

ConfigReader::ConfigReader(const std::string &path)
    :m_workers(0)
    ,m_name(CONFIGREADER)
    ,m_path(path)
{
}

ConfigReader::~ConfigReader()
{

}

void ConfigReader::initialize()
{
    m_listenSockets.clear();
}

void ConfigReader::finalize()
{
    m_listenSockets.clear();
}

std::string ConfigReader::name()
{
    return m_name;
}

std::vector<ListenSocket> ConfigReader::listenSockets()
{
#ifdef TEST
    ListenSocket lsocket;
    lsocket.fd = -1;
    lsocket.family = AF_INET;
    lsocket.hostname = "localhost";
    lsocket.ip = "127.0.0.1";
    lsocket.port = 36500;
    m_listenSockets.push_back(lsocket);
    return m_listenSockets;
#else
    return m_listenSockets;
#endif
}

int ConfigReader::workers()
{
#ifdef TEST
    return 1;
#else
    return m_workers;
#endif
}
