#include <stdlib.h>
#include <string.h>

#include "include/capi/cef_render_process_handler_capi.h"
#include "include/capi/cef_v8_capi.h"

#include "cef/base.h"
#include "cef/initializers.h"
#include "util.h"

CEF_CALLBACK void render_process_handler_on_render_thread_created(struct _cef_render_process_handler_t *self, struct _cef_list_value_t *extra_info)
{
	DEBUG_ONCE("");
}

CEF_CALLBACK void render_process_handler_on_web_kit_initialized(struct _cef_render_process_handler_t *self)
{
	DEBUG_ONCE("");
}

CEF_CALLBACK void render_process_handler_on_browser_created(struct _cef_render_process_handler_t *self, struct _cef_browser_t *browser)
{
	DEBUG_ONCE("");
}

CEF_CALLBACK void render_process_handler_on_browser_destroyed(struct _cef_render_process_handler_t *self, struct _cef_browser_t *browser)
{
	DEBUG_ONCE("");
}

CEF_CALLBACK struct _cef_load_handler_t *render_process_handler_get_load_handler(struct _cef_render_process_handler_t *self)
{
	DEBUG_ONCE("");
	return NULL;
}

CEF_CALLBACK int render_process_handler_on_before_navigation(struct _cef_render_process_handler_t *self, struct _cef_browser_t *browser, struct _cef_frame_t *frame, struct _cef_request_t *request, cef_navigation_type_t navigation_type, int is_redirect)
{
	DEBUG_ONCE("");
	return 0;
}

CEF_CALLBACK void render_process_handler_on_context_created(
	struct _cef_render_process_handler_t *self, 
	struct _cef_browser_t *browser, 
	struct _cef_frame_t *frame, 
	struct _cef_v8context_t *context)
{
	DEBUG_PRINT("creating a v8handler for "INJECTED_JS_FUNCTION_NAME);

	// create a V8 handler
	struct _cef_v8handler_t* const handler = init_v8handler();

	// create a V8 function
	cef_string_userfree_t cefFunctionName = cef_string_userfree_alloc();
	cef_string_from_ascii(INJECTED_JS_FUNCTION_NAME, strlen(INJECTED_JS_FUNCTION_NAME), cefFunctionName);

	struct _cef_v8value_t* const function = cef_v8value_create_function(cefFunctionName, handler);

	// attach it to the window object
	struct _cef_v8value_t* const windowObject = context->get_global(context);
	windowObject->set_value_bykey(windowObject, cefFunctionName, function, V8_PROPERTY_ATTRIBUTE_NONE);

	cef_string_userfree_free(cefFunctionName);

	//KAI: release any other references like windowObject?
	//KAI: use RINC, RDEC, examples in app.c?
}

CEF_CALLBACK void render_process_handler_on_context_released(struct _cef_render_process_handler_t *self, struct _cef_browser_t *browser, struct _cef_frame_t *frame, struct _cef_v8context_t *context)
{
	DEBUG_ONCE("");
}

CEF_CALLBACK void render_process_handler_on_uncaught_exception(struct _cef_render_process_handler_t *self, struct _cef_browser_t *browser, struct _cef_frame_t *frame, struct _cef_v8context_t *context, struct _cef_v8exception_t *exception, struct _cef_v8stack_trace_t *stackTrace)
{
	DEBUG_ONCE("");
}

CEF_CALLBACK void render_process_handler_on_focused_node_changed(struct _cef_render_process_handler_t *self, struct _cef_browser_t *browser, struct _cef_frame_t *frame, struct _cef_domnode_t *node)
{
	DEBUG_ONCE("");
}

CEF_CALLBACK int render_process_handler_on_process_message_received(struct _cef_render_process_handler_t *self, struct _cef_browser_t *browser, cef_process_id_t source_process, struct _cef_process_message_t *message)
{
	DEBUG_ONCE("");
	return 0;
}

struct _cef_render_process_handler_t *init_render_process_handler()
{
	struct _cef_render_process_handler_t *ret = NULL;
	struct refcount *r = NULL;
	char *cp = NULL;

	DEBUG_ONCE("called");
	if (!(r = calloc(sizeof(struct refcount) + sizeof(struct _cef_render_process_handler_t), 1))) {
		DEBUG_PRINT("#### out of memory! #####");;
		return NULL;
	}

	cp = (char*)r;
	cp += sizeof(struct refcount);
	ret = (struct _cef_render_process_handler_t*)cp;

	if(!init_base((cef_base_t*)ret, sizeof(struct _cef_render_process_handler_t))) {
		free(r);
		return NULL;
	}
	ret->base.add_ref((cef_base_t*)ret);

	// callbacks
	ret->on_before_navigation = &render_process_handler_on_before_navigation;
	ret->on_browser_destroyed = &render_process_handler_on_browser_destroyed;
	ret->on_context_created = &render_process_handler_on_context_created;
	ret->on_context_released = &render_process_handler_on_context_released;
	ret->on_uncaught_exception = &render_process_handler_on_uncaught_exception;
	ret->on_browser_created = &render_process_handler_on_browser_created;
	ret->on_focused_node_changed = &render_process_handler_on_focused_node_changed;
	ret->on_web_kit_initialized = &render_process_handler_on_web_kit_initialized;
	ret->get_load_handler = &render_process_handler_get_load_handler;
	ret->on_render_thread_created = &render_process_handler_on_render_thread_created;
	ret->on_process_message_received = &render_process_handler_on_process_message_received;

	return ret;
}