#ifndef IEXECUTOR_H
#define IEXECUTOR_H

class IEXecutor
{
public:
    virtual void initialize() = 0;
    virtual void execute() = 0;
    virtual void finalize() = 0;
};

#endif // IEXECUTOR_H
