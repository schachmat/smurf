#include <stdlib.h>
#include <X11/Xlib.h>

#include "include/capi/cef_client_capi.h"
#include "include/capi/cef_focus_handler_capi.h"
#include "include/capi/cef_keyboard_handler_capi.h"
#include "include/capi/cef_life_span_handler_capi.h"

#include "cef/base.h"
#include "cef/initializers.h"
#include "util.h"

/*******************************************************************************
 * CLIENT
 ******************************************************************************/

struct Client *client_parent(struct _cef_client_t *self)
{
	struct Client *c;
	for (c = clients; c; c = c->next)
		if (c->client == self)
			return c;
	return NULL;
}

CEF_CALLBACK struct _cef_context_menu_handler_t *client_get_context_menu_handler(struct _cef_client_t *self)
{
	DEBUG_ONCE("");
	return NULL;
}

CEF_CALLBACK struct _cef_dialog_handler_t *client_get_dialog_handler(struct _cef_client_t *self)
{
	DEBUG_ONCE("");
	return NULL;
}

CEF_CALLBACK struct _cef_display_handler_t *client_get_display_handler(struct _cef_client_t *self)
{
	DEBUG_ONCE("");
	return NULL;
}

CEF_CALLBACK struct _cef_download_handler_t *client_get_download_handler(struct _cef_client_t *self)
{
	DEBUG_ONCE("");
	return NULL;
}

CEF_CALLBACK struct _cef_drag_handler_t *client_get_drag_handler(struct _cef_client_t *self)
{
	DEBUG_ONCE("");
	return NULL;
}

CEF_CALLBACK struct _cef_focus_handler_t *client_get_focus_handler(struct _cef_client_t *self)
{
	static struct _cef_focus_handler_t *foch = NULL;

	DEBUG_ONCE("");

	if (!foch)
		foch = init_focus_handler();

	RINC(foch);
	return foch;
}

CEF_CALLBACK struct _cef_geolocation_handler_t *client_get_geolocation_handler(struct _cef_client_t *self)
{
	DEBUG_ONCE("");
	return NULL;
}

CEF_CALLBACK struct _cef_jsdialog_handler_t *client_get_jsdialog_handler(struct _cef_client_t *self)
{
	DEBUG_ONCE("");
	return NULL;
}

CEF_CALLBACK struct _cef_keyboard_handler_t *client_get_keyboard_handler(struct _cef_client_t *self)
{
	static struct _cef_keyboard_handler_t *kh = NULL;

	DEBUG_ONCE("");

	if (!kh)
		kh = init_keyboard_handler();

	RINC(kh);
	return kh;
}

CEF_CALLBACK struct _cef_life_span_handler_t *client_get_life_span_handler(struct _cef_client_t *self)
{
	struct Client *c;

	DEBUG_ONCE("");

	if (!(c = client_parent(self)))
		return NULL;

	if (!c->lsh)
		c->lsh = init_life_span_handler();

	RINC(c->lsh);
	return c->lsh;
}

CEF_CALLBACK struct _cef_load_handler_t *client_get_load_handler(struct _cef_client_t *self)
{
	DEBUG_ONCE("");
	return NULL;
}

CEF_CALLBACK struct _cef_render_handler_t *client_get_render_handler(struct _cef_client_t *self)
{
	DEBUG_ONCE("");
	return NULL;
}

CEF_CALLBACK struct _cef_request_handler_t *client_get_request_handler(struct _cef_client_t *self)
{
	DEBUG_ONCE("");
	return NULL;
}

CEF_CALLBACK int client_on_process_message_received(struct _cef_client_t *self, struct _cef_browser_t *browser, cef_process_id_t source_process, struct _cef_process_message_t *message)
{
	DEBUG_ONCE("");
	RDEC(browser);
	RDEC(message);
	return 0;
}

struct _cef_client_t *init_client()
{
	struct _cef_client_t *ret = NULL;
	struct refcount *r = NULL;
	char *cp = NULL;

	DEBUG_ONCE("init_client()");
	if (!(r = calloc(sizeof(struct refcount) + sizeof(struct _cef_client_t), 1))) {
		eprintf("out of memory");
		return NULL;
	}

