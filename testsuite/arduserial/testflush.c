#include "../../libarduserial/arduserial.h"
#include <stdio.h>
#include <stdlib.h>
#include "../testdef.h"

int main() {
	int tty; /* This is the file descriptor of the serial port */
	int reads; /* return value of ser_read */
	char buf[MAXLINE]; /* buffer for the returned string */
	char message[MAXLINE]; /* message sent to the arduino */

	/* try to initialize the connection to the arduino */
	tty = ser_init(ARDUINOPATH, BAUDRATE);
	/* Skip the test if the arduino was not found */
	if (tty == -1)
		exit(TESTSKIP);
	sleep(2); /* Wait until the arduino has reset */


	ser_flush(tty);
	printf("Serial I/O flushed\n");
	sprintf(message, "Test");
	printf("Print \"%s\" to Arduino\n", message);
	ser_println(tty, message);

	/* read the string from the arduino */
	reads = ser_readln(tty, buf);
	if (strcmp(reads,"Test program for testing libarduserial") == 0){
		printf("Test failed, flush did not work correctly");
		exit(TESTFAIL);
	}

	return TESTPASS;
}
