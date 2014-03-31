/* This program tests if it is possible to put something onto a
   stringStack and recover something from the stack. */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../libarduserial/arduserialtools.h"
#include "../testdef.h"

int main(int argc, char **argv)
{
  stringStack *s;
  char retVal[10];
  s = push(s,"TEST");
  if ( s == NULL ) {
    // No element could be pushed to the stack.  exiting.
    exit(TESTFAIL);
  }
  s = pop(s, retVal);
  if ( strcmp("TEST",retVal) == 0 ) {
    exit(TESTPASS);
  } else {
    exit(TESTFAIL);
  };
}
