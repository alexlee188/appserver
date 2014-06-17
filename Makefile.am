

bin_PROGRAMS = appserver

appserver_CFLAGS = -O3 -Wall -Wno-unused-function $(THREAD_DEBUG_CFLAGS) @SIMD_FLAGS@
appserver_LDADD = -lrt -lssl -lcrypto 

appserver_SOURCES = main.c\
         client.c\
client.h       \
main.h

