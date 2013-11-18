#copyright (c) 2013 Automatak, LLC

ACLOCAL_AMFLAGS = -I m4

CORE_BOOST_LIBS = $(BOOST_SYSTEM_LIB)
TEST_BOOST_LIBS = $(CORE_BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB)

#AM_CXXFLAGS = $(BOOST_CPPFLAGS) -DBOOST_TEST_DYN_LINK -DBOOST_ASIO_ENABLE_CANCELIO -DBOOST_REGEX_NO_LIB -D_GLIBCXX_USE_NANOSLEEP
BOOST_DEFINES = $(BOOST_CPPFLAGS) -DBOOST_TEST_DYN_LINK -DBOOST_ASIO_ENABLE_CANCELIO -DBOOST_REGEX_NO_LIB -D_GLIBCXX_USE_NANOSLEEP
#AM_LDFLAGS  = $(BOOST_LDFLAGS)

lib_LTLIBRARIES = libopenpal.la libasiopal.la libopendnp3.la

#ls openpal/src/* -1 | awk '{print $0" \\"}'
libopenpal_la_LDFLAGS = -version-info 1:1:0
libopenpal_la_CXXFLAGS = -I$(top_srcdir)/openpal/include
libopenpal_la_SOURCES = \
openpal/src/AsyncLayerInterfaces.cpp \
openpal/src/Exception.cpp \
openpal/src/IHandlerAsync.cpp \
openpal/src/LogEntry.cpp \
openpal/src/Loggable.cpp \
openpal/src/Logger.cpp \
openpal/src/LogTypes.cpp \
openpal/src/MonotonicTimestamp.cpp \
openpal/src/Parsing.cpp \
openpal/src/TimeDuration.cpp \
openpal/src/ToHex.cpp

#ls asiopal/src/*.cpp -1 | awk '{print $0" \\"}'
libasiopal_la_LDFLAGS = -version-info 1:1:0
libasiopal_la_CXXFLAGS = -I$(top_srcdir)/openpal/include -I$(top_srcdir)/asiopal/include
libasiopal_la_SOURCES = \
asiopal/src/UTCTimeSource.cpp

