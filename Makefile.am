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
cpp/libs/openpal/src/openpal/container/RingBuffer.cpp \
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
cpp/libs/opendnp3/src/opendnp3/app/PointIndexes.cpp \
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
cpp/libs/opendnp3/src/opendnp3/outstation/DynamicallyAllocatedDatabase.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/DynamicallyAllocatedEventBuffer.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/EventBufferConfig.cpp \
cpp/libs/opendnp3/src/opendnp3/outstation/EventBufferFacade.cpp \
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

apdufuzzer_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I$(OPENDNP3_INCLUDE) -Wl,--no-as-needed
apdufuzzer_LDFLAGS = -pthread
apdufuzzer_LDADD = libopenpal.la libopendnp3.la
apdufuzzer_SOURCES = cpp/tests/apdufuzzer/ApduFuzzer.cpp

check_PROGRAMS = openpaltest dnp3test apdufuzzer
#TESTS = openpaltest dnp3test

#ls cpp/tests/openpaltests/src/*.cpp -1 | awk '{print $0" \\"}'
openpaltest_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I$(CATCH_INCLUDE)
openpaltest_LDADD = libopenpal.la
openpaltest_SOURCES = \
cpp/tests/openpaltests/src/CatchTestStart.cpp \
cpp/tests/openpaltests/src/LinkedListAdapterTestSuite.cpp \
cpp/tests/openpaltests/src/RingBufferTestSuite.cpp \
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


