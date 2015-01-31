#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/capi/cef_app_capi.h"

int main(int argc, char** argv) {
	// Main args.
	cef_main_args_t mainArgs = {};
	mainArgs.argc = argc;
	mainArgs.argv = argv;

	// Application handler and its callbacks.
	// cef_app_t structure must be filled. It must implement
	// reference counting. You cannot pass a structure
	// initialized with zeroes.
	cef_app_t app = {};
	initialize_app_handler(&app);

	// Execute subprocesses.
	printf("cef_execute_process, argc=%d\n", argc);
	int code = cef_execute_process(&mainArgs, &app, NULL);
	if (code >= 0) {
		_exit(code);
	}

	// Application settings.
	// It is mandatory to set the "size" member.
	cef_settings_t settings = {};
	settings.size = sizeof(cef_settings_t);
	settings.no_sandbox = 1;

	// Initialize CEF.
	printf("cef_initialize\n");
	cef_initialize(&mainArgs, &settings, &app, NULL);

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
	cef_client_t client = {};
	initialize_client_handler(&client);

	// Create browser.
	printf("cef_browser_host_create_browser\n");
	cef_browser_host_create_browser(NULL, &client, &cefUrl, &browserSettings, NULL);

	// Message loop.
	printf("cef_run_message_loop\n");
	cef_run_message_loop();

	// Shutdown CEF.
	printf("cef_shutdown\n");
	cef_shutdown();

	return 0;
}
