#ifndef _SMURF_UTIL_H
#define _SMURF_UTIL_H

#include <sys/types.h>
#include <unistd.h>

void eprintf(const char *format, ...);

#define DEBUG_ONCE(str) do{ static int first_call = 1; if (first_call) { first_call = 0; eprintf("%d: debug_once: %s", getpid(), str); } }while(0)
#define DEBUG_PRINT(str) do{ eprintf("debug: %s", str); }while(0)

#endif
