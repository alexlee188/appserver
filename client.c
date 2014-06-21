
// client.c

/* Copyright (C) 
* 2014 - Alex Lee
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/



#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <time.h>
#include <sys/timeb.h>
#include <samplerate.h>
#ifdef _OPENMP
#include <omp.h>
#endif
/* For fcntl */
#include <fcntl.h>

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "client.h"
#include "main.h"

static pthread_t client_thread_id;

#define BASE_PORT 8000
static int port=BASE_PORT;

#define BASE_PORT_SSL 9000
static int port_ssl=BASE_PORT_SSL;

#define MSG_SIZE 32

// Client_list is the HEAD of a queue of connected clients
TAILQ_HEAD(, _client_entry) Client_list;


static sem_t bufferevent_semaphore;

void* client_thread(void* arg);

void client_set_samples(char *client_samples, float* samples,int size);

//============================= xml codes ==========================
//#define MY_ENCODING "ISO-8859-1"
#define MY_ENCODING "UTF-8"

xmlBufferPtr testXmlwriterMemory(void);
xmlChar *ConvertInput(const char *in, const char *encoding);

xmlBufferPtr
testXmlwriterMemory()
{
    int rc;
    xmlTextWriterPtr writer;
    xmlBufferPtr buf;
    xmlChar *tmp;

    /* Create a new XML buffer, to which the XML document will be
     * written */
    buf = xmlBufferCreate();
    if (buf == NULL) {
        printf("testXmlwriterMemory: Error creating the xml buffer\n");
        return NULL;
    }

    /* Create a new XmlWriter for memory, with no compression.
     * Remark: there is no compression for this kind of xmlTextWriter */
    writer = xmlNewTextWriterMemory(buf, 0);
    if (writer == NULL) {
        printf("testXmlwriterMemory: Error creating the xml writer\n");
        return NULL;
    }

    /* Start the document with the xml default for the version,
     * encoding ISO 8859-1 and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartDocument\n");
        return NULL;
    }

    /* Start an element named "EXAMPLE". Since thist is the first
     * element, this will be the root element of the document. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "EXAMPLE");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
        return NULL;
    }

    /* Write a comment as child of EXAMPLE.
     * Please observe, that the input to the xmlTextWriter functions
     * HAS to be in UTF-8, even if the output XML is encoded
     * in iso-8859-1 */
    tmp = ConvertInput("This is a comment with special chars: <äöü>",
                       MY_ENCODING);
    rc = xmlTextWriterWriteComment(writer, tmp);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteComment\n");
        return NULL;
    }
    if (tmp != NULL) xmlFree(tmp);

    /* Start an element named "ORDER" as child of EXAMPLE. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ORDER");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
        return NULL;
    }

    /* Add an attribute with name "version" and value "1.0" to ORDER. */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "version",
                                     BAD_CAST "1.0");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteAttribute\n");
        return NULL;
    }

    /* Add an attribute with name "xml:lang" and value "de" to ORDER. */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xml:lang",
                                     BAD_CAST "de");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteAttribute\n");
        return NULL;
    }

    /* Write a comment as child of ORDER */
    tmp = ConvertInput("<äöü>", MY_ENCODING);
    rc = xmlTextWriterWriteFormatComment(writer,
		     "This is another comment with special chars: %s",
                                         tmp);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteFormatComment\n");
        return NULL;
    }
    if (tmp != NULL) xmlFree(tmp);

    /* Start an element named "HEADER" as child of ORDER. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "HEADER");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
        return NULL;
    }

    /* Write an element named "X_ORDER_ID" as child of HEADER. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "X_ORDER_ID",
                                         "%010d", 53535);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteFormatElement\n");
        return NULL;
    }

    /* Write an element named "CUSTOMER_ID" as child of HEADER. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "CUSTOMER_ID",
                                         "%d", 1010);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteFormatElement\n");
        return NULL;
    }

    /* Write an element named "NAME_1" as child of HEADER. */
    tmp = ConvertInput("Müller", MY_ENCODING);
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "NAME_1", tmp);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteElement\n");
        return NULL;
    }
    if (tmp != NULL) xmlFree(tmp);

    /* Write an element named "NAME_2" as child of HEADER. */
    tmp = ConvertInput("Jörg", MY_ENCODING);
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "NAME_2", tmp);

    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteElement\n");
        return NULL;
    }
    if (tmp != NULL) xmlFree(tmp);

    /* Close the element named HEADER. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
        return NULL;
    }

    /* Start an element named "ENTRIES" as child of ORDER. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ENTRIES");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
        return NULL;
    }

    /* Start an element named "ENTRY" as child of ENTRIES. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ENTRY");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
        return NULL;
    }

    /* Write an element named "ARTICLE" as child of ENTRY. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "ARTICLE",
                                   BAD_CAST "<Test>");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteElement\n");
        return NULL;
    }

    /* Write an element named "ENTRY_NO" as child of ENTRY. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "ENTRY_NO", "%d",
                                         10);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteFormatElement\n");
        return NULL;
    }

    /* Close the element named ENTRY. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
        return NULL;
    }

    /* Start an element named "ENTRY" as child of ENTRIES. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ENTRY");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
        return NULL;
    }

    /* Write an element named "ARTICLE" as child of ENTRY. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "ARTICLE",
                                   BAD_CAST "<Test 2>");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteElement\n");
        return NULL;
    }

    /* Write an element named "ENTRY_NO" as child of ENTRY. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "ENTRY_NO", "%d",
                                         20);
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteFormatElement\n");
        return NULL;
    }

    /* Close the element named ENTRY. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
        return NULL;
    }

    /* Close the element named ENTRIES. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
        return NULL;
    }

    /* Start an element named "FOOTER" as child of ORDER. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "FOOTER");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
        return NULL;
    }

    /* Write an element named "TEXT" as child of FOOTER. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "TEXT",
                                   BAD_CAST "This is a text.");
    if (rc < 0) {
        printf
            ("testXmlwriterMemory: Error at xmlTextWriterWriteElement\n");
        return NULL;
    }

    /* Close the element named FOOTER. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf("testXmlwriterMemory: Error at xmlTextWriterEndElement\n");
        return NULL;
    }

    /* Here we could close the elements ORDER and EXAMPLE using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work. */
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf("testXmlwriterMemory: Error at xmlTextWriterEndDocument\n");
        return NULL;
    }

    xmlFreeTextWriter(writer);

    return buf;
}

