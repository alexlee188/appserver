/* Copyright (C) 
* 2014 Alex Lee
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

const char *version = "20140617 - appserver"; //YYYYMMDD; text desc

// main.c

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/param.h>

#include "client.h"
#include "main.h"

char propertyPath[128];

enum {
    OPT_SOUNDCARD = 1,
    OPT_RECEIVER,
    OPT_SERVER,
    OPT_OFFSET,
    OPT_TIMING,
    OPT_LOOKUPCOUNTRY,
    OPT_SHARE,
    OPT_SHARECONFIG,
    OPT_LO,
    OPT_HPSDR,
    OPT_NOCORRECTIQ,
    OPT_DEBUG,
    OPT_THREAD_DEBUG,
    OPT_HPSDRLOC
};

struct option longOptions[] = {
    {"soundcard",required_argument, NULL, OPT_SOUNDCARD},
    {"receiver",required_argument, NULL, OPT_RECEIVER},
    {"server",required_argument, NULL, OPT_SERVER},
    {"offset",required_argument, NULL, OPT_OFFSET},
    {"timing",no_argument, NULL, OPT_TIMING},
    {"lookupcountry",no_argument, NULL, OPT_LOOKUPCOUNTRY},
    {"share",no_argument, NULL, OPT_SHARE},
    {"shareconfig",required_argument, NULL, OPT_SHARECONFIG},
    {"lo",required_argument, NULL, OPT_LO},
    {"hpsdr",no_argument, NULL, OPT_HPSDR},
    {"nocorrectiq",no_argument, NULL, OPT_NOCORRECTIQ},
    {"debug",no_argument, NULL, OPT_DEBUG},
    {"hpsdrloc",no_argument, NULL, OPT_HPSDRLOC},
#ifdef THREAD_DEBUG
    {"debug-threads",no_argument, NULL, OPT_THREAD_DEBUG},
#endif /* THREAD_DEBUG */
    {0,0,0,0}
};

char* shortOptions="";

void signal_shutdown(int signum);

/* --------------------------------------------------------------------------*/
/** 
* @brief Process program arguments 
* 
* @param argc
* @param argv
*/
/* ----------------------------------------------------------------------------*/
void processCommands(int argc,char** argv) {
    int c;
    while((c=getopt_long(argc,argv,shortOptions,longOptions,NULL))!=-1) {
        switch(c) {
            case OPT_SOUNDCARD:
		break;
            case OPT_RECEIVER:
                break;
            case OPT_SERVER:
                break;
            case OPT_OFFSET:
                break;
            case OPT_TIMING:
                break;
            case OPT_LOOKUPCOUNTRY:
                break;
            case OPT_SHARE:
                break;
            case OPT_SHARECONFIG:
                break;
            case OPT_LO:
                break;
            case OPT_HPSDR:
                break;
            case OPT_HPSDRLOC:
                break;
            case OPT_NOCORRECTIQ:
                break;
            case OPT_DEBUG:
                break;
            case OPT_THREAD_DEBUG:
                break;

       default:
                fprintf(stderr,"Usage: \n");
                fprintf(stderr,"  dspserver --receiver N (default 0)\n");
                fprintf(stderr,"            --server 0.0.0.0 (default 127.0.0.1)\n");
                fprintf(stderr,"            --soundcard (machine dependent)\n");
                fprintf(stderr,"            --offset 0 \n");
                fprintf(stderr,"            --share (will register this server for other users \n");
                fprintf(stderr,"                     use the default config file ~/.dspserver.conf) \n");
		fprintf(stderr,"            --lo 0 (if no LO offset desired in DDC receivers, or 9000 in softrocks\n");
		fprintf(stderr,"            --hpsdr (if using hpsdr hardware with no local mike and headphone)\n");
		fprintf(stderr,"            --hpsdrloc (if using hpsdr hardware with LOCAL mike and headphone)\n");
		fprintf(stderr,"            --nocorrectiq (select if using non QSD receivers, like Hermes, Perseus, HiQSDR, Mercury)\n");
#ifdef THREAD_DEBUG
                fprintf(stderr,"            --debug-threads (enable threading assertions)\n");
#endif /* THREAD_DEBUG */
                exit(1);

        }
    }
}


/* --------------------------------------------------------------------------*/
/** 
* @brief  Main - it all starts here
* 
* @param argc
* @param argv[]
* 
* @return 
*/
/* ----------------------------------------------------------------------------*/

int main(int argc,char* argv[]) {
    // Register signal and signal handler
    signal(SIGINT, signal_shutdown);    
    processCommands(argc,argv);

    client_init(0);

    while(1) {
        sleep(10000);
    }

    return 0;
}

void signal_shutdown(int signum)
{
   // catch a ctrl c etc
   printf("Caught signal %d\n",signum);

   // Terminate program
   exit(signum);
}
