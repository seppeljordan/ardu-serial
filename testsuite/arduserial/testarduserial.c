/* This program is a test driver for the arduserial library. */
#include <stdio.h>
#include <stdlib.h>
#include "arduserial.h"

#define FAILURE 1

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
  int retValue = ser_init("/dev/nodevice", 9600);
  if (retValue != -1)
    exit(FAILURE);
  exit(0);
}
