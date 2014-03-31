#include "arduserialtools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARDUSERIAL_MAXSTACKSIZE 20


stringStack *stackAlloc(void);
char *stringAlloc(int length);
void freeStack(stringStack *stackElement);
stringStack *allocElem(int sizeOfString);

stringStack *newStringStack(void)
/* Create a new empty stack */
{
  return NULL;
}

stringStack *push(stackPtr oldStack, char *text)
/* push a string onto the stack */
{
  /* We want to put a new string onto the stack.  We create a new
     element, feed the new element with the string we want to store
     and let the next field point to the element that was on top of
     the stack before.  We return a NULL pointer if something went
     wrong.*/
  // is there already something on the stack?
  int isFirst = (oldStack == NULL);
  if ( !isFirst) {
    // check there is already an element on the stack
    if (oldStack->size == ARDUSERIAL_MAXSTACKSIZE)
      // check if maximum stack size is reached
      {
	printError("Stack overflow error in push()");
	return NULL;
      };
  };
  // create a new stack element with space for the string allocated
  stringStack *newElem = allocElem(strlen(text));
  // copy the string to the new element
  strcpy(newElem->text,text);
  // set next element to old head of the stack
  newElem->next = oldStack;
  // update the size of the stack
  if ( isFirst ) {
    newElem->size = 0;
  } else {
    newElem->size = oldStack->size + 1;
  };
  // return new head of the stack
  return newElem;
}

stringStack *pop(stringStack *oldStack, char *returnValue)
/* We want to take an element from the stack.  We copy the string
   value of the elements text onto a helper variable and then free the
   old stack element.  We also have to return the new head of the
   stack. */
{ 
  // newStack points to the new head
  stringStack *newHead = oldStack->next;
  // copy string to return variable
  strcpy(returnValue,oldStack->text);
  // free the memory of the old stack element
  freeStack(oldStack);
  // return a pointer to the new head
  return newHead; 
}

void freeStack(stringStack *s)
/* We want to free the memory from a stack element.  This includes
   freeing the memory of the stored string. */
{
  free(s->text);
  free(s);
}

char *stringAlloc(int l)
{
  return (char *) malloc( sizeof(char) * (l + 1));
}

stringStack *stackAlloc(){
  return (stringStack *) malloc(sizeof (stringStack));
}

stringStack *allocElem(int size) 
/* We want to allocate the memory for a stack element with a string of
   a certain size.  First we allocate the memory for the stack element
   itself and then we allocate the memory for the string. */
{ 
  stringStack *newElem = stackAlloc();
  newElem->text = stringAlloc(size);
  // return the pointer to the newly created stack element
  return newElem;
}

void printError(char *e)
/* We want to print an error message to stderr.  We assume that the
   error msg is NULL terminated. */
{
  fprintf(stderr, "ERROR: %s\n", e);
}
