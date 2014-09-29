#include "Utility/Egx_Log.h"
#include <stdarg.h>
#include <stdio.h>
void Log(LogLevel level, const char *params, ...)
{
    va_list args;
    va_start(args,params);
    char str[1024];
    vsnprintf(str, 1024 - 1, params, args);
    switch (level) {
    case Debug:
//        printf("[%ld][%s]: %s",getpid(),"Debug",str);
        break;
    case Normal:
        printf("[%ld][%s]: %s",getpid(),"Normal",str);
        break;
    case Warning:
        printf("[%ld][%s]: %s",getpid(),"Warning",str);
        break;
    case Error:
        printf("[%ld][%s]: %s",getpid(),"Error",str);
        break;
    case Invalid:
        break;
    default:
        break;
    }
    va_end(args);
}
