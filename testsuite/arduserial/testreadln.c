#include "arduserial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARDUINOPATH "/dev/ttyACM0"
#define BAUDRATE 9600
#define TESTFAIL 1
#define TESTSKIP 77
#define TESTPASS 0

int main(int argc, char **argv){
  int charactersRead;
  char *readBuffer;
  int fd;
  printf("Start des Initializierungsprozesses");
  fd = ser_init(ARDUINOPATH, BAUDRATE);
  sleep(2);
  if (fd == -1){
    /* Test is skipt because the arduino cannot be initialized */
    exit(TESTSKIP);
  }
  printf("Arduino initialisiert");
  charactersRead = ser_readln(fd, readBuffer);
  if (charactersRead != 38){
    /* Test failed */
    exit(TESTFAIL);
  }
  if (strncmp("Test program for testing libarduserial", readBuffer, 38) != 0){
    /* Test failed, String read from arduino is not correct */
    exit(TESTFAIL);
  }
  exit(TESTPASS);
}
