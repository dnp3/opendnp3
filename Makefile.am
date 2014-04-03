#copyright (c) 2013 Automatak, LLC

ACLOCAL_AMFLAGS = -I m4

ASIO_CONFIG = -I $(ASIO_HOME)/include -DASIO_STANDALONE

OPENPAL_INCLUDE = $(top_srcdir)/cpp/openpal/src
OPENDNP3_INCLUDE = $(top_srcdir)/cpp/opendnp3/src
ASIOPAL_INCLUDE = $(top_srcdir)/cpp/asiopal/src
ASIODNP3_INCLUDE = $(top_srcdir)/cpp/asiodnp3/include
CATCH_INCLUDE = $(top_srcdir)/cpp/catch

#bin_PROGRAMS = masterdemo outstationdemo apdufuzz

lib_LTLIBRARIES = libopenpal.la libopendnp3.la

#ls cpp/openpal/src/* -1 | awk '{print $0" \\"}'
libopenpal_la_LDFLAGS = -version-info 1:1:0
libopenpal_la_CXXFLAGS = -I$(OPENPAL_INCLUDE)
libopenpal_la_SOURCES = \
cpp/openpal/src/openpal/BufferWrapper.cpp \
cpp/openpal/src/openpal/LogEntry.cpp \
cpp/openpal/src/openpal/Logger.cpp \
cpp/openpal/src/openpal/LogRoot.cpp \
cpp/openpal/src/openpal/MonotonicTimestamp.cpp \
cpp/openpal/src/openpal/ReverseEndian.cpp \
cpp/openpal/src/openpal/Runnable.cpp \
cpp/openpal/src/openpal/TimeDuration.cpp \
cpp/openpal/src/openpal/ToHex.cpp

