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
#define MAXPATHLENGTH 1024

#define print_err_msg fprintf(stderr, "%s\n" ,strerror(errno))

stringStack *stackAlloc(void);
char *stringAlloc(int length);
void freeStack(stringStack *stackElement);
stringStack *allocElem(int sizeOfString);
stringStack *unstack(stringStack *stack);
int file_exists(const char *filename);
char *optFilename(char *filename);

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

stringStack *ser_listarduinos()
/* We want to make a list of all arduinos connected to the computer.
   We will scan the directory SERIALDIR for any serial connections
   with an arduino in the id.  All the paths resulting from the scan
   go into the stack.  A serial connection counts as an arduino if its
   id starts with 'usb-Arduino'. */
{
  // pointer to the dir object 'SERIALDIR'
  DIR *d;
  // stores the file descriptor when we iterate through the directory
  struct dirent *file;
  // store the compiled regular expression
  regex_t regexArduino;
  // stack to store the paths to the arduinos
  stringStack *stack;

  // compile regular expression for detection pattern
  regcomp(&regexArduino, "^usb-Arduino", 0);
  
  // check if the path to the serial connections exists and quit if it
  // doesn't
  if (! (file_exists(SERIALDIR))) {
    return NULL;
  }

  // iterate throug the directory SERIALDIR
  d = opendir(SERIALDIR);
  if (d) {
    while ((file = readdir(d)) != NULL) {
      // check if we found a valid connection
      if (regexec(&regexArduino, file->d_name, 0, NULL, 0) == 0) {
	
	/* Here goes the target of the softlink in /dev/serial/by-id/ */
	char linkTarget[ARDUSERIAL_MAXLINE] = SERIALPATH;
	char *linkTargetOpt = NULL;
	/* Buffer for the path of the link */
	char pathBuffer[ARDUSERIAL_MAXLINE] = SERIALPATH;
	char linkRelPath[ARDUSERIAL_MAXLINE];
	int targetLen=0;
	/* convert link path to absolute path */
	strcat(pathBuffer,file->d_name);
	if ((targetLen = readlink(pathBuffer,linkRelPath,
				  ARDUSERIAL_MAXLINE)) == -1)
	  {
	    print_err_msg;
	    return NULL;
	  };
	linkRelPath[targetLen] = '\0';
	strcat(linkTarget,linkRelPath);
	linkTargetOpt = optFilename(linkTarget);
	stack = push(stack, linkTargetOpt);
	/* we must not forget to free the memory allocated by
	   optFilename */
	free(linkTargetOpt);
      }
    }
    
    closedir(d);
  }
  return stack;
}

int ser_autodetect(int baudRate) 
/* Autodetect a arduino connected to the system via usb.  We will use
   the ser_listarduinos() procedure to detect a possible connection.
   We check if there is any serial connection before trying to connect
   and return -2 if we did not find any connections.  We will connect
   to the arduino that is on top of the stack.  We have to kill the
   stack in end. */
{
  stringStack *s;
  int fd;
  
  // check if the path to the serial connections exists and quit it
  // does not.
  if (! (file_exists(SERIALDIR))) {
    // return -2 for 'no connection is available'
    return -2;
  };
  
  // get a list with all arduinos connected
  s = ser_listarduinos();
  if ( s == NULL )
    return(-1);
  
  // connect to the first arduino on the stack
  fd = ser_init(s->text, baudRate);

  // kill the stack
  killStack(s);
  
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

#define ARDUSERIAL_MAXSTACKSIZE 20


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

void killStack(stringStack *s)
/* We want to free all allocated memory used by the stack passed as an
   argument.  We cannot reference s after calling it killStack on
   s.  */
{
  while ( s != NULL ) {
    stringStack *p = s;
    s = p->next;
    free(p);
  }
}

char *stringAlloc(int l)
/* Allocate memory for a string of length l including the space for a
   terminating \0 character. */
{
  return (char *) malloc( sizeof(char) * (l + 1));
}

stringStack *stackAlloc()
/* Allocate memory for an element of a stack without allocating memory
   for the string. */
{
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

stringStack *unstack(stringStack *s)
/* Free the memory of the top element of the stack.  Return the adress
   of the new top element of the stack.  Do nothing if s points to
   NULL. */
{
  /* check if s points to null */
  if (s == NULL) {
    return s;
  } else {
    /* point newHead to next element on the stack, free the memory of
     the old stack head, return a pointer to the new head */
    stringStack *newHead = s->next;
    freeStack(s);
    return newHead; 
  }
}

void printError(char *e)
/* We want to print an error message to stderr.  We assume that the
   error msg is NULL terminated. */
{
  fprintf(stderr, "ERROR: %s\n", e);
}

char *optFilename(char *oldPath)
/* We want to optimize the path refering to a file in the local file
   system.  This routine is only working on POSIX systems.  We split
   the path into tokens and push the tokens to a stack.  When we find
   a '..'  token then we unstack the last element instead of pushing
   '..' to the stack.  We must not forget to free the memory where the
   new path is stored because it is dynamically allocated.  We expect
   the path passed to this routine to be null terminated. */
{
  stringStack *pathStack = newStringStack();
  char pathElement[MAXPATHLENGTH];
  char *token;
  char *saveptr;
  char *returnValue = stringAlloc(strlen(oldPath));
  int originalLength = strlen(oldPath);
  // tokenize the path
  for (; ; oldPath = NULL) {
    token = strtok_r(oldPath, "/", &saveptr);
    if (token == NULL) break;
    if ( strcmp("..", token) == 0 ) {
      pathStack = unstack(pathStack);
    } else if (strcmp(".", token) == 0 ) {
	continue;
    } else {
      pathStack = push(pathStack, token);
    }
  }
  returnValue[0] = '\0';
  while (pathStack != NULL) {
    char *helper = stringAlloc(originalLength);
    pathStack = pop(pathStack, pathElement);
    sprintf(helper, "/%s%s\0", pathElement, returnValue);
    strcpy(returnValue, helper);
    free(helper);
  }
  return returnValue;
}
