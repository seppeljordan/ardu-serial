
#ifndef __TOOLS_H__
#define __TOOLS_H__

typedef struct stringStack
{
  char *text;
  stringStack *next;
  int size;
} stringStack;

stringStack *newStringStack(void);
stringStack *push(stringStack oldStack, char *text);
stringStack *pop(stringStack *oldStack, char *returnValue);

#endif
