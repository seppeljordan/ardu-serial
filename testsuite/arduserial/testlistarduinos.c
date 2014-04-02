/* testlistarduinos.c */

#include "../testdef.h"
#include "../../libarduserial/arduserial.h"
#include <stdlib.h>
#include <stdio.h>

int main(void);

int main(void)
/* We want to test, if the listarduinos() procedure works correctly.
   Therefore we fist test, if it returns either NULL or a valid string
   stack.  If it does not return NULL then we check if we can
   initialize a connection with this file name.  */
{
  stringStack *ardulist;
  char s[512];
  ardulist = ser_listarduinos();
  if (ardulist == NULL) {
    printError("Skipping test. No arduinos detected.");
    exit(TESTSKIP);
  }
  printf("There were some arduinos detected\n");
  while ( ardulist != NULL )
    {
      ardulist = pop(ardulist, s);
      printf("arduino found at %s\n", s);
      int fd = ser_init(s, 9600);
      if (fd == -1) {
	char *errmsg;
	sprintf(errmsg, "Connection to %s failed", s);
	printError(errmsg);
	exit(TESTFAIL);
      };
      close(fd);
    };
  exit(TESTPASS);
}
