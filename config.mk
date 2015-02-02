# Customize below to fit your system

# includes and libs
INCS = -I. -I/usr/include $(pkg-config --cflags gtk+-2.0)
LIBS = -L/usr/lib -lc -lcef $(pkg-config --libs gtk+-2.0)

# flags
CPPFLAGS =
CFLAGS = -std=c99 -Wall -O0 -g ${INCS} ${CPPFLAGS}
#CFLAGS = -std=c99 -pedantic -Wall -Os ${INCS} ${CPPFLAGS}
DLDFLAGS = -g -Wl,-rpath,. -Lbin/debug ${LIBS}
RLDFLAGS = -s -Wl,-rpath,. -Lbin/release ${LIBS}

# compiler and linker
CC = cc
