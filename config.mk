# Customize below to fit your system

# includes and libs
INCS = -I. -I/usr/X11/include
LIBS = -L. -lcef -lc -lX11

# flags
CFLAGS = -std=c99 -Wall -O0 -g -pthread ${INCS}
#CFLAGS = -std=c99 -pedantic -Wall -Os ${INCS}
DLDFLAGS = -g -Wl,-rpath,. -pthread -Lbin/debug ${LIBS}
RLDFLAGS = -s -Wl,-rpath,. -pthread -Lbin/release ${LIBS}

# compiler and linker
CC = cc
