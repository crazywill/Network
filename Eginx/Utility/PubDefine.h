#ifndef PUBDEFINE_H
#define PUBDEFINE_H

#include <sched.h>

#define LISTENQ 1024

#define CONFIGREADER "ConfigReader"
#define MASTER "Master"
#define WORKER "Worker"
#define EGXPROCMUTEX "EgxProcMutex"

#define SPIN 100

#define egx_cpu_pause()  __asm__ ("pause")
#define egx_sched_yield()  sched_yield() /*usleep(1)*/

#endif // PUBDEFINE_H
