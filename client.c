
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
#include <math.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#ifdef _OPENMP
#include <omp.h>
#endif

/* For fcntl */
#include <fcntl.h>


#include "client.h"
#include "main.h"
#include "db_functions.h"
#include "xml_functions.h"

static pthread_t client_thread_id;

#define BASE_PORT 8000
static int port=BASE_PORT;

#define BASE_PORT_SSL 9000
static int port_ssl=BASE_PORT_SSL;

#define XML_HEADER_SIZE (38+4)

// Client_list is the HEAD of a queue of connected clients
TAILQ_HEAD(, _client_entry) Client_list;

void* client_thread(void* arg);

void client_init(int channel) {
    int rc;

    evthread_use_pthreads();

    TAILQ_INIT(&Client_list);

    signal(SIGPIPE, SIG_IGN);


    // initialise mySQL
    db_functions_init();
    
    // initialise xml
    xml_functions_init();

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
            bufferevent_free(bev);
    	    int client_count = 0;
    	    /* NB: Clobbers item */
   	    TAILQ_FOREACH(item, &Client_list, entries){
        	client_count++;
   	    }
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
    bufferevent_setwatermark(bev, EV_READ, XML_HEADER_SIZE, 0);
    bufferevent_setwatermark(bev, EV_WRITE, 4096, 0);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
    item->bev = bev;
    TAILQ_INSERT_TAIL(&Client_list, item, entries);

    int client_count = 0;
    /* NB: Clobbers item */
    TAILQ_FOREACH(item, &Client_list, entries){
        client_count++;
    }
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
    bufferevent_setwatermark(bev, EV_READ, XML_HEADER_SIZE, 0);
    bufferevent_setwatermark(bev, EV_WRITE, 4096, 0);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
    item->bev = bev;
    TAILQ_INSERT_TAIL(&Client_list, item, entries);

    int client_count = 0;
    /* NB: Clobbers item */
    TAILQ_FOREACH(item, &Client_list, entries){
        client_count++;
    }
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
    mysql_close(con);

    return NULL;
}

#define MSG_LENGTH 1024

