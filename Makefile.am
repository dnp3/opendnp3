#copyright (c) 2013 Automatak, LLC

ACLOCAL_AMFLAGS = -I m4

CORE_BOOST_LIBS = $(BOOST_SYSTEM_LIB)
TEST_BOOST_LIBS = $(CORE_BOOST_LIBS) $(BOOST_UNIT_TEST_FRAMEWORK_LIB)

AM_CXXFLAGS = -std=c++11

lib_LTLIBRARIES = libopendnp3.la

libopendnp3_la_SOURCES = \
	cpp/src/AlwaysOpeningVtoRouter.cpp \
	cpp/src/AnalogOutput.cpp \
	cpp/src/APDUConstants.cpp \
	cpp/src/APDU.cpp \
	cpp/src/AppChannelStates.cpp \
	cpp/src/AppHeader.cpp \
	cpp/src/AppInterfaces.cpp \
	cpp/src/AppLayerChannel.cpp \
	cpp/src/AppLayer.cpp \
	cpp/src/ApplicationStack.cpp \
	cpp/src/ASIOExecutor.cpp \
	cpp/src/ASIOSerialHelpers.cpp \
	cpp/src/AsyncLayerInterfaces.cpp \
	cpp/src/AsyncTaskBase.cpp \
	cpp/src/AsyncTaskContinuous.cpp \
	cpp/src/AsyncTaskGroup.cpp \
	cpp/src/AsyncTaskNonPeriodic.cpp \
	cpp/src/AsyncTaskPeriodic.cpp \
	cpp/src/AsyncTaskScheduler.cpp \
	cpp/src/BaseDataTypes.cpp \
	cpp/src/BufferTypes.cpp \
	cpp/src/ChangeBuffer.cpp \
	cpp/src/ClassCounter.cpp \
	cpp/src/CommandStatus.cpp \
	cpp/src/CommandTask.cpp \
	cpp/src/ConstantCommandProcessor.cpp \
	cpp/src/ControlRelayOutputBlock.cpp \
	cpp/src/CopyableBuffer.cpp \
	cpp/src/CRC.cpp \
	cpp/src/Database.cpp \
	cpp/src/DataPoll.cpp \
	cpp/src/DestructorHook.cpp \
	cpp/src/DeviceTemplate.cpp \
	cpp/src/DNP3Channel.cpp \
	cpp/src/DNP3Manager.cpp \
	cpp/src/DNPCrc.cpp \
	cpp/src/EnhancedVto.cpp \
	cpp/src/EnhancedVtoRouter.cpp \
	cpp/src/Exception.cpp \
	cpp/src/ExecutorPause.cpp \
	cpp/src/FlexibleDataObserver.cpp \
	cpp/src/HeaderReadIterator.cpp \
	cpp/src/IExecutor.cpp \
	cpp/src/IHandlerAsync.cpp \
	cpp/src/IndexedWriteIterator.cpp \
	cpp/src/IOServiceThreadPool.cpp \
	cpp/src/IStack.cpp \
	cpp/src/IStackObserver.cpp \
	cpp/src/ITimeSource.cpp \
	cpp/src/LinkFrame.cpp \
	cpp/src/LinkHeader.cpp \
	cpp/src/LinkLayerConstants.cpp \
	cpp/src/LinkLayer.cpp \
	cpp/src/LinkLayerReceiver.cpp \
	cpp/src/LinkLayerRouter.cpp \
	cpp/src/LinkReceiverStates.cpp \
	cpp/src/LinkRoute.cpp \
	cpp/src/Log.cpp \
	cpp/src/LogEntryCircularBuffer.cpp \
	cpp/src/LogEntry.cpp \
	cpp/src/Loggable.cpp \
	cpp/src/Logger.cpp \
	cpp/src/LogToFile.cpp \
	cpp/src/LogToStdio.cpp \
	cpp/src/LogTypes.cpp \
	cpp/src/LowerLayerToPhysAdapter.cpp \
	cpp/src/Master.cpp \
	cpp/src/MasterSchedule.cpp \
	cpp/src/MasterStackImpl.cpp \
	cpp/src/MasterStates.cpp \
	cpp/src/MasterTaskBase.cpp \
	cpp/src/MultiplexingDataObserver.cpp \
	cpp/src/ObjectHeader.cpp \
	cpp/src/ObjectInterfaces.cpp \
	cpp/src/ObjectReadIterator.cpp \
	cpp/src/Objects.cpp \
	cpp/src/ObjectWriteIterator.cpp \
	cpp/src/OutstationStackImpl.cpp \
	cpp/src/PackingUnpacking.cpp \
	cpp/src/Parsing.cpp \
	cpp/src/PhysicalLayerAsyncBase.cpp \
	cpp/src/PhysicalLayerAsyncBaseTCP.cpp \
	cpp/src/PhysicalLayerAsyncSerial.cpp \
	cpp/src/PhysicalLayerAsyncTCPClient.cpp \
	cpp/src/PhysicalLayerAsyncTCPServer.cpp \
	cpp/src/PhysicalLayerMonitor.cpp \
	cpp/src/PhysicalLayerMonitorStates.cpp \
	cpp/src/PhysicalLayerStates.cpp \
	cpp/src/PhysLoopback.cpp \
	cpp/src/PointClass.cpp \
	cpp/src/PriLinkLayerStates.cpp \
	cpp/src/ProtocolUtil.cpp \
	cpp/src/QualityConverter.cpp \
	cpp/src/QueuedCommandProcessor.cpp \
	cpp/src/RandomizedBuffer.cpp \
	cpp/src/ResponseContext.cpp \
	cpp/src/ResponseLoader.cpp \
	cpp/src/SecLinkLayerStates.cpp \
	cpp/src/ShiftableBuffer.cpp \
	cpp/src/SimpleCommandHandler.cpp \
	cpp/src/SimpleDataObserver.cpp \
	cpp/src/SlaveConfig.cpp \
	cpp/src/Slave.cpp \
	cpp/src/SlaveEventBuffer.cpp \
	cpp/src/SlaveResponseTypes.cpp \
	cpp/src/SlaveStates.cpp \
	cpp/src/SolicitedChannel.cpp \
	cpp/src/StartupTasks.cpp \
	cpp/src/SubjectBase.cpp \
	cpp/src/Threadable.cpp \
	cpp/src/Thread.cpp \
	cpp/src/TimerASIO.cpp \
	cpp/src/TimeSource.cpp \
	cpp/src/TLS_Base.cpp \
	cpp/src/ToHex.cpp \
	cpp/src/TrackingTaskGroup.cpp \
	cpp/src/TransportLayer.cpp \
	cpp/src/TransportRx.cpp \
	cpp/src/TransportStates.cpp \
	cpp/src/TransportTx.cpp \
	cpp/src/UnsolicitedChannel.cpp \
	cpp/src/Util.cpp \
	cpp/src/VtoData.cpp \
	cpp/src/VtoEndpointImpl.cpp \
	cpp/src/VtoReader.cpp \
	cpp/src/VtoRouter.cpp \
	cpp/src/VtoRouterSettings.cpp \
	cpp/src/VtoTransmitTask.cpp \
	cpp/src/VtoWriter.cpp

