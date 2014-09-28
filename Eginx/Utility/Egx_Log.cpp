#include "Utility/Egx_Log.h"
#include <stdarg.h>
#include <stdio.h>
void Log(LogLevel level, char *params, ...)
{
    va_list args;
    va_start(args,params);
    char str[1024];
    vsnprintf(str, 1024 - 1, params, args);
    switch (level) {
    case Debug:
        printf("%s: %s","Debug",str);
        break;
    case Normal:
        printf("%s: %s","Normal",str);
        break;
    case Warning:
        printf("%s: %s","Warning",str);
        break;
    case Error:
        printf("%s: %s","Error",str);
        break;
    case Invalid:
        break;
    default:
        break;
    }
    va_end(args);
}
