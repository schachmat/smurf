#ifndef _SMURF_UTIL_H
#define _SMURF_UTIL_H

#include <pthread.h>

struct refcount {
	size_t refs;
	pthread_mutex_t lock;
};

void eprintf(const char *format, ...);

#define DEBUG_ONCE(str) do{ static int first_call = 1; if (first_call) { first_call = 0; eprintf("debug_once: %s", str); } }while(0)
#define DEBUG(str) do{ eprintf("debug: %s", str); }while(0)

#endif
