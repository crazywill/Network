#ifndef ICONFIGREADER_H
#define ICONFIGREADER_H

#include "IComponent.h"
#include "Utility/PackDef.h"
#include <vector>


class IConfigReader : public IComponent
{
public:
    virtual ~IConfigReader(){}
    virtual void initialize() = 0;
    virtual void finalize() = 0;
    virtual std::string name() = 0;
    virtual std::vector<ListenSocket> listenSockets() = 0;
    virtual int workers() = 0;
private:

};

#endif // ICONFIGREADER_H
