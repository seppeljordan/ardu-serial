#include "arduserial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../testdef.h"

int main(int argc, char **argv) {
	int charactersRead;
	char readBuffer[MAXLINE];
	int fd;
	printf("Start des Initializierungsprozesses\n");
	fd = ser_init(ARDUINOPATH, BAUDRATE);
	sleep(2);
	if (fd == -1) {
		/* Test is skipt because the arduino cannot be initialized */
		exit(TESTSKIP);
	}
	printf("Arduino initialisiert\n");
	charactersRead = ser_readln(fd, readBuffer);
	if (charactersRead != 38) {
		/* Test failed */
		printf("Not the correct amount of characters were read [%i]\n",
				charactersRead);
		exit(TESTFAIL);
	}
	if (strcmp("Test program for testing libarduserial", readBuffer) != 0) {
		/* Test failed, String read from arduino is not correct */
		printf("Failed to read the correct string from the arduino.\n");
		printf("\"%s\" was read\n", readBuffer);
		exit(TESTFAIL);
	}
	printf("String read from the arduino was correct.\n");
	printf("\"%s\" was read\n", readBuffer);
	exit(TESTPASS);
}
