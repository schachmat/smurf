#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

#include "include/capi/cef_app_capi.h"
#include "include/capi/cef_client_capi.h"

#include "smurf.h"
#include "util.h"
#include "cef/base.h"
#include "cef/initializers.h"


struct _cef_app_t *app;
struct _cef_browser_process_handler_t *browserh;


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
	app = init_app();

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
//	settings.no_sandbox = 1;

	// Initialize CEF.
	printf("cef_initialize\n");
	RINC(app);
	cef_initialize(&mainArgs, &settings, app, NULL);

	// Message loop.
	printf("cef_run_message_loop\n");
	cef_run_message_loop();

	// Shutdown CEF.
	printf("cef_shutdown\n");
	cef_shutdown();

	return 0;
}
