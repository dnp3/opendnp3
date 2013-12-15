#copyright (c) 2013 Automatak, LLC

ACLOCAL_AMFLAGS = -I m4

CORE_BOOST_LIBS = $(BOOST_SYSTEM_LIB)
TEST_BOOST_LIBS = $(CORE_BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB)

BOOST_DEFINES = $(BOOST_CPPFLAGS) -DBOOST_TEST_DYN_LINK -DBOOST_ASIO_ENABLE_CANCELIO -DBOOST_REGEX_NO_LIB -D_GLIBCXX_USE_NANOSLEEP

OPENPAL_INCLUDE = $(top_srcdir)/cpp/openpal/include
ASIOPAL_INCLUDE = $(top_srcdir)/cpp/asiopal/include
OPENDNP3_INCLUDE = $(top_srcdir)/cpp/opendnp3/include
ASIODNP3_INCLUDE = $(top_srcdir)/cpp/asiodnp3/include
OPENDNP3_SRCS = $(top_srcdir)/cpp/opendnp3/src

bin_PROGRAMS = masterdemo outstationdemo apdufuzz

lib_LTLIBRARIES = libopenpal.la libasiopal.la libopendnp3.la

#ls cpp/openpal/src/* -1 | awk '{print $0" \\"}'
libopenpal_la_LDFLAGS = -version-info 1:1:0
libopenpal_la_CXXFLAGS = -I$(OPENPAL_INCLUDE)
libopenpal_la_SOURCES = \
cpp/openpal/src/gen/LogLevel.cpp \
cpp/openpal/src/AsyncLayerInterfaces.cpp \
cpp/openpal/src/BufferWrapper.cpp \
cpp/openpal/src/Exception.cpp \
cpp/openpal/src/IHandlerAsync.cpp \
cpp/openpal/src/LogEntry.cpp \
cpp/openpal/src/Loggable.cpp \
cpp/openpal/src/Logger.cpp \
cpp/openpal/src/MonotonicTimestamp.cpp \
cpp/openpal/src/ReverseEndian.cpp \
cpp/openpal/src/TimeDuration.cpp \
cpp/openpal/src/ToHex.cpp

#ls cpp/asiopal/src/*.cpp -1 | awk '{print $0" \\"}'
libasiopal_la_LDFLAGS = -version-info 1:1:0
libasiopal_la_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I$(ASIOPAL_INCLUDE) $(BOOST_DEFINES)
libasiopal_la_SOURCES = \
cpp/asiopal/src/ASIOExecutor.cpp \
cpp/asiopal/src/ASIOSerialHelpers.cpp \
cpp/asiopal/src/IOServiceThreadPool.cpp \
cpp/asiopal/src/Log.cpp \
cpp/asiopal/src/LogToStdio.cpp \
cpp/asiopal/src/PhysicalLayerAsyncBase.cpp \
cpp/asiopal/src/PhysicalLayerAsyncBaseTCP.cpp \
cpp/asiopal/src/PhysicalLayerAsyncSerial.cpp \
cpp/asiopal/src/PhysicalLayerAsyncTCPClient.cpp \
cpp/asiopal/src/PhysicalLayerAsyncTCPServer.cpp \
cpp/asiopal/src/SerialTypes.cpp \
cpp/asiopal/src/TimerASIO.cpp \
cpp/asiopal/src/UTCTimeSource.cpp

