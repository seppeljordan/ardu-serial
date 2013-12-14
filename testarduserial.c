/* This program is a test driver for the arduserial library. */
#include <stdio.h>
#include <stdlib.h>
#include "arduserial.h"

int strToInt(char *s) {
	char c;
	int i = 0;
	while ((*(s++) = c) != '\0') {
		i = i*10 + c - '0';
	}
	return i;
}

int main(int argc, char **argv)
{
	switch (atoi(argv[1])) {
		case 0: /* ser_init */
		{
			int baudrate = strToInt(argv[3]);
			int returncode;
			returncode = ser_init (argv[2], baudrate);
			printf("%d\n", returncode);
			exit(0);
		} break;
	}
}