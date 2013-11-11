#copyright (c) 2013 Automatak, LLC

ACLOCAL_AMFLAGS = -I m4

CORE_BOOST_LIBS = $(BOOST_SYSTEM_LIB)
TEST_BOOST_LIBS = $(CORE_BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB)

#AM_CXXFLAGS = $(BOOST_CPPFLAGS) -DBOOST_TEST_DYN_LINK -DBOOST_ASIO_ENABLE_CANCELIO -DBOOST_REGEX_NO_LIB -D_GLIBCXX_USE_NANOSLEEP
#AM_LDFLAGS  = $(BOOST_LDFLAGS)

lib_LTLIBRARIES = libopenpal.la

#ls openpal/src/* -1 | awk '{print $0" \\"}'
libopenpal_la_LDFLAGS = -version-info 1:1:0
libopenpal_la_CXXFLAGS = -I$(top_srcdir)/openpal/include
libopenpal_la_SOURCES = \
openpal/src/AsyncLayerInterfaces.cpp \
openpal/src/Clock.cpp \
openpal/src/Exception.cpp \
openpal/src/IHandlerAsync.cpp \
openpal/src/LogEntry.cpp \
openpal/src/Loggable.cpp \
openpal/src/Logger.cpp \
openpal/src/LogTypes.cpp \
openpal/src/Parsing.cpp \
openpal/src/ToHex.cpp

#libopendnp3_la_LDFLAGS = $(BOOST_LDFLAGS) -version-info 1:1:0
#libopendnp3_la_LIBADD = $(CORE_BOOST_LIBS)
#libopendnp3_la_SOURCES = \
#cpp/src/opendnp3/AlwaysOpeningVtoRouter.cpp

#pkginclude_HEADERS = \
#cpp/include/opendnp3/AnalogOutput.h


#check_PROGRAMS = dnp3test
#TESTS = dnp3test
#dnp3test_CPPFLAGS = -I$(top_srcdir)/cpp/src
#dnp3test_LDADD = libopendnp3.la $(TEST_BOOST_LIBS)
#dnp3test_SOURCES = \
#cpp/tests/AppLayerTest.cpp


if WANT_JAVA
	
#lib_LTLIBRARIES += libopendnp3java.la
#libopendnp3java_la_CPPFLAGS = -I./cpp/src
#libopendnp3java_la_LDFLAGS = -version-info 1:1:0
#libopendnp3java_la_LIBADD = libopendnp3.la
#libopendnp3java_la_SOURCES = \
#	java/cpp/com_automatak_dnp3_impl_ChannelImpl.cpp \
#	java/cpp/com_automatak_dnp3_impl_CommandProcessorImpl.cpp \
#	java/cpp/com_automatak_dnp3_impl_DataObserverImpl.cpp \
#	java/cpp/com_automatak_dnp3_impl_ManagerImpl.cpp \
#	java/cpp/com_automatak_dnp3_impl_MasterImpl.cpp \
#	java/cpp/com_automatak_dnp3_impl_OutstationImpl.cpp \
#	java/cpp/com_automatak_dnp3_impl_VTOEndpointImpl.cpp \
#	java/cpp/com_automatak_dnp3_impl_StackBase.cpp \
#	java/cpp/CommandHandlerAdapter.cpp \
#	java/cpp/ConfigReader.cpp \
#	java/cpp/DataObserverAdapter.cpp \
#	java/cpp/JNIHelpers.cpp \
#	java/cpp/LogSubscriberAdapter.cpp
endif

