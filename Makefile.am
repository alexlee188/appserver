

bin_PROGRAMS = appserver

appserver_CFLAGS = -O3 -Wall -Wno-unused-function $(THREAD_DEBUG_CFLAGS) @SIMD_FLAGS@
appserver_CPPFLAGS = $(XML_CFLAGS)
appserver_LDADD = -lrt -lssl -lcrypto
appserver_LDFLAGS = $(XML_LIBS)
ACLOCAL_AMFLAGS = -I build-aux
appserver_SOURCES = main.c\
         client.c\
client.h       \
main.h

