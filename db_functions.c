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
    my_bool reconnect_option = 1;

    con = mysql_init(NULL);

    if (con == NULL) 
    {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
    }

    if (mysql_options(con, MYSQL_OPT_RECONNECT, &reconnect_option)) 
    {
	finish_with_error(con);
    } 

//    if (mysql_real_connect(con, "localhost", "gcm_user", "gcm_user188", 
    if (mysql_real_connect(con, "gcmdbinstance.cegfhjvyp8lf.ap-southeast-1.rds.amazonaws.com", "gcm_user", "gcm_user188", 
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

int assign_job_to_user(char* job_id, char* gcm_regid){
	char buf[4096];
	MYSQL_RES *result;
	MYSQL_ROW row;

	// first check that this gcm_user has a verified status
	strcpy(buf, "select verified, balance from gcm_users where gcm_regid = '");
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
  
    	row = mysql_fetch_row(result);
	float bal = atof(row[1]);
	if ((strncmp(row[0], "yes", 3) != 0) || (bal < 2.0f)){
	    mysql_free_result(result);
	    finish_with_warning(con);
	    return -1;
	}


	mysql_free_result(result);

	// verified is "yes"

	strcpy(buf, "start transaction;");
    	if (mysql_query(con, buf)) {      
    		finish_with_warning(con);
		return -1;
    	}
	strcpy(buf, "update gcm_users set balance = balance - 2.00 where gcm_regid = '");
	strcat(buf, gcm_regid);
	strcat(buf, "';");
    	if (mysql_query(con, buf)) {      
    		finish_with_warning(con);
		return -1;
    	}
        if (mysql_affected_rows(con) != 1) 
        {
	    strcpy(buf, "rollback;");
    	    if (mysql_query(con, buf)) {      
    		finish_with_warning(con);
		return -1;
    	    }
            finish_with_warning(con);
	    return -1;
        }

	strcpy(buf, "update JOB set JOB_STATUS = 'requested', JOB_ASSIGNED_ID = (select id from gcm_users where gcm_regid = '");
	strcat(buf, gcm_regid);
	strcat(buf, "') where JOB_ID ='");
	strcat(buf, job_id);
	strcat(buf, "' and JOB_STATUS = 'open';");
    	if (mysql_query(con, buf)) {      
    		finish_with_warning(con);
		return -1;
    	}
        if (mysql_affected_rows(con) != 1) 
        {
	    strcpy(buf, "rollback;");
    	    if (mysql_query(con, buf)) {      
    		finish_with_warning(con);
		return -1;
    	    }
            finish_with_warning(con);
	    return -1;
        }

	strcpy(buf, "commit;");
    	if (mysql_query(con, buf)) {      
    		finish_with_warning(con);
		return -1;
    	}



	return 0;  // success
}

int insert_registration_to_db(char* name, char* gcm_regid, char* email, char* phone,
		char* NRIC, char* date_of_birth, char* gender, char* nurse_type,
		char* have_insurance){
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
	strcat(buf, "', NRIC='");
	strcat(buf, NRIC);
	strcat(buf, "', date_of_birth='");
	strcat(buf, date_of_birth);
	strcat(buf, "', gender='");
	strcat(buf, gender);
	strcat(buf, "', nurse_type='");
	strcat(buf, nurse_type);
	strcat(buf, "', have_insurance='");
	strcat(buf, have_insurance);
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

    	strcpy(buf, "insert into gcm_users(name, gcm_regid, email, phone, NRIC, ");
	strcat(buf, "date_of_birth, gender, nurse_type, have_insurance) values ('");
   	 strcat(buf, name);
    	strcat(buf, "','");
    	strcat(buf, gcm_regid);
	strcat(buf, "','");
	strcat(buf, email);
	strcat(buf, "','");
	strcat(buf, phone);
	strcat(buf, "','");
	strcat(buf, NRIC);
	strcat(buf, "','");
	strcat(buf, date_of_birth);
	strcat(buf, "','");
	strcat(buf, gender);
	strcat(buf, "','");
	strcat(buf, nurse_type);
	strcat(buf, "','");
	strcat(buf, have_insurance);
    	strcat(buf, "');");
    	if (mysql_query(con, buf)) {      
    		finish_with_warning(con);
    	}
    	return 0;  // success   
    }
}

