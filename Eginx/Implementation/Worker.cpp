#include "Worker.h"
#include "Utility/PubDefine.h"
Worker::Worker()
    :m_name(WORKER)
{
}

Worker::~Worker()
{

}

void Worker::initialize()
{

}

IEXecutor::ExecRet Worker::execute()
{
    return NormalReturn;
}

void Worker::finalize()
{

}

std::string Worker::name()
{
    return m_name;
}
