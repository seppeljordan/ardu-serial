
#ifndef __TOOLS_H__
#define __TOOLS_H__

typedef struct strStack *stackPtr;

typedef struct strStack
{
  char *text;
  stackPtr next;
  int size;
} stringStack;

stringStack *newStringStack(void);
stringStack *push (stackPtr oldStack, char *text);
stringStack *pop (stringStack *oldStack, char *returnValue);
void printError(char *errorMsg);

#endif
