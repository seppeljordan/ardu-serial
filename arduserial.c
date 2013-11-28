#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "ardu-serial.h"

#define READTRYS 10
#define READTRYTIME 100 * 1000

#define SERIALDEBUG

/* initialize a connection to the arduino */
int ser_init(char *dev, int br)
{
	speed_t baud; /*connection rate */
	int tt; /* file descriptor of the serial connection */
	struct termios term; /* terminal conection to serial device */
	
	/* Test, if serial port is available */
	tt = open(dev, O_RDWR | O_NOCTTY);
	if ( tt == -1 )
	{
		fprintf(stderr, "Unable to open serial port %s\n", dev);
		return -1;
	}
	
	/* Get terminal attributes */
	if ( tcgetattr(tt, &term) < 0) {
		fprintf( stderr, "Unable to get terminal attributes for %s\n", dev);
		return -1;
	}
	
	/* set connection rate (baud)*/
	switch (br)
	{
		case 50:	baud = B50; break;
		case 75:	baud = B75; break;
		case 110:	baud = B110; break;
		case 134:	baud = B134; break;
		case 150:	baud = B150; break;
		case 200:	baud = B200; break;
		case 300:	baud = B300; break;
		case 600:	baud = B600; break;
		case 1200:	baud = B1200; break;
		case 1800:	baud = B1800; break;
		case 2400:	baud = B2400; break;
		case 4800:	baud = B4800; break;
		case 9600:	baud = B9600; break;
		case 19200:	baud = B19200; break;
		case 38400:	baud = B38400; break;
		case 57600:	baud = B57600; break;
		case 115200:	baud = B115200; break;
		case 230400:	baud = B230400; break;
		default:	fprintf(stderr, "Baud rate of %d is invalid \n", br); return -1;
	}
	cfsetospeed(&term, baud);
	
	term.c_cflag &= ~PARENB; /* Disable parity bit */
	term.c_cflag &= ~CSTOPB; /* No stop bit */
	term.c_cflag &= ~CSIZE; /* No size bit */
	term.c_cflag |= CS8; /* 8 bit words */
	term.c_cflag &= ~CRTSCTS; /* disable hardware flow controll */
	term.c_iflag &= ~(IXON | IXOFF | IXANY); /* disable input/output flow controll, 
	                                            disable restart chars*/
	term.c_lflag &= ~(ECHO | ECHOE | ISIG); /* no echo disable visually erase chars,
						   disable terminal-generated signas*/
	term.c_lflag |= ICANON; /* non-canonical mode */
	term.c_oflag &= ~OPOST; /* disable output processing */
	term.c_cc[VMIN] = 1; /* only read if input is available */
	term.c_cc[VTIME] = 0; /* try reading for at least 1 deci second */
		
	/* set terminal attributes for the serial connection */
	if (tcsetattr(tt, TCSANOW, &term) == -1) {
		fprintf(stderr, "Unable to set terminal attributes for serial connection %s\n", dev);
		return -1;
	}
	
	return tt;
}


/* Read the next character from the serial device */
int ser_getc(int fd, char *c)
{
	int r; /* store return value of read */ 
	int rt = READTRYS; /* set number of retries */

	do {
		r = read(fd, c, 1);
		if (r == -1) {
			if (rt-- > 0) {
				usleep(READTRYTIME);
				continue;
			}
			return -1;
		}
	} while (r != 1); /* repeat reading until one character was succesfully read */
	return r;	
}

/* flush the buffer of the serial device */
int ser_flush(int fd)
{
	return tcflush( fd, TCIOFLUSH);
}

/* Read from the serial device until a new line character is detected 
   Return value:
   Returns the number of characters read.
*/
int ser_readln(int fd, char *b)
{
	char c; /* read character is stored in c */
	int i; /* count to detect whether MAXLINE is reached */
	int r; /* return value of ser_getc */
	
	for (i = 0; i< (MAXLINE -1) ; i++) {
		/* Read next character from serial connection */
		r = ser_getc(fd, &c);
		
		/* catch error made while reading */
		if ( r == -1) { /* exit the function if an read error was detected */
			return -1;
		}

		/* strip of the new line and replace it with termination character \0 */
		if ( c == '\n') {
			*(b+i) = '\0';
			return i ;
		}
		else {
			*(b+i) = c; /* write next character to the output buffer */
		}
	}
	*( b+i+1 ) = '\0';
	return i;
}

/* write a single character to the serial connection */
int ser_putc(int f, char c)
{
	return write(f, &c, 1);
}

/* write a string to the serial connection and terminate with a newline character */
int ser_println(int f, char *b)
{
/*	while ( *b != '\0') {
		ser_putc(f, *(b++));
	}
	ser_putc(f, '\n');
*/
	int l = strlen(b);
	if (-1 == write(f, b, l))
		return -1;
	if (-1 == ser_putc(f, '\n'))
		return -1;
	return 1;
}
