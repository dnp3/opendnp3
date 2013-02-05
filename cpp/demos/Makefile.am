#copyright (c) 2013 Automatak, LLC

ACLOCAL_AMFLAGS = -I ../../m4

CORE_BOOST_LIBS = $(BOOST_SYSTEM_LIB)
TEST_BOOST_LIBS = $(CORE_BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB)

AM_CXXFLAGS = -std=c++11 -lopendnp3
AM_LDFLAGS  = $(BOOST_LDFLAGS)

bin_PROGRAMS = masterdemo outstationdemo

masterdemo_LDFLAGS = $(BOOST_LDFLAGS)
masterdemo_LDADD = $(CORE_BOOST_LIBS)
masterdemo_SOURCES = \
	master/DemoMain.cpp

outstationdemo_LDFLAGS = $(BOOST_LDFLAGS)
outstationdemo_LDADD = $(CORE_BOOST_LIBS)
outstationdemo_SOURCES = \
	outstation/DemoMain.cpp