#ls cpp/opendnp3/src/opendnp3/*.cpp -1 | awk '{print $0" \\"}'
libopendnp3_la_LDFLAGS = $(BOOST_LDFLAGS) -version-info 1:1:0
libopendnp3_la_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I$(ASIOPAL_INCLUDE) -I $(OPENDNP3_INCLUDE)
libopendnp3_la_SOURCES = \
cpp/opendnp3/src/opendnp3/gen/ChannelState.cpp \
cpp/opendnp3/src/opendnp3/gen/CommandResult.cpp \
cpp/opendnp3/src/opendnp3/gen/CommandStatus.cpp \
cpp/opendnp3/src/opendnp3/gen/ControlCode.cpp \
cpp/opendnp3/src/opendnp3/gen/EventAnalogResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/EventBinaryResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/EventCounterResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/FunctionCode.cpp \
cpp/opendnp3/src/opendnp3/gen/LinkFunction.cpp \
cpp/opendnp3/src/opendnp3/gen/QualifierCode.cpp \
cpp/opendnp3/src/opendnp3/gen/StackState.cpp \
cpp/opendnp3/src/opendnp3/gen/StaticAnalogResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/StaticBinaryResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/StaticCounterResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/StaticSetpointStatusResponse.cpp \
cpp/opendnp3/src/opendnp3/objects/Group10.cpp \
cpp/opendnp3/src/opendnp3/objects/Group11.cpp \
cpp/opendnp3/src/opendnp3/objects/Group12.cpp \
cpp/opendnp3/src/opendnp3/objects/Group1.cpp \
cpp/opendnp3/src/opendnp3/objects/Group20.cpp \
cpp/opendnp3/src/opendnp3/objects/Group21.cpp \
cpp/opendnp3/src/opendnp3/objects/Group22.cpp \
cpp/opendnp3/src/opendnp3/objects/Group23.cpp \
cpp/opendnp3/src/opendnp3/objects/Group2.cpp \
cpp/opendnp3/src/opendnp3/objects/Group30.cpp \
cpp/opendnp3/src/opendnp3/objects/Group32.cpp \
cpp/opendnp3/src/opendnp3/objects/Group40.cpp \
cpp/opendnp3/src/opendnp3/objects/Group41.cpp \
cpp/opendnp3/src/opendnp3/objects/Group42.cpp \
cpp/opendnp3/src/opendnp3/objects/Group50.cpp \
cpp/opendnp3/src/opendnp3/objects/Group51.cpp \
cpp/opendnp3/src/opendnp3/objects/Group52.cpp \
cpp/opendnp3/src/opendnp3/AnalogOutput.cpp \
cpp/opendnp3/src/opendnp3/APDU.cpp \
cpp/opendnp3/src/opendnp3/APDUParser.cpp \
cpp/opendnp3/src/opendnp3/APDUHeaderParser.cpp \
cpp/opendnp3/src/opendnp3/AppChannelStates.cpp \
cpp/opendnp3/src/opendnp3/AppControlField.cpp \
cpp/opendnp3/src/opendnp3/AppHeader.cpp \
cpp/opendnp3/src/opendnp3/AppLayerChannel.cpp \
cpp/opendnp3/src/opendnp3/AppLayer.cpp \
cpp/opendnp3/src/opendnp3/ApplicationStack.cpp \
cpp/opendnp3/src/opendnp3/AsyncTaskBase.cpp \
cpp/opendnp3/src/opendnp3/AsyncTaskContinuous.cpp \
cpp/opendnp3/src/opendnp3/AsyncTaskGroup.cpp \
cpp/opendnp3/src/opendnp3/AsyncTaskNonPeriodic.cpp \
cpp/opendnp3/src/opendnp3/AsyncTaskPeriodic.cpp \
cpp/opendnp3/src/opendnp3/BaseDataTypes.cpp \
cpp/opendnp3/src/opendnp3/BitReader.cpp \
cpp/opendnp3/src/opendnp3/BufferType.cpp \
cpp/opendnp3/src/opendnp3/ChangeBuffer.cpp \
cpp/opendnp3/src/opendnp3/ClassCounter.cpp \
cpp/opendnp3/src/opendnp3/CommandResponse.cpp \
cpp/opendnp3/src/opendnp3/CommandTask.cpp \
cpp/opendnp3/src/opendnp3/ConstantCommandProcessor.cpp \
cpp/opendnp3/src/opendnp3/ControlRelayOutputBlock.cpp \
cpp/opendnp3/src/opendnp3/CopyableBuffer.cpp \
cpp/opendnp3/src/opendnp3/CRC.cpp \
cpp/opendnp3/src/opendnp3/Database.cpp \
cpp/opendnp3/src/opendnp3/DataPoll.cpp \
cpp/opendnp3/src/opendnp3/DestructorHook.cpp \
cpp/opendnp3/src/opendnp3/DeviceTemplate.cpp \
cpp/opendnp3/src/opendnp3/DNP3Channel.cpp \
cpp/opendnp3/src/opendnp3/DNP3Manager.cpp \
cpp/opendnp3/src/opendnp3/DNPCrc.cpp \
cpp/opendnp3/src/opendnp3/ExecutorPause.cpp \
cpp/opendnp3/src/opendnp3/FunctionCodeHelpers.cpp \
cpp/opendnp3/src/opendnp3/GroupVariation.cpp \
cpp/opendnp3/src/opendnp3/HeaderHandlerBase.cpp \
cpp/opendnp3/src/opendnp3/HeaderReadIterator.cpp \
cpp/opendnp3/src/opendnp3/IAppUser.cpp \
cpp/opendnp3/src/opendnp3/IINField.cpp \
cpp/opendnp3/src/opendnp3/IMeasurementHandler.cpp \
cpp/opendnp3/src/opendnp3/IndexedWriteIterator.cpp \
cpp/opendnp3/src/opendnp3/IStack.cpp \
cpp/opendnp3/src/opendnp3/ITimeWriteHandler.cpp \
cpp/opendnp3/src/opendnp3/LinkFrame.cpp \
cpp/opendnp3/src/opendnp3/LinkHeader.cpp \
cpp/opendnp3/src/opendnp3/LinkLayer.cpp \
cpp/opendnp3/src/opendnp3/LinkLayerReceiver.cpp \
cpp/opendnp3/src/opendnp3/LinkLayerRouter.cpp \
cpp/opendnp3/src/opendnp3/LinkReceiverStates.cpp \
cpp/opendnp3/src/opendnp3/LinkRoute.cpp \
cpp/opendnp3/src/opendnp3/Master.cpp \
cpp/opendnp3/src/opendnp3/MasterScan.cpp \
cpp/opendnp3/src/opendnp3/MasterSchedule.cpp \
cpp/opendnp3/src/opendnp3/MasterStackImpl.cpp \
cpp/opendnp3/src/opendnp3/MasterStates.cpp \
cpp/opendnp3/src/opendnp3/MasterTaskBase.cpp \
cpp/opendnp3/src/opendnp3/MeasurementHandler.cpp \
cpp/opendnp3/src/opendnp3/MeasurementHelpers.cpp \
cpp/opendnp3/src/opendnp3/MeasurementUpdate.cpp \
cpp/opendnp3/src/opendnp3/MultiplexingDataObserver.cpp \
cpp/opendnp3/src/opendnp3/ObjectHeader.cpp \
cpp/opendnp3/src/opendnp3/ObjectInterfaces.cpp \
cpp/opendnp3/src/opendnp3/ObjectReadIterator.cpp \
cpp/opendnp3/src/opendnp3/Objects.cpp \
cpp/opendnp3/src/opendnp3/ObjectWriteIterator.cpp \
cpp/opendnp3/src/opendnp3/OctetData.cpp \
cpp/opendnp3/src/opendnp3/OutstationSBOHandler.cpp \
cpp/opendnp3/src/opendnp3/OutstationStackImpl.cpp \
cpp/opendnp3/src/opendnp3/PhysicalLayerMonitor.cpp \
cpp/opendnp3/src/opendnp3/PhysicalLayerMonitorStates.cpp \
cpp/opendnp3/src/opendnp3/PointClass.cpp \
cpp/opendnp3/src/opendnp3/PostingMeasurementHandler.cpp \
cpp/opendnp3/src/opendnp3/PriLinkLayerStates.cpp \
cpp/opendnp3/src/opendnp3/ProtocolUtil.cpp \
cpp/opendnp3/src/opendnp3/QueuedCommandProcessor.cpp \
cpp/opendnp3/src/opendnp3/ResponseContext.cpp \
cpp/opendnp3/src/opendnp3/SecLinkLayerStates.cpp \
cpp/opendnp3/src/opendnp3/ShiftableBuffer.cpp \
cpp/opendnp3/src/opendnp3/SimpleCommandHandler.cpp \
cpp/opendnp3/src/opendnp3/SimpleDataObserver.cpp \
cpp/opendnp3/src/opendnp3/SlaveConfig.cpp \
cpp/opendnp3/src/opendnp3/Slave.cpp \
cpp/opendnp3/src/opendnp3/SlaveEventBuffer.cpp \
cpp/opendnp3/src/opendnp3/SlaveResponseTypes.cpp \
cpp/opendnp3/src/opendnp3/SlaveStates.cpp \
cpp/opendnp3/src/opendnp3/SolicitedChannel.cpp \
cpp/opendnp3/src/opendnp3/StackBase.cpp \
cpp/opendnp3/src/opendnp3/StartupTasks.cpp \
cpp/opendnp3/src/opendnp3/SubjectBase.cpp \
cpp/opendnp3/src/opendnp3/TimeTransaction.cpp \
cpp/opendnp3/src/opendnp3/TLS_Base.cpp \
cpp/opendnp3/src/opendnp3/TrackingTaskGroup.cpp \
cpp/opendnp3/src/opendnp3/TransportLayer.cpp \
cpp/opendnp3/src/opendnp3/TransportRx.cpp \
cpp/opendnp3/src/opendnp3/TransportStates.cpp \
cpp/opendnp3/src/opendnp3/TransportTx.cpp \
cpp/opendnp3/src/opendnp3/UnsolicitedChannel.cpp \
cpp/opendnp3/src/opendnp3/Util.cpp

