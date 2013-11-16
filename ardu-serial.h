/* ardu-serial.h
   
   Headers for the ardu-serial library for the language c
*/

#ifndef __ARDU_SERIAL_H__
#define __ARDU_SERIAL_H__

#define MAXLINE 1000

int ser_init(char *device, int BaudRate);
int ser_getc(int file, char *readCharacter);
int ser_flush(int file);
int ser_readln(int file, char *buffer);

#endif