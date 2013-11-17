CFLAGS = -pedantic -Wall -ansi
OBJ = ardu-serial.o
VERSION = 1.0
ABIVERSION = 1

all: libarduserial.so

ardu-serial.o: ardu-serial.c ardu-serial.h
	gcc ardu-serial.c -c -o ardu-serial.o -fPIC -g $(CFLAGS)

libarduserial.so.$(VERSION): $(OBJ)
	gcc --shared -Wl,-soname,libarduserial.so.$(ABIVERSION) \
	    -o libarduserial.so.$(VERSION) ardu-serial.o -lc

libarduserial.so.$(ABIVERSION): libarduserial.so.$(VERSION)
	ln -s libarduserial.so.$(VERSION) libarduserial.so.$(ABIVERSION)
	
libarduserial.so: libarduserial.so.$(ABIVERSION)
	ln -s libarduserial.so.$(ABIVERSION) libarduserial.so

install: ardu-serial.h all
	cp ardu-serial.h /usr/local/include
	cp libarduserial.so libarduserial.so.$(ABIVERSION) libarduserial.so.$(VERSION) \
	   /usr/local/lib