/**
 * ConvertInput:
 * @in: string in a given encoding
 * @encoding: the encoding used
 *
 * Converts @in into UTF-8 for processing with libxml2 APIs
 *
 * Returns the converted UTF-8 string, or NULL in case of error.
 */
xmlChar *
ConvertInput(const char *in, const char *encoding)
{
    xmlChar *out;
    int ret;
    int size;
    int out_size;
    int temp;
    xmlCharEncodingHandlerPtr handler;

    if (in == 0)
        return 0;

    handler = xmlFindCharEncodingHandler(encoding);

    if (!handler) {
        printf("ConvertInput: no encoding handler found for '%s'\n",
               encoding ? encoding : "");
        return 0;
    }

    size = (int) strlen(in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char *) xmlMalloc((size_t) out_size);

    if (out != 0) {
        temp = size - 1;
        ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
        if ((ret < 0) || (temp - size + 1)) {
            if (ret < 0) {
                printf("ConvertInput: conversion wasn't successful.\n");
            } else {
                printf
                    ("ConvertInput: conversion wasn't successful. converted: %i octets.\n",
                     temp);
            }

            xmlFree(out);
            out = 0;
        } else {
            out = (unsigned char *) xmlRealloc(out, out_size + 1);
            out[out_size] = 0;  /*null terminating out */
        }
    } else {
        printf("ConvertInput: no mem\n");
    }

    return out;
}


void client_init(int channel) {
    int rc;

    evthread_use_pthreads();

    TAILQ_INIT(&Client_list);

    sem_init(&bufferevent_semaphore,0,1);
    signal(SIGPIPE, SIG_IGN);

    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    port=BASE_PORT+channel;
    port_ssl = BASE_PORT_SSL + channel;
    rc=pthread_create(&client_thread_id,NULL,client_thread,NULL);

    if(rc != 0) {
        fprintf(stderr,"pthread_create failed on client_thread: rc=%d\n", rc);
    }
    else rc=pthread_detach(client_thread_id);
}

void do_accept(evutil_socket_t listener, short event, void *arg);
void readcb(struct bufferevent *bev, void *ctx);
void writecb(struct bufferevent *bev, void *ctx);

