#ifndef WORKER_H
#define WORKER_H

#include "Interface/IExecutor.h"

class Worker : public IEXecutor
{
public:
    Worker();
    void initialize();
    void execute();
    void finalize();
};

#endif // WORKER_H
