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
#include "xml_functions.h"


//============================= xml codes ==========================
//#define MY_ENCODING "ISO-8859-1"
#define MY_ENCODING "UTF-8"

xmlBufferPtr GetJobs(void);
xmlChar *ConvertInput(const char *in, const char *encoding);

xmlTextWriterPtr writer;

void xml_functions_init(){
    /*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

}

xmlBufferPtr GetJobs()
{
    int rc, i;

    xmlBufferPtr buf;
    xmlChar *tmp;

    /* Create a new XML buffer, to which the XML document will be
     * written */
    buf = xmlBufferCreate();
    if (buf == NULL) {
        printf("GetJobs: Error creating the xml buffer\n");
        return NULL;
    }

    /* Create a new XmlWriter for memory, with no compression.
     * Remark: there is no compression for this kind of xmlTextWriter */
    writer = xmlNewTextWriterMemory(buf, 0);
    if (writer == NULL) {
        printf("GetJobs: Error creating the xml writer\n");
        return NULL;
    }

    /* Start the document with the xml default for the version,
     * encoding ISO 8859-1 and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0) {
        printf
            ("GetJobs: Error at xmlTextWriterStartDocument\n");
        return NULL;
    }

    /* Start an element named "JOBS". Since thist is the first
     * element, this will be the root element of the document. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "JOBS");
    if (rc < 0) {
        printf
            ("GetJobs: Error at xmlTextWriterStartElement\n");
        return NULL;
    }

    /* Write a comment as child of JOBS.
     * Please observe, that the input to the xmlTextWriter functions
     * HAS to be in UTF-8, even if the output XML is encoded
     * in iso-8859-1 */
    tmp = ConvertInput("Jobs available",
                       MY_ENCODING);
    rc = xmlTextWriterWriteComment(writer, tmp);
    if (rc < 0) {
        printf
            ("GetJobs: Error at xmlTextWriterWriteComment\n");
        return NULL;
    }
    if (tmp != NULL) xmlFree(tmp);

    if (mysql_query(con, "select JOB_STATUS, JOB_ID, CUSTOMER_ID, ADDR_POSTCODE, JOB_DESC, JOB_NEED_1, JOB_NEED_2, JOB_NEED_3, JOB_START_TIME, JOB_DURATION from CUSTOMER natural join JOB")) {      
    	finish_with_error(con);
    }

    MYSQL_RES *result = mysql_store_result(con);
  
    if (result == NULL) 
    {
      finish_with_error(con);
    }

    int num_fields = mysql_num_fields(result);
    if (num_fields != 10){
	finish_with_error(con);
	}

    MYSQL_ROW row;
  
    while ((row = mysql_fetch_row(result))) 
    { 
      /* Start an element named "JOB" as child of JOBS. */
      rc = xmlTextWriterStartElement(writer, BAD_CAST "JOB");
      if (rc < 0) {
        printf
            ("GetJobs: Error at xmlTextWriterStartElement\n");
        return 0;
      }

      /* Add an attribute with name "status" and value to JOB. */
      tmp = ConvertInput(row[0]?row[0]:"NULL", MY_ENCODING);
      rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "STATUS",
                                     "%s", tmp);
      if (rc < 0) {
        printf
            ("GetJobs: Error at xmlTextWriterWriteFormatAttribute\n");
        return 0;
      }

      if (tmp != NULL) xmlFree(tmp);

      /* Write an element named "JOB_ID" as child of JOB. */
      rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "JOB_ID",
                                         "%s", row[1]?row[1]:"NULL");
      if (rc < 0) {
        printf
            ("GetJobs: Error at xmlTextWriterWriteFormatElement\n");
        return 0;
      }

      /* Write an element named "CUSTOMER_ID" as child of JOB. */
      rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "CUSTOMER_ID",
                                         "%s", row[2]?row[2]:"NULL");
      if (rc < 0) {
        printf
            ("GetJobs: Error at xmlTextWriterWriteFormatElement\n");
        return 0;
      }

      /* Write an element named "ADDR_POSTCODE" as child of JOB. */
      rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "ADDR_POSTCODE",
                                         "%s", row[3]?row[3]:"NULL");
      if (rc < 0) {
        printf
            ("GetJobs: Error at xmlTextWriterWriteFormatElement\n");
        return 0;
      }

      rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "JOB_DESC",
                                         "%s", row[4]?row[4]:"NULL");
      if (rc < 0) {
        printf
            ("GetJobs: Error at xmlTextWriterWriteFormatElement\n");
        return 0;
      }

      /* Start an element named "NEEDS" as child of JOB. */
      rc = xmlTextWriterStartElement(writer, BAD_CAST "NEEDS");
      if (rc < 0) {
        printf
            ("GetJobs: Error at xmlTextWriterStartElement\n");
        return 0;
      }

      for (i=5; i<8; i++){  // next 3 columns are JOB_NEED_1, JOB_NEED_2 and JOB_NEED_3
      /* Write an element named "NEED" as child of NEEDS. */
	if (row[i]){
    	  rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "NEED", "%s", row[i]?row[i]:"NULL");
    	  if (rc < 0) {
          printf
            ("GetJobs: Error at xmlTextWriterWriteFormatElement\n");
          return 0;
          }
	}
      }

      // end NEEDS
      rc = xmlTextWriterEndElement(writer);
      if (rc < 0) {
        printf
            ("GetJobs: Error at xmlTextWriterEndElement\n");
        return 0;
      }

    /* Write an element named "JOB_START_TIME" as child of JOB. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "JOB_START_TIME",
                                         "%s", row[8]?row[8]:"NULL");
    if (rc < 0) {
        printf
            ("GetJobs: Error at xmlTextWriterWriteFormatElement\n");
        return 0;
    }

    /* Write an element named "JOB_DURATION" as child of JOB. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "JOB_DURATION",
                                         "%s HR", row[9]?row[9]:"NULL");
    if (rc < 0) {
        printf
            ("GetJobs: Error at xmlTextWriterWriteFormatElement\n");
        return 0;
    }

    // end JOB
    rc = xmlTextWriterEndElement(writer);

    if (rc < 0) {
        printf
            ("GetJobs: Error at xmlTextWriterEndElement\n");
        return 0;
	}

    } // end while database row

    mysql_free_result(result);


    /* Here we could close the elements using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work. */
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf("GetJobs: Error at xmlTextWriterEndDocument\n");
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