#ls cpp/src/opendnp3/*.cpp -1 | awk '{print $0" \\"}'
libopendnp3_la_LDFLAGS = $(BOOST_LDFLAGS) -version-info 1:1:0
libopendnp3_la_CXXFLAGS = -I$(top_srcdir)/openpal/include -I$(top_srcdir)/cpp/include
libopendnp3_la_SOURCES = \
cpp/src/opendnp3/AnalogOutput.cpp \
cpp/src/opendnp3/APDUConstants.cpp \
cpp/src/opendnp3/APDU.cpp \
cpp/src/opendnp3/AppChannelStates.cpp \
cpp/src/opendnp3/AppHeader.cpp \
cpp/src/opendnp3/AppInterfaces.cpp \
cpp/src/opendnp3/AppLayerChannel.cpp \
cpp/src/opendnp3/AppLayer.cpp \
cpp/src/opendnp3/ApplicationStack.cpp \
cpp/src/opendnp3/ASIOExecutor.cpp \
cpp/src/opendnp3/ASIOSerialHelpers.cpp \
cpp/src/opendnp3/AsyncTaskBase.cpp \
cpp/src/opendnp3/AsyncTaskContinuous.cpp \
cpp/src/opendnp3/AsyncTaskGroup.cpp \
cpp/src/opendnp3/AsyncTaskNonPeriodic.cpp \
cpp/src/opendnp3/AsyncTaskPeriodic.cpp \
cpp/src/opendnp3/BaseDataTypes.cpp \
cpp/src/opendnp3/BufferTypes.cpp \
cpp/src/opendnp3/ChangeBuffer.cpp \
cpp/src/opendnp3/ChannelStates.cpp \
cpp/src/opendnp3/ClassCounter.cpp \
cpp/src/opendnp3/CommandStatus.cpp \
cpp/src/opendnp3/CommandTask.cpp \
cpp/src/opendnp3/ConstantCommandProcessor.cpp \
cpp/src/opendnp3/ControlRelayOutputBlock.cpp \
cpp/src/opendnp3/CopyableBuffer.cpp \
cpp/src/opendnp3/CRC.cpp \
cpp/src/opendnp3/Database.cpp \
cpp/src/opendnp3/DataPoll.cpp \
cpp/src/opendnp3/DestructorHook.cpp \
cpp/src/opendnp3/DeviceTemplate.cpp \
cpp/src/opendnp3/DNP3Channel.cpp \
cpp/src/opendnp3/DNP3Manager.cpp \
cpp/src/opendnp3/DNPCrc.cpp \
cpp/src/opendnp3/ExecutorPause.cpp \
cpp/src/opendnp3/HeaderReadIterator.cpp \
cpp/src/opendnp3/IndexedWriteIterator.cpp \
cpp/src/opendnp3/IOServiceThreadPool.cpp \
cpp/src/opendnp3/IStack.cpp \
cpp/src/opendnp3/ITimeWriteHandler.cpp \
cpp/src/opendnp3/LinkFrame.cpp \
cpp/src/opendnp3/LinkHeader.cpp \
cpp/src/opendnp3/LinkLayerConstants.cpp \
cpp/src/opendnp3/LinkLayer.cpp \
cpp/src/opendnp3/LinkLayerReceiver.cpp \
cpp/src/opendnp3/LinkLayerRouter.cpp \
cpp/src/opendnp3/LinkReceiverStates.cpp \
cpp/src/opendnp3/LinkRoute.cpp \
cpp/src/opendnp3/Log.cpp \
cpp/src/opendnp3/LogToStdio.cpp \
cpp/src/opendnp3/Master.cpp \
cpp/src/opendnp3/MasterSchedule.cpp \
cpp/src/opendnp3/MasterStackImpl.cpp \
cpp/src/opendnp3/MasterStates.cpp \
cpp/src/opendnp3/MasterTaskBase.cpp \
cpp/src/opendnp3/MultiplexingDataObserver.cpp \
cpp/src/opendnp3/ObjectHeader.cpp \
cpp/src/opendnp3/ObjectInterfaces.cpp \
cpp/src/opendnp3/ObjectReadIterator.cpp \
cpp/src/opendnp3/Objects.cpp \
cpp/src/opendnp3/ObjectWriteIterator.cpp \
cpp/src/opendnp3/OutstationSBOHandler.cpp \
cpp/src/opendnp3/OutstationStackImpl.cpp \
cpp/src/opendnp3/PackingUnpacking.cpp \
cpp/src/opendnp3/PhysicalLayerAsyncBase.cpp \
cpp/src/opendnp3/PhysicalLayerAsyncBaseTCP.cpp \
cpp/src/opendnp3/PhysicalLayerAsyncSerial.cpp \
cpp/src/opendnp3/PhysicalLayerAsyncTCPClient.cpp \
cpp/src/opendnp3/PhysicalLayerAsyncTCPServer.cpp \
cpp/src/opendnp3/PhysicalLayerMonitor.cpp \
cpp/src/opendnp3/PhysicalLayerMonitorStates.cpp \
cpp/src/opendnp3/PointClass.cpp \
cpp/src/opendnp3/PriLinkLayerStates.cpp \
cpp/src/opendnp3/ProtocolUtil.cpp \
cpp/src/opendnp3/QueuedCommandProcessor.cpp \
cpp/src/opendnp3/ResponseContext.cpp \
cpp/src/opendnp3/ResponseLoader.cpp \
cpp/src/opendnp3/SecLinkLayerStates.cpp \
cpp/src/opendnp3/SerialTypes.cpp \
cpp/src/opendnp3/ShiftableBuffer.cpp \
cpp/src/opendnp3/SimpleCommandHandler.cpp \
cpp/src/opendnp3/SimpleDataObserver.cpp \
cpp/src/opendnp3/SlaveConfig.cpp \
cpp/src/opendnp3/Slave.cpp \
cpp/src/opendnp3/SlaveEventBuffer.cpp \
cpp/src/opendnp3/SlaveResponseTypes.cpp \
cpp/src/opendnp3/SlaveStates.cpp \
cpp/src/opendnp3/SolicitedChannel.cpp \
cpp/src/opendnp3/StackBase.cpp \
cpp/src/opendnp3/StackState.cpp \
cpp/src/opendnp3/StartupTasks.cpp \
cpp/src/opendnp3/SubjectBase.cpp \
cpp/src/opendnp3/Threadable.cpp \
cpp/src/opendnp3/Thread.cpp \
cpp/src/opendnp3/TimerASIO.cpp \
cpp/src/opendnp3/TimeTransaction.cpp \
cpp/src/opendnp3/TLS_Base.cpp \
cpp/src/opendnp3/TrackingTaskGroup.cpp \
cpp/src/opendnp3/TransportLayer.cpp \
cpp/src/opendnp3/TransportRx.cpp \
cpp/src/opendnp3/TransportStates.cpp \
cpp/src/opendnp3/TransportTx.cpp \
cpp/src/opendnp3/UnsolicitedChannel.cpp \
cpp/src/opendnp3/Util.cpp \
cpp/src/opendnp3/VtoData.cpp

#pkginclude_HEADERS = \
#cpp/include/opendnp3/AnalogOutput.h

