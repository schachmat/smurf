include config.mk

SRC = smurf.c
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
	@${CC} -c ${CFLAGS} $<

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
