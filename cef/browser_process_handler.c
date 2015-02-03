#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>

#include "include/capi/cef_app_capi.h"
#include "include/capi/cef_browser_process_handler_capi.h"
#include "include/capi/cef_client_capi.h"

#include "smurf.h"
#include "util.h"
#include "cef/base.h"
#include "cef/initializers.h"

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

CEF_CALLBACK void browser_process_handler_on_context_initialized(struct _cef_browser_process_handler_t *self)
{
	DEBUG_ONCE("browser_process_handler_on_context_initialized() called");

	cef_window_info_t windowInfo = {};
//	windowInfo.parent_window = root;
//	windowInfo.window = win;

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
//	RDEC(command_line);
}

CEF_CALLBACK void browser_process_handler_on_render_process_thread_created(struct _cef_browser_process_handler_t *self, struct _cef_list_value_t *extra_info)
{
	DEBUG_ONCE("browser_process_handler_on_render_process_thread_created() called");
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
