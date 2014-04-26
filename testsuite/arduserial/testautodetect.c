/*
 * testautodetect.c
 *
 *  Created on: 22.12.2013
 *      Author: Sebastian Jordan
 */

#include "../testdef.h"
#include "../../libarduserial/arduserial.h"
#include <stdlib.h>

int file_exists(const char *filename);

int main (){
	/* return value of function calls */
	int rv;

	rv = ser_autodetect(9600);
	switch (rv) {
	case -1:
	  printf("Tests skipped, no arduinos detected.\n");
	  exit(TESTSKIP);
	  break;
	case -2:
	  printf("Test skipped, No serial usb devices detected\n");
	  exit(TESTSKIP);
	}
	close(rv);
	printf("Test passed.\n");
	exit(TESTPASS);
}
