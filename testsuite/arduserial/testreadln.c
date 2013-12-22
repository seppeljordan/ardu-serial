#include "../../libarduserial/arduserial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../testdef.h"

/* This is the string that is sent by the arduino on start up */
#define WELCOME_STRING "Test program for testing libarduserial"

int main(int argc, char **argv) {
	int charactersRead;
	char readBuffer[MAXLINE];
	int fd;
	printf("Start initialization process\n");
	fd = ser_init(ARDUINOPATH, BAUDRATE);
	sleep(2);
	if (fd == -1) {
		/* Test is skipt because the arduino cannot be initialized */
		exit(TESTSKIP);
	}
	printf("Arduino initialized\n");
	charactersRead = ser_readln(fd, readBuffer);
	if (charactersRead != strlen(WELCOME_STRING)) {
		/* Test failed */
		printf("Not the correct amount of characters was read [%i]\n",
				charactersRead);
		exit(TESTFAIL);
	}
	if (strlen(readBuffer) != charactersRead) {
		printf("Read string is not 0-terminated.\n");
		exit(TESTFAIL);
	}
	if (strncmp(WELCOME_STRING, readBuffer, charactersRead) != 0) {
		/* Test failed, String read from arduino is not correct */
		printf("Failed to read the correct string from the arduino.\n");
		printf("\"%s\" was read\n", readBuffer);
		exit(TESTFAIL);
	}
	printf("String read from the arduino was correct.\n");
	printf("\"%s\" was read\n", readBuffer);
	exit(TESTPASS);
}
