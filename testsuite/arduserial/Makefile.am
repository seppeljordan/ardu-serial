TESTS = testinit testflush testreadln testautodetect

libarduserialpath = $(top_builddir)/libarduserial/libarduserial.la

bin_PROGRAMS = $(TESTS)

testinit_LDADD        = $(libarduserialpath)
testreadln_LDADD      = $(libarduserialpath)
testflush_LDADD       = $(libarduserialpath)
testautodetect_LDADD  = $(libarduserialpath)

noinst_PROGRAMS = testinit testreadln testflush testautodetect
