#copyright (c) 2013 Automatak, LLC

ACLOCAL_AMFLAGS = -I m4

CORE_BOOST_LIBS = $(BOOST_SYSTEM_LIB)
TEST_BOOST_LIBS = $(CORE_BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB)

AM_CXXFLAGS = -std=c++11 $(BOOST_CPPFLAGS) -DBOOST_TEST_DYN_LINK -DBOOST_ASIO_ENABLE_CANCELIO -DBOOST_REGEX_NO_LIB -D_GLIBCXX_USE_NANOSLEEP
AM_LDFLAGS  = $(BOOST_LDFLAGS)

lib_LTLIBRARIES = libopendnp3.la

libopendnp3_la_LDFLAGS = $(BOOST_LDFLAGS) -version-info 1:1:0
libopendnp3_la_LIBADD = $(CORE_BOOST_LIBS)
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

pkginclude_HEADERS = \
	cpp/src/opendnp3/AnalogOutput.h \
	cpp/src/opendnp3/APDUConstants.h \
	cpp/src/opendnp3/AppConfig.h \
	cpp/src/opendnp3/BaseDataTypes.h \
	cpp/src/opendnp3/ClassMask.h \
	cpp/src/opendnp3/CommandResponse.h \
	cpp/src/opendnp3/CommandStatus.h \
	cpp/src/opendnp3/ControlRelayOutputBlock.h \
	cpp/src/opendnp3/DataInterfaces.h \
	cpp/src/opendnp3/DataTypes.h \
	cpp/src/opendnp3/DestructorHook.h \
	cpp/src/opendnp3/DeviceTemplate.h \
	cpp/src/opendnp3/DeviceTemplateTypes.h \
	cpp/src/opendnp3/DNP3Channel.h \
	cpp/src/opendnp3/DNP3Manager.h \
	cpp/src/opendnp3/DNPConstants.h \
	cpp/src/opendnp3/DNPDatabaseTypes.h \
	cpp/src/opendnp3/DNPExceptions.h \
	cpp/src/opendnp3/Exception.h \
	cpp/src/opendnp3/IChannel.h \
	cpp/src/opendnp3/ICommandHandler.h \
	cpp/src/opendnp3/ICommandProcessor.h \
	cpp/src/opendnp3/IMaster.h \
	cpp/src/opendnp3/IOutstation.h \
	cpp/src/opendnp3/IStack.h \
	cpp/src/opendnp3/IStackObserver.h \
	cpp/src/opendnp3/ITransactable.h \
	cpp/src/opendnp3/IVtoEndpoint.h \
	cpp/src/opendnp3/LinkLayerConstants.h \
	cpp/src/opendnp3/LinkConfig.h \
	cpp/src/opendnp3/LogBase.h \
	cpp/src/opendnp3/LogEntry.h \
	cpp/src/opendnp3/Logger.h \
	cpp/src/opendnp3/LogToFile.h \
	cpp/src/opendnp3/LogToStdio.h \
	cpp/src/opendnp3/LogTypes.h \
	cpp/src/opendnp3/MasterConfig.h \
	cpp/src/opendnp3/MasterConfigTypes.h \
	cpp/src/opendnp3/MasterStackConfig.h \
	cpp/src/opendnp3/ObjectInterfaces.h \
	cpp/src/opendnp3/Parsing.h \
	cpp/src/opendnp3/PointClass.h \
	cpp/src/opendnp3/QualityMasks.h \
	cpp/src/opendnp3/QualityConverter.h \
	cpp/src/opendnp3/SerialTypes.h \
	cpp/src/opendnp3/SimpleCommandHandler.h \
	cpp/src/opendnp3/SimpleDataObserver.h \
	cpp/src/opendnp3/Singleton.h \
	cpp/src/opendnp3/SlaveConfig.h \
	cpp/src/opendnp3/SlaveStackConfig.h \
	cpp/src/opendnp3/TransportConstants.h \
	cpp/src/opendnp3/Types.h \
	cpp/src/opendnp3/Uncopyable.h \
	cpp/src/opendnp3/Util.h \
	cpp/src/opendnp3/VtoConfig.h \
	cpp/src/opendnp3/VtoRouterSettings.h

check_PROGRAMS = dnp3test
TESTS = dnp3test
dnp3test_CPPFLAGS = -I./cpp/src
dnp3test_LDADD = libopendnp3.la $(TEST_BOOST_LIBS)
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
	cpp/tests/IntegrationTest.cpp \
	cpp/tests/LinkLayerRouterTest.cpp \
	cpp/tests/LinkLayerTest.cpp \
	cpp/tests/LogTester.cpp \
	cpp/tests/LoopbackPhysicalLayerAsync.cpp \
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
	cpp/tests/TestEnhancedVtoRouter.cpp \
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
	cpp/tests/TestQualityMasks.cpp \
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
	cpp/tests/TestVtoInterface.cpp \
	cpp/tests/TestVtoRouter.cpp \
	cpp/tests/TestVtoWriter.cpp \
	cpp/tests/Timeout.cpp \
	cpp/tests/TransportIntegrationStack.cpp \
	cpp/tests/TransportLoopbackTestObject.cpp \
	cpp/tests/TransportScalabilityTestObject.cpp \
	cpp/tests/TransportStackPair.cpp \
	cpp/tests/TransportTestObject.cpp \
	cpp/tests/VtoIntegrationTestBase.cpp \
	cpp/tests/WrappedTcpPipe.cpp

if WANT_JAVA
	
lib_LTLIBRARIES += libopendnp3java.la
libopendnp3java_la_CPPFLAGS = -I./cpp/src
libopendnp3java_la_LDFLAGS = -version-info 1:1:0
libopendnp3java_la_LIBADD = libopendnp3.la
libopendnp3java_la_SOURCES = \
	java/cpp/com_automatak_dnp3_impl_ChannelImpl.cpp \
	java/cpp/com_automatak_dnp3_impl_CommandProcessorImpl.cpp \
	java/cpp/com_automatak_dnp3_impl_DataObserverImpl.cpp \
	java/cpp/com_automatak_dnp3_impl_ManagerImpl.cpp \
	java/cpp/com_automatak_dnp3_impl_MasterImpl.cpp \
	java/cpp/com_automatak_dnp3_impl_OutstationImpl.cpp \
	java/cpp/CommandHandlerAdapter.cpp \
	java/cpp/ConfigReader.cpp \
	java/cpp/DataObserverAdapter.cpp \
	java/cpp/JNIHelpers.cpp \
	java/cpp/LogSubscriberAdapter.cpp

endif