#ls cpp/opendnp3tests/src/*.cpp -1 | awk '{print $0" \\"}'
check_PROGRAMS = dnp3test
TESTS = dnp3test
dnp3test_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I$(ASIOPAL_INCLUDE) -I $(OPENDNP3_INCLUDE) -I$(OPENDNP3_SRCS) $(BOOST_DEFINES)
dnp3test_LDADD = libopenpal.la libasiopal.la libopendnp3.la $(TEST_BOOST_LIBS)
dnp3test_LDFLAGS = $(BOOST_LDFLAGS)
dnp3test_SOURCES = \
cpp/opendnp3tests/src/AppLayerTest.cpp \
cpp/opendnp3tests/src/AsyncPhysBaseTest.cpp \
cpp/opendnp3tests/src/AsyncPhysTestObject.cpp \
cpp/opendnp3tests/src/AsyncSerialTestObject.cpp \
cpp/opendnp3tests/src/AsyncTestObjectASIO.cpp \
cpp/opendnp3tests/src/AsyncTestObject.cpp \
cpp/opendnp3tests/src/BufferHelpers.cpp \
cpp/opendnp3tests/src/BufferTestObject.cpp \
cpp/opendnp3tests/src/ComparingDataObserver.cpp \
cpp/opendnp3tests/src/DNPHelpers.cpp \
cpp/opendnp3tests/src/FlexibleDataObserver.cpp \
cpp/opendnp3tests/src/IntegrationTest.cpp \
cpp/opendnp3tests/src/LinkLayerRouterTest.cpp \
cpp/opendnp3tests/src/LinkLayerTest.cpp \
cpp/opendnp3tests/src/LogTester.cpp \
cpp/opendnp3tests/src/LoopbackPhysicalLayerAsync.cpp \
cpp/opendnp3tests/src/LowerLayerToPhysAdapter.cpp \
cpp/opendnp3tests/src/MasterTestObject.cpp \
cpp/opendnp3tests/src/MockAppLayer.cpp \
cpp/opendnp3tests/src/MockAppUser.cpp \
cpp/opendnp3tests/src/MockExecutor.cpp \
cpp/opendnp3tests/src/MockFrameSink.cpp \
cpp/opendnp3tests/src/MockLowerLayer.cpp \
cpp/opendnp3tests/src/MockPhysicalLayerAsync.cpp \
cpp/opendnp3tests/src/MockPhysicalLayerMonitor.cpp \
cpp/opendnp3tests/src/MockUpperLayer.cpp \
cpp/opendnp3tests/src/PhysicalLayerWrapper.cpp \
cpp/opendnp3tests/src/PhysLoopback.cpp \
cpp/opendnp3tests/src/RandomizedBuffer.cpp \
cpp/opendnp3tests/src/SlaveTestObject.cpp \
cpp/opendnp3tests/src/StopWatch.cpp \
cpp/opendnp3tests/src/TestAPDU.cpp \
cpp/opendnp3tests/src/TestAPDUParsing.cpp \
cpp/opendnp3tests/src/TestAPDUWriting.cpp \
cpp/opendnp3tests/src/TestAppLayer.cpp \
cpp/opendnp3tests/src/TestASIO.cpp \
cpp/opendnp3tests/src/TestASIOThreadPool.cpp \
cpp/opendnp3tests/src/TestAsyncTask.cpp \
cpp/opendnp3tests/src/TestCastLongLongDouble.cpp \
cpp/opendnp3tests/src/TestChangeBuffer.cpp \
cpp/opendnp3tests/src/TestCommandHelpers.cpp \
cpp/opendnp3tests/src/TestCommandTask.cpp \
cpp/opendnp3tests/src/TestCRC.cpp \
cpp/opendnp3tests/src/TestDatabase.cpp \
cpp/opendnp3tests/src/TestDNP3Manager.cpp \
cpp/opendnp3tests/src/TestEventBufferBase.cpp \
cpp/opendnp3tests/src/TestEventBuffers.cpp \
cpp/opendnp3tests/src/TestIntegration.cpp \
cpp/opendnp3tests/src/TestLazyCollection.cpp \
cpp/opendnp3tests/src/TestLinkFrameDNP.cpp \
cpp/opendnp3tests/src/TestLinkLayer.cpp \
cpp/opendnp3tests/src/TestLinkLayerRouter.cpp \
cpp/opendnp3tests/src/TestLinkReceiver.cpp \
cpp/opendnp3tests/src/TestLinkRoute.cpp \
cpp/opendnp3tests/src/TestLog.cpp \
cpp/opendnp3tests/src/TestMaster.cpp \
cpp/opendnp3tests/src/TestMeasurementUpdate.cpp \
cpp/opendnp3tests/src/TestMisc.cpp \
cpp/opendnp3tests/src/TestObjects.cpp \
cpp/opendnp3tests/src/TestPhysicalLayerAsyncBase.cpp \
cpp/opendnp3tests/src/TestPhysicalLayerAsyncSerial.cpp \
cpp/opendnp3tests/src/TestPhysicalLayerAsyncTCP.cpp \
cpp/opendnp3tests/src/TestPhysicalLayerLoopback.cpp \
cpp/opendnp3tests/src/TestPhysicalLayerMonitor.cpp \
cpp/opendnp3tests/src/TestSerialization.cpp \
cpp/opendnp3tests/src/TestShiftableBuffer.cpp \
cpp/opendnp3tests/src/TestSlave.cpp \
cpp/opendnp3tests/src/TestSlaveEventBuffer.cpp \
cpp/opendnp3tests/src/TestStartBoostUTF.cpp \
cpp/opendnp3tests/src/TestTime.cpp \
cpp/opendnp3tests/src/TestTimers.cpp \
cpp/opendnp3tests/src/TestTransportLayer.cpp \
cpp/opendnp3tests/src/TestTransportLoopback.cpp \
cpp/opendnp3tests/src/TestTransportScalability.cpp \
cpp/opendnp3tests/src/TestTypes.cpp \
cpp/opendnp3tests/src/TestUtil.cpp \
cpp/opendnp3tests/src/Timeout.cpp \
cpp/opendnp3tests/src/TransportIntegrationStack.cpp \
cpp/opendnp3tests/src/TransportLoopbackTestObject.cpp \
cpp/opendnp3tests/src/TransportScalabilityTestObject.cpp \
cpp/opendnp3tests/src/TransportStackPair.cpp \
cpp/opendnp3tests/src/TransportTestObject.cpp \
cpp/opendnp3tests/src/WrappedTcpPipe.cpp


