#include <stdlib.h>

#include "include/capi/cef_app_capi.h"

#include "smurf.h"
#include "cef/base.h"
#include "cef/initializers.h"
#include "util.h"

CEF_CALLBACK void app_on_before_command_line_processing(struct _cef_app_t *self, const cef_string_t *process_type, struct _cef_command_line_t *command_line)
{
	DEBUG_ONCE("");
	RDEC(command_line);
}

CEF_CALLBACK void app_on_register_custom_schemes(struct _cef_app_t *self, struct _cef_scheme_registrar_t *registrar)
{
	DEBUG_ONCE("");
	RDEC(registrar);
}

CEF_CALLBACK struct _cef_resource_bundle_handler_t *app_get_resource_bundle_handler(struct _cef_app_t *self)
{
	static struct _cef_resource_bundle_handler_t *resh = NULL;

	DEBUG_ONCE("");

	if (!resh)
		resh = init_resource_bundle_handler();

	RINC(resh);
	return resh;
}

CEF_CALLBACK struct _cef_browser_process_handler_t *app_get_browser_process_handler(struct _cef_app_t *self)
{
	static struct _cef_browser_process_handler_t *browserh = NULL;

	DEBUG_ONCE("");

	if (!browserh)
		browserh = init_browser_process_handler();

	RINC(browserh);
	return browserh;
}

CEF_CALLBACK struct _cef_render_process_handler_t *app_get_render_process_handler(struct _cef_app_t *self)
{
	DEBUG_ONCE("");
	return NULL;
}

struct _cef_app_t *init_app()
{
	struct _cef_app_t *ret = NULL;
	struct refcount *r = NULL;
	char *cp = NULL;

	DEBUG_ONCE("");
	if (!(r = calloc(sizeof(struct refcount) + sizeof(struct _cef_app_t), 1))) {
		DEBUG_PRINT("#### out of memory! #####");;
		return NULL;
	}

	cp = (char*)r;
	cp += sizeof(struct refcount);
	ret = (struct _cef_app_t*)cp;

	if(!init_base((cef_base_t*)ret, sizeof(struct _cef_app_t))) {
		free(r);
		return NULL;
	}
	ret->base.add_ref((cef_base_t*)ret);

	// callbacks
	ret->on_register_custom_schemes = &app_on_register_custom_schemes;
	ret->on_before_command_line_processing = &app_on_before_command_line_processing;
	ret->get_resource_bundle_handler = &app_get_resource_bundle_handler;
	ret->get_render_process_handler = &app_get_render_process_handler;
	ret->get_browser_process_handler = &app_get_browser_process_handler;

	return ret;
}
