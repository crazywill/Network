#include "Egx_ProcMutex.h"
#include "Utility/PubDefine.h"
#include "Utility/Egx_Log.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
Egx_ProcMutex::Egx_ProcMutex()
    :m_shmfd(-1)
    ,m_shmptr(NULL)
    ,m_mutex(0)
{
    LogDebug("Egx_ProcMutex::%s\n",__FUNCTION__);
    shm_unlink(EGXPROCMUTEX);
    m_shmfd = shm_open(EGXPROCMUTEX,O_RDWR|O_CREAT,
                       S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    ftruncate(m_shmfd,sizeof(m_mutex));//The initialized memory is zero-filled.
}

int Egx_ProcMutex::initialize()
{
    LogDebug("Egx_ProcMutex::%s\n",__FUNCTION__);
    m_shmptr = (int *) mmap(NULL,sizeof(m_mutex),PROT_READ|PROT_WRITE,MAP_SHARED,m_shmfd,0);
    return m_shmptr==NULL?-1:0;
}

int Egx_ProcMutex::finalize()
{
    LogDebug("Egx_ProcMutex::%s\n",__FUNCTION__);
    return munmap(m_shmptr,sizeof(m_mutex));
}

void Egx_ProcMutex::lock(int spin)
{
    LogDebug("Egx_ProcMutex::%s\n",__FUNCTION__);
    for(;;)
    {
        if((*m_shmptr==0) && (__sync_bool_compare_and_swap(m_shmptr,0,getpid())))
        {
            return;
        }
        for(int i=0;i<spin;++i)
        {
            egx_cpu_pause();
        }
        if((*m_shmptr==0) && (__sync_bool_compare_and_swap(m_shmptr,0,getpid())))
        {
            return;
        }
        egx_sched_yield();
//        sys_sched_yield();
    }
}

void Egx_ProcMutex::unlock()
{
    LogDebug("Egx_ProcMutex::%s\n",__FUNCTION__);
    __sync_bool_compare_and_swap(m_shmptr,getpid(),0);
}

bool Egx_ProcMutex::tryLock()
{
    LogDebug("Egx_ProcMutex::%s\n",__FUNCTION__);
    return((m_shmptr) && (*m_shmptr==0) && (__sync_bool_compare_and_swap(m_shmptr,0,getpid())));
}


