# Copyright (C) 2013 Automatak, LLC

AM_CXXFLAGS = -std=c++11 -I ../

bin_PROGRAMS = testapl
testapl_LDADD = libasyncprotocol.la libapltesttools.la $(TEST_BOOST_LIBS)
testapl_SOURCES = \
	AsyncPhysBaseTest.cpp \
	AsyncSerialTestObject.cpp \
	TestASIO.cpp \
	TestASIOThreadPool.cpp \
	TestAsyncTask.cpp \
	TestCastLongLongDouble.cpp \
	TestChangeBuffer.cpp \
	TestLog.cpp \
	TestMisc.cpp \
	TestPackingUnpacking.cpp \
	TestParsing.cpp \
	TestPhysicalLayerAsyncBase.cpp \
	TestPhysicalLayerAsyncSerial.cpp \
	TestPhysicalLayerAsyncTCP.cpp \
	TestPhysicalLayerLoopback.cpp \
	TestPhysicalLayerMonitor.cpp \
	TestQualityMasks.cpp \
	TestShiftableBuffer.cpp \
	TestTime.cpp \
	TestTimers.cpp \
	TestTypes.cpp \
	TestUtil.cpp
