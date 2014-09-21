#ifndef ICOMPONENT_H
#define ICOMPONENT_H

class IComponent
{
public:
    virtual void initialize() = 0;
    virtual void finalize() = 0;
};

#endif // ICOMPONENT_H
