## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = dejagnu
check_PROGRAMS = testarduserial
testarduserial_SOURCES = testarduserial.c
testarduserial_LDADD = libarduserial.la

lib_LTLIBRARIES = libarduserial.la
libarduserial_la_SOURCES = arduserial.c
libarduserial_la_LDFLAGS = -version-info 1:1:0
include_HEADERS = arduserial.h

CLEANFILES = *~
DISTCLEANFILES = *~


dist_doc_DATA = README

EXTRA_DIST = testsuite

RUNTESTDEFAULTFLAGS = --tool testarduserial TESTARDUSERIAL=`pwd`/testarduserial \
                      --srcdir $$srcdir/testsuite 
