# Customize below to fit your system

# includes and libs
INCS = -I. -I/usr/include `pkg-config --cflags gtk+-2.0`
LIBS = -L/usr/lib -lcef -lc `pkg-config --libs gtk+-2.0`

# flags
CFLAGS = -std=c99 -Wall -O0 -g ${INCS}
#CFLAGS = -std=c99 -pedantic -Wall -Os ${INCS}
DLDFLAGS = -g -Wl,-rpath,. -Lbin/debug ${LIBS}
RLDFLAGS = -s -Wl,-rpath,. -Lbin/release ${LIBS}

# compiler and linker
CC = cc
