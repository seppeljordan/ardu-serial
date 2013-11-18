CFLAGS = -pedantic -Wall -ansi
OBJ = ardu-serial.o
VERSION = 1.2
ABIVERSION = 1
LIBNAME = libarduserial.so

LIBPATH = /usr/lib
INCPATH = /usr/include
MANPATH = /usr/share/man

all: libarduserial.so manpages

ardu-serial.o: ardu-serial.c ardu-serial.h
	gcc ardu-serial.c -c -o ardu-serial.o -fPIC -g $(CFLAGS)

$(LIBNAME).$(VERSION): $(OBJ)
	gcc --shared -Wl,-soname,$(LIBNAME).$(ABIVERSION) \
	    -o $(LIBNAME).$(VERSION) $(OBJ) -lc

$(LIBNAME).$(ABIVERSION): $(LIBNAME).$(VERSION)
	ln -s $(LIBNAME).$(VERSION) $(LIBNAME).$(ABIVERSION)
	
$(LIBNAME): $(LIBNAME).$(ABIVERSION)
	ln -s $(LIBNAME).$(ABIVERSION) $(LIBNAME)

manpages: man/man3/ardu-serial.3
	gzip -kf man/man3/ardu-serial.3

install: all
	install ardu-serial.h $(INCPATH)
	cp -d $(LIBNAME) $(LIBNAME).$(ABIVERSION) $(LIBNAME).$(VERSION) \
	   $(LIBPATH)
	ldconfig -n $(LIBPATH)
	install man/man3/ardu-serial.3.gz $(MANPATH)/man3/

.PHONY: clean manpages

clean:
	rm -f $(OBJ) $(LIBNAME).$(ABIVERSION) $(LIBNAME).$(VERSION) $(LIBNAME)
