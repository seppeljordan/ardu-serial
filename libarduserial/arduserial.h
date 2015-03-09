/* arduserial.h

 Headers for the ardu-serial library for the language c
 */

#ifndef __ARDU_SERIAL_H__
#define __ARDU_SERIAL_H__

typedef struct strStack *stackPtr;

typedef struct strStack
{
  char *text;
  stackPtr next;
  int size;
} stringStack;

#define ARDUSERIAL_MAXLINE 1000

stringStack *newStringStack(void);
stringStack *push (stackPtr oldStack, char *text);
stringStack *pop (stringStack *oldStack, char *returnValue, int buffersize);
void killStack(stringStack *stack);
void printError(char *errorMsg);
int ser_init(char *device, int BaudRate);
int ser_getc(int file, char *readCharacter);
int ser_flush(int file);
int ser_readln(int file, char *buffer);
int ser_putc(int file, char character);
int ser_println(int file, char *buffer);
int ser_autodetect(int baudRate);
stringStack *ser_listarduinos(void);

#endif
