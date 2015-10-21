#include <stdlib.h>

#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <linux/limits.h>

#include "include/capi/cef_app_capi.h"
#include "include/capi/cef_browser_process_handler_capi.h"
#include "include/capi/cef_client_capi.h"
#include "include/capi/cef_frame_capi.h"

#include "config.h"
#include "util.h"
#include "cef/base.h"
#include "cef/initializers.h"

static void configurenotify(struct Client *c, const XEvent *e);

struct Client *clients = NULL;

static void (*handler[LASTEvent]) (struct Client *, const XEvent *) = {
//	[ClientMessage] = clientmessage,
	[ConfigureNotify] = configurenotify,
//	[ConfigureRequest] = configurerequest,
//	[DestroyNotify] = destroynotify,
//	[KeyPress] = keypress,
//	[PropertyNotify] = propertynotify,
};

static void configurenotify(struct Client *c, const XEvent *e)
{
	const XConfigureEvent *ev = &e->xconfigure;
	XWindowChanges changes = {0};

	if (ev->window == c->win && c->cwin && c->host && c->host->notify_move_or_resize_started) {
		c->host->notify_move_or_resize_started(c->host);
		changes.width = ev->width;
		changes.height = ev->height;
		XConfigureWindow(c->dpy, c->cwin, CWHeight | CWWidth, &changes);
	}
}

static void *runx(void *arg)
{
	struct Client *c = (struct Client*)arg;
	cef_window_info_t windowInfo = {};

	char path[PATH_MAX] = "";
	getcwd(path, LENGTH(path));

	char url[PATH_MAX] = "file://";
	strcat(url, path);
	strcat(url, "/test.html");

	cef_string_t cefUrl = {};
	cef_browser_settings_t browserSettings = {.size = sizeof(cef_browser_settings_t)};
	XEvent ev;

	DEBUG_PRINT("------------ starting X handling thread --------------");

	if (!(c->dpy = XOpenDisplay(NULL)))
		die("Can't open display\n");
	c->scr = XDefaultScreen(c->dpy);
	c->vis = XDefaultVisual(c->dpy, c->scr);

	c->attrs.event_mask = 
		FocusChangeMask | 
		KeyPressMask |
		KeyReleaseMask | 
		ExposureMask |
		VisibilityChangeMask | 
		StructureNotifyMask | 
		ButtonMotionMask |
		ButtonPressMask | 
		ButtonReleaseMask;

	//	if (!(opt_embed && (parent = strtol(opt_embed, NULL, 0))))
	//		parent = XRootWindow(c->dpy, c->scr);
	c->win = XCreateWindow(c->dpy, XRootWindow(c->dpy, c->scr), 0, 0, 800, 600,
						   0, XDefaultDepth(c->dpy, c->scr), InputOutput,
						   c->vis, CWEventMask, &c->attrs);

	DEBUG_PRINT("WINID: %d", c->win);

	// request the window closed message so we can exit the thread
	Atom wm_delete_window = XInternAtom(c->dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(c->dpy, c->win, &wm_delete_window, 1);

	XMapWindow(c->dpy, c->win);
	XSync(c->dpy, False);

	windowInfo.parent_window = c->win;
	cef_string_utf8_to_utf16(url, strlen(url), &cefUrl);
	c->client = init_client();

	RINC(c->client);
	cef_browser_host_create_browser(&windowInfo, c->client, &cefUrl, &browserSettings, NULL);

	DEBUG_PRINT("browser creation requested");

	static int dumbCounter = 0;

	int running = 1;
	while(running) {
		XNextEvent(c->dpy, &ev);

		DEBUG_PRINT("event WINID: %d, %d", c->win, ev.type);
		if(handler[ev.type]) {
			(handler[ev.type])(c, &ev);
		}
		if (ev.type == 22) {
			// not sure what this is, but I'm using it to try running some javascript
			if (++dumbCounter == 20) {
				struct _cef_frame_t* const mainFrame = c->browser->get_main_frame(c->browser);

				DEBUG_PRINT("MAINFRAME PTR------------------ %d", mainFrame);

				const char szJS[] = "callFromHost('one', 'two', 'three');";
				cef_string_userfree_t js = cef_string_userfree_alloc();
				cef_string_from_ascii(szJS, strlen(szJS), js);

				mainFrame->execute_java_script(mainFrame, js, 0, 0);

				cef_string_userfree_free(js);

				//KAI: this correct?
				RDEC(mainFrame);
			}
		}
		if (ev.type == ClientMessage && ev.xclient.data.l[0] == wm_delete_window) {
			DEBUG_PRINT("destroying window");

			XDestroyWindow(c->dpy, c->win);
			running = 0;
		}
	}
	DEBUG_PRINT("exiting X event loop");

	XCloseDisplay(c->dpy);
	return NULL;
}

CEF_CALLBACK void browser_process_handler_on_context_initialized(struct _cef_browser_process_handler_t *self)
{
	struct Client *c;

	DEBUG_ONCE("");

	if (!(c = calloc(sizeof(struct Client), 1))) {
		eprintf("calloc failed:");
		return;
	}

	c->next = clients;
	clients = c;

	if (0 != (errno = pthread_create(&c->thread, NULL, &runx, (void*)c))) {
		eprintf("pthread_create failed:");
		return;
	}
}

CEF_CALLBACK void browser_process_handler_on_before_child_process_launch(struct _cef_browser_process_handler_t *self, struct _cef_command_line_t *command_line)
{
	DEBUG_ONCE("");
	RDEC(command_line);
}

CEF_CALLBACK void browser_process_handler_on_render_process_thread_created(struct _cef_browser_process_handler_t *self, struct _cef_list_value_t *extra_info)
{
	DEBUG_ONCE("");
	RDEC(extra_info);
}

CEF_CALLBACK struct _cef_print_handler_t *browser_process_handler_get_print_handler(struct _cef_browser_process_handler_t *self)
{
	DEBUG_ONCE("");
	return NULL;
}

struct _cef_browser_process_handler_t *init_browser_process_handler()
{
	struct _cef_browser_process_handler_t *ret = NULL;
	struct refcount *r = NULL;
	char *cp = NULL;

	DEBUG_ONCE("");
	if (!(r = calloc(sizeof(struct refcount) + sizeof(struct _cef_browser_process_handler_t), 1))) {
		DEBUG_PRINT("out of memory");
		return NULL;
	}

	cp = (char*)r;
	cp += sizeof(struct refcount);
	ret = (struct _cef_browser_process_handler_t*)cp;

	if(!init_base((cef_base_t*)ret, sizeof(struct _cef_browser_process_handler_t))) {
		free(r);
		return NULL;
	}
	ret->base.add_ref((cef_base_t*)ret);

	// callbacks
	ret->get_print_handler = &browser_process_handler_get_print_handler;
	ret->on_context_initialized = &browser_process_handler_on_context_initialized;
	ret->on_render_process_thread_created = &browser_process_handler_on_render_process_thread_created;
	ret->on_before_child_process_launch = &browser_process_handler_on_before_child_process_launch;

	return ret;
}
