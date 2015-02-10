#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>

#include "include/capi/cef_app_capi.h"
#include "include/capi/cef_browser_process_handler_capi.h"
#include "include/capi/cef_client_capi.h"

#include "config.h"
#include "smurf.h"
#include "util.h"
#include "cef/base.h"
#include "cef/initializers.h"

static struct {
	Display *dpy;
	Colormap cmap;
	Window win;
//	GtkWidget* win;
	GtkWidget* vpan;
	GtkWidget* view;
	Drawable buf;
	Atom xembed, wmdeletewin, netwmname, netwmpid;
	XIM xim;
	XIC xic;
//	Draw draw;
	Visual *vis;
	XSetWindowAttributes attrs;
	int scr;
//	bool isfixed; /* is fixed geometry? */
	int l, t; /* left and top offset */
	int gm; /* geometry mask */
	int w, h; /* window width and height */
} xw;

void app_terminate_signal(int signal)
{
	DEBUG_PRINT("app_terminate_signal() called");
	cef_quit_message_loop();
}

void window_destroy_signal(GtkWidget* widget, gpointer data)
{
	DEBUG_PRINT("window_destroy_signal() called");
	cef_quit_message_loop();
}

gboolean window_focus_in(GtkWidget* widget, GdkEventFocus* event, gpointer data)
{
	cef_browser_host_t *h;
	if (event->in && c.browser && c.browser->get_host &&
		(h = c.browser->get_host(c.browser)) && h->set_focus) {
		h->set_focus(h, 1);
		return TRUE;
	}
	return FALSE;
}


void size_alloc(GtkWidget* widget, GtkAllocation* allocation, void* data)
{
	cef_browser_host_t *h;
	Window xwin;

	if (c.browser && c.browser->get_host && (h = c.browser->get_host(c.browser))
		&& h->get_window_handle && (xwin = h->get_window_handle(h))) {
		// Size the browser window to match the GTK widget.
		XWindowChanges changes = {0};
		changes.width = allocation->width;
		changes.height = allocation->height; // - devtools height?
		XConfigureWindow(cef_get_xdisplay(), xwin, CWHeight | CWWidth, &changes);
	}
}

//static gboolean keypress(GtkAccelGroup *group, GObject *obj, guint key, GdkModifierType mods, struct BrowserWin *c)
static gboolean keypress(GtkWidget *w, GdkEventKey *ev, gpointer *data)
{
	guint i;
	gboolean processed = FALSE;
	guint key = gdk_keyval_to_lower(ev->keyval);
	guint mods = CLEANMASK(ev->state);

//	updatewinid(c);
	eprintf("key %d pressed, state %d", key, mods);
	return FALSE;
	for (i = 0; i < LENGTH(keys); i++)
		if (key == keys[i].keyval && mods == keys[i].mod && keys[i].func) {
			keys[i].func(&c, &(keys[i].arg));
			processed = TRUE;
		}

	return processed;
}

