#ifndef _SMURF_UTIL_H
#define _SMURF_UTIL_H

#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include <X11/Xlib.h>
#include <gtk/gtk.h>

#include "include/capi/cef_browser_capi.h"
#include "include/capi/cef_client_capi.h"
#include "include/capi/cef_life_span_handler_capi.h"

struct Client {
	Display *dpy;
	int scr;
	Visual *vis;
	Window win;
	Window cwin;
	XSetWindowAttributes attrs;
	cef_browser_t *browser;
	cef_browser_host_t *host;
	cef_client_t *client;
	cef_life_span_handler_t *lsh;
	pthread_t thread;
	struct Client *next;
};

typedef union {
	gboolean b;
	gint i;
	const void *v;
} Arg;

typedef struct {
	guint mod;
	guint keyval;
	void (*func)(struct Client *c, const Arg *arg);
	const Arg arg;
} Key;

void eprintf(const char *format, ...);
void die(const char *msg);

struct Client *clients;

#define DEBUG_ONCE(str) do{ static int first_call = 1; if (first_call) { first_call = 0; eprintf("%d: debug_once: %s", getpid(), str); } }while(0)
#define DEBUG_PRINT(str) do{ eprintf("%d: debug: %s", getpid(), str); }while(0)
#define LENGTH(x) (sizeof x / sizeof x[0])
#define CLEANMASK(mask)         (mask & (MODKEY|GDK_SHIFT_MASK))

#endif
