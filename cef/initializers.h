#ifndef _SMURF_CEF_INITIALIZERS_H
#define _SMURF_CEF_INITIALIZERS_H

#include "include/capi/cef_app_capi.h"

// forward decl's of constructors for our interface implementations
struct _cef_app_t *init_app();
struct _cef_client_t *init_client();
struct _cef_browser_process_handler_t *init_browser_process_handler();
struct _cef_render_process_handler_t *init_render_process_handler();
struct _cef_resource_bundle_handler_t *init_resource_bundle_handler();
struct _cef_focus_handler_t *init_focus_handler();
struct _cef_keyboard_handler_t *init_keyboard_handler();
struct _cef_life_span_handler_t *init_life_span_handler();
struct _cef_v8handler_t *init_v8handler();

#endif
