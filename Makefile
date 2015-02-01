include config.mk

SRC = smurf.c                                      \
      util.c                                       \
      cef/app.c                                    \
      cef/stubs/allow_certificate_error_callback.c \
      cef/stubs/auth_callback.c                    \
      cef/stubs/before_download_callback.c         \
      cef/stubs/binary_value.c                     \
      cef/stubs/browser.c                          \
      cef/stubs/browser_host.c                     \
      cef/stubs/browser_process_handler.c          \
      cef/stubs/callback.c                         \
      cef/stubs/client.c                           \
      cef/stubs/command_line.c                     \
      cef/stubs/completion_callback.c              \
      cef/stubs/context_menu_handler.c             \
      cef/stubs/context_menu_params.c              \
      cef/stubs/cookie_manager.c                   \
      cef/stubs/cookie_visitor.c                   \
      cef/stubs/dialog_handler.c                   \
      cef/stubs/dictionary_value.c                 \
      cef/stubs/display_handler.c                  \
      cef/stubs/domdocument.c                      \
      cef/stubs/domnode.c                          \
      cef/stubs/domvisitor.c                       \
      cef/stubs/download_handler.c                 \
      cef/stubs/download_item.c                    \
      cef/stubs/download_item_callback.c           \
      cef/stubs/drag_data.c                        \
      cef/stubs/drag_handler.c                     \
      cef/stubs/end_tracing_callback.c             \
      cef/stubs/file_dialog_callback.c             \
      cef/stubs/focus_handler.c                    \
      cef/stubs/frame.c                            \
      cef/stubs/geolocation_callback.c             \
      cef/stubs/geolocation_handler.c              \
      cef/stubs/get_geolocation_callback.c         \
      cef/stubs/jsdialog_callback.c                \
      cef/stubs/jsdialog_handler.c                 \
      cef/stubs/keyboard_handler.c                 \
      cef/stubs/life_span_handler.c                \
      cef/stubs/list_value.c                       \
      cef/stubs/load_handler.c                     \
      cef/stubs/menu_model.c                       \
      cef/stubs/navigation_entry.c                 \
      cef/stubs/navigation_entry_visitor.c         \
      cef/stubs/post_data.c                        \
      cef/stubs/post_data_element.c                \
      cef/stubs/print_dialog_callback.c            \
      cef/stubs/print_handler.c                    \
      cef/stubs/print_job_callback.c               \
      cef/stubs/print_settings.c                   \
      cef/stubs/process_message.c                  \
      cef/stubs/quota_callback.c                   \
      cef/stubs/read_handler.c                     \
      cef/stubs/render_handler.c                   \
      cef/stubs/render_process_handler.c           \
      cef/stubs/request.c                          \
      cef/stubs/request_context.c                  \
      cef/stubs/request_context_handler.c          \
      cef/stubs/request_handler.c                  \
      cef/stubs/resource_bundle_handler.c          \
      cef/stubs/resource_handler.c                 \
      cef/stubs/response.c                         \
      cef/stubs/run_file_dialog_callback.c         \
      cef/stubs/scheme_handler_factory.c           \
      cef/stubs/scheme_registrar.c                 \
      cef/stubs/stream_reader.c                    \
      cef/stubs/stream_writer.c                    \
      cef/stubs/string_visitor.c                   \
      cef/stubs/task.c                             \
      cef/stubs/task_runner.c                      \
      cef/stubs/urlrequest.c                       \
      cef/stubs/urlrequest_client.c                \
      cef/stubs/v8accessor.c                       \
      cef/stubs/v8context.c                        \
      cef/stubs/v8exception.c                      \
      cef/stubs/v8handler.c                        \
      cef/stubs/v8stack_frame.c                    \
      cef/stubs/v8stack_trace.c                    \
      cef/stubs/v8value.c                          \
      cef/stubs/web_plugin_info.c                  \
      cef/stubs/web_plugin_info_visitor.c          \
      cef/stubs/web_plugin_unstable_callback.c     \
      cef/stubs/write_handler.c                    \
      cef/stubs/xml_reader.c                       \
      cef/stubs/zip_reader.c                       \
      cef/base.c
#      cef/stubs/app.c                              \

OBJ = ${SRC:.c=.o}

all: options smurf.d

options:
	@echo smurf build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "DLDFLAGS = ${DLDFLAGS}"
	@echo "RLDFLAGS = ${RLDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $< -o $@

${OBJ}: config.mk

smurf.d: ${OBJ}
	@echo CC -o bin/debug/$@
	@${CC} -o bin/debug/$@ ${OBJ} ${DLDFLAGS}

smurf.r: ${OBJ}
	@echo CC -o bin/release/$@
	@${CC} -o bin/release/$@ ${OBJ} ${RLDFLAGS}

clean:
	@echo cleaning
	@rm -f smurf ${OBJ}

.PHONY: all options smurf.d smurf.r clean
