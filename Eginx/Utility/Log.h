#ifndef LOG_H
#define LOG_H

enum LogLevel
{
    LogDebug,
    LogNormal,
    LogWarning,
    LogError,
    Invalid
};

void Log(LogLevel level, ...)
{
    switch (level) {
    case LogDebug:
        break;
    case LogNormal:
        break;
    case LogWarning:
        break;
    case LogError:
        break;
    case Invalid:
        break;
    default:
        break;
    }
}

#endif // LOG_H
