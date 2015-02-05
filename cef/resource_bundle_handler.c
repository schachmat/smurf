#include <stdlib.h>

#include "include/capi/cef_resource_bundle_handler_capi.h"

#include "cef/base.h"
#include "util.h"

CEF_CALLBACK int resource_bundle_handler_get_localized_string(struct _cef_resource_bundle_handler_t *self, int message_id, cef_string_t *string)
{
	DEBUG_ONCE("resource_bundle_handler_get_localized_string() called");
	return 0;
}

CEF_CALLBACK int resource_bundle_handler_get_data_resource(struct _cef_resource_bundle_handler_t *self, int resource_id, void **data, size_t *data_size)
{
	DEBUG_ONCE("resource_bundle_handler_get_data_resource() called");
	return 0;
}

struct _cef_resource_bundle_handler_t *init_resource_bundle_handler()
{
	struct _cef_resource_bundle_handler_t *ret = NULL;
	struct refcount *r = NULL;
	char *cp = NULL;

	DEBUG_ONCE("init_resource_bundle_handler() called");
	if (!(r = calloc(sizeof(struct refcount) + sizeof(struct _cef_resource_bundle_handler_t), 1))) {
		eprintf("out of memory");
		return NULL;
	}

	cp = (char*)r;
	cp += sizeof(struct refcount);
	ret = (struct _cef_resource_bundle_handler_t*)cp;

	if(!init_base((cef_base_t*)ret, sizeof(struct _cef_resource_bundle_handler_t))) {
		free(r);
		return NULL;
	}
	ret->base.add_ref((cef_base_t*)ret);

	// callbacks
	ret->get_data_resource = &resource_bundle_handler_get_data_resource;
	ret->get_localized_string = &resource_bundle_handler_get_localized_string;

	return ret;
}
