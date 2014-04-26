/* This program is a test driver for the arduserial library.  We want
   to test the functionality of the ser_init procedure. */
#include <stdio.h>
#include <stdlib.h>
#include "../../libarduserial/arduserial.h"
#include "../testdef.h"

void evaltest(int (* test)());
int test_invalidDevice();

int main(int argc, char **argv)
{
  evaltest(test_invalidDevice);
}

void evaltest(int (* test)())
/* Evaluate a test and exit if the test does not pass. */
{
  int result = (* test)();
  if (result != TESTPASS) {
    exit(result);
  }
}

int test_invalidDevice()
/* We want to test the behavior of the ser_init procedure when trying
   to open an invalid device.  We expect the procedure to fail.  Our
   non-existing device will have the name "/dev/nodevice". */
{
  int retValue = ser_init("/dev/nodevice", 9600);
  if (retValue != -1)
    exit(TESTFAIL);
  return(TESTPASS);
}