#ls cpp/src/opendnp3/*.cpp -1 | awk '{print $0" \\"}'
check_PROGRAMS = dnp3test
TESTS = dnp3test
dnp3test_CXXFLAGS = -I$(top_srcdir)/cpp/src -I$(top_srcdir)/cpp/include -I$(top_srcdir)/openpal/include -I$(top_srcdir)/asiopal/include $(BOOST_DEFINES)
dnp3test_LDADD = libopenpal.la libasiopal.la libopendnp3.la $(TEST_BOOST_LIBS)
dnp3test_LDFLAGS = $(BOOST_LDFLAGS)
dnp3test_SOURCES = \
cpp/tests/AppLayerTest.cpp \
cpp/tests/AsyncPhysBaseTest.cpp \
cpp/tests/AsyncPhysTestObject.cpp \
cpp/tests/AsyncSerialTestObject.cpp \
cpp/tests/AsyncTestObjectASIO.cpp \
cpp/tests/AsyncTestObject.cpp \
cpp/tests/BufferHelpers.cpp \
cpp/tests/BufferTestObject.cpp \
cpp/tests/ComparingDataObserver.cpp \
cpp/tests/DNPHelpers.cpp \
cpp/tests/FlexibleDataObserver.cpp \
cpp/tests/IntegrationTest.cpp \
cpp/tests/LinkLayerRouterTest.cpp \
cpp/tests/LinkLayerTest.cpp \
cpp/tests/LogTester.cpp \
cpp/tests/LoopbackPhysicalLayerAsync.cpp \
cpp/tests/LowerLayerToPhysAdapter.cpp \
cpp/tests/MasterTestObject.cpp \
cpp/tests/MockAppLayer.cpp \
cpp/tests/MockAppUser.cpp \
cpp/tests/MockExecutor.cpp \
cpp/tests/MockFrameSink.cpp \
cpp/tests/MockLowerLayer.cpp \
cpp/tests/MockPhysicalLayerAsync.cpp \
cpp/tests/MockPhysicalLayerMonitor.cpp \
cpp/tests/MockUpperLayer.cpp \
cpp/tests/PhysicalLayerWrapper.cpp \
cpp/tests/PhysLoopback.cpp \
cpp/tests/RandomizedBuffer.cpp \
cpp/tests/ResponseLoaderTestObject.cpp \
cpp/tests/SlaveTestObject.cpp \
cpp/tests/StopWatch.cpp \
cpp/tests/TestAPDU.cpp \
cpp/tests/TestAPDUWriting.cpp \
cpp/tests/TestAppLayer.cpp \
cpp/tests/TestASIO.cpp \
cpp/tests/TestASIOThreadPool.cpp \
cpp/tests/TestAsyncTask.cpp \
cpp/tests/TestCastLongLongDouble.cpp \
cpp/tests/TestChangeBuffer.cpp \
cpp/tests/TestCommandHelpers.cpp \
cpp/tests/TestCommandTask.cpp \
cpp/tests/TestCommandTypes.cpp \
cpp/tests/TestCRC.cpp \
cpp/tests/TestDatabase.cpp \
cpp/tests/TestDNP3Manager.cpp \
cpp/tests/TestEventBufferBase.cpp \
cpp/tests/TestEventBuffers.cpp \
cpp/tests/TestIntegration.cpp \
cpp/tests/TestLinkFrameDNP.cpp \
cpp/tests/TestLinkLayer.cpp \
cpp/tests/TestLinkLayerRouter.cpp \
cpp/tests/TestLinkReceiver.cpp \
cpp/tests/TestLinkRoute.cpp \
cpp/tests/TestLog.cpp \
cpp/tests/TestMaster.cpp \
cpp/tests/TestMisc.cpp \
cpp/tests/TestObjects.cpp \
cpp/tests/TestPackingUnpacking.cpp \
cpp/tests/TestParsing.cpp \
cpp/tests/TestPhysicalLayerAsyncBase.cpp \
cpp/tests/TestPhysicalLayerAsyncSerial.cpp \
cpp/tests/TestPhysicalLayerAsyncTCP.cpp \
cpp/tests/TestPhysicalLayerLoopback.cpp \
cpp/tests/TestPhysicalLayerMonitor.cpp \
cpp/tests/TestResponseLoader.cpp \
cpp/tests/TestShiftableBuffer.cpp \
cpp/tests/TestSlave.cpp \
cpp/tests/TestSlaveEventBuffer.cpp \
cpp/tests/TestStartBoostUTF.cpp \
cpp/tests/TestTime.cpp \
cpp/tests/TestTimers.cpp \
cpp/tests/TestTransportLayer.cpp \
cpp/tests/TestTransportLoopback.cpp \
cpp/tests/TestTransportScalability.cpp \
cpp/tests/TestTypes.cpp \
cpp/tests/TestUtil.cpp \
cpp/tests/Timeout.cpp \
cpp/tests/TransportIntegrationStack.cpp \
cpp/tests/TransportLoopbackTestObject.cpp \
cpp/tests/TransportScalabilityTestObject.cpp \
cpp/tests/TransportStackPair.cpp \
cpp/tests/TransportTestObject.cpp \
cpp/tests/WrappedTcpPipe.cpp


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

