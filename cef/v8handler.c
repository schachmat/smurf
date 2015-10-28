#include <stdlib.h>
#include <string.h>

#include "include/capi/cef_v8_capi.h"

#include "cef/base.h"
#include "util.h"

CEF_CALLBACK int v8handler_execute(
	struct _cef_v8handler_t *self, 
	const cef_string_t *name, 
	struct _cef_v8value_t *object, 
	size_t argumentsCount, 
	struct _cef_v8value_t * const* arguments, 
	struct _cef_v8value_t **retval, 
	cef_string_t *exception)
{
	DEBUG_PRINT("received a V8 call");

	const char functionName[] = INJECTED_JS_FUNCTION_NAME;
	cef_string_userfree_t cefFunctionName = cef_string_userfree_alloc();
	cef_string_from_ascii(functionName, strlen(functionName), cefFunctionName);

	if (cef_string_utf16_cmp(cefFunctionName, name) == 0) {

		DEBUG_PRINT("found our function "INJECTED_JS_FUNCTION_NAME", %d arguments ", argumentsCount);
		for (int i = 0; i < argumentsCount; ++i) {

			struct _cef_v8value_t* const argValue = arguments[i];

			cef_string_userfree_t strValue = argValue->get_string_value(argValue);

			cef_string_userfree_utf8_t strValue_utf8 = cef_string_userfree_utf8_alloc();
			cef_string_utf16_to_utf8(strValue->str, strValue->length, strValue_utf8);

			DEBUG_PRINT("string: %s", strValue_utf8->str);

			cef_string_userfree_free(strValue);
			cef_string_userfree_utf8_free(strValue_utf8);
		}
	}
	cef_string_userfree_free(cefFunctionName);

	//KAI: fill in retval - have to increment its refcount?

	//KAI: clean up all references
	return 1;
}

struct _cef_v8handler_t *init_v8handler()
{
	struct _cef_v8handler_t *ret = NULL;
	struct refcount *r = NULL;
	char *cp = NULL;

	DEBUG_ONCE("called");
	if (!(r = calloc(sizeof(struct refcount) + sizeof(struct _cef_v8handler_t), 1))) {
		DEBUG_PRINT("#### out of memory! #####");;
		return NULL;
	}

	cp = (char*)r;
	cp += sizeof(struct refcount);
	ret = (struct _cef_v8handler_t*)cp;

	if(!init_base((cef_base_t*)ret, sizeof(struct _cef_v8handler_t))) {
		free(r);
		return NULL;
	}
	ret->base.add_ref((cef_base_t*)ret);

	// callbacks
	ret->execute = &v8handler_execute;

	return ret;
}