	cp = (char*)r;
	cp += sizeof(struct refcount);
	ret = (struct _cef_client_t*)cp;

	if(!init_base((cef_base_t*)ret, sizeof(struct _cef_client_t))) {
		free(r);
		return NULL;
	}
	ret->base.add_ref((cef_base_t*)ret);

	// callbacks
	ret->get_life_span_handler = &client_get_life_span_handler;
	ret->get_keyboard_handler = &client_get_keyboard_handler;
	ret->get_dialog_handler = &client_get_dialog_handler;
	ret->get_load_handler = &client_get_load_handler;
	ret->get_render_handler = &client_get_render_handler;
	ret->get_request_handler = &client_get_request_handler;
	ret->get_jsdialog_handler = &client_get_jsdialog_handler;
	ret->get_context_menu_handler = &client_get_context_menu_handler;
	ret->get_drag_handler = &client_get_drag_handler;
	ret->get_display_handler = &client_get_display_handler;
	ret->get_download_handler = &client_get_download_handler;
	ret->on_process_message_received = &client_on_process_message_received;
	ret->get_focus_handler = &client_get_focus_handler;
	ret->get_geolocation_handler = &client_get_geolocation_handler;

	return ret;
}


/*******************************************************************************
 * FOCUS HANDLER
 ******************************************************************************/

CEF_CALLBACK void focus_handler_on_take_focus(struct _cef_focus_handler_t *self, struct _cef_browser_t *browser, int next)
{
	DEBUG_ONCE("");
}

CEF_CALLBACK int focus_handler_on_set_focus(struct _cef_focus_handler_t *self, struct _cef_browser_t *browser, cef_focus_source_t source)
{
	DEBUG_ONCE("");
	return 0;
}

CEF_CALLBACK void focus_handler_on_got_focus(struct _cef_focus_handler_t *self, struct _cef_browser_t *browser)
{
	cef_browser_host_t *h;

	DEBUG_ONCE("");

	if (browser && browser->get_host &&
		(h = browser->get_host(browser)) && h->set_focus) {
		h->set_focus(h, 1);
	}
}

struct _cef_focus_handler_t *init_focus_handler()
{
	struct _cef_focus_handler_t *ret = NULL;
	struct refcount *r = NULL;
	char *cp = NULL;

	DEBUG_ONCE("");
	if (!(r = calloc(sizeof(struct refcount) + sizeof(struct _cef_focus_handler_t), 1))) {
		eprintf("out of memory");
		return NULL;
	}

	cp = (char*)r;
	cp += sizeof(struct refcount);
	ret = (struct _cef_focus_handler_t*)cp;

	if(!init_base((cef_base_t*)ret, sizeof(struct _cef_focus_handler_t))) {
		free(r);
		return NULL;
	}
	ret->base.add_ref((cef_base_t*)ret);

	// callbacks
	ret->on_set_focus = &focus_handler_on_set_focus;
	ret->on_got_focus = &focus_handler_on_got_focus;
	ret->on_take_focus = &focus_handler_on_take_focus;

	return ret;
}


/*******************************************************************************
 * KEYBOARD HANDLER
 ******************************************************************************/

CEF_CALLBACK int keyboard_handler_on_pre_key_event(struct _cef_keyboard_handler_t *self, struct _cef_browser_t *browser, const struct _cef_key_event_t *event, XEvent *os_event, int *is_keyboard_shortcut)
{
//	DEBUG_PRINT("keyboard_handler_on_pre_key_event()");
	return 0;
}

CEF_CALLBACK int keyboard_handler_on_key_event(struct _cef_keyboard_handler_t *self, struct _cef_browser_t *browser, const struct _cef_key_event_t *event, XEvent *os_event)
{
//	DEBUG_PRINT("keyboard_handler_on_key_event()");
	if (event->focus_on_editable_field && event->type == KEYEVENT_RAWKEYDOWN)
		eprintf("%d %d", event->modifiers, event->native_key_code);
	return 0;
}

struct _cef_keyboard_handler_t *init_keyboard_handler()
{
	struct _cef_keyboard_handler_t *ret = NULL;
	struct refcount *r = NULL;
	char *cp = NULL;

