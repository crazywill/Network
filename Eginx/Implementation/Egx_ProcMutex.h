#ifndef EGX_PROCMUTEX_H
#define EGX_PROCMUTEX_H

#include "Interface/Egx_IProcMutex.h"
#include <sys/mman.h>
class Egx_ProcMutex : public Egx_IProcMutex
{
public:
    Egx_ProcMutex();
    int initialize();
    int finalize();
    void lock();
    void unlock();
    bool tryLock();
private:
    int m_shmfd;
    int *m_shmptr;
    int m_mutex;
};

#endif // EGX_PROCMUTEX_H
