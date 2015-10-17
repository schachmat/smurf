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


int main(int argc, char** argv) {
	cef_main_args_t mainArgs = {};

	mainArgs.argc = argc;
	mainArgs.argv = argv;

	// Application handler and its callbacks.
	// cef_app_t structure must be filled. It must implement
	// reference counting. You cannot pass a structure
	// initialized with zeroes.
	DEBUG_PRINT("smurf::main(), init_app");
	app = init_app();

	// Execute subprocesses.
	RINC(app);
	int code = cef_execute_process(&mainArgs, app, NULL);
	if (code >= 0) {
		DEBUG_PRINT("cef_execute_process > 0, EXITING");
		return code;
	}
	DEBUG_PRINT("cef_execute_process RETURNED, initializing CEF");

	// Application settings.
	// It is mandatory to set the "size" member.
	cef_settings_t settings = {};
	settings.size = sizeof(cef_settings_t);
//	settings.no_sandbox = 1;

	// Initialize CEF.
//	printf("cef_initialize\n");
	RINC(app);
	cef_initialize(&mainArgs, &settings, app, NULL);

	// Message loop.
	DEBUG_ONCE("cef_run_message_loop");
	cef_run_message_loop();

	// Shutdown CEF.
	printf("cef_shutdown\n");
	cef_shutdown();

	return 0;
}
