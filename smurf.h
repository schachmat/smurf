#ifndef _SMURF_H
#define _SMURF_H

#include <gtk/gtk.h>

#include "include/capi/cef_app_capi.h"
#include "include/capi/cef_browser_process_handler_capi.h"

GtkWidget* root;
GtkWidget* win;

struct _cef_app_t *app;
struct _cef_browser_process_handler_t *browserh;

#endif
