TESTS = testinit testflush testreadln testautodetect testlistarduinos testpathopt

libarduserialpath = $(top_builddir)/libarduserial/libarduserial.la

bin_PROGRAMS = $(TESTS)

testinit_LDADD        = $(libarduserialpath)
testreadln_LDADD      = $(libarduserialpath)
testflush_LDADD       = $(libarduserialpath)
testautodetect_LDADD  = $(libarduserialpath)
testlistarduinos_LDADD = $(libarduserialpath)
testpathopt_LDADD = $(libarduserialpath)

noinst_PROGRAMS = $(TESTS)
