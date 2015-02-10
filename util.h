#ifndef _SMURF_UTIL_H
#define _SMURF_UTIL_H

#include <sys/types.h>
#include <unistd.h>

#include <gtk/gtk.h>

#include "include/capi/cef_browser_capi.h"

struct BrowserWin {
	cef_browser_t *browser;
};

typedef union {
	gboolean b;
	gint i;
	const void *v;
} Arg;

typedef struct {
	guint mod;
	guint keyval;
	void (*func)(struct BrowserWin *bw, const Arg *arg);
	const Arg arg;
} Key;

void eprintf(const char *format, ...);
void die(const char *msg);

struct BrowserWin c;

#define DEBUG_ONCE(str) do{ static int first_call = 1; if (first_call) { first_call = 0; eprintf("%d: debug_once: %s", getpid(), str); } }while(0)
#define DEBUG_PRINT(str) do{ eprintf("debug: %s", str); }while(0)
#define LENGTH(x) (sizeof x / sizeof x[0])
#define CLEANMASK(mask)         (mask & (MODKEY|GDK_SHIFT_MASK))

#endif
