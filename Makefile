include config.mk

# to implement an interface:
#   1) copy the auto-generated interface implementation C file from cef/stubs to cef/
#   2) add it to the list below to compile it
#   3) add the init_*() constructor function for the implementation to initializers.h, if you need it
SRC = smurf.c                                      \
      util.c                                       \
      cef/app.c                                    \
      cef/base.c                                   \
      cef/browser_process_handler.c                \
      cef/client.c                                 \
      cef/load_handler.c                           \
      cef/keyboard_handler.c                       \
      cef/render_process_handler.c                 \
      cef/v8handler.c                              \
      cef/stubs/resource_bundle_handler.c          \

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

${OBJ}: config.mk util.h cef/base.h cef/initializers.h

config.h:
	@echo creating $@ from config.def.h
	@cp config.def.h $@

smurf.d: ${OBJ}
	@echo CC -o bin/debug/$@
	@${CC} -o bin/debug/$@ ${OBJ} ${DLDFLAGS}

smurf.r: ${OBJ}
	@echo CC -o bin/release/$@
	@${CC} -o bin/release/$@ ${OBJ} ${RLDFLAGS}

clean:
	@echo cleaning
	@rm -f smurf ${OBJ}
	@rm -f cef/stubs/*.o

run: smurf.d
	@echo running debug version
	@bin/debug/smurf.d

.PHONY: all options smurf.d smurf.r clean run
