#include "Egx_ProcMutex.h"
#include "Utility/PubDefine.h"
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
    shm_unlink(EGXPROCMUTEX);
    m_shmfd = shm_open(EGXPROCMUTEX,O_RDWR|O_CREAT,
                       S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    ftruncate(m_shmfd,sizeof(m_mutex));//The initialized memory is zero-filled.
}

int Egx_ProcMutex::initialize()
{
    m_shmptr = (int *) mmap(NULL,sizeof(m_mutex),PROT_READ|PROT_WRITE,MAP_SHARED,m_shmfd,0);
    return m_shmptr==NULL?-1:0;
}

int Egx_ProcMutex::finalize()
{
    return munmap(m_shmptr,sizeof(m_mutex));
}

void Egx_ProcMutex::lock(int spin)
{
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
    __sync_bool_compare_and_swap(m_shmptr,getpid(),0);
}

bool Egx_ProcMutex::tryLock()
{
    return((*m_shmptr==0) && (__sync_bool_compare_and_swap(m_shmptr,0,getpid())));
}


