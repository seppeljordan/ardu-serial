#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "arduserial.h"
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <regex.h>

#define READTRYS 10
#define READTRYTIME 100 * 1000
#define SERIALPATH "/dev/serial/by-id/"
#define SERIALDIR "/dev/serial/by-id"

#define print_err_msg fprintf(stderr, "%s\n" ,strerror(errno))

/* initialize a connection to the arduino */
int ser_init(char *dev, int br) {
	speed_t baud; /*connection rate */
	int tt; /* file descriptor of the serial connection */
	struct termios term; /* terminal conection to serial device */

	/* Test, if serial port is available */
	tt = open(dev, O_RDWR | O_NOCTTY);
	if (tt == -1) {
		fprintf(stderr, "Unable to open serial port %s\n", dev);
		return -1;
	}

	/* Get terminal attributes */
	if (tcgetattr(tt, &term) < 0) {
		fprintf( stderr, "Unable to get terminal attributes for %s\n", dev);
		return -1;
	}

	/* set connection rate (baud)*/
	switch (br) {
	case 50:
		baud = B50;
		break;
	case 75:
		baud = B75;
		break;
	case 110:
		baud = B110;
		break;
	case 134:
		baud = B134;
		break;
	case 150:
		baud = B150;
		break;
	case 200:
		baud = B200;
		break;
	case 300:
		baud = B300;
		break;
	case 600:
		baud = B600;
		break;
	case 1200:
		baud = B1200;
		break;
	case 1800:
		baud = B1800;
		break;
	case 2400:
		baud = B2400;
		break;
	case 4800:
		baud = B4800;
		break;
	case 9600:
		baud = B9600;
		break;
	case 19200:
		baud = B19200;
		break;
	case 38400:
		baud = B38400;
		break;
	case 57600:
		baud = B57600;
		break;
	case 115200:
		baud = B115200;
		break;
	case 230400:
		baud = B230400;
		break;
	default:
		fprintf(stderr, "Baud rate of %d is invalid \n", br);
		return -1;
	}
	cfsetospeed(&term, baud);

	/* Disable: parity bit, stop bit, size bit, hardware flow control) */
	term.c_cflag &= ~(PARENB | CSTOPB | CSIZE | CRTSCTS);
	term.c_cflag |= CS8; /* 8 bit words */
	term.c_iflag &= ~(IXON | IXOFF | IXANY); /* disable input/output flow controll, 
	 disable restart chars*/
	term.c_lflag &= ~(ECHO | ECHOE | ISIG); /* no echo disable visually erase chars,
	 disable terminal-generated signas*/
	term.c_lflag |= ICANON; /* canonical mode */
	term.c_oflag &= ~OPOST; /* disable output processing */
	term.c_cc[VMIN] = 1; /* read one character at a time */
	term.c_cc[VTIME] = 0; /* no time limit */

	/* set terminal attributes for the serial connection */
	if (tcsetattr(tt, TCSANOW, &term) == -1) {
		fprintf(stderr,
				"Unable to set terminal attributes for serial connection %s\n",
				dev);
		return -1;
	}

	return tt;
}

/* Read the next character from the serial device */
int ser_getc(int fd, char *c) {
	int r; /* store return value of read */
	int rt = READTRYS; /* set number of retries */

	do {
		r = read(fd, c, 1);
		if (r == -1) {
			if (rt-- > 0) {
				usleep(READTRYTIME);
				continue;
			}
			print_err_msg;
			return -1;
		}
	} while (r != 1); /* repeat reading until one character was succesfully read */
	return r;
}

/* flush the buffer of the serial device */
int ser_flush(int fd) {
	return tcflush(fd, TCIOFLUSH);
}

/* Read from the serial device until a new line character is detected 
 * Return value: Returns the number of characters read excluding the newline.
 */
int ser_readln(int fd, char *b) {
	char c; /* read character is stored in c */
	int i; /* count to detect whether MAXLINE is reached */
	int r; /* return value of ser_getc */

	for (i = 0; i < ARDUSERIAL_MAXLINE; i++) {
		/* Read next character from serial connection */
		r = ser_getc(fd, &c);

		/* strip of the carriage return and replace it with termination character \0 */
		if ((c == '\r') || (c == '\n')) {
			*(b + i) = '\0';
			return i;
		} else if (r != -1) {
			*(b + i) = c; /* write next character to the output buffer */
		} else
		/* catch error made while reading */
		{ /* exit the function if an read error was detected */
			return -1;
		}
	}
	/* There were read so many characters that MAXLINE was reached */
	*(b + i + 1) = '\0';
	return i + 1;
}

/* write a single character to the serial connection */
int ser_putc(int f, char c) {
	int r = write(f, &c, 1);
	if (r == -1) {
		print_err_msg;
		return -1;
	} else
		return r;
}

/* Write a string of characters to the serial connection and terminate with a
 * newline character.
 * 
 * This function prints the string gives as the second parameter to the serial
 * connection given with the first parameter.
 * 
 * Return: ser_println() will return 1 if the data has been writen succesfully 
 * to the serial connection and will return -1 if something went wrong. */

int ser_println(int f, char *b) {
	int l = strlen(b);

	/* write string to serial connection */
	if (write(f, b, l) == -1) {
		print_err_msg;
		return -1;
	}

	/* write new line to serial connection */
	if (ser_putc(f, '\n') == -1) {
		print_err_msg;
		return -1;
	}
	return 1;
}

int ser_autodetect(int baudRate) 
/* Autodetect a arduino connected to the system via usb. */
{

	/* Pointer to the directory */
	DIR *d;
	/* File read from the directory */
	struct dirent *file;

	/* stores the regular expression for finding the arduino */
	regex_t regex;
	/* stores information about the matching of the regular
	   expression */
	int match = 0;

	/* stores the file descriptor of the arduino to be accessed */
	int fd = -1;

	/* compile the regular expression and exit if an error
	   occurs */
	if (regcomp(&regex, "^usb-Arduino", 0) != 0) {
		print_err_msg;
		return -1;
	}

	// check if the path to the serial connections exists and quit
	// it does not.
	if (! (file_exists(SERIALDIR))) {
	  // return -2 for 'no connection is available'
	  return -2;
	};

	d = opendir(SERIALDIR);
	if (d) {
		while ((file = readdir(d)) != NULL) {
			if (regexec(&regex, file->d_name, 0, NULL, 0) == 0) {

				/* Here goes the target of the softlink in /dev/serial/by-id/ */
				char linkTarget[ARDUSERIAL_MAXLINE] = SERIALPATH;
				/* Buffer for the path of the link */
				char pathBuffer[ARDUSERIAL_MAXLINE] = SERIALPATH;
				char linkRelPath[ARDUSERIAL_MAXLINE];
				int targetLen=0;
				strcat(pathBuffer,file->d_name); /* convert link path to absolute path */
				if ((targetLen = readlink(pathBuffer,linkRelPath,ARDUSERIAL_MAXLINE)) == -1){
					print_err_msg;
					return -1;
				}
				linkRelPath[targetLen] = '\0';
				strcat(linkTarget,linkRelPath);
				printf("Try to open %s\n",linkTarget);
				fd = ser_init(linkTarget, 9600);
				break;
			}
		}

		closedir(d);
	}
	return fd;
}

int file_exists(const char *filename)
/* Check if a file exists. */
{
  FILE *file;
  if ( file = fopen(filename, "r"))
    {
      fclose(file);
      return 1;
    }
  return 0;
}
