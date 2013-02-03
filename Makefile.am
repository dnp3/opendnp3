#copyright (c) 2013 Automatak, LLC

ACLOCAL_AMFLAGS = -I m4

CORE_BOOST_LIBS = $(BOOST_SYSTEM_LIB)
TEST_BOOST_LIBS = $(CORE_BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB)

AM_CXXFLAGS = -std=c++11

lib_LTLIBRARIES = libopendnp3.la

libopendnp3_la_SOURCES = \
	cpp/src/opendnp3/AlwaysOpeningVtoRouter.cpp \
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
	cpp/src/opendnp3/AsyncLayerInterfaces.cpp \
	cpp/src/opendnp3/AsyncTaskBase.cpp \
	cpp/src/opendnp3/AsyncTaskContinuous.cpp \
	cpp/src/opendnp3/AsyncTaskGroup.cpp \
	cpp/src/opendnp3/AsyncTaskNonPeriodic.cpp \
	cpp/src/opendnp3/AsyncTaskPeriodic.cpp \
	cpp/src/opendnp3/AsyncTaskScheduler.cpp \
	cpp/src/opendnp3/BaseDataTypes.cpp \
	cpp/src/opendnp3/BufferTypes.cpp \
	cpp/src/opendnp3/ChangeBuffer.cpp \
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
	cpp/src/opendnp3/EnhancedVto.cpp \
	cpp/src/opendnp3/EnhancedVtoRouter.cpp \
	cpp/src/opendnp3/Exception.cpp \
	cpp/src/opendnp3/ExecutorPause.cpp \
	cpp/src/opendnp3/FlexibleDataObserver.cpp \
	cpp/src/opendnp3/HeaderReadIterator.cpp \
	cpp/src/opendnp3/IExecutor.cpp \
	cpp/src/opendnp3/IHandlerAsync.cpp \
	cpp/src/opendnp3/IndexedWriteIterator.cpp \
	cpp/src/opendnp3/IOServiceThreadPool.cpp \
	cpp/src/opendnp3/IStack.cpp \
	cpp/src/opendnp3/IStackObserver.cpp \
	cpp/src/opendnp3/ITimeSource.cpp \
	cpp/src/opendnp3/LinkFrame.cpp \
	cpp/src/opendnp3/LinkHeader.cpp \
	cpp/src/opendnp3/LinkLayerConstants.cpp \
	cpp/src/opendnp3/LinkLayer.cpp \
	cpp/src/opendnp3/LinkLayerReceiver.cpp \
	cpp/src/opendnp3/LinkLayerRouter.cpp \
	cpp/src/opendnp3/LinkReceiverStates.cpp \
	cpp/src/opendnp3/LinkRoute.cpp \
	cpp/src/opendnp3/Log.cpp \
	cpp/src/opendnp3/LogEntryCircularBuffer.cpp \
	cpp/src/opendnp3/LogEntry.cpp \
	cpp/src/opendnp3/Loggable.cpp \
	cpp/src/opendnp3/Logger.cpp \
	cpp/src/opendnp3/LogToFile.cpp \
	cpp/src/opendnp3/LogToStdio.cpp \
	cpp/src/opendnp3/LogTypes.cpp \
	cpp/src/opendnp3/LowerLayerToPhysAdapter.cpp \
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
	cpp/src/opendnp3/OutstationStackImpl.cpp \
	cpp/src/opendnp3/PackingUnpacking.cpp \
	cpp/src/opendnp3/Parsing.cpp \
	cpp/src/opendnp3/PhysicalLayerAsyncBase.cpp \
	cpp/src/opendnp3/PhysicalLayerAsyncBaseTCP.cpp \
	cpp/src/opendnp3/PhysicalLayerAsyncSerial.cpp \
	cpp/src/opendnp3/PhysicalLayerAsyncTCPClient.cpp \
	cpp/src/opendnp3/PhysicalLayerAsyncTCPServer.cpp \
	cpp/src/opendnp3/PhysicalLayerMonitor.cpp \
	cpp/src/opendnp3/PhysicalLayerMonitorStates.cpp \
	cpp/src/opendnp3/PhysicalLayerStates.cpp \
	cpp/src/opendnp3/PhysLoopback.cpp \
	cpp/src/opendnp3/PointClass.cpp \
	cpp/src/opendnp3/PriLinkLayerStates.cpp \
	cpp/src/opendnp3/ProtocolUtil.cpp \
	cpp/src/opendnp3/QualityConverter.cpp \
	cpp/src/opendnp3/QueuedCommandProcessor.cpp \
	cpp/src/opendnp3/RandomizedBuffer.cpp \
	cpp/src/opendnp3/ResponseContext.cpp \
	cpp/src/opendnp3/ResponseLoader.cpp \
	cpp/src/opendnp3/SecLinkLayerStates.cpp \
	cpp/src/opendnp3/ShiftableBuffer.cpp \
	cpp/src/opendnp3/SimpleCommandHandler.cpp \
	cpp/src/opendnp3/SimpleDataObserver.cpp \
	cpp/src/opendnp3/SlaveConfig.cpp \
	cpp/src/opendnp3/Slave.cpp \
	cpp/src/opendnp3/SlaveEventBuffer.cpp \
	cpp/src/opendnp3/SlaveResponseTypes.cpp \
	cpp/src/opendnp3/SlaveStates.cpp \
	cpp/src/opendnp3/SolicitedChannel.cpp \
	cpp/src/opendnp3/StartupTasks.cpp \
	cpp/src/opendnp3/SubjectBase.cpp \
	cpp/src/opendnp3/Threadable.cpp \
	cpp/src/opendnp3/Thread.cpp \
	cpp/src/opendnp3/TimerASIO.cpp \
	cpp/src/opendnp3/TimeSource.cpp \
	cpp/src/opendnp3/TLS_Base.cpp \
	cpp/src/opendnp3/ToHex.cpp \
	cpp/src/opendnp3/TrackingTaskGroup.cpp \
	cpp/src/opendnp3/TransportLayer.cpp \
	cpp/src/opendnp3/TransportRx.cpp \
	cpp/src/opendnp3/TransportStates.cpp \
	cpp/src/opendnp3/TransportTx.cpp \
	cpp/src/opendnp3/UnsolicitedChannel.cpp \
	cpp/src/opendnp3/Util.cpp \
	cpp/src/opendnp3/VtoData.cpp \
	cpp/src/opendnp3/VtoEndpointImpl.cpp \
	cpp/src/opendnp3/VtoReader.cpp \
	cpp/src/opendnp3/VtoRouter.cpp \
	cpp/src/opendnp3/VtoRouterSettings.cpp \
	cpp/src/opendnp3/VtoTransmitTask.cpp \
	cpp/src/opendnp3/VtoWriter.cpp

