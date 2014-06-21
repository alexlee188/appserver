// client.h

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

#if ! defined __CLIENT_H__
#define __CLIENT_H__

#include <sys/queue.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <event.h>
#include <event2/thread.h>
//#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/bufferevent_ssl.h>

struct _address {
	char number[20];
	char street1[40];
	char street2[40];
	char postcode[8];
	};

typedef struct _client_entry {
	struct sockaddr_in client;
	struct bufferevent * bev;
	char name[80];
	char phone[20];
	struct _address address;
	TAILQ_ENTRY(_client_entry) entries;
} client_entry;


void client_init(int receiver);

char servername[21];
static SSL_CTX *evssl_init(void);

#endif