void errorcb(struct bufferevent *bev, short error, void *ctx)
{
    client_entry *item;

    if ((error & BEV_EVENT_EOF) || (error & BEV_EVENT_ERROR)) {
        /* connection has been closed, or error has occured, do any clean up here */
        /* ... */
            sem_wait(&bufferevent_semaphore);
            for (item = TAILQ_FIRST(&Client_list); item != NULL; item = TAILQ_NEXT(item, entries)){
	        if (item->bev == bev){
                    char ipstr[16];
                    inet_ntop(AF_INET, (void *)&item->client.sin_addr, ipstr, sizeof(ipstr));
                    fprintf(stderr, "Client disconnection from %s:%d\n",
                            ipstr, ntohs(item->client.sin_port));
                    TAILQ_REMOVE(&Client_list, item, entries);
                    free(item);
                    break;
	        }
            }
            sem_post(&bufferevent_semaphore);
            bufferevent_free(bev);
    	    int client_count = 0;
    	    sem_wait(&bufferevent_semaphore);
    	    /* NB: Clobbers item */
   	    TAILQ_FOREACH(item, &Client_list, entries){
        	client_count++;
   	    }
    	    sem_post(&bufferevent_semaphore);
	    if (client_count <= 1)
    	    	fprintf(stderr, "There is %d client\n", client_count);
	    else fprintf(stderr, "There are %d clients\n", client_count);

    } else if (error & BEV_EVENT_ERROR) {
        /* check errno to see what error occurred */
        /* ... */
        fprintf(stderr, "special EVUTIL_SOCKET_ERROR() %d: %s\n",  EVUTIL_SOCKET_ERROR(), evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()));
    } else if (error & BEV_EVENT_TIMEOUT) {
        /* must be a timeout event handle, handle it */
        /* ... */
    } else if (error & BEV_EVENT_CONNECTED){
        fprintf(stderr, "BEV_EVENT_CONNECTED: completed SSL handshake connection\n");
    }

}

void do_accept(evutil_socket_t listener, short event, void *arg){

    client_entry *item;
    struct event_base *base = arg;
    struct sockaddr_in ss;
    socklen_t slen = sizeof(ss);

    int fd = accept(listener, (struct sockaddr*)&ss, &slen);
    if (fd < 0) {
        fprintf(stderr, "accept failed\n");
        return;
    }
    char ipstr[16];
    // add newly connected client to Client_list
    item = malloc(sizeof(*item));
    memset(item, 0, sizeof(*item));
    memcpy(&item->client, &ss, sizeof(ss));

    inet_ntop(AF_INET, (void *)&item->client.sin_addr, ipstr, sizeof(ipstr));
    fprintf(stderr, "Client connection from %s:%d\n",
            ipstr, ntohs(item->client.sin_port));

    struct bufferevent *bev;
    evutil_make_socket_nonblocking(fd);
    evutil_make_socket_closeonexec(fd);
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE);
    bufferevent_setcb(bev, readcb, NULL, errorcb, NULL);
    bufferevent_setwatermark(bev, EV_READ, MSG_SIZE, 0);
    bufferevent_setwatermark(bev, EV_WRITE, 4096, 0);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
    item->bev = bev;
    sem_wait(&bufferevent_semaphore);
    TAILQ_INSERT_TAIL(&Client_list, item, entries);
    sem_post(&bufferevent_semaphore);

    int client_count = 0;
    sem_wait(&bufferevent_semaphore);
    /* NB: Clobbers item */
    TAILQ_FOREACH(item, &Client_list, entries){
        client_count++;
    }
    sem_post(&bufferevent_semaphore);
    if (client_count <= 1)
    	fprintf(stderr, "There is %d client\n", client_count);
    else fprintf(stderr, "There are %d clients\n", client_count);
}

/**
   Create a new SSL bufferevent to send its data over an SSL * on a socket.

   @param base An event_base to use to detect reading and writing
   @param fd A socket to use for this SSL
   @param ssl A SSL* object from openssl.
   @param state The current state of the SSL connection
   @param options One or more bufferevent_options
   @return A new bufferevent on success, or NULL on failure.
*/
struct bufferevent *
bufferevent_openssl_socket_new(struct event_base *base,
    evutil_socket_t fd,
    struct ssl_st *ssl,
    enum bufferevent_ssl_state state,
    int options);