CEF_CALLBACK void browser_process_handler_on_context_initialized(struct _cef_browser_process_handler_t *self)
{
	int i;
	GtkAccelGroup *group = gtk_accel_group_new();

	DEBUG_ONCE("browser_process_handler_on_context_initialized() called");

//	gtk_init(0, NULL);
//	xw.win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//	gtk_window_set_default_size(GTK_WINDOW(xw.win), 800, 600);
///	gdk_window_set_events(gtk_widget_get_window(GTK_WIDGET(xw.win)), GDK_BUTTON_PRESS_MASK);
//	g_signal_connect(xw.win, "destroy", G_CALLBACK(window_destroy_signal), NULL);
//	g_signal_connect(xw.win, "focus-in-event", G_CALLBACK(window_focus_in), NULL);
//
///	// add keybindings
///	for(i = 0; i < LENGTH(keys); i++)
///		gtk_accel_group_connect(group, keys[i].keyval, keys[i].mod, 0,
///		                        g_cclosure_new(G_CALLBACK(keypress), &c, NULL));
///	gtk_window_add_accel_group(GTK_WINDOW(xw.win), group);
//
//
//	xw.vpan = gtk_vpaned_new();
//	gtk_container_add(GTK_CONTAINER(xw.win), xw.vpan);
//	g_signal_connect(xw.vpan, "size-allocate", G_CALLBACK(size_alloc), NULL);
//
//	xw.view = gtk_drawing_area_new();
//	gtk_paned_pack1(GTK_PANED(xw.vpan), xw.view, TRUE, TRUE);
//	g_signal_connect(xw.view, "key-press-event", G_CALLBACK(keypress), NULL);


///	g_signal_connect(G_OBJECT(xw.win), "delete_event", G_CALLBACK(delete_event), xw.win);

//	gtk_widget_show_all(GTK_WIDGET(xw.win));

	if(!(xw.dpy = XOpenDisplay(NULL)))
		die("Can't open display\n");
	xw.scr = XDefaultScreen(xw.dpy);
	xw.vis = XDefaultVisual(xw.dpy, xw.scr);

	/* adjust fixed window geometry */
	xw.w = 800;
	xw.h = 600;
	xw.l = 0;
	xw.t = 0;

	/* Events */
	xw.attrs.bit_gravity = NorthWestGravity;
	xw.attrs.event_mask = FocusChangeMask | KeyPressMask
		| ExposureMask | VisibilityChangeMask | StructureNotifyMask
		| ButtonMotionMask | ButtonPressMask | ButtonReleaseMask;

//	if (!(opt_embed && (parent = strtol(opt_embed, NULL, 0))))
//		parent = XRootWindow(xw.dpy, xw.scr);
//	xw.win = XCreateWindow(xw.dpy, parent, xw.l, xw.t,
	xw.win = XCreateWindow(xw.dpy, XRootWindow(xw.dpy, xw.scr), xw.l, xw.t,
			xw.w, xw.h, 0, XDefaultDepth(xw.dpy, xw.scr), InputOutput,
			xw.vis, CWBackPixel | CWBorderPixel | CWBitGravity
			| CWEventMask | CWColormap, &xw.attrs);

	XMapWindow(xw.dpy, xw.win);
	XSync(xw.dpy, False);

	cef_window_info_t windowInfo = {};
//	windowInfo.parent_window = GDK_WINDOW_XID(gtk_widget_get_window(xw.view));
	windowInfo.parent_window = xw.win;

	// Initial url.
	char *url = "https://startpage.com/";
	// There is no _cef_string_t type.
	cef_string_t cefUrl = {};
	cef_string_utf8_to_utf16(url, strlen(url), &cefUrl);

	// Browser settings.
	// It is mandatory to set the "size" member.
	cef_browser_settings_t browserSettings = {};
	browserSettings.size = sizeof(cef_browser_settings_t);

	// Client handler and its callbacks.
	// cef_client_t structure must be filled. It must implement
	// reference counting. You cannot pass a structure
	// initialized with zeroes.
	cef_client_t *client = init_client();

	// Create browser.
	RINC(client);
	cef_browser_host_create_browser(&windowInfo, client, &cefUrl, &browserSettings, NULL);
}

CEF_CALLBACK void browser_process_handler_on_before_child_process_launch(struct _cef_browser_process_handler_t *self, struct _cef_command_line_t *command_line)
{
	DEBUG_ONCE("browser_process_handler_on_before_child_process_launch() called");
	RDEC(command_line);
}

CEF_CALLBACK void browser_process_handler_on_render_process_thread_created(struct _cef_browser_process_handler_t *self, struct _cef_list_value_t *extra_info)
{
	DEBUG_ONCE("browser_process_handler_on_render_process_thread_created() called");
	RDEC(extra_info);
}

CEF_CALLBACK struct _cef_print_handler_t *browser_process_handler_get_print_handler(struct _cef_browser_process_handler_t *self)
{
	DEBUG_ONCE("browser_process_handler_get_print_handler() called");
	return NULL;
}

struct _cef_browser_process_handler_t *init_browser_process_handler()
{
	struct _cef_browser_process_handler_t *ret = NULL;
	struct refcount *r = NULL;
	char *cp = NULL;

	DEBUG_ONCE("init_browser_process_handler() called");
	if (!(r = calloc(sizeof(struct refcount) + sizeof(struct _cef_browser_process_handler_t), 1))) {
		eprintf("out of memory");
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