masterdemo_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I$(ASIOPAL_INCLUDE) -I $(OPENDNP3_INCLUDE) $(BOOST_DEFINES)
masterdemo_LDFLAGS = $(BOOST_LDFLAGS) -pthread
masterdemo_LDADD = libopenpal.la libasiopal.la libopendnp3.la $(CORE_BOOST_LIBS)
masterdemo_SOURCES = cpp/masterdemo/DemoMain.cpp

outstationdemo_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I$(ASIOPAL_INCLUDE) -I $(OPENDNP3_INCLUDE) $(BOOST_DEFINES)
outstationdemo_LDFLAGS = $(BOOST_LDFLAGS) -pthread
outstationdemo_LDADD = libopenpal.la libasiopal.la libopendnp3.la $(CORE_BOOST_LIBS)
outstationdemo_SOURCES = cpp/outstationdemo/DemoMain.cpp

apdufuzz_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I$(OPENDNP3_INCLUDE) -I$(OPENDNP3_SRCS) 
apdufuzz_LDFLAGS = -pthread
apdufuzz_LDADD = libopenpal.la libopendnp3.la
apdufuzz_SOURCES = cpp/apdufuzz/ApduFuzz.cpp

if WANT_JAVA

#ls cpp/asiodnp3/src/asiodnp3/*.cpp -1 | awk '{print $0" \\"}'
lib_LTLIBRARIES += libasiodnp3.la
libasiodnp3_la_CPPFLAGS = -I$(OPENPAL_INCLUDE) -I$(ASIOPAL_INCLUDE) -I $(OPENDNP3_INCLUDE) -I$(ASIODNP3_INCLUDE) $(BOOST_DEFINES)
libasiodnp3_la_LDFLAGS = -version-info 1:1:0
libasiodnp3_la_SOURCES = \
cpp/asiodnp3/src/asiodnp3/ASIODNP3Manager.cpp

