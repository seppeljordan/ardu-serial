/* ardu-serial.h

 Headers for the ardu-serial library for the language c
 */

#ifndef __ARDU_SERIAL_H__
#define __ARDU_SERIAL_H__

#define ARDUSERIAL_MAXLINE 1000

int ser_init(char *device, int BaudRate);
int ser_getc(int file, char *readCharacter);
int ser_flush(int file);
int ser_readln(int file, char *buffer);
int ser_putc(int file, char character);
int ser_println(int file, char *buffer);

/* Auto detect a connected arduino connected to the computer
 *
 * Returns -1 if an error occured.
 * */
int ser_autodetect(int baudRate);

#endif
