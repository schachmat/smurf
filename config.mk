# Customize below to fit your system

# includes and libs
INCS = -I.
LIBS = -L. -lcef -lX11

# flags
CFLAGS = -DDebug -std=c99 -Wall -O0 -g -pthread ${INCS}
DLDFLAGS = -g -Wl,-rpath,. -pthread -Lbin/debug ${LIBS}
RLDFLAGS = -s -Wl,-rpath,. -pthread -Lbin/release ${LIBS}

# compiler and linker
CC = gcc
