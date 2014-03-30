#include "tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARDUSERIAL_MAXSTACKSIZE 20

stringStack *stackAlloc(void);
char *stringAlloc(int length);

stringStack *newStringStack(void)
/* Create a new empty stack */
{
  return NULL;
}

stringStack *push(stringStack oldStack, char *text)
/* push a string onto the stack */
{
  if (oldStack->size == ARDUSERIAL_MAXSTACKSIZE)
    {
      return oldStack;
    };
  stringStack *newElem = stackAlloc();
  newElem->text = stringAlloc(strlen(text));
  strcpy(newElem->text,text);
  newElem->next = oldStack;
  newElem->size = oldStack->size + 1;
  return newElem;
}

stringStack *pop(stringStack *oldStack, char *returnValue)
{
  stringStack *newStack = oldStack->next;
  strcpy(*returnValue,oldStack->text);
  free(oldStack);
  return newStack;
}

char *stringAlloc(int l)
{
  return (char *) malloc( sizeof(char) * (l + 1));
}

stringStack *stackAlloc(void){
  return (stringStack *) malloc(sizeof (stringStack));
}