	DEBUG_ONCE("");
	if (!(r = calloc(sizeof(struct refcount) + sizeof(struct _cef_keyboard_handler_t), 1))) {
		eprintf("out of memory");
		return NULL;
	}

	cp = (char*)r;
	cp += sizeof(struct refcount);
	ret = (struct _cef_keyboard_handler_t*)cp;

	if(!init_base((cef_base_t*)ret, sizeof(struct _cef_keyboard_handler_t))) {
		free(r);
		return NULL;
	}
	ret->base.add_ref((cef_base_t*)ret);

	// callbacks
	ret->on_key_event = &keyboard_handler_on_key_event;
	ret->on_pre_key_event = &keyboard_handler_on_pre_key_event;

	return ret;
}


/*******************************************************************************
 * LIFE SPAN HANDLER
 ******************************************************************************/

struct Client *life_span_handler_parent(struct _cef_life_span_handler_t *self)
{
	struct Client *c;
	for (c = clients; c; c = c->next)
		if (c->lsh == self)
			return c;
	return NULL;
}


CEF_CALLBACK int life_span_handler_on_before_popup(struct _cef_life_span_handler_t* self, struct _cef_browser_t* browser, struct _cef_frame_t* frame, const cef_string_t* target_url, const cef_string_t* target_frame_name, cef_window_open_disposition_t target_disposition, int user_gesture, const struct _cef_popup_features_t* popupFeatures, struct _cef_window_info_t* windowInfo, struct _cef_client_t** client, struct _cef_browser_settings_t* settings, int* no_javascript_access)
{
	DEBUG_ONCE("");
	return 0;
}

int g_browserInstances = 0;
CEF_CALLBACK void life_span_handler_on_after_created(struct _cef_life_span_handler_t *self, struct _cef_browser_t *browser)
{
	struct Client *c;

	++g_browserInstances;
	eprintf("life_span_handler_on_after_created() called, %d instances\n", g_browserInstances);

	if ((c = life_span_handler_parent(self))) {
		c->browser = browser;
		if (browser && browser->get_host) {
			c->host = browser->get_host(browser);
			if (c->host && c->host->get_window_handle)
				c->cwin = c->host->get_window_handle(c->host);
		}
	}
}

CEF_CALLBACK int life_span_handler_run_modal(struct _cef_life_span_handler_t *self, struct _cef_browser_t *browser)
{
	DEBUG_ONCE("");
	return 0;
}

CEF_CALLBACK int life_span_handler_do_close(struct _cef_life_span_handler_t *self, struct _cef_browser_t *browser)
{
	return 0;
}

CEF_CALLBACK void life_span_handler_on_before_close(struct _cef_life_span_handler_t *self, struct _cef_browser_t *browser)
{
	DEBUG_ONCE("");

	--g_browserInstances;
	eprintf("%d instances\n", g_browserInstances);

	if (g_browserInstances == 0) {
		DEBUG_PRINT("stopping cef message loop");
		cef_quit_message_loop();
	}
}

struct _cef_life_span_handler_t *init_life_span_handler()
{
	struct _cef_life_span_handler_t *ret = NULL;
	struct refcount *r = NULL;
	char *cp = NULL;

	DEBUG_ONCE("");
	if (!(r = calloc(sizeof(struct refcount) + sizeof(struct _cef_life_span_handler_t), 1))) {
		eprintf("out of memory");
		return NULL;
	}

	cp = (char*)r;
	cp += sizeof(struct refcount);
	ret = (struct _cef_life_span_handler_t*)cp;

	if(!init_base((cef_base_t*)ret, sizeof(struct _cef_life_span_handler_t))) {
		free(r);
		return NULL;
	}
	ret->base.add_ref((cef_base_t*)ret);

	// callbacks
	ret->on_before_close = &life_span_handler_on_before_close;
	ret->do_close = &life_span_handler_do_close;
	ret->on_before_popup = &life_span_handler_on_before_popup;
	ret->run_modal = &life_span_handler_run_modal;
	ret->on_after_created = &life_span_handler_on_after_created;

	return ret;
}
