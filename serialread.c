/* This program will read from a serial character device */

#include <stdio.h>
#include <stdlib.h>
#include "ardu-serial.h"

/* initializes a connection to the arduino */


int main(int argc, char **argv)
{
	char devicePath[] = "/dev/ttyACM0";
	int connection = initPort( devicePath, 9600);
	char c;
	char b[MAXLINE];
	ser_flush(connection);
	printf("readln test\n");
	ser_readln(connection, b);
	printf("%s\n", b);
	printf("getc test\n");
	while (ser_getc(connection,&c)) {
		putc(c,stdout);
		/*if (c == '\n')
			break;
		*/
	}
	exit(0);
}
