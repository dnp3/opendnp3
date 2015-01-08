#copyright (c) 2013 Automatak, LLC

ACLOCAL_AMFLAGS = -I m4

ASIO_CONFIG = -I $(ASIO_HOME) -DASIO_STANDALONE

OPENPAL_INCLUDE = $(top_srcdir)/cpp/libs/openpal/src
OPENDNP3_INCLUDE = $(top_srcdir)/cpp/libs/opendnp3/src
ASIOPAL_INCLUDE = $(top_srcdir)/cpp/libs/asiopal/src
ASIODNP3_INCLUDE = $(top_srcdir)/cpp/libs/asiodnp3/src
CATCH_INCLUDE = $(top_srcdir)/cpp/tests/catch

bin_PROGRAMS = masterdemo outstationdemo

lib_LTLIBRARIES = libopenpal.la libasiopal.la libopendnp3.la libasiodnp3.la

# ls cpp/libs/openpal/src/openpal/**/*.cpp -1 | awk '{print $0" \\"}'
libopenpal_la_LDFLAGS = -version-info 1:1:0
libopenpal_la_CXXFLAGS = -I$(OPENPAL_INCLUDE)
libopenpal_la_SOURCES = \
cpp/libs/openpal/src/openpal/container/DynamicBuffer.cpp \
cpp/libs/openpal/src/openpal/container/ReadOnlyBuffer.cpp \
cpp/libs/openpal/src/openpal/container/WriteBuffer.cpp \
cpp/libs/openpal/src/openpal/executor/Erasure.cpp \
cpp/libs/openpal/src/openpal/executor/MonotonicTimestamp.cpp \
cpp/libs/openpal/src/openpal/executor/Runnable.cpp \
cpp/libs/openpal/src/openpal/executor/TimeDuration.cpp \
cpp/libs/openpal/src/openpal/logging/LogEntry.cpp \
cpp/libs/openpal/src/openpal/logging/Logger.cpp \
cpp/libs/openpal/src/openpal/logging/LogRoot.cpp \
cpp/libs/openpal/src/openpal/logging/StringFormatting.cpp \
cpp/libs/openpal/src/openpal/serialization/ByteSerialization.cpp \
cpp/libs/openpal/src/openpal/serialization/UInt48LE.cpp \
cpp/libs/openpal/src/openpal/util/Limits.cpp \
cpp/libs/openpal/src/openpal/util/ToHex.cpp

#ls cpp/libs/asiopal/src/asiopal/*.cpp -1 | awk '{print $0" \\"}'
libasiopal_la_LDFLAGS = -version-info 1:1:0
libasiopal_la_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I$(ASIOPAL_INCLUDE) $(ASIO_CONFIG)
libasiopal_la_SOURCES = \
cpp/libs/asiopal/src/asiopal/ASIOExecutor.cpp \
cpp/libs/asiopal/src/asiopal/ASIOSerialHelpers.cpp \
cpp/libs/asiopal/src/asiopal/IOServiceThreadPool.cpp \
cpp/libs/asiopal/src/asiopal/LogFanoutHandler.cpp \
cpp/libs/asiopal/src/asiopal/PhysicalLayerBase.cpp \
cpp/libs/asiopal/src/asiopal/PhysicalLayerBaseTCP.cpp \
cpp/libs/asiopal/src/asiopal/PhysicalLayerSerial.cpp \
cpp/libs/asiopal/src/asiopal/PhysicalLayerTCPClient.cpp \
cpp/libs/asiopal/src/asiopal/PhysicalLayerTCPServer.cpp \
cpp/libs/asiopal/src/asiopal/SerialTypes.cpp \
cpp/libs/asiopal/src/asiopal/TimerASIO.cpp \
cpp/libs/asiopal/src/asiopal/UTCTimeSource.cpp

