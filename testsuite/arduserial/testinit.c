/* This program is a test driver for the arduserial library. */
#include <stdio.h>
#include <stdlib.h>
#include "arduserial.h"

#define FAILURE 1

int main(int argc, char **argv)
{
  int retValue = ser_init("/dev/nodevice", 9600);
  if (retValue != -1)
    exit(FAILURE);
  exit(0);
}
