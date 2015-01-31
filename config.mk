# Customize below to fit your system

# includes and libs
INCS = -I. -I/usr/include
LIBS = -L/usr/lib -lc -lcef

# flags
CPPFLAGS =
CFLAGS = -std=c99 -Wall -O0 -g ${INCS} ${CPPFLAGS}
#CFLAGS = -std=c99 -pedantic -Wall -Os ${INCS} ${CPPFLAGS}
DLDFLAGS = -g -Lbin/debug ${LIBS}
RLDFLAGS = -s -Lbin/release ${LIBS}

# compiler and linker
CC = cc
