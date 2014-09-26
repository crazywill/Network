#ifndef IEXECUTOR_H
#define IEXECUTOR_H

#include <string>

class IExecutor
{
public:
    enum ExecRet{
        WorkerStart,
        NormalReturn,
        Invalid
    };
    virtual ~IExecutor(){}
    virtual void initialize() = 0;
    virtual ExecRet execute() = 0;
    virtual void finalize() = 0;
    virtual std::string name() = 0;
};

#endif // IEXECUTOR_H
