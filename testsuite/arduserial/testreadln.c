#include "../../libarduserial/arduserial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../testdef.h"

/* This is the string that is sent by the arduino on start up */
#define WELCOME_STRING "Test program for testing libarduserial"

int main(int argc, char **argv) {
	int charactersAmount;
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
	charactersAmount = ser_readln(fd, readBuffer);

	/* Test for null termination of the string */
	if (strlen(readBuffer) != charactersAmount) {
		printf("Read string is not 0-terminated.\n");
		exit(TESTFAIL);
	}

	/* Check for the correct return value of ser_readln */
	if (charactersAmount != strlen(WELCOME_STRING)) {
		/* Test failed */
		printf("Not the correct amount of characters was read [%i]\n",
				charactersAmount);
		printf("\"%s\" read", readBuffer);
		exit(TESTFAIL);
	}

	if (strncmp(WELCOME_STRING, readBuffer, charactersAmount) != 0) {
		/* Test failed, String read from arduino is not correct */
		printf("Failed to read the correct string from the arduino.\n");
		printf("\"%s\" was read\n", readBuffer);
		exit(TESTFAIL);
	}
	printf("String read from the arduino was correct.\n");
	printf("\"%s\" was read\n", readBuffer);
	printf("Test: PASS\n");
	exit(TESTPASS);
}