static void
do_accept_ssl(struct evconnlistener *serv, int sock, struct sockaddr *sa,
             int sa_len, void *arg)
{
    struct event_base *evbase;
    struct bufferevent *bev;
    SSL_CTX *server_ctx;
    SSL *client_ctx;

    server_ctx = (SSL_CTX *)arg;
    client_ctx = SSL_new(server_ctx);
    evbase = evconnlistener_get_base(serv);
    evutil_make_socket_nonblocking(sock);
    bev = bufferevent_openssl_socket_new(evbase, sock, client_ctx,
                                         BUFFEREVENT_SSL_ACCEPTING,
                                         BEV_OPT_CLOSE_ON_FREE|BEV_OPT_THREADSAFE);

    client_entry *item;

    // add newly connected client to Client_list
    item = malloc(sizeof(*item));
    memset(item, 0, sizeof(*item));

    bufferevent_setcb(bev, readcb, NULL, errorcb, NULL);
    bufferevent_setwatermark(bev, EV_READ, MSG_SIZE, 0);
    bufferevent_setwatermark(bev, EV_WRITE, 4096, 0);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
    item->bev = bev;
    sem_wait(&bufferevent_semaphore);
    TAILQ_INSERT_TAIL(&Client_list, item, entries);
    sem_post(&bufferevent_semaphore);

    int client_count = 0;
    sem_wait(&bufferevent_semaphore);
    /* NB: Clobbers item */
    TAILQ_FOREACH(item, &Client_list, entries){
        client_count++;
    }
    sem_post(&bufferevent_semaphore);
    if (client_count <= 1)
    	    fprintf(stderr, "There is %d client\n", client_count);
    else fprintf(stderr, "There are %d clients\n", client_count);
    bufferevent_enable(bev, EV_READ);
    bufferevent_setcb(bev, readcb, NULL, errorcb, NULL);
}

SSL_CTX *evssl_init(void)
{
    SSL_CTX  *server_ctx;

    /* Initialize the OpenSSL library */
    SSL_load_error_strings();
    SSL_library_init();
    /* We MUST have entropy, or else there's no point to crypto. */
    if (!RAND_poll())
        return NULL;

    server_ctx = SSL_CTX_new(SSLv23_server_method());

    if (! SSL_CTX_use_certificate_chain_file(server_ctx, "cert") ||
        ! SSL_CTX_use_PrivateKey_file(server_ctx, "pkey", SSL_FILETYPE_PEM)) {
        puts("Couldn't read 'pkey' or 'cert' file.  To generate a key\n"
           "and self-signed certificate, run:\n"
           "  openssl genrsa -out pkey 2048\n"
           "  openssl req -new -key pkey -out cert.req\n"
           "  openssl x509 -req -days 365 -in cert.req -signkey pkey -out cert");
        return NULL;
    }
    SSL_CTX_set_options(server_ctx, SSL_OP_NO_SSLv2);

    return server_ctx;
}

static pthread_mutex_t *lock_cs;
static long *lock_count;

void pthreads_locking_callback(int mode, int type, char *file,
	     int line);
unsigned long pthreads_thread_id(void);

void thread_setup(void)
	{
	int i;

	lock_cs=OPENSSL_malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));
	lock_count=OPENSSL_malloc(CRYPTO_num_locks() * sizeof(long));
	for (i=0; i<CRYPTO_num_locks(); i++)
		{
		lock_count[i]=0;
		pthread_mutex_init(&(lock_cs[i]),NULL);
		}

	CRYPTO_set_id_callback((unsigned long (*)())pthreads_thread_id);
	CRYPTO_set_locking_callback((void (*)())pthreads_locking_callback);
	}

void thread_cleanup(void)
	{
	int i;

	CRYPTO_set_locking_callback(NULL);
	fprintf(stderr,"cleanup\n");
	for (i=0; i<CRYPTO_num_locks(); i++)
		{
		pthread_mutex_destroy(&(lock_cs[i]));
		fprintf(stderr,"%8ld:%s\n",lock_count[i],
			CRYPTO_get_lock_name(i));
		}
	OPENSSL_free(lock_cs);
	OPENSSL_free(lock_count);

	fprintf(stderr,"done cleanup\n");
	}

void pthreads_locking_callback(int mode, int type, char *file,
	     int line){
	if (mode & CRYPTO_LOCK)
		{
		pthread_mutex_lock(&(lock_cs[type]));
		lock_count[type]++;
		}
	else
		{
		pthread_mutex_unlock(&(lock_cs[type]));
		}
}

unsigned long pthreads_thread_id(void){
	unsigned long ret;

	ret=(unsigned long)pthread_self();
	return(ret);
}

