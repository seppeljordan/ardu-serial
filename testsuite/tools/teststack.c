/* This program tests if it is possible to put something onto a
   stringStack and recover something from the stack. */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../libarduserial/arduserial.h"
#include "../testdef.h"

int main(int argc, char **argv)
{
  stringStack *s;
  char retVal[10];
  s = push(s,"TEST");
  if ( s == NULL ) {
    printf("Could not push element to stack.\n");
    // No element could be pushed to the stack.  exiting.
    exit(TESTFAIL);
  }
  s = pop(s, retVal, 10);
  if (s != NULL) {
    printf("Unable to pop from stack");
    exit(TESTFAIL);
  }
  if ( strcmp("TEST",retVal) == 0 ) {
    exit(TESTPASS);
  } else {
    printf("Original message does not equal message from the stack \"%s\"\n",retVal);
    exit(TESTFAIL);
  };
}