# ls cpp/libs/opendnp3/src/opendnp3/**/*.cpp -1 | awk '{print $0" \\"}'
libopendnp3_la_LDFLAGS = -version-info 2:0:0
libopendnp3_la_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I $(OPENDNP3_INCLUDE)
libopendnp3_la_SOURCES = \
cpp/libs/opendnp3/src/opendnp3/app/AnalogOutput.cpp \
cpp/libs/opendnp3/src/opendnp3/app/APDUBuilders.cpp \
cpp/libs/opendnp3/src/opendnp3/app/APDUHandlerBase.cpp \
cpp/libs/opendnp3/src/opendnp3/app/APDUHeader.cpp \
cpp/libs/opendnp3/src/opendnp3/app/APDUHeaderParser.cpp \
cpp/libs/opendnp3/src/opendnp3/app/APDULogging.cpp \
cpp/libs/opendnp3/src/opendnp3/app/APDUParser.cpp \
cpp/libs/opendnp3/src/opendnp3/app/APDURequest.cpp \
cpp/libs/opendnp3/src/opendnp3/app/APDUResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/app/APDUWrapper.cpp \
cpp/libs/opendnp3/src/opendnp3/app/AppControlField.cpp \
cpp/libs/opendnp3/src/opendnp3/app/BitReader.cpp \
cpp/libs/opendnp3/src/opendnp3/app/ClassField.cpp \
cpp/libs/opendnp3/src/opendnp3/app/ControlRelayOutputBlock.cpp \
cpp/libs/opendnp3/src/opendnp3/app/EventTriggers.cpp \
cpp/libs/opendnp3/src/opendnp3/app/FunctionHelpers.cpp \
cpp/libs/opendnp3/src/opendnp3/app/GroupVariationRecord.cpp \
cpp/libs/opendnp3/src/opendnp3/app/IINField.cpp \
cpp/libs/opendnp3/src/opendnp3/app/MeasurementTypes.cpp \
cpp/libs/opendnp3/src/opendnp3/app/ObjectWriter.cpp \
cpp/libs/opendnp3/src/opendnp3/app/OctetData.cpp \
cpp/libs/opendnp3/src/opendnp3/app/StaticRange.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/ChannelState.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/CommandResult.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/CommandStatus.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/ControlCode.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/DoubleBit.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/EventAnalogOutputStatusResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/EventAnalogResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/EventBinaryOutputStatusResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/EventBinaryResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/EventCounterResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/EventDoubleBinaryResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/EventFrozenCounterResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/FunctionCode.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/GroupVariation.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/LinkFunction.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/PointClass.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/PollState.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/QualifierCode.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/StaticAnalogOutputStatusResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/StaticAnalogResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/StaticBinaryOutputStatusResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/StaticBinaryResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/StaticCounterResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/StaticDoubleBinaryResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/StaticFrozenCounterResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/gen/TimeSyncMode.cpp \
cpp/libs/opendnp3/src/opendnp3/link/CRC.cpp \
cpp/libs/opendnp3/src/opendnp3/link/IOpenDelayStrategy.cpp \
cpp/libs/opendnp3/src/opendnp3/link/LinkFrame.cpp \
cpp/libs/opendnp3/src/opendnp3/link/LinkHeader.cpp \
cpp/libs/opendnp3/src/opendnp3/link/LinkLayer.cpp \
cpp/libs/opendnp3/src/opendnp3/link/LinkLayerParser.cpp \
cpp/libs/opendnp3/src/opendnp3/link/PriLinkLayerStates.cpp \
cpp/libs/opendnp3/src/opendnp3/link/SecLinkLayerStates.cpp \
cpp/libs/opendnp3/src/opendnp3/link/ShiftableBuffer.cpp \
cpp/libs/opendnp3/src/opendnp3/master/ClearRestartTask.cpp \
cpp/libs/opendnp3/src/opendnp3/master/CommandMarshaller.cpp \
cpp/libs/opendnp3/src/opendnp3/master/CommandResponse.cpp \
cpp/libs/opendnp3/src/opendnp3/master/CommandTask.cpp \
cpp/libs/opendnp3/src/opendnp3/master/ConstantCommandProcessor.cpp \
cpp/libs/opendnp3/src/opendnp3/master/DisableUnsolicitedTask.cpp \
cpp/libs/opendnp3/src/opendnp3/master/EnableUnsolicitedTask.cpp \
cpp/libs/opendnp3/src/opendnp3/master/IMasterState.cpp \
cpp/libs/opendnp3/src/opendnp3/master/ISOEHandler.cpp \
cpp/libs/opendnp3/src/opendnp3/master/ITaskLock.cpp \
cpp/libs/opendnp3/src/opendnp3/master/MasterContext.cpp \
cpp/libs/opendnp3/src/opendnp3/master/Master.cpp \
cpp/libs/opendnp3/src/opendnp3/master/MasterParams.cpp \
cpp/libs/opendnp3/src/opendnp3/master/MasterScan.cpp \
cpp/libs/opendnp3/src/opendnp3/master/MasterScheduler.cpp \
cpp/libs/opendnp3/src/opendnp3/master/MasterTasks.cpp \
cpp/libs/opendnp3/src/opendnp3/master/MeasurementHandler.cpp \
cpp/libs/opendnp3/src/opendnp3/master/NullResponseTask.cpp \
cpp/libs/opendnp3/src/opendnp3/master/PollTaskBase.cpp \
cpp/libs/opendnp3/src/opendnp3/master/PollTask.cpp \
cpp/libs/opendnp3/src/opendnp3/master/SerialTimeSyncTask.cpp \
cpp/libs/opendnp3/src/opendnp3/master/SingleResponseTask.cpp \
cpp/libs/opendnp3/src/opendnp3/master/StartupIntegrityPoll.cpp \
cpp/libs/opendnp3/src/opendnp3/master/TaskRecord.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group10.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group11.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group12.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group1.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group20.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group21.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group22.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group23.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group2.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group30.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group32.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group3.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group40.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group41.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group42.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group4.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group50.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group51.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group52.cpp \
cpp/libs/opendnp3/src/opendnp3/objects/Group60.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/ApplicationIIN.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/ClassBasedRequestHandler.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/CommandActionAdapter.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/CommandResponseHandler.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/Database.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/DatabaseBuffers.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/EventBufferConfig.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/EventCount.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/EventResponseConfig.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/EventResponseContext.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/EventResponseTypes.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/EventWriteOperation.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/IINHelpers.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/IOutstationApplication.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/OutstationContext.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/Outstation.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/OutstationEventBuffer.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/OutstationParams.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/OutstationSolicitedStates.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/OutstationUnsolicitedStates.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/ReadHandler.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/ResponseContext.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/SelectionCriteria.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/SelectionWriter.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/SimpleCommandHandler.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/SOERecord.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/StaticLoadFunctions.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/StaticResponseConfig.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/StaticResponseContext.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/TimeTransaction.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/WriteHandler.cpp \
cpp/libs/opendnp3/src/opendnp3/transport/TransportLayer.cpp \
cpp/libs/opendnp3/src/opendnp3/transport/TransportRx.cpp \
cpp/libs/opendnp3/src/opendnp3/transport/TransportStack.cpp \
cpp/libs/opendnp3/src/opendnp3/transport/TransportTx.cpp


#ls cpp/libs/asiodnp3/src/asiodnp3/*.cpp -1 | awk '{print $0" \\"}'
libasiodnp3_la_CPPFLAGS = -I$(OPENPAL_INCLUDE) -I$(ASIOPAL_INCLUDE) -I $(OPENDNP3_INCLUDE) -I$(ASIODNP3_INCLUDE) $(ASIO_CONFIG)
libasiodnp3_la_LDFLAGS = -version-info 1:1:0
libasiodnp3_la_SOURCES = \
cpp/libs/asiodnp3/src/asiodnp3/impl/LinkLayerRouter.cpp \
cpp/libs/asiodnp3/src/asiodnp3/impl/PhysicalLayerMonitor.cpp \
cpp/libs/asiodnp3/src/asiodnp3/impl/PhysicalLayerMonitorStates.cpp \
cpp/libs/asiodnp3/src/asiodnp3/BlockingCommandCallback.cpp \
cpp/libs/asiodnp3/src/asiodnp3/ChannelSet.cpp \
cpp/libs/asiodnp3/src/asiodnp3/ConsoleLogger.cpp \
cpp/libs/asiodnp3/src/asiodnp3/DefaultMasterApplication.cpp \
cpp/libs/asiodnp3/src/asiodnp3/DestructorHook.cpp \
cpp/libs/asiodnp3/src/asiodnp3/DNP3Channel.cpp \
cpp/libs/asiodnp3/src/asiodnp3/DNP3Manager.cpp \
cpp/libs/asiodnp3/src/asiodnp3/MasterStackImpl.cpp \
cpp/libs/asiodnp3/src/asiodnp3/MultidropTaskLock.cpp \
cpp/libs/asiodnp3/src/asiodnp3/OutstationStackImpl.cpp \
cpp/libs/asiodnp3/src/asiodnp3/PrintingSOEHandler.cpp \
cpp/libs/asiodnp3/src/asiodnp3/StackActionHandler.cpp