void* client_thread(void* arg) {
 
    int on=1;
    struct event_base *base;
    struct event *listener_event;
    struct sockaddr_in server;
    int serverSocket;

    SSL_CTX *ctx;
    struct evconnlistener *listener;
    struct sockaddr_in server_ssl;

    fprintf(stderr,"client_thread\n");

    // setting up non-ssl open serverSocket
    serverSocket=socket(AF_INET,SOCK_STREAM,0);
    if(serverSocket==-1) {
        perror("client socket");
        return NULL;
    }

    evutil_make_socket_nonblocking(serverSocket);
    evutil_make_socket_closeonexec(serverSocket);

#ifndef WIN32
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
#endif

    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=htonl(INADDR_ANY);
    server.sin_port=htons(port);

    if(bind(serverSocket,(struct sockaddr *)&server,sizeof(server))<0) {
        perror("client bind");
        fprintf(stderr,"port=%d\n",port);
        return NULL;
    }

    fprintf(stderr, "client_thread: listening on port %d\n", port);

    if (listen(serverSocket, 5) == -1) {
	perror("client listen");
	exit(1);
    }

    // setting up ssl server
    memset(&server_ssl,0,sizeof(server_ssl));
    server_ssl.sin_family=AF_INET;
    server_ssl.sin_addr.s_addr=htonl(INADDR_ANY);
    server_ssl.sin_port=htons(port_ssl);

    ctx = evssl_init();
    if (ctx == NULL){
        perror("client ctx init failed");
        exit(1);
    }
    // setup openssl thread-safe callbacks
    thread_setup();

    // this is the Event base for both non-ssl and ssl servers
    base = event_base_new();

    // add the non-ssl listener to event base
    listener_event = event_new(base, serverSocket, EV_READ|EV_PERSIST, do_accept, (void*)base);
    event_add(listener_event, NULL);

    // add the ssl listener to event base
    listener = evconnlistener_new_bind(
                         base, do_accept_ssl, (void *)ctx,
                         LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE | 
                         LEV_OPT_THREADSAFE, 1024,
                         (struct sockaddr *)&server_ssl, sizeof(server_ssl));

    fprintf(stderr, "client_thread: listening on port %d for ssl connection\n", port_ssl);

    // this will be an endless loop to service all the network events
    event_base_loop(base, 0);

    // if for whatever reason the Event loop terminates, cleanup
    evconnlistener_free(listener);
    thread_cleanup();
    SSL_CTX_free(ctx);

    return NULL;
}

/* The maximum number of arguments a command can have and pass through
 * the tokenize_cmd tokenizer.  If you need more than this, bump it
 * up. */
#define MAX_CMD_TOKENS 10

/*
 * Tokenize the remaining words of a command, saving them to list and
 * returning the number of tokens found.  Do not attempt to find more
 * than 'tokens' tokens.
 */ 
static int tokenize_cmd(char **saveptr, char *list[], int tokens)
{
    int i = 0;
    char *token;

    if (tokens > MAX_CMD_TOKENS) {
        fprintf(stderr, "tokenize_cmd called with tokens > MAX_CMD_TOKENS\n");
        tokens = MAX_CMD_TOKENS;
    }
    for (i = 0; i < tokens && (token = strtok_r(NULL, " ", saveptr)); i++) {
        list[i] = token;
    }

    return i;
}


void readcb(struct bufferevent *bev, void *ctx){
//    char *cmd, *saveptr;
//    char *tokens[MAX_CMD_TOKENS];
//    int i;
    int bytesRead = 0;
    char message[MSG_SIZE];
    struct evbuffer *inbuf;
    xmlBufferPtr buf;
    const char* xml_string;

    /* The documentation for evbuffer_get_length is somewhat unclear as
     * to the actual definition of "length".  It appears to be the
     * amount of space *available* in the buffer, not occupied by data;
     * However, the code for reading from an evbuffer will read as many
     * bytes as it would return, so this behavior is not different from
     * what was here before. */
    inbuf = bufferevent_get_input(bev);
    while (evbuffer_get_length(inbuf) >= MSG_SIZE) {
        bytesRead = bufferevent_read(bev, message, MSG_SIZE);
        if (bytesRead != MSG_SIZE) {
            fprintf(stderr, "Short read from client; shouldn't happen\n");
            return;
            }
        message[bytesRead-1]=0;			// for Linux strings terminating in NULL
	}
    fprintf(stdout, "%s\n", message);
    buf = testXmlwriterMemory();
    xml_string = (const char*) buf->content;
    fprintf(stdout, "%s", xml_string);
    bufferevent_write(bev, xml_string, strlen(xml_string));
    xmlBufferFree(buf);
    

}

void printversion(){
	 fprintf(stderr,"dspserver string: %s\n",version);
}
