// db_functions.h

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

#if ! defined __DB_FUNCTIONS_H__
#define __DB_FUNCTIONS_H__

typedef unsigned int uint;
typedef unsigned long ulong;
#include <mysql/my_global.h>
#include <mysql/mysql.h>

extern MYSQL *con;

void db_functions_init(void);
void finish_with_warning(MYSQL *);
void finish_with_error(MYSQL *);
int insert_registration_to_db(char*, char*, char*, char*, char*, char*, char*, char*, char*);
int assign_job_to_user(char*, char*);

#endif
