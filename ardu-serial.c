#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include "ardu-serial.h"


int initPort(char *dev, int br)
{
	speed_t baud; /*connection rate */
	int tt; /* file descriptor of the serial connection */
	struct termios term; /* terminal conection to serial device */
	
	/* Test, if serial port is available */
	tt = open(dev, O_RDWR | O_NONBLOCK);
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
		case 50:		baud = B50; break;
		case 75:		baud = B75; break;
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
		default:		fprintf(stderr, "Baud rate of %d is invalid \n", br); return -1;
	}
	cfsetospeed(&term, baud);
		
	/* set terminal attributes for the serial connection */
	if (tcsetattr(tt, TCSANOW, &term) == -1) {
		fprintf(stderr, "Unable to set terminal attributes for serial connection %s\n", dev);
		return -1;
	}
	
	return tt;
}



int ser_getc(int fd, char *c)
{
	int r; 
	do {
		r = read(fd, c, 1);
		if (r == -1) {
			fprintf(stderr, "Cannot read from serial connection");
			return -1;
		}
	} while (r == 0);
	return r;	
}

int ser_flush(int fd)
{
	sleep(1);
	return tcflush( fd, TCIOFLUSH);
}

int ser_readln(int fd, char *b)
{
	char c;
	int i, r;
	for (i = 0; i< MAXLINE; i++) {
		r = ser_getc(fd, &c);
		if ( r == -1) {
			return -1;
		}
		if ( c != '\n')
			*(b+i) = c;
		else {
			*(b+i) = '\0';
			break;
		}
	}
	return 1;
}