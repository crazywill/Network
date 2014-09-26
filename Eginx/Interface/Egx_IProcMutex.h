#ifndef EGX_IPROCMUTEX_H
#define EGX_IPROCMUTEX_H

#include "Utility/PubDefine.h"
class Egx_IProcMutex
{
public:
    virtual void lock(int spin = SPIN)=0;
    virtual void unlock()=0;
    virtual bool tryLock()=0;
    virtual int initialize()=0;
    virtual int finalize()=0;
};

#endif // EGX_IPROCMUTEX_H
