#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gtk.h"
#include "include/capi/cef_app_capi.h"
#include "include/capi/cef_client_capi.h"

#include "cef/base.h"
#include "cef/initializers.h"

// increment/decrement refcounters
#define RINC(p) ((cef_base_t*)p)->add_ref((cef_base_t*)p)
#define RDEC(p) ((cef_base_t*)p)->release((cef_base_t*)p)

int main(int argc, char** argv) {
	int i;
	// Main args.
	cef_main_args_t mainArgs = {};
	mainArgs.argc = argc;
	mainArgs.argv = argv;

	// Application handler and its callbacks.
	// cef_app_t structure must be filled. It must implement
	// reference counting. You cannot pass a structure
	// initialized with zeroes.
	cef_app_t *app = init_app();

	// Execute subprocesses.
	printf("cef_execute_process, argc=%d\n", argc);
	for(i = 0; i < argc; i++)
		printf("arg %d: %s\n", i, argv[i]);
	RINC(app);
	int code = cef_execute_process(&mainArgs, app, NULL);
	if (code >= 0) {
		return code;
	}

	// Application settings.
	// It is mandatory to set the "size" member.
	cef_settings_t settings = {};
	settings.size = sizeof(cef_settings_t);
	settings.no_sandbox = 1;

	// Initialize CEF.
	printf("cef_initialize\n");
	RINC(app);
	cef_initialize(&mainArgs, &settings, app, NULL);

	// Create GTK window. You can pass a NULL handle
	// to CEF and then it will create a window of its own.
	initialize_gtk();
	GtkWidget* hwnd = create_gtk_window("smurf", 1024, 768);
	cef_window_info_t windowInfo = {};
	windowInfo.parent_widget = hwnd;

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
	printf("cef_browser_host_create_browser\n");
	RINC(client);
	cef_browser_host_create_browser(&windowInfo, client, &cefUrl, &browserSettings, NULL);

	// Message loop.
	printf("cef_run_message_loop\n");
	cef_run_message_loop();

	// Shutdown CEF.
	printf("cef_shutdown\n");
	cef_shutdown();

	return 0;
}
