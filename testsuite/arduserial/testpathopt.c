#include <stdio.h>
#include "../../libarduserial/arduserial.h"
#include "../testdef.h"
#include <stdlib.h>
#include <regex.h>

int isMinimal(char *p);

int main()
/* We want to check if the paths returned by the ser_listarduinos
   procedure are minimal.  That means that we check for ../ and ./ in
   paths and return a FAIL if the test failed. */
{
  stringStack *ardulist;
  char s[ARDUSERIAL_MAXLINE];
  ardulist = ser_listarduinos();
  if (ardulist == NULL) {
    printError("Skipping test. No arduinos detected.");
    exit(TESTSKIP);
  }
  
  /* check if our subprocedure checkPath detects paths that are not
     minimal */

  if ((isMinimal("/test/../test"))
      || (!isMinimal("/test/test"))) {
    printError("Testing routine is not working correctly:");
    printError("    checkpath did not detect a non-minimal path.");
    exit(TESTERROR);
  };

  printf("There were some arduinos detected\n");
  while ( ardulist != NULL )
    {
      ardulist = pop(ardulist, s);
      if (! (isMinimal(s) )) {
	printf("Path to arduino \"%s\" is not optimal, test FAILED.\n", s);
	exit(TESTFAIL);
      }
    };
  printf("Test passed \n.");
  exit(TESTPASS);
  
}

int isMinimal(char *p)
/* We want to check if p is a minimal path.  We use regular
   expressions for this purpose.  A minimal path should contain no
   elements such as '/./' or '/../'.  We will check if the path
   contains such elements and if it starts with an '/' */
{
  regex_t patternCompiled;
  int result = 0;

  /* This is the pattern that is used to determine if a path is
     optimal or not. */
  char *patternString = "/(\\.\\.|\\.)/";
  /* Choices are extended regular expression syntax. */
  int regexFlags = REG_EXTENDED; 
  result = regcomp(&patternCompiled, patternString, regexFlags);

  /* Check match of the compiled regular expression against char
     *p. */
  result = regexec(&patternCompiled, p, 0, NULL, 0);

  /* regexec returns 0 if the pattern was matched on the string */
  return (result == REG_NOMATCH);
}
