/*
 * testautodetect.c
 *
 *  Created on: 22.12.2013
 *      Author: Sebastian Jordan
 */

#include "../testdef.h"
#include "../../libarduserial/arduserial.h"
#include <stdlib.h>

int main (){
	/* return value of function calls */
	int rv;

	rv = ser_autodetect(9600);
	if (rv == -1) {
		exit(TESTFAIL);
	}
	close(rv);
	exit(TESTPASS);
}
