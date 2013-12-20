#include "arduserial.h"
#include <stdio.h>
#include <stdlib.h>
#include "../testdef.h"

int main() {
	int i;
	int tty = ser_init(ARDUINOPATH, BAUDRATE);
	/* Skip the test if the arduino was not found */
	if (tty == -1)
		exit(TESTSKIP);
	sleep(2);
	ser_flush(tty);
	printf("Serial I/O flushed\n");	
	for (i = 0; i < 4; i++) {
		int reads; /* return value of ser_read */
		char message[MAXLINE];
		char buf[MAXLINE]; /* buffer for the returned string */

		/* Write the message to the arduino */
		sprintf(message, "Test[%d]", i);
		printf("Print \"%s\" to Arduino\n", message);
		ser_println(tty, message);

		/* Read the message from the arduino */
		printf("[%d] Read from Arduino:\n", i);
		reads = ser_readln(tty, buf);
		if (reads == -1) {
			fprintf(stderr, "Read failed\n");
		}
		printf("%s\n", buf);
	}
	return TESTPASS;
}
