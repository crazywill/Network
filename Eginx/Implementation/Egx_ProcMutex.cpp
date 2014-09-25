#include "Egx_ProcMutex.h"
#include "Utility/PubDefine.h"
#include <bits/fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
Egx_ProcMutex::Egx_ProcMutex()
    :m_shmfd(-1)
    ,m_shmptr(NULL)
    ,m_mutex(0)
{
    shm_unlink(EGXPROCMUTEX);
    m_shmfd = shm_open(EGXPROCMUTEX,O_RDWR|O_CREAT,
                       S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
    ftruncate(m_shmfd,sizeof(m_mutex));
}

int Egx_ProcMutex::initialize()
{
    m_shmptr = (int *) mmap(NULL,sizeof(m_mutex),PROT_READ|PROT_WRITE,MAP_SHARED,m_shmfd,0);
//    if(m_shmptr){
//        *m_shmptr = 0;
//    }
    return m_shmptr==NULL?-1:0;
}

int Egx_ProcMutex::finalize()
{
    munmap(m_shmptr,sizeof(m_mutex));
}

void Egx_ProcMutex::lock()
{
}

void Egx_ProcMutex::unlock()
{
}

bool Egx_ProcMutex::tryLock()
{
}


