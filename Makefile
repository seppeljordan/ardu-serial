CFLAGS = -pedantic -Wall -ansi


ardu-serial.o:	ardu-serial.c
		gcc ardu-serial.c -c -o ardu-serial.o $(CFLAGS)

all:		ardu-serial.o