#ls java/cpp/*.cpp -1 | awk '{print $0" \\"}'	
lib_LTLIBRARIES += libopendnp3java.la
libopendnp3java_la_CPPFLAGS = -I$(OPENPAL_INCLUDE) -I$(ASIOPAL_INCLUDE) -I $(OPENDNP3_INCLUDE) -I$(ASIODNP3_INCLUDE) $(BOOST_DEFINES)
libopendnp3java_la_LDFLAGS = -version-info 1:1:0
libopendnp3java_la_LIBADD = libopenpal.la libasiopal.la libopendnp3.la libasiodnp3.la $(CORE_BOOST_LIBS)
libopendnp3java_la_SOURCES = \
java/cpp/com_automatak_dnp3_impl_ChannelImpl.cpp \
java/cpp/com_automatak_dnp3_impl_CommandProcessorImpl.cpp \
java/cpp/com_automatak_dnp3_impl_DataObserverImpl.cpp \
java/cpp/com_automatak_dnp3_impl_ManagerImpl.cpp \
java/cpp/com_automatak_dnp3_impl_MasterImpl.cpp \
java/cpp/com_automatak_dnp3_impl_OutstationImpl.cpp \
java/cpp/com_automatak_dnp3_impl_StackBase.cpp \
java/cpp/CommandHandlerAdapter.cpp \
java/cpp/ConfigReader.cpp \
java/cpp/DataObserverAdapter.cpp \
java/cpp/JNIHelpers.cpp \
java/cpp/LogSubscriberAdapter.cpp


endif

