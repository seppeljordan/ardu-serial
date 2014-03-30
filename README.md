<div id="table-of-contents">
<h2>Table of Contents</h2>
<div id="text-table-of-contents">
<ul>
<li><a href="#sec-1">1. about</a></li>
<li><a href="#sec-2">2. Installation</a></li>
<li><a href="#sec-3">3. Implemented functions</a></li>
<li><a href="#sec-4">4. Tasks to be done</a>
<ul>
<li><a href="#sec-4-1">4.1. <span class="todo TODO">TODO</span> Implement procedure that returns a list of connected arduinos</a>
<ul>
<li><a href="#sec-4-1-1">4.1.1. <span class="todo TODO">TODO</span> Include tools.c in compilation process of arduserial.c</a></li>
</ul>
</li>
</ul>
</li>
<li><a href="#sec-5">5. Patch notes</a>
<ul>
<li><a href="#sec-5-1">5.1. version 2.1</a></li>
<li><a href="#sec-5-2">5.2. version 2</a></li>
<li><a href="#sec-5-3">5.3. version 1.9</a></li>
<li><a href="#sec-5-4">5.4. version 1.8</a></li>
<li><a href="#sec-5-5">5.5. version 1.7</a></li>
<li><a href="#sec-5-6">5.6. version 1.6</a></li>
<li><a href="#sec-5-7">5.7. version 1.5</a></li>
<li><a href="#sec-5-8">5.8. version 1.4</a></li>
<li><a href="#sec-5-9">5.9. version 1.3</a></li>
<li><a href="#sec-5-10">5.10. version 1.2</a></li>
<li><a href="#sec-5-11">5.11. version 1.1</a></li>
</ul>
</li>
</ul>
</div>
</div>
# about

This software package aims to deliver a c library for estableshing a
serial connectin to a arduino board.

This package is under heavy devolopment and only tested for the
Arduino Uno.  The current version is 2.0

# Installation

Read "INSTALL" for instructions on the installation process.

If you downloaded the software directly from the github repo then you
need autotools to get this package installed and running.

-   libtoolize

-   aclocal

-   autoheader

-   autoconf

-   automake &#x2013;add-missing &#x2013;include-deps

Now you can follow the instructions from "INSTALL".

# Implemented functions

-   **`int ser_init(char *device, int BaudRate)`:** Initializes a
    connection to the arduino at the given path "device" with the
    given baud rate "BaudRate".

-   **`int ser_flush(int file)`:** Flushes the serial interface "file".

-   **`int ser_getc(int file, char *readCharacter)`:** Reads one character
    from the serial interface.  Returns -1 if something went wrong
    and prints an error message to stderr.

-   **`int ser_readln(int file, char *buffer)`:** Reads from the serial
    interface until a newline character is detected.  The newline
    character will be stript and replaced by a null character (\\0)
    to terminate the string.

-   **`int ser_putc(int file, char character)`:** Writes one character to
    the serial interface. Returns 0 if something went wrong and
    prints an error message to the standard output if something
    went wrong.

-   **`int ser_println(int file, char *buffer)`:** Prints a string to the
    serial interface "file" and additionally prints a newline
    character to the serial interface.

-   **`int ser_autodetect(int baudRate)`:** Auto detect a connected
    arduino connected to the computer.  Pass the baud rate as an
    integer to the function.  Returns -1 if an error occured and -2
    if no serial connection via usb is available.

# Tasks to be done

## TODO Implement procedure that returns a list of connected arduinos

We want to have a procedure that returns a list of paths to the
available arduinos connected to the computer.

### TODO Include tools.c in compilation process of arduserial.c

Include implemeted stack in the procedure for generating the list
of the available arduinos connected to the computer.

# Patch notes

## version 2.1

-   The `ser_autodetect` procedure returns -2 when no serial connection
    is available

## version 2

-   added a new functionality: `ser_autodetect(int baudrate)` that
    automatically detects an arduino connected to the computer.  Just
    pass the baud rate.

## version 1.9

-   fixed a but with head includes that prevented compiling

-   improved automated tests

## version 1.8

-   fixed a bug in ser<sub>readln</sub> function

-   added some tests (`make check`)

## version 1.7

-   removed the files that were generated automatically from the
    version control

-   slight changes to the initialization process prints more error
    messages when the library does system calls that fail

## version 1.6

-   fixed a bug in ser<sub>writeln</sub> should work properly now

-   increased library version number to 1:0:0

## version 1.5

-   fixed installation routine by adding the right autotool files to
    the repo

## version 1.4

-   improved initialization process of the serial connection

-   fixed a bug with readln

## version 1.3

-   added autotoolsupport to make the library (potentially) more
    portable

-   set library version to 0:0:0

## version 1.2

-   added a manpage

## version 1.1

-   fixed a bug in `ser_println`
