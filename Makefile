CFLAGS = -pedantic -Wall -ansi
OBJ = ardu-serial.o
VERSION = 1.1
ABIVERSION = 1
LIBNAME = libarduserial.so

LIBPATH = /usr/lib
INCPATH = /usr/include

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
	cp ardu-serial.h $(INCPATH)
	cp -d $(LIBNAME) $(LIBNAME).$(ABIVERSION) $(LIBNAME).$(VERSION) \
	   $(LIBPATH)
	ldconfig -n $(LIBPATH)

.PHONY: clean

clean:
	rm -f $(OBJ) $(LIBNAME).$(ABIVERSION) $(LIBNAME).$(VERSION) $(LIBNAME)
