CFLAGS = -pedantic -Wall -ansi
OBJ = ardu-serial.o
VERSION = 1.0
ABIVERSION = 1
LIBNAME = libarduserial.so

all: libarduserial.so

ardu-serial.o: ardu-serial.c ardu-serial.h
	gcc ardu-serial.c -c -o ardu-serial.o -fPIC -g $(CFLAGS)

$(LIBNAME).$(VERSION): $(OBJ)
	gcc --shared -Wl,-soname,$(LIBNAME).$(ABIVERSION) \
	    -o $(LIBNAME).$(VERSION) $(OBJ) -lc

$(LIBNAME).$(ABIVERSION): $(LIBNAME).$(VERSION)
	ln -s $(LIBNAME).$(VERSION) $(LIBNAME).$(ABIVERSION)
	
$(LIBNAME): $(LIBNAME).$(ABIVERSION)
	ln -s $(LIBNAME).$(ABIVERSION) $(LIBNAME)

install: ardu-serial.h all
	cp ardu-serial.h /usr/local/include
	cp $(LIBNAME) $(LIBNAME).$(ABIVERSION) $(LIBNAME).$(VERSION) \
	   /usr/local/lib

.PHONY: clean

clean:
	rm -f $(OBJ) $(LIBNAME).$(ABIVERSION) $(LIBNAME).$(VERSION) $(LIBNAME)