#ls cpp/opendnp3/src/opendnp3/*.cpp -1 | awk '{print $0" \\"}'
libopendnp3_la_LDFLAGS = -version-info 2:0:0
libopendnp3_la_CXXFLAGS = -I$(OPENPAL_INCLUDE) -I $(OPENDNP3_INCLUDE)
libopendnp3_la_SOURCES = \
cpp/opendnp3/src/opendnp3/app/AnalogOutput.cpp \
cpp/opendnp3/src/opendnp3/app/APDUHandlerBase.cpp \
cpp/opendnp3/src/opendnp3/app/APDUHeaderParser.cpp \
cpp/opendnp3/src/opendnp3/app/APDUParser.cpp \
cpp/opendnp3/src/opendnp3/app/APDURequest.cpp \
cpp/opendnp3/src/opendnp3/app/APDUResponse.cpp \
cpp/opendnp3/src/opendnp3/app/APDUWrapper.cpp \
cpp/opendnp3/src/opendnp3/app/AppChannelStates.cpp \
cpp/opendnp3/src/opendnp3/app/AppControlField.cpp \
cpp/opendnp3/src/opendnp3/app/AppLayerChannel.cpp \
cpp/opendnp3/src/opendnp3/app/AppLayer.cpp \
cpp/opendnp3/src/opendnp3/app/ApplicationStack.cpp \
cpp/opendnp3/src/opendnp3/app/BitReader.cpp \
cpp/opendnp3/src/opendnp3/app/ControlRelayOutputBlock.cpp \
cpp/opendnp3/src/opendnp3/app/DownSampling.cpp \
cpp/opendnp3/src/opendnp3/app/EventTriggers.cpp \
cpp/opendnp3/src/opendnp3/app/FunctionCodeHelpers.cpp \
cpp/opendnp3/src/opendnp3/app/GroupVariation.cpp \
cpp/opendnp3/src/opendnp3/app/IAppUser.cpp \
cpp/opendnp3/src/opendnp3/app/IINField.cpp \
cpp/opendnp3/src/opendnp3/app/ObjectWriter.cpp \
cpp/opendnp3/src/opendnp3/app/OctetData.cpp \
cpp/opendnp3/src/opendnp3/app/PointClass.cpp \
cpp/opendnp3/src/opendnp3/app/SolicitedChannel.cpp \
cpp/opendnp3/src/opendnp3/app/StaticRange.cpp \
cpp/opendnp3/src/opendnp3/app/UnsolicitedChannel.cpp \
cpp/opendnp3/src/opendnp3/gen/ChannelState.cpp \
cpp/opendnp3/src/opendnp3/gen/CommandResult.cpp \
cpp/opendnp3/src/opendnp3/gen/CommandStatus.cpp \
cpp/opendnp3/src/opendnp3/gen/ControlCode.cpp \
cpp/opendnp3/src/opendnp3/gen/DoubleBit.cpp \
cpp/opendnp3/src/opendnp3/gen/EventAnalogOutputStatusResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/EventAnalogResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/EventBinaryOutputStatusResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/EventBinaryResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/EventCounterResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/EventDoubleBinaryResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/EventFrozenCounterResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/FunctionCode.cpp \
cpp/opendnp3/src/opendnp3/gen/LinkFunction.cpp \
cpp/opendnp3/src/opendnp3/gen/QualifierCode.cpp \
cpp/opendnp3/src/opendnp3/gen/ScanStatus.cpp \
cpp/opendnp3/src/opendnp3/gen/StaticAnalogOutputStatusResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/StaticAnalogResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/StaticBinaryOutputStatusResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/StaticBinaryResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/StaticCounterResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/StaticDoubleBinaryResponse.cpp \
cpp/opendnp3/src/opendnp3/gen/StaticFrozenCounterResponse.cpp \
cpp/opendnp3/src/opendnp3/link/DNPCrc.cpp \
cpp/opendnp3/src/opendnp3/link/IOpenDelayStrategy.cpp \
cpp/opendnp3/src/opendnp3/link/LinkFrame.cpp \
cpp/opendnp3/src/opendnp3/link/LinkHeader.cpp \
cpp/opendnp3/src/opendnp3/link/LinkLayer.cpp \
cpp/opendnp3/src/opendnp3/link/LinkLayerReceiver.cpp \
cpp/opendnp3/src/opendnp3/link/LinkLayerRouter.cpp \
cpp/opendnp3/src/opendnp3/link/LinkReceiverStates.cpp \
cpp/opendnp3/src/opendnp3/link/LinkRoute.cpp \
cpp/opendnp3/src/opendnp3/link/PhysicalLayerMonitor.cpp \
cpp/opendnp3/src/opendnp3/link/PhysicalLayerMonitorStates.cpp \
cpp/opendnp3/src/opendnp3/link/PriLinkLayerStates.cpp \
cpp/opendnp3/src/opendnp3/link/SecLinkLayerStates.cpp \
cpp/opendnp3/src/opendnp3/link/ShiftableBuffer.cpp \
cpp/opendnp3/src/opendnp3/master/AsyncTaskBase.cpp \
cpp/opendnp3/src/opendnp3/master/AsyncTaskContinuous.cpp \
cpp/opendnp3/src/opendnp3/master/AsyncTaskGroup.cpp \
cpp/opendnp3/src/opendnp3/master/AsyncTaskNonPeriodic.cpp \
cpp/opendnp3/src/opendnp3/master/AsyncTaskPeriodic.cpp \
cpp/opendnp3/src/opendnp3/master/CommandAction.cpp \
cpp/opendnp3/src/opendnp3/master/CommandResponse.cpp \
cpp/opendnp3/src/opendnp3/master/CommandTask.cpp \
cpp/opendnp3/src/opendnp3/master/ConstantCommandProcessor.cpp \
cpp/opendnp3/src/opendnp3/master/DataPoll.cpp \
cpp/opendnp3/src/opendnp3/master/ISOEHandler.cpp \
cpp/opendnp3/src/opendnp3/master/Master.cpp \
cpp/opendnp3/src/opendnp3/master/MasterScan.cpp \
cpp/opendnp3/src/opendnp3/master/MasterSchedule.cpp \
cpp/opendnp3/src/opendnp3/master/MasterStates.cpp \
cpp/opendnp3/src/opendnp3/master/MasterTaskBase.cpp \
cpp/opendnp3/src/opendnp3/master/MeasurementHandler.cpp \
cpp/opendnp3/src/opendnp3/master/QueuedCommandProcessor.cpp \
cpp/opendnp3/src/opendnp3/master/StartupTasks.cpp \
cpp/opendnp3/src/opendnp3/master/TrackingTaskGroup.cpp \
cpp/opendnp3/src/opendnp3/objects/Group10.cpp \
cpp/opendnp3/src/opendnp3/objects/Group11.cpp \
cpp/opendnp3/src/opendnp3/objects/Group12.cpp \
cpp/opendnp3/src/opendnp3/objects/Group1.cpp \
cpp/opendnp3/src/opendnp3/objects/Group20.cpp \
cpp/opendnp3/src/opendnp3/objects/Group21.cpp \
cpp/opendnp3/src/opendnp3/objects/Group22.cpp \
cpp/opendnp3/src/opendnp3/objects/Group23.cpp \
cpp/opendnp3/src/opendnp3/objects/Group2.cpp \
cpp/opendnp3/src/opendnp3/objects/Group3.cpp \
cpp/opendnp3/src/opendnp3/objects/Group30.cpp \
cpp/opendnp3/src/opendnp3/objects/Group32.cpp \
cpp/opendnp3/src/opendnp3/objects/Group4.cpp \
cpp/opendnp3/src/opendnp3/objects/Group40.cpp \
cpp/opendnp3/src/opendnp3/objects/Group41.cpp \
cpp/opendnp3/src/opendnp3/objects/Group42.cpp \
cpp/opendnp3/src/opendnp3/objects/Group50.cpp \
cpp/opendnp3/src/opendnp3/objects/Group51.cpp \
cpp/opendnp3/src/opendnp3/objects/Group52.cpp \
cpp/opendnp3/src/opendnp3/objects/Group60.cpp \
cpp/opendnp3/src/opendnp3/outstation/CachedRequest.cpp \
cpp/opendnp3/src/opendnp3/outstation/CommandActionAdapter.cpp \
cpp/opendnp3/src/opendnp3/outstation/CommandResponseHandler.cpp \
cpp/opendnp3/src/opendnp3/outstation/Database.cpp \
cpp/opendnp3/src/opendnp3/outstation/DynamicallyAllocatedDatabase.cpp \
cpp/opendnp3/src/opendnp3/outstation/DynamicallyAllocatedEventBuffer.cpp \
cpp/opendnp3/src/opendnp3/outstation/EventCount.cpp \
cpp/opendnp3/src/opendnp3/outstation/EventBufferConfig.cpp \
cpp/opendnp3/src/opendnp3/outstation/EventBufferFacade.cpp \
cpp/opendnp3/src/opendnp3/outstation/EventResponseContext.cpp \
cpp/opendnp3/src/opendnp3/outstation/IINHelpers.cpp \
cpp/opendnp3/src/opendnp3/outstation/ITimeWriteHandler.cpp \
cpp/opendnp3/src/opendnp3/outstation/OutstationConfig.cpp \
cpp/opendnp3/src/opendnp3/outstation/OutstationEventBuffer.cpp \
cpp/opendnp3/src/opendnp3/outstation/OutstationStates.cpp \
cpp/opendnp3/src/opendnp3/outstation/Outstation.cpp \
cpp/opendnp3/src/opendnp3/outstation/ReadHandler.cpp \
cpp/opendnp3/src/opendnp3/outstation/ResponseContext.cpp \
cpp/opendnp3/src/opendnp3/outstation/SelectBuffer.cpp \
cpp/opendnp3/src/opendnp3/outstation/SelectionCriteria.cpp \
cpp/opendnp3/src/opendnp3/outstation/SelectionIterator.cpp \
cpp/opendnp3/src/opendnp3/outstation/SimpleCommandHandler.cpp \
cpp/opendnp3/src/opendnp3/outstation/StaticResponseContext.cpp \
cpp/opendnp3/src/opendnp3/outstation/StaticResponseTypes.cpp \
cpp/opendnp3/src/opendnp3/outstation/TimeTransaction.cpp \
cpp/opendnp3/src/opendnp3/outstation/WriteHandler.cpp \
cpp/opendnp3/src/opendnp3/transport/TransportLayer.cpp \
cpp/opendnp3/src/opendnp3/transport/TransportRx.cpp \
cpp/opendnp3/src/opendnp3/transport/TransportStates.cpp \
cpp/opendnp3/src/opendnp3/transport/TransportTx.cpp \
cpp/opendnp3/src/opendnp3/LogLevelInterpreter.cpp


