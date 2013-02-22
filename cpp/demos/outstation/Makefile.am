#copyright (c) 2013 Automatak, LLC

ACLOCAL_AMFLAGS = -I ../../../m4

CORE_BOOST_LIBS = $(BOOST_SYSTEM_LIB)
TEST_BOOST_LIBS = $(CORE_BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB)

AM_CXXFLAGS = -std=c++11 -lopendnp3 -DOPENDNP3_NO_MASTER
AM_LDFLAGS  = $(BOOST_LDFLAGS)

bin_PROGRAMS = outstationdemo

outstationdemo_LDFLAGS = $(BOOST_LDFLAGS)
outstationdemo_LDADD = $(CORE_BOOST_LIBS)
outstationdemo_SOURCES = DemoMain.cpp