check_PROGRAMS = dnp3test
TESTS = dnp3test
dnp3test_CPPFLAGS = -I./cpp/src
dnp3test_LDADD = libopendnp3.la $(BOOST_TEST_LIBS)
dnp3test_SOURCES = \
	cpp/test/AsyncPhysBaseTest.cpp \
	cpp/test/AsyncPhysTestObject.cpp \
	cpp/test/AsyncSerialTestObject.cpp \
	cpp/test/AsyncTestObjectASIO.cpp \
	cpp/test/AsyncTestObject.cpp \
	cpp/test/BufferHelpers.cpp \
	cpp/test/BufferTestObject.cpp \
	cpp/test/LogTester.cpp \
	cpp/test/LoopbackPhysicalLayerAsync.cpp \
	cpp/test/MockExecutor.cpp \
	cpp/test/MockLowerLayer.cpp \
	cpp/test/MockPhysicalLayerAsync.cpp \
	cpp/test/MockPhysicalLayerMonitor.cpp \
	cpp/test/MockUpperLayer.cpp \
	cpp/test/PhysicalLayerWrapper.cpp \
	cpp/test/StopWatch.cpp \
	cpp/test/TestASIO.cpp \
	cpp/test/TestASIOThreadPool.cpp \
	cpp/test/TestAsyncTask.cpp \
	cpp/test/TestCastLongLongDouble.cpp \
	cpp/test/TestChangeBuffer.cpp \
	cpp/test/TestLog.cpp \
	cpp/test/TestMisc.cpp \
	cpp/test/TestPackingUnpacking.cpp \
	cpp/test/TestParsing.cpp \
	cpp/test/TestPhysicalLayerAsyncBase.cpp \
	cpp/test/TestPhysicalLayerAsyncSerial.cpp \
	cpp/test/TestPhysicalLayerAsyncTCP.cpp \
	cpp/test/TestPhysicalLayerLoopback.cpp \
	cpp/test/TestPhysicalLayerMonitor.cpp \
	cpp/test/TestQualityMasks.cpp \
	cpp/test/TestShiftableBuffer.cpp \
	cpp/test/TestTime.cpp \
	cpp/test/TestTimers.cpp \
	cpp/test/TestTypes.cpp \
	cpp/test/TestUtil.cpp \
	cpp/test/Timeout.cpp \
	cpp/test/WrappedTcpPipe.cpp




