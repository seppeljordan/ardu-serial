TESTS = testinit testflush testreadln

libarduserialpath = $(top_builddir)/libarduserial/libarduserial.la

bin_PROGRAMS = testinit testreadln testflush

testinit_LDADD   = $(libarduserialpath)
testreadln_LDADD = $(libarduserialpath)
testflush_LDADD  = $(libarduserialpath)

noinst_PROGRAMS = testinit testreadln testflush
