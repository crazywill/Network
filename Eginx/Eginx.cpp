#include "Eginx.h"

Eginx::Eginx()
{
}

void Eginx::initialize()
{
    //components and executor initialize
}

void Eginx::finalize()
{
    //components and executor finalize
}

void Eginx::run()
{
    m_executor->execute();
}
