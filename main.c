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
    OPT_LOG = 1
};

struct option longOptions[] = {
//    {"log",required_argument, NULL, OPT_LOG},
    {"log",no_argument, NULL, OPT_LOG},
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
            case OPT_LOG:
	    break;
       default:
                fprintf(stderr,"Usage: \n");
                fprintf(stderr,"  appserver --log\n");
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
