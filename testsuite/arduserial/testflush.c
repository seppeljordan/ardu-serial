#include "../../libarduserial/arduserial.h"
#include <stdio.h>
#include <stdlib.h>
#include "../testdef.h"

void evaltest(int (* test)());

int main() {
	int tty; /* This is the file descriptor of the serial port */
	int reads; /* return value of ser_read */
	int retval;/*store return values for evaluation of function calls*/
	char buf[ARDUSERIAL_MAXLINE]; /* buffer for the returned string */
	char *message = "Test"; /* message sent to the arduino */

	/* try to initialize the connection to the arduino */
	tty = ser_autodetect(BAUDRATE);
	/* Skip the test if the arduino was not found */
	if (tty == -1)
		exit(TESTSKIP);
	sleep(2); /* Wait until the arduino has reset */

	retval = ser_flush(tty);
	printf("Return value of ser_flush: %i\n", retval);
	printf("Serial I/O flushed\n");
	printf("Print \"%s\" to Arduino\n", message);
	retval = ser_println(tty, message);
	if (retval != 1){
		printf("Print \"%s\" to arduino failed. Aborting test.", message);
		exit(TESTFAIL);
	}
	printf("\"%s\" printed succesfully to the arduino.\n", message);

		/* read the string from the arduino */
		reads = ser_readln(tty, buf);
	if (strcmp(buf, "Test program for testing libarduserial") == 0) {
		printf("Test failed, flush did not work correctly./n");
		exit(TESTFAIL);
	}
	printf("Test pass\n");
	return TESTPASS;
}

void evaltest(int (* test)())
/* Evaluate a test and exit if the test does not pass. */
{
  int result = (* test)();
  if (result != TESTPASS) {
    exit(result);
  }
}
