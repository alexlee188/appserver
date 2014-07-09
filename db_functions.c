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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/types.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#include "db_functions.h"

MYSQL *con;

void db_functions_init(){
    con = mysql_init(NULL);

    if (con == NULL) 
    {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
    }

    if (mysql_real_connect(con, "gcmdbinstance.cegfhjvyp8lf.ap-southeast-1.rds.amazonaws.com", "gcm_user", "gcm_user188", 
    //if (mysql_real_connect(con, "localhost", "gcm_user", "gcm_user188",
          "gcm", 0, NULL, 0) == NULL) 
    {
	finish_with_error(con);
    }  
}

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}

void finish_with_warning(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));     
}

int insert_registration_to_db(char* name, char* gcm_regid, char* email, char* phone){
    char buf[4096];
    MYSQL_RES *result;

    strcpy(buf, "select * from gcm_users where gcm_regid = '");
    strcat(buf, gcm_regid);
    strcat(buf, "';");
    if (mysql_query(con, buf)) {      
    	finish_with_warning(con);
	return -1;
    }

    result = mysql_store_result(con);
    if (result == NULL) 
    {
        finish_with_warning(con);
	return -1;
    }
  
    if (mysql_fetch_row(result)){  // if not 0, gcm_regid is already in table
	strcpy(buf, "update gcm_users set name='");
	strcat(buf, name);
	strcat(buf, "', email='");
	strcat(buf, email);
	strcat(buf, "', phone='");
	strcat(buf, phone);
	strcat(buf, "' where gcm_regid = '");
	strcat(buf, gcm_regid);
	strcat(buf, "';");
    	if (mysql_query(con, buf)) {      
    		finish_with_warning(con);
		return -1;
    	}
	mysql_free_result(result);
	return 0; // success
    } else {

    	strcpy(buf, "insert into gcm_users(name, gcm_regid, email, phone) values ('");
   	 strcat(buf, name);
    	strcat(buf, "','");
    	strcat(buf, gcm_regid);
	strcat(buf, "','");
	strcat(buf, email);
	strcat(buf, "','");
	strcat(buf, phone);
    	strcat(buf, "');");
    	if (mysql_query(con, buf)) {      
    		finish_with_warning(con);
    	}
    	return 0;  // success   
    }
}