masterdemo_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I$(ASIOPAL_INCLUDE) -I $(OPENDNP3_INCLUDE) -I$(ASIODNP3_INCLUDE) $(ASIO_CONFIG) -Wl,--no-as-needed
masterdemo_LDFLAGS = -pthread
masterdemo_LDADD = libopenpal.la libasiopal.la libopendnp3.la libasiodnp3.la
masterdemo_SOURCES = cpp/examples/master/DemoMain.cpp

outstationdemo_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I$(ASIOPAL_INCLUDE) -I $(OPENDNP3_INCLUDE) -I$(ASIODNP3_INCLUDE) $(ASIO_CONFIG) -Wl,--no-as-needed
outstationdemo_LDFLAGS = -pthread
outstationdemo_LDADD = libopenpal.la libasiopal.la libopendnp3.la libasiodnp3.la
outstationdemo_SOURCES = cpp/examples/outstation/DemoMain.cpp

check_PROGRAMS = openpaltest dnp3test
#TESTS = openpaltest dnp3test

#ls cpp/tests/openpaltests/src/*.cpp -1 | awk '{print $0" \\"}'
openpaltest_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I$(CATCH_INCLUDE)
openpaltest_LDFLAGS = -pthread
openpaltest_LDADD = libopenpal.la
openpaltest_SOURCES = \
cpp/tests/openpaltests/src/CatchTestStart.cpp \
cpp/tests/openpaltests/src/LinkedListAdapterTestSuite.cpp \
cpp/tests/openpaltests/src/StackAdapterTestSuite.cpp \
cpp/tests/openpaltests/src/StaticQueueTestSuite.cpp

#ls cpp/tests/opendnp3tests/src/*.cpp -1 | awk '{print $0" \\"}'
dnp3test_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I$(ASIOPAL_INCLUDE) -I $(OPENDNP3_INCLUDE) -I$(ASIODNP3_INCLUDE) -I$(CATCH_INCLUDE) $(ASIO_CONFIG)
dnp3test_LDFLAGS = -pthread
dnp3test_LDADD = libopenpal.la libasiopal.la libopendnp3.la libasiodnp3.la
dnp3test_SOURCES = \
cpp/tests/opendnp3tests/src/APDUHelpers.cpp \
cpp/tests/opendnp3tests/src/APDUHexBuilders.cpp \
cpp/tests/opendnp3tests/src/BufferHelpers.cpp \
cpp/tests/opendnp3tests/src/BufferTestObject.cpp \
cpp/tests/opendnp3tests/src/CatchTestStart.cpp \
cpp/tests/opendnp3tests/src/CopyableBuffer.cpp \
cpp/tests/opendnp3tests/src/DNPHelpers.cpp \
cpp/tests/opendnp3tests/src/HexConversions.cpp \
cpp/tests/opendnp3tests/src/LinkLayerRouterTest.cpp \
cpp/tests/opendnp3tests/src/LinkLayerTest.cpp \
cpp/tests/opendnp3tests/src/LogTester.cpp \
cpp/tests/opendnp3tests/src/LoopbackPhysicalLayer.cpp \
cpp/tests/opendnp3tests/src/LowerLayerToPhysAdapter.cpp \
cpp/tests/opendnp3tests/src/MasterTestObject.cpp \
cpp/tests/opendnp3tests/src/MockExecutor.cpp \
cpp/tests/opendnp3tests/src/MockFrameSink.cpp \
cpp/tests/opendnp3tests/src/MockLowerLayer.cpp \
cpp/tests/opendnp3tests/src/MockPhysicalLayer.cpp \
cpp/tests/opendnp3tests/src/MockPhysicalLayerMonitor.cpp \
cpp/tests/opendnp3tests/src/MockTransportLayer.cpp \
cpp/tests/opendnp3tests/src/MockUpperLayer.cpp \
cpp/tests/opendnp3tests/src/MultiplexingDataObserver.cpp \
cpp/tests/opendnp3tests/src/OutstationTestObject.cpp \
cpp/tests/opendnp3tests/src/PhysBaseTest.cpp \
cpp/tests/opendnp3tests/src/PhysLoopback.cpp \
cpp/tests/opendnp3tests/src/PhysTestObject.cpp \
cpp/tests/opendnp3tests/src/ProtocolUtil.cpp \
cpp/tests/opendnp3tests/src/RandomizedBuffer.cpp \
cpp/tests/opendnp3tests/src/SerialTestObject.cpp \
cpp/tests/opendnp3tests/src/StopWatch.cpp \
cpp/tests/opendnp3tests/src/TestAPDUParsing.cpp \
cpp/tests/opendnp3tests/src/TestAPDUWriting.cpp \
cpp/tests/opendnp3tests/src/TestASIO.cpp \
cpp/tests/opendnp3tests/src/TestASIOThreadPool.cpp \
cpp/tests/opendnp3tests/src/TestCastLongLongDouble.cpp \
cpp/tests/opendnp3tests/src/TestCRC.cpp \
cpp/tests/opendnp3tests/src/TestDatabase.cpp \
cpp/tests/opendnp3tests/src/TestDNP3Manager.cpp \
cpp/tests/opendnp3tests/src/TestLazyCollection.cpp \
cpp/tests/opendnp3tests/src/TestLinkFrame.cpp \
cpp/tests/opendnp3tests/src/TestLinkLayer.cpp \
cpp/tests/opendnp3tests/src/TestLinkLayerRouter.cpp \
cpp/tests/opendnp3tests/src/TestLinkReceiver.cpp \
cpp/tests/opendnp3tests/src/TestLinkRoute.cpp \
cpp/tests/opendnp3tests/src/TestLog.cpp \
cpp/tests/opendnp3tests/src/TestMasterCommandRequests.cpp \
cpp/tests/opendnp3tests/src/TestMaster.cpp \
cpp/tests/opendnp3tests/src/TestMasterMultidrop.cpp \
cpp/tests/opendnp3tests/src/TestMasterUnsolBehaviors.cpp \
cpp/tests/opendnp3tests/src/TestObjectASIO.cpp \
cpp/tests/opendnp3tests/src/TestObject.cpp \
cpp/tests/opendnp3tests/src/TestOutstationCommandResponses.cpp \
cpp/tests/opendnp3tests/src/TestOutstation.cpp \
cpp/tests/opendnp3tests/src/TestOutstationEventResponses.cpp \
cpp/tests/opendnp3tests/src/TestOutstationUnsolicitedResponses.cpp \
cpp/tests/opendnp3tests/src/TestPhysicalLayerAsyncBase.cpp \
cpp/tests/opendnp3tests/src/TestPhysicalLayerAsyncSerial.cpp \
cpp/tests/opendnp3tests/src/TestPhysicalLayerAsyncTCP.cpp \
cpp/tests/opendnp3tests/src/TestPhysicalLayerLoopback.cpp \
cpp/tests/opendnp3tests/src/TestPhysicalLayerMonitor.cpp \
cpp/tests/opendnp3tests/src/TestSerialization.cpp \
cpp/tests/opendnp3tests/src/TestShiftableBuffer.cpp \
cpp/tests/opendnp3tests/src/TestTime.cpp \
cpp/tests/opendnp3tests/src/TestTimers.cpp \
cpp/tests/opendnp3tests/src/TestTransportLayer.cpp \
cpp/tests/opendnp3tests/src/TestTransportLoopback.cpp \
cpp/tests/opendnp3tests/src/TestTransportScalability.cpp \
cpp/tests/opendnp3tests/src/TestTypes.cpp \
cpp/tests/opendnp3tests/src/TestUtil.cpp \
cpp/tests/opendnp3tests/src/TestWriteConversions.cpp \
cpp/tests/opendnp3tests/src/Timeout.cpp \
cpp/tests/opendnp3tests/src/TransportIntegrationStack.cpp \
cpp/tests/opendnp3tests/src/TransportLoopbackTestObject.cpp \
cpp/tests/opendnp3tests/src/TransportScalabilityTestObject.cpp \
cpp/tests/opendnp3tests/src/TransportStackPair.cpp \
cpp/tests/opendnp3tests/src/TransportTestObject.cpp

