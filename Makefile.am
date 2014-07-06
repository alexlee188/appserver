

bin_PROGRAMS = appserver

appserver_CFLAGS = -O3 -Wall -std=c99 -Wno-unused-function $(THREAD_DEBUG_CFLAGS) `mysql_config --cflags --libs`
appserver_CPPFLAGS = $(XML_CFLAGS)
appserver_LDADD = -lrt -lssl -lcrypto
appserver_LDFLAGS = $(XML_LIBS)
ACLOCAL_AMFLAGS = -I build-aux
appserver_SOURCES = main.c\
        client.c\
	db_functions.c \
	xml_functions.c \
	client.h      \
	main.h	\
	db_function.h \
	xml_functions.h


