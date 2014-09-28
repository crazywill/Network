#ifndef EGX_LOG_H
#define EGX_LOG_H

#include <string>
enum LogLevel
{
    Debug,
    Normal,
    Warning,
    Error,
    Invalid
};

#define LogDebug(params,...) Log(Debug,params,## __VA_ARGS__)
#define LogNormal(params,...) Log(Normal,params,## __VA_ARGS__)
#define LogWarning(params,...) Log(Warning,params,## __VA_ARGS__)
#define LogError(params,...) Log(Error,params,## __VA_ARGS__)

void Log(LogLevel level,char* params, ...);

#endif // LOG_H