include_HEADERS = \
cpp/libs/asiodnp3/src/asiodnp3/BlockingCommandCallback.h \
cpp/libs/asiodnp3/src/asiodnp3/ChannelSet.h \
cpp/libs/asiodnp3/src/asiodnp3/ConsoleLogger.h \
cpp/libs/asiodnp3/src/asiodnp3/DNP3Channel.h \
cpp/libs/asiodnp3/src/asiodnp3/DNP3Manager.h \
cpp/libs/asiodnp3/src/asiodnp3/DefaultMasterApplication.h \
cpp/libs/asiodnp3/src/asiodnp3/DestructorHook.h \
cpp/libs/asiodnp3/src/asiodnp3/IChannel.h \
cpp/libs/asiodnp3/src/asiodnp3/IMaster.h \
cpp/libs/asiodnp3/src/asiodnp3/IOutstation.h \
cpp/libs/asiodnp3/src/asiodnp3/IStack.h \
cpp/libs/asiodnp3/src/asiodnp3/MasterStackImpl.h \
cpp/libs/asiodnp3/src/asiodnp3/MultidropTaskLock.h \
cpp/libs/asiodnp3/src/asiodnp3/Mutex.h \
cpp/libs/asiodnp3/src/asiodnp3/OutstationStackImpl.h \
cpp/libs/asiodnp3/src/asiodnp3/PrintingSOEHandler.h \
cpp/libs/asiodnp3/src/asiodnp3/StackActionHandler.h \
cpp/libs/asiodnp3/src/asiodnp3/impl/LinkLayerRouter.h \
cpp/libs/asiodnp3/src/asiodnp3/impl/PhysicalLayerMonitor.h \
cpp/libs/asiodnp3/src/asiodnp3/impl/PhysicalLayerMonitorStates.h \
cpp/libs/asiopal/src/asiopal/ASIOExecutor.h \
cpp/libs/asiopal/src/asiopal/ASIOSerialHelpers.h \
cpp/libs/asiopal/src/asiopal/IOServiceThreadPool.h \
cpp/libs/asiopal/src/asiopal/LogFanoutHandler.h \
cpp/libs/asiopal/src/asiopal/PhysicalLayerASIO.h \
cpp/libs/asiopal/src/asiopal/PhysicalLayerBase.h \
cpp/libs/asiopal/src/asiopal/PhysicalLayerBaseTCP.h \
cpp/libs/asiopal/src/asiopal/PhysicalLayerSerial.h \
cpp/libs/asiopal/src/asiopal/PhysicalLayerTCPClient.h \
cpp/libs/asiopal/src/asiopal/PhysicalLayerTCPServer.h \
cpp/libs/asiopal/src/asiopal/SerialTypes.h \
cpp/libs/asiopal/src/asiopal/StrandGetters.h \
cpp/libs/asiopal/src/asiopal/Synchronized.h \
cpp/libs/asiopal/src/asiopal/TimerASIO.h \
cpp/libs/asiopal/src/asiopal/UTCTimeSource.h \
cpp/libs/opendnp3/src/opendnp3/ErrorCodes.h \
cpp/libs/opendnp3/src/opendnp3/LayerInterfaces.h \
cpp/libs/opendnp3/src/opendnp3/LogLevels.h \
cpp/libs/opendnp3/src/opendnp3/Route.h \
cpp/libs/opendnp3/src/opendnp3/StackStatistics.h \
cpp/libs/opendnp3/src/opendnp3/app/APDUBuilders.h \
cpp/libs/opendnp3/src/opendnp3/app/APDUHandlerBase.h \
cpp/libs/opendnp3/src/opendnp3/app/APDUHeader.h \
cpp/libs/opendnp3/src/opendnp3/app/APDUHeaderParser.h \
cpp/libs/opendnp3/src/opendnp3/app/APDULogging.h \
cpp/libs/opendnp3/src/opendnp3/app/APDUParser.h \
cpp/libs/opendnp3/src/opendnp3/app/APDURequest.h \
cpp/libs/opendnp3/src/opendnp3/app/APDUResponse.h \
cpp/libs/opendnp3/src/opendnp3/app/APDUWrapper.h \
cpp/libs/opendnp3/src/opendnp3/app/AnalogOutput.h \
cpp/libs/opendnp3/src/opendnp3/app/AppConstants.h \
cpp/libs/opendnp3/src/opendnp3/app/AppControlField.h \
cpp/libs/opendnp3/src/opendnp3/app/BaseMeasurementTypes.h \
cpp/libs/opendnp3/src/opendnp3/app/BitReader.h \
cpp/libs/opendnp3/src/opendnp3/app/BitfieldRangeWriteIterator.h \
cpp/libs/opendnp3/src/opendnp3/app/ClassField.h \
cpp/libs/opendnp3/src/opendnp3/app/ControlRelayOutputBlock.h \
cpp/libs/opendnp3/src/opendnp3/app/CountWriteIterator.h \
cpp/libs/opendnp3/src/opendnp3/app/DNP3Serializer.h \
cpp/libs/opendnp3/src/opendnp3/app/DownSampling.h \
cpp/libs/opendnp3/src/opendnp3/app/EventTriggers.h \
cpp/libs/opendnp3/src/opendnp3/app/EventType.h \
cpp/libs/opendnp3/src/opendnp3/app/FunctionHelpers.h \
cpp/libs/opendnp3/src/opendnp3/app/GroupVariationID.h \
cpp/libs/opendnp3/src/opendnp3/app/GroupVariationRecord.h \
cpp/libs/opendnp3/src/opendnp3/app/HeaderWriter.h \
cpp/libs/opendnp3/src/opendnp3/app/IAPDUHandler.h \
cpp/libs/opendnp3/src/opendnp3/app/IINField.h \
cpp/libs/opendnp3/src/opendnp3/app/ITransactable.h \
cpp/libs/opendnp3/src/opendnp3/app/IndexParser.h \
cpp/libs/opendnp3/src/opendnp3/app/IndexedValue.h \
cpp/libs/opendnp3/src/opendnp3/app/IterableBuffer.h \
cpp/libs/opendnp3/src/opendnp3/app/IterableTransforms.h \
cpp/libs/opendnp3/src/opendnp3/app/LazyIterable.h \
cpp/libs/opendnp3/src/opendnp3/app/MeasurementFactory.h \
cpp/libs/opendnp3/src/opendnp3/app/MeasurementTypes.h \
cpp/libs/opendnp3/src/opendnp3/app/OctetData.h \
cpp/libs/opendnp3/src/opendnp3/app/OctetString.h \
cpp/libs/opendnp3/src/opendnp3/app/PrefixedWriteIterator.h \
cpp/libs/opendnp3/src/opendnp3/app/QualityMasks.h \
cpp/libs/opendnp3/src/opendnp3/app/Range.h \
cpp/libs/opendnp3/src/opendnp3/app/RangeWriteIterator.h \
cpp/libs/opendnp3/src/opendnp3/app/SequenceInfo.h \
cpp/libs/opendnp3/src/opendnp3/app/StaticRange.h \
cpp/libs/opendnp3/src/opendnp3/app/VTOData.h \
cpp/libs/opendnp3/src/opendnp3/app/WriteConversionTemplates.h \
cpp/libs/opendnp3/src/opendnp3/app/WriteConversions.h \
cpp/libs/opendnp3/src/opendnp3/gen/AnalogQuality.h \
cpp/libs/opendnp3/src/opendnp3/gen/BinaryOutputStatusQuality.h \
cpp/libs/opendnp3/src/opendnp3/gen/BinaryQuality.h \
cpp/libs/opendnp3/src/opendnp3/gen/ChannelState.h \
cpp/libs/opendnp3/src/opendnp3/gen/CommandResult.h \
cpp/libs/opendnp3/src/opendnp3/gen/CommandStatus.h \
cpp/libs/opendnp3/src/opendnp3/gen/ControlCode.h \
cpp/libs/opendnp3/src/opendnp3/gen/CounterQuality.h \
cpp/libs/opendnp3/src/opendnp3/gen/DoubleBit.h \
cpp/libs/opendnp3/src/opendnp3/gen/DoubleBitBinaryQuality.h \
cpp/libs/opendnp3/src/opendnp3/gen/EventAnalogOutputStatusResponse.h \
cpp/libs/opendnp3/src/opendnp3/gen/EventAnalogResponse.h \
cpp/libs/opendnp3/src/opendnp3/gen/EventBinaryOutputStatusResponse.h \
cpp/libs/opendnp3/src/opendnp3/gen/EventBinaryResponse.h \
cpp/libs/opendnp3/src/opendnp3/gen/EventCounterResponse.h \
cpp/libs/opendnp3/src/opendnp3/gen/EventDoubleBinaryResponse.h \
cpp/libs/opendnp3/src/opendnp3/gen/EventFrozenCounterResponse.h \
cpp/libs/opendnp3/src/opendnp3/gen/FunctionCode.h \
cpp/libs/opendnp3/src/opendnp3/gen/GroupVariation.h \
cpp/libs/opendnp3/src/opendnp3/gen/LinkFunction.h \
cpp/libs/opendnp3/src/opendnp3/gen/PointClass.h \
cpp/libs/opendnp3/src/opendnp3/gen/PollState.h \
cpp/libs/opendnp3/src/opendnp3/gen/QualifierCode.h \
cpp/libs/opendnp3/src/opendnp3/gen/RestartMode.h \
cpp/libs/opendnp3/src/opendnp3/gen/StaticAnalogOutputStatusResponse.h \
cpp/libs/opendnp3/src/opendnp3/gen/StaticAnalogResponse.h \
cpp/libs/opendnp3/src/opendnp3/gen/StaticBinaryOutputStatusResponse.h \
cpp/libs/opendnp3/src/opendnp3/gen/StaticBinaryResponse.h \
cpp/libs/opendnp3/src/opendnp3/gen/StaticCounterResponse.h \
cpp/libs/opendnp3/src/opendnp3/gen/StaticDoubleBinaryResponse.h \
cpp/libs/opendnp3/src/opendnp3/gen/StaticFrozenCounterResponse.h \
cpp/libs/opendnp3/src/opendnp3/gen/TimeSyncMode.h \
cpp/libs/opendnp3/src/opendnp3/gen/TimestampMode.h \
cpp/libs/opendnp3/src/opendnp3/link/CRC.h \
cpp/libs/opendnp3/src/opendnp3/link/IChannelStateListener.h \
cpp/libs/opendnp3/src/opendnp3/link/IFrameSink.h \
cpp/libs/opendnp3/src/opendnp3/link/ILinkLayer.h \
cpp/libs/opendnp3/src/opendnp3/link/ILinkRouter.h \
cpp/libs/opendnp3/src/opendnp3/link/ILinkSession.h \
cpp/libs/opendnp3/src/opendnp3/link/IOpenDelayStrategy.h \
cpp/libs/opendnp3/src/opendnp3/link/ITransportSegment.h \
cpp/libs/opendnp3/src/opendnp3/link/LinkChannelStatistics.h \
cpp/libs/opendnp3/src/opendnp3/link/LinkConfig.h \
cpp/libs/opendnp3/src/opendnp3/link/LinkFrame.h \
cpp/libs/opendnp3/src/opendnp3/link/LinkHeader.h \
cpp/libs/opendnp3/src/opendnp3/link/LinkLayer.h \
cpp/libs/opendnp3/src/opendnp3/link/LinkLayerConstants.h \
cpp/libs/opendnp3/src/opendnp3/link/LinkLayerParser.h \
cpp/libs/opendnp3/src/opendnp3/link/PriLinkLayerStates.h \
cpp/libs/opendnp3/src/opendnp3/link/SecLinkLayerStates.h \
cpp/libs/opendnp3/src/opendnp3/link/ShiftableBuffer.h \
cpp/libs/opendnp3/src/opendnp3/link/Singleton.h \
cpp/libs/opendnp3/src/opendnp3/master/ClearRestartTask.h \
cpp/libs/opendnp3/src/opendnp3/master/CommandMarshaller.h \
cpp/libs/opendnp3/src/opendnp3/master/CommandResponse.h \
cpp/libs/opendnp3/src/opendnp3/master/CommandSequence.h \
cpp/libs/opendnp3/src/opendnp3/master/CommandTask.h \
cpp/libs/opendnp3/src/opendnp3/master/ConstantCommandProcessor.h \
cpp/libs/opendnp3/src/opendnp3/master/DisableUnsolicitedTask.h \
cpp/libs/opendnp3/src/opendnp3/master/EnableUnsolicitedTask.h \
cpp/libs/opendnp3/src/opendnp3/master/ICommandCallback.h \
cpp/libs/opendnp3/src/opendnp3/master/ICommandProcessor.h \
cpp/libs/opendnp3/src/opendnp3/master/IMasterApplication.h \
cpp/libs/opendnp3/src/opendnp3/master/IMasterScheduler.h \
cpp/libs/opendnp3/src/opendnp3/master/IMasterState.h \
cpp/libs/opendnp3/src/opendnp3/master/IMasterTask.h \
cpp/libs/opendnp3/src/opendnp3/master/IPollListener.h \
cpp/libs/opendnp3/src/opendnp3/master/ISOEHandler.h \
cpp/libs/opendnp3/src/opendnp3/master/IScheduleCallback.h \
cpp/libs/opendnp3/src/opendnp3/master/ITaskLock.h \
cpp/libs/opendnp3/src/opendnp3/master/Master.h \
cpp/libs/opendnp3/src/opendnp3/master/MasterContext.h \
cpp/libs/opendnp3/src/opendnp3/master/MasterParams.h \
cpp/libs/opendnp3/src/opendnp3/master/MasterScan.h \
cpp/libs/opendnp3/src/opendnp3/master/MasterScheduler.h \
cpp/libs/opendnp3/src/opendnp3/master/MasterStackConfig.h \
cpp/libs/opendnp3/src/opendnp3/master/MasterTasks.h \
cpp/libs/opendnp3/src/opendnp3/master/MeasurementHandler.h \
cpp/libs/opendnp3/src/opendnp3/master/NullResponseTask.h \
cpp/libs/opendnp3/src/opendnp3/master/PollTask.h \
cpp/libs/opendnp3/src/opendnp3/master/PollTaskBase.h \
cpp/libs/opendnp3/src/opendnp3/master/ScanResult.h \
cpp/libs/opendnp3/src/opendnp3/master/SerialTimeSyncTask.h \
cpp/libs/opendnp3/src/opendnp3/master/SingleResponseTask.h \
cpp/libs/opendnp3/src/opendnp3/master/StartupIntegrityPoll.h \
cpp/libs/opendnp3/src/opendnp3/master/TaskBitmask.h \
cpp/libs/opendnp3/src/opendnp3/master/TaskRecord.h \
cpp/libs/opendnp3/src/opendnp3/master/TaskResult.h \
cpp/libs/opendnp3/src/opendnp3/master/TimeSyncHandler.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group1.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group10.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group11.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group12.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group2.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group20.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group21.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group22.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group23.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group3.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group30.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group32.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group4.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group40.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group41.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group42.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group50.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group51.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group52.h \
cpp/libs/opendnp3/src/opendnp3/objects/Group60.h \
cpp/libs/opendnp3/src/opendnp3/outstation/ApplicationIIN.h \
cpp/libs/opendnp3/src/opendnp3/outstation/ClassBasedRequestHandler.h \
cpp/libs/opendnp3/src/opendnp3/outstation/CommandActionAdapter.h \
cpp/libs/opendnp3/src/opendnp3/outstation/CommandResponseHandler.h \
cpp/libs/opendnp3/src/opendnp3/outstation/ConstantCommandAction.h \
cpp/libs/opendnp3/src/opendnp3/outstation/CountOf.h \
cpp/libs/opendnp3/src/opendnp3/outstation/Database.h \
cpp/libs/opendnp3/src/opendnp3/outstation/DatabaseBuffers.h \
cpp/libs/opendnp3/src/opendnp3/outstation/DatabaseTemplate.h \
cpp/libs/opendnp3/src/opendnp3/outstation/DeferredRequest.h \
cpp/libs/opendnp3/src/opendnp3/outstation/DualValue.h \
cpp/libs/opendnp3/src/opendnp3/outstation/Event.h \
cpp/libs/opendnp3/src/opendnp3/outstation/EventBufferConfig.h \
cpp/libs/opendnp3/src/opendnp3/outstation/EventCount.h \
cpp/libs/opendnp3/src/opendnp3/outstation/EventMetadata.h \
cpp/libs/opendnp3/src/opendnp3/outstation/EventResponseConfig.h \
cpp/libs/opendnp3/src/opendnp3/outstation/EventResponseContext.h \
cpp/libs/opendnp3/src/opendnp3/outstation/EventResponseTypes.h \
cpp/libs/opendnp3/src/opendnp3/outstation/EventWriteFunctions.h \
cpp/libs/opendnp3/src/opendnp3/outstation/EventWriteOperation.h \
cpp/libs/opendnp3/src/opendnp3/outstation/ICommandAction.h \
cpp/libs/opendnp3/src/opendnp3/outstation/ICommandHandler.h \
cpp/libs/opendnp3/src/opendnp3/outstation/IDatabase.h \
cpp/libs/opendnp3/src/opendnp3/outstation/IEventBuffer.h \
cpp/libs/opendnp3/src/opendnp3/outstation/IINHelpers.h \
cpp/libs/opendnp3/src/opendnp3/outstation/IOutstationApplication.h \
cpp/libs/opendnp3/src/opendnp3/outstation/Outstation.h \
cpp/libs/opendnp3/src/opendnp3/outstation/OutstationConfig.h \
cpp/libs/opendnp3/src/opendnp3/outstation/OutstationContext.h \
cpp/libs/opendnp3/src/opendnp3/outstation/OutstationEventBuffer.h \
cpp/libs/opendnp3/src/opendnp3/outstation/OutstationParams.h \
cpp/libs/opendnp3/src/opendnp3/outstation/OutstationSolicitedStates.h \
cpp/libs/opendnp3/src/opendnp3/outstation/OutstationStackConfig.h \
cpp/libs/opendnp3/src/opendnp3/outstation/OutstationUnsolicitedStates.h \
cpp/libs/opendnp3/src/opendnp3/outstation/ReadHandler.h \
cpp/libs/opendnp3/src/opendnp3/outstation/ResponseContext.h \
cpp/libs/opendnp3/src/opendnp3/outstation/SOERecord.h \
cpp/libs/opendnp3/src/opendnp3/outstation/SelectionCriteria.h \
cpp/libs/opendnp3/src/opendnp3/outstation/SelectionWriter.h \
cpp/libs/opendnp3/src/opendnp3/outstation/SimpleCommandHandler.h \
cpp/libs/opendnp3/src/opendnp3/outstation/StaticLoadFunctions.h \
cpp/libs/opendnp3/src/opendnp3/outstation/StaticLoader.h \
cpp/libs/opendnp3/src/opendnp3/outstation/StaticResponseConfig.h \
cpp/libs/opendnp3/src/opendnp3/outstation/StaticResponseContext.h \
cpp/libs/opendnp3/src/opendnp3/outstation/TimeTransaction.h \
cpp/libs/opendnp3/src/opendnp3/outstation/ValueMetadataPair.h \
cpp/libs/opendnp3/src/opendnp3/outstation/WriteHandler.h \
cpp/libs/opendnp3/src/opendnp3/transport/TransportConstants.h \
cpp/libs/opendnp3/src/opendnp3/transport/TransportLayer.h \
cpp/libs/opendnp3/src/opendnp3/transport/TransportRx.h \
cpp/libs/opendnp3/src/opendnp3/transport/TransportStack.h \
cpp/libs/opendnp3/src/opendnp3/transport/TransportTx.h \
cpp/libs/openpal/src/openpal/Configure.h \
cpp/libs/openpal/src/openpal/channel/ChannelStatistics.h \
cpp/libs/openpal/src/openpal/channel/IPhysicalLayer.h \
cpp/libs/openpal/src/openpal/channel/IPhysicalLayerCallbacks.h \
cpp/libs/openpal/src/openpal/container/DynamicArray.h \
cpp/libs/openpal/src/openpal/container/DynamicBuffer.h \
cpp/libs/openpal/src/openpal/container/HasSize.h \
cpp/libs/openpal/src/openpal/container/Indexable.h \
cpp/libs/openpal/src/openpal/container/IndexableIterator.h \
cpp/libs/openpal/src/openpal/container/LinkedList.h \
cpp/libs/openpal/src/openpal/container/Pair.h \
cpp/libs/openpal/src/openpal/container/Queue.h \
cpp/libs/openpal/src/openpal/container/ReadOnlyBuffer.h \
cpp/libs/openpal/src/openpal/container/RingBuffer.h \
cpp/libs/openpal/src/openpal/container/Settable.h \
cpp/libs/openpal/src/openpal/container/Stack.h \
cpp/libs/openpal/src/openpal/container/WriteBuffer.h \
cpp/libs/openpal/src/openpal/executor/Action0.h \
cpp/libs/openpal/src/openpal/executor/Erasure.h \
cpp/libs/openpal/src/openpal/executor/Function0.h \
cpp/libs/openpal/src/openpal/executor/Function1.h \
cpp/libs/openpal/src/openpal/executor/IExecutor.h \
cpp/libs/openpal/src/openpal/executor/IMutex.h \
cpp/libs/openpal/src/openpal/executor/ITimer.h \
cpp/libs/openpal/src/openpal/executor/IUTCTimeSource.h \
cpp/libs/openpal/src/openpal/executor/MonotonicTimestamp.h \
cpp/libs/openpal/src/openpal/executor/TimeDuration.h \
cpp/libs/openpal/src/openpal/executor/UTCTimestamp.h \
cpp/libs/openpal/src/openpal/logging/ILogHandler.h \
cpp/libs/openpal/src/openpal/logging/Location.h \
cpp/libs/openpal/src/openpal/logging/LogEntry.h \
cpp/libs/openpal/src/openpal/logging/LogFilters.h \
cpp/libs/openpal/src/openpal/logging/LogLevels.h \
cpp/libs/openpal/src/openpal/logging/LogMacros.h \
cpp/libs/openpal/src/openpal/logging/LogRoot.h \
cpp/libs/openpal/src/openpal/logging/Logger.h \
cpp/libs/openpal/src/openpal/logging/StringFormatting.h \
cpp/libs/openpal/src/openpal/serialization/ByteSerialization.h \
cpp/libs/openpal/src/openpal/serialization/FloatSerializationTemplates.h \
cpp/libs/openpal/src/openpal/serialization/Serialization.h \
cpp/libs/openpal/src/openpal/serialization/SerializationTemplatesBE.h \
cpp/libs/openpal/src/openpal/serialization/SerializationTemplatesLE.h \
cpp/libs/openpal/src/openpal/serialization/Serializer.h \
cpp/libs/openpal/src/openpal/serialization/UInt48LE.h \
cpp/libs/openpal/src/openpal/util/Comparisons.h \
cpp/libs/openpal/src/openpal/util/Limits.h \
cpp/libs/openpal/src/openpal/util/ToHex.h \
cpp/libs/openpal/src/openpal/util/Uncopyable.h \
cpp/tests/opendnp3tests/src/APDUHelpers.h \
cpp/tests/opendnp3tests/src/APDUHexBuilders.h \
cpp/tests/opendnp3tests/src/BufferHelpers.h \
cpp/tests/opendnp3tests/src/BufferTestObject.h \
cpp/tests/opendnp3tests/src/CopyableBuffer.h \
cpp/tests/opendnp3tests/src/DNPHelpers.h \
cpp/tests/opendnp3tests/src/DatabaseTestObject.h \
cpp/tests/opendnp3tests/src/FanoutDataObserver.h \
cpp/tests/opendnp3tests/src/HexConversions.h \
cpp/tests/opendnp3tests/src/LinkLayerRouterTest.h \
cpp/tests/opendnp3tests/src/LinkLayerTest.h \
cpp/tests/opendnp3tests/src/LinkReceiverTest.h \
cpp/tests/opendnp3tests/src/LogTester.h \
cpp/tests/opendnp3tests/src/LoopbackPhysicalLayer.h \
cpp/tests/opendnp3tests/src/LowerLayerToPhysAdapter.h \
cpp/tests/opendnp3tests/src/MasterTestObject.h \
cpp/tests/opendnp3tests/src/MeasurementComparisons.h \
cpp/tests/opendnp3tests/src/MockAPDUHeaderHandler.h \
cpp/tests/opendnp3tests/src/MockCommandCallback.h \
cpp/tests/opendnp3tests/src/MockCommandHandler.h \
cpp/tests/opendnp3tests/src/MockExecutor.h \
cpp/tests/opendnp3tests/src/MockFrameSink.h \
cpp/tests/opendnp3tests/src/MockLinkLayer.h \
cpp/tests/opendnp3tests/src/MockLowerLayer.h \
cpp/tests/opendnp3tests/src/MockMasterApplication.h \
cpp/tests/opendnp3tests/src/MockOutstationApplication.h \
cpp/tests/opendnp3tests/src/MockPhysicalLayer.h \
cpp/tests/opendnp3tests/src/MockPhysicalLayerMonitor.h \
cpp/tests/opendnp3tests/src/MockSOEHandler.h \
cpp/tests/opendnp3tests/src/MockTransportLayer.h \
cpp/tests/opendnp3tests/src/MockUpperLayer.h \
cpp/tests/opendnp3tests/src/MultiplexingDataObserver.h \
cpp/tests/opendnp3tests/src/OutstationTestObject.h \
cpp/tests/opendnp3tests/src/PhysBaseTest.h \
cpp/tests/opendnp3tests/src/PhysLoopback.h \
cpp/tests/opendnp3tests/src/PhysTestObject.h \
cpp/tests/opendnp3tests/src/PhysicalLayerWrapperFactory.h \
cpp/tests/opendnp3tests/src/ProtocolUtil.h \
cpp/tests/opendnp3tests/src/Random.h \
cpp/tests/opendnp3tests/src/RandomDouble.h \
cpp/tests/opendnp3tests/src/RandomizedBuffer.h \
cpp/tests/opendnp3tests/src/SerialTestObject.h \
cpp/tests/opendnp3tests/src/StopWatch.h \
cpp/tests/opendnp3tests/src/TestHelpers.h \
cpp/tests/opendnp3tests/src/TestObject.h \
cpp/tests/opendnp3tests/src/TestObjectASIO.h \
cpp/tests/opendnp3tests/src/TestTypedefs.h \
cpp/tests/opendnp3tests/src/Timeout.h \
cpp/tests/opendnp3tests/src/TransportIntegrationStack.h \
cpp/tests/opendnp3tests/src/TransportLoopbackTestObject.h \
cpp/tests/opendnp3tests/src/TransportScalabilityTestObject.h \
cpp/tests/opendnp3tests/src/TransportStackPair.h \
cpp/tests/opendnp3tests/src/TransportTestObject.h
