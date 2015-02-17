# Customize below to fit your system

# includes and libs
INCS = -I. -I/usr/include -I/usr/X11R6/include `pkg-config --cflags gtk+-2.0`
LIBS = -L/usr/lib -L/usr/X11R6/lib -lcef -lc -lX11 `pkg-config --libs gtk+-2.0`

# flags
CFLAGS = -std=c99 -Wall -O0 -g -pthread ${INCS}
#CFLAGS = -std=c99 -pedantic -Wall -Os ${INCS}
DLDFLAGS = -g -Wl,-rpath,. -pthread -Lbin/debug ${LIBS}
RLDFLAGS = -s -Wl,-rpath,. -pthread -Lbin/release ${LIBS}

# compiler and linker
CC = cc
