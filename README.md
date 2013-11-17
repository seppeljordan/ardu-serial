ardu-serial
===========

accessing serial communication with the arduino board


Implemented functions
---------------------

* int ser_init(char *device, int BaudRate);
* int ser_getc(int file, char *readCharacter);
* int ser_flush(int file);
* int ser_readln(int file, char *buffer);
* int ser_putc(int file, char character);
* int ser_println(int file, char *buffer);
