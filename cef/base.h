#ifndef _SMURF_REFCOUNT_H
#define _SMURF_REFCOUNT_H

#include "include/capi/cef_base_capi.h"

CEF_CALLBACK void add_ref(struct _cef_base_t* self);
CEF_CALLBACK int release(struct _cef_base_t* self);
CEF_CALLBACK int has_one_ref(struct _cef_base_t* self);
int init_base(struct _cef_base_t* self, size_t sz);

#endif
