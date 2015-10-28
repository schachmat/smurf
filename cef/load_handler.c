/**********************************************************
 * BEWARE: This file is a autogenerated stub!
 *********************************************************/

#include <stdlib.h>

#include "include/capi/cef_load_handler_capi.h"

#include "cef/base.h"
#include "util.h"

CEF_CALLBACK void load_handler_on_loading_state_change(struct _cef_load_handler_t *self, struct _cef_browser_t *browser, int isLoading, int canGoBack, int canGoForward)
{
	DEBUG_PRINT("isLoading %d, canGoBack %d, canGoForward %d", isLoading, canGoBack, canGoForward);
}

CEF_CALLBACK void load_handler_on_load_start(struct _cef_load_handler_t *self, struct _cef_browser_t *browser, struct _cef_frame_t *frame)
{
	DEBUG_PRINT("")
}

CEF_CALLBACK void load_handler_on_load_end(struct _cef_load_handler_t *self, struct _cef_browser_t *browser, struct _cef_frame_t *frame, int httpStatusCode)
{
	DEBUG_PRINT("httpStatusCode %d", httpStatusCode);
}

CEF_CALLBACK void load_handler_on_load_error(struct _cef_load_handler_t *self, struct _cef_browser_t *browser, struct _cef_frame_t *frame, cef_errorcode_t errorCode, const cef_string_t *errorText, const cef_string_t *failedUrl)
{
	DEBUG_PRINT("");
}

struct _cef_load_handler_t *init_load_handler()
{
	struct _cef_load_handler_t *ret = NULL;
	struct refcount *r = NULL;
	char *cp = NULL;

	DEBUG_ONCE("called");
	if (!(r = calloc(sizeof(struct refcount) + sizeof(struct _cef_load_handler_t), 1))) {
		DEBUG_PRINT("#### out of memory! #####");
		return NULL;
	}

	cp = (char*)r;
	cp += sizeof(struct refcount);
	ret = (struct _cef_load_handler_t*)cp;

	if(!init_base((cef_base_t*)ret, sizeof(struct _cef_load_handler_t))) {
		free(r);
		return NULL;
	}
	ret->base.add_ref((cef_base_t*)ret);

	// callbacks
	ret->on_load_error = &load_handler_on_load_error;
	ret->on_load_start = &load_handler_on_load_start;
	ret->on_loading_state_change = &load_handler_on_loading_state_change;
	ret->on_load_end = &load_handler_on_load_end;

	return ret;
}