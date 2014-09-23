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
    return m_listenSockets;
}

int ConfigReader::workers()
{
#ifdef Debug
    return 1;
#else
    return m_workers;
#endif
}
