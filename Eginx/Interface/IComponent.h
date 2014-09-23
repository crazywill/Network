#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include <string>

class IComponent
{
public:
    virtual void initialize() = 0;
    virtual void finalize() = 0;
    virtual std::string name() = 0;
};

#endif // ICOMPONENT_H