void readcb(struct bufferevent *bev, void *ctx){
    struct evbuffer *inbuf;
    xmlBufferPtr buf;
    const char* xml_string;
    unsigned char *mem;
    char length[5]; // 4 bytes plus string terminator
    int message_length;
    char message[MSG_LENGTH];
    xmlTextReaderPtr reader;
    int ret;
    const char xml_insert_result_success[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<INSERT>success</INSERT>\0";
    const char xml_insert_result_fail[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<INSERT>fail</INSERT>\0";
    const char xml_assign_result_success[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<ASSIGN>success</ASSIGN>\0";
    const char xml_assign_result_fail[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<ASSIGN>fail</ASSIGN>\0";
    const char dummy_xml_jobs[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<JOBS></JOBS>\0";

    inbuf = bufferevent_get_input(bev);
    mem = evbuffer_pullup(inbuf, XML_HEADER_SIZE);
    if (mem == NULL){ // not enough data to process 4 bytes of length + xml header
	return;
    } else {
	memcpy(length, mem, 4);
        length[4] = 0;  // string terminating char
        message_length = atoi((const char*)length);
        fprintf(stderr, "Message Length = %d\n", message_length);
	if (message_length >= MSG_LENGTH){
		fprintf(stderr, "Message Length too long\n");
		evbuffer_drain(inbuf, MSG_LENGTH*2); // just remove as much as possible
		return;
	}
        mem = evbuffer_pullup(inbuf, message_length);
        if (mem == NULL) return;
        else {
	    memcpy(message, mem+4, message_length-4);
            message[message_length-4] = 0;
            evbuffer_drain(inbuf, message_length);
        }
    }
    fprintf(stdout, "%s\n", message);
    reader = xmlReaderForMemory(message, message_length-4, "noname.xml", NULL, 0);
    if (reader != NULL){
	ret = xmlTextReaderRead(reader);
	while (ret == 1){
	    const xmlChar *name, *value;
	    int type;
            name = xmlTextReaderConstName(reader);
	    type = xmlTextReaderNodeType(reader);
	    if ((type == 1) && (name != NULL) && (strncmp((char*)name, "QUERY", 5) == 0)){
		// check whether the QUERY comes with a gcm_regid attribute.  If it does
		// it is a GetJobs of jobs assigned to the particular user
		xmlChar * gcm_regid = xmlTextReaderGetAttribute(reader, BAD_CAST "gcm_regid");
		ret = xmlTextReaderRead(reader);  
		value = xmlTextReaderConstValue(reader);
		type = xmlTextReaderNodeType(reader); // next node should be a #TEXT
		if ((type == 3) && (value != NULL) && (strncmp((char*)value, "GetJobs", 7) == 0)){
    			buf = GetJobs((char*)gcm_regid);
			if (buf == NULL){ 
	    			xml_string = dummy_xml_jobs;
	    			sprintf(length, "%04d", (int)strlen(xml_string)+3);
	    			bufferevent_write(bev, length, 4);
	    			bufferevent_write(bev, xml_string, strlen(xml_string)-1);
			} else {
	    			xml_string = (const char*) buf->content;
	    			sprintf(length, "%04d", (int)strlen(xml_string)+3);
	    			bufferevent_write(bev, length, 4);
	    			bufferevent_write(bev, xml_string, strlen(xml_string)-1);
	    			xmlBufferFree(buf);
			}
		} else if ((type == 3) && (value != NULL) && (strncmp((char*)value, "GetAccount", 10) == 0)){
		// getting the account balance.  The QUERY should come with a gcm_reqid
			buf = GetAccount((char*)gcm_regid);
    			xml_string = (const char*) buf->content;
    			sprintf(length, "%04d", (int)strlen(xml_string)+3);
    			bufferevent_write(bev, length, 4);
    			bufferevent_write(bev, xml_string, strlen(xml_string)-1);
    			xmlBufferFree(buf);
		};
		if (gcm_regid != NULL) free(gcm_regid);
            }  // END QUERY
	    else if ((type == 1) && (name != NULL) && (strncmp((char*)name, "INSERT", 6) == 0)){
		xmlChar * name = xmlTextReaderGetAttribute(reader, BAD_CAST "name");
		xmlChar * gcm_regid = xmlTextReaderGetAttribute(reader, BAD_CAST "gcm_regid");
		xmlChar * email = xmlTextReaderGetAttribute(reader, BAD_CAST "email");
		xmlChar * phone = xmlTextReaderGetAttribute(reader, BAD_CAST "phone");
		xmlChar * NRIC = xmlTextReaderGetAttribute(reader, BAD_CAST "NRIC");
		xmlChar * date_of_birth = xmlTextReaderGetAttribute(reader, BAD_CAST "date_of_birth");
		xmlChar * gender = xmlTextReaderGetAttribute(reader, BAD_CAST "gender");
		xmlChar * nurse_type = xmlTextReaderGetAttribute(reader, BAD_CAST "nurse_type");
		xmlChar * have_insurance = xmlTextReaderGetAttribute(reader, BAD_CAST "have_insurance");
		if ((name != NULL) && (gcm_regid != NULL) && (email != NULL) &&
			(phone != NULL) && (NRIC != NULL) && (date_of_birth != NULL) &&
			(gender != NULL) && (nurse_type != NULL) && (have_insurance != NULL)){
		if (insert_registration_to_db((char*) name, (char*) gcm_regid,
			(char*) email, (char*) phone, (char*) NRIC, (char*) date_of_birth,
			(char*) gender, (char*) nurse_type, (char*) have_insurance
			) == 0){
			sprintf(length, "%04d", (int)strlen(xml_insert_result_success)+3);
			bufferevent_write(bev, length, 4);
			bufferevent_write(bev, xml_insert_result_success, strlen(xml_insert_result_success)-1);
			fprintf(stderr, "Insert/Update of gcm_regid, name, etc. successful\n");
		} else {
			sprintf(length, "%04d", (int)strlen(xml_insert_result_fail)+3);
			bufferevent_write(bev, length, 4);
			bufferevent_write(bev, xml_insert_result_fail, strlen(xml_insert_result_fail)-1);
			fprintf(stderr, "Insert/Update of gcm_regid, name, etc. failed\n");
		}
		
		if (name != NULL) free(name);
		if (gcm_regid != NULL) free(gcm_regid);
		if (email != NULL) free(email);
		if (phone != NULL) free(phone);
		if (NRIC != NULL) free(NRIC);
		if (date_of_birth != NULL) free(date_of_birth);
		if (gender != NULL) free (gender);
		if (nurse_type != NULL) free (nurse_type);
		if (have_insurance != NULL) free (have_insurance);
		}
	    } // END INSERT
	    else if ((type == 1) && (name != NULL) && (strncmp((char*)name, "ASSIGN", 6) == 0)){
		xmlChar * job_id = xmlTextReaderGetAttribute(reader, BAD_CAST "job_id");
		xmlChar * gcm_regid = xmlTextReaderGetAttribute(reader, BAD_CAST "gcm_regid");
		if ((job_id != NULL) && (gcm_regid != NULL)){
		if (assign_job_to_user((char*) job_id, (char*) gcm_regid) == 0){
			sprintf(length, "%04d", (int)strlen(xml_assign_result_success)+3);
			bufferevent_write(bev, length, 4);
			bufferevent_write(bev, xml_assign_result_success, strlen(xml_assign_result_success)-1);
		} else {
			sprintf(length, "%04d", (int)strlen(xml_assign_result_fail)+3);
			bufferevent_write(bev, length, 4);
			bufferevent_write(bev, xml_assign_result_fail, strlen(xml_assign_result_fail)-1);
		}
		
		if (job_id != NULL) free(job_id);
		if (gcm_regid != NULL) free(gcm_regid);
		}
	    }; // END ASSIGN
            ret = xmlTextReaderRead(reader);
        } // ret == 1
        xmlFreeTextReader(reader);
	if (ret != 0) return; // failed to parse xml

    } // reader != NULL
}

