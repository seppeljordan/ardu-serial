#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

ACLOCAL_AMFLAGS([[-I m4]])
AC_INIT([arduserial], [1.8], [sebastian.jordan.mail@googlemail.com])
AC_CONFIG_SRCDIR([libarduserial/arduserial.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall])
AM_PROG_AR
LT_INIT

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile 
    libarduserial/Makefile 
    testsuite/Makefile 
    testsuite/arduserial/Makefile])
AC_OUTPUT
