cread:		serialread.o ardu-serial.o
			gcc -o cread ardu-serial.o serialread.o
			

serialread.o:	serialread.c
			gcc serialread.c -c -o serialread.o -pedantic -Wall -ansi

ardu-serial.o:	ardu-serial.c
			gcc ardu-serial.c -c -o ardu-serial.o -pedantic -Wall -ansi