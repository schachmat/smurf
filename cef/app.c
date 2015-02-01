#include <stdlib.h>

#include "include/capi/cef_app_capi.h"

#include "cef/base.h"
#include "util.h"

CEF_CALLBACK void app_on_before_command_line_processing(struct _cef_app_t *self, const cef_string_t *process_type, struct _cef_command_line_t *command_line)
{
	DEBUG_ONCE("app_on_before_command_line_processing");
}

CEF_CALLBACK void app_on_register_custom_schemes(struct _cef_app_t *self, struct _cef_scheme_registrar_t *registrar)
{
	DEBUG_ONCE("app_on_register_custom_schemes");
}

CEF_CALLBACK struct _cef_resource_bundle_handler_t *app_get_resource_bundle_handler(struct _cef_app_t *self)
{
	DEBUG_ONCE("app_get_resource_bundle_handler");
	return NULL;
}

CEF_CALLBACK struct _cef_browser_process_handler_t *app_get_browser_process_handler(struct _cef_app_t *self)
{
	DEBUG_ONCE("app_get_browser_process_handler");
	return NULL;
}

CEF_CALLBACK struct _cef_render_process_handler_t *app_get_render_process_handler(struct _cef_app_t *self)
{
	DEBUG_ONCE("app_get_render_process_handler");
	return NULL;
}

struct _cef_app_t *init_app()
{
	struct _cef_app_t *ret = NULL;
	struct refcount *r = NULL;
	char *cp = NULL;

	DEBUG_ONCE("init_app_handler\n");
	if (!(r = calloc(sizeof(struct refcount) + sizeof(struct _cef_app_t), 1))) {
		eprintf("out of memory");
		return NULL;
	}

	cp = (char*)r;
	cp += sizeof(struct refcount);
	ret = (struct _cef_app_t*)cp;

	init_base((cef_base_t*)ret, sizeof(struct _cef_app_t));
	ret->base.add_ref((cef_base_t*)ret);

	// callbacks
	ret->on_before_command_line_processing = &app_on_before_command_line_processing;
	ret->on_register_custom_schemes = &app_on_register_custom_schemes;
	ret->get_resource_bundle_handler = &app_get_resource_bundle_handler;
	ret->get_browser_process_handler = &app_get_browser_process_handler;
	ret->get_render_process_handler = &app_get_render_process_handler;

	return ret;
}
