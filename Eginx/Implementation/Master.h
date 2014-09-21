#ifndef MASTER_H
#define MASTER_H

#include "Interface/IExecutor.h"

class Master : public IEXecutor
{
public:
    Master();
    void initialize();
    void execute();
    void finalize();
};

#endif // MASTER_H
