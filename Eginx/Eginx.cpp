#include "Eginx.h"
#include "Implementation/Master.h"
#include "Implementation/Worker.h"
#include "Implementation/Egx_ProcMutex.h"
Eginx::Eginx(const std::string &cfg)
    :m_configPath(cfg)
    ,m_procMutex(new Egx_ProcMutex())
{
}

void Eginx::initialize()
{
    //components and executor initialize
    m_executor.reset(new Master(m_configPath));
    m_executor->initialize();

}

void Eginx::finalize()
{
    //components and executor finalize
    if(m_executor.get()){
        m_executor->finalize();
    }
}

void Eginx::run()
{
    int ret;
    for(;;){
        ret = m_executor->execute();
        switch (ret) {
        case IExecutor::WorkerStart:
            m_executor->finalize();
            m_executor.reset(new Worker(static_cast<Master*>(m_executor.get())->ListenSockets(),m_procMutex));
            m_executor->initialize();
            break;
        case IExecutor::NormalReturn:
            //Do Nothing
            break;
        default:
            break;
        }
    }

}
