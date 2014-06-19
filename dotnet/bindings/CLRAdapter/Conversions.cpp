#include <msclr\marshal_cppstd.h>
#include "Conversions.h"

using namespace DNP3::Interface;

#ifdef ERROR
#undef ERROR
#endif

namespace DNP3
{
namespace Adapter
{

std::string Conversions::ConvertString(System::String^ s)
{
	return msclr::interop::marshal_as<std::string>(s);
}

System::String^ Conversions::ConvertString(const std::string& s)
{
	return gcnew System::String(s.c_str());
}

ChannelState Conversions::ConvertChannelState(opendnp3::ChannelState aState)
{
	return (ChannelState) aState;
}

IChannelStatistics^ Conversions::ConvertChannelStats(const opendnp3::LinkChannelStatistics& stats)
{
	ChannelStatistics^ ret = gcnew ChannelStatistics();
	ret->numBytesRx = stats.numBytesRx;
	ret->numBytesTx = stats.numBytesTx;
	ret->numOpen = stats.numOpen;
	ret->numOpenFail = stats.numOpenFail;
	ret->numClose = stats.numClose;
	ret->numLinkFrameRx = stats.numLinkFrameRx;
	ret->numLinkFrameTx = stats.numLinkFrameTx;
	ret->numBadLinkFrameRx = stats.numBadLinkFrameRx;
	ret->numCrcError = stats.numCrcError;
	return ret;
}

IStackStatistics^ Conversions::ConvertStackStats(const opendnp3::StackStatistics& statistics)
{
	StackStatistics^ ret = gcnew StackStatistics();
	ret->numTransportErrorRx = statistics.numTransportErrorRx;
	ret->numTransportRx = statistics.numTransportRx;
	ret->numTransportTx = statistics.numTransportTx;
	return ret;
}

CommandResponse Conversions::ConvertCommandResponse(opendnp3::CommandResponse response)
{
	auto result = ConvertCommandResult(response.GetResult());
	auto status = ConvertCommandStatus(response.GetStatus());
	return CommandResponse(result, status);
}

CommandResult Conversions::ConvertCommandResult(opendnp3::CommandResult result)
{
	return (CommandResult) result;
}

CommandStatus Conversions::ConvertCommandStatus(opendnp3::CommandStatus status)
{
	return (CommandStatus) status;
}

opendnp3::CommandStatus Conversions::ConvertCommandStatus(CommandStatus status)
{
	return (opendnp3::CommandStatus) status;
}

ControlCode Conversions::ConvertControlCode(opendnp3::ControlCode code)
{
	return (ControlCode) ControlCodeToType(code);
}

opendnp3::ControlCode Conversions::ConvertControlCode(ControlCode code)
{
	return (opendnp3::ControlCode) code;
}

ControlRelayOutputBlock^ Conversions::ConvertCommand(const opendnp3::ControlRelayOutputBlock& bo)
{
	return gcnew ControlRelayOutputBlock(ConvertControlCode(bo.functionCode), bo.count, bo.onTimeMS, bo.offTimeMS);
}

opendnp3::ControlRelayOutputBlock Conversions::ConvertCommand(ControlRelayOutputBlock^ bo)
{
	return opendnp3::ControlRelayOutputBlock(ConvertControlCode(bo->code), bo->count, bo->onTime, bo->offTime);
}

opendnp3::AnalogOutputInt32 Conversions::ConvertCommand(AnalogOutputInt32^ sp)
{
	return opendnp3::AnalogOutputInt32(sp->value);
}

AnalogOutputInt32^ Conversions::ConvertCommand(const opendnp3::AnalogOutputInt32& sp)
{
	return gcnew AnalogOutputInt32(sp.value);
}

opendnp3::AnalogOutputInt16 Conversions::ConvertCommand(AnalogOutputInt16^ sp)
{
	return opendnp3::AnalogOutputInt16(sp->value);
}

AnalogOutputInt16^ Conversions::ConvertCommand(const opendnp3::AnalogOutputInt16& sp)
{
	return gcnew AnalogOutputInt16(sp.value);
}

opendnp3::AnalogOutputFloat32 Conversions::ConvertCommand(AnalogOutputFloat32^ sp)
{
	return opendnp3::AnalogOutputFloat32(sp->value);
}

AnalogOutputFloat32^ Conversions::ConvertCommand(const opendnp3::AnalogOutputFloat32& sp)
{
	return gcnew AnalogOutputFloat32(sp.value);
}

opendnp3::AnalogOutputDouble64 Conversions::ConvertCommand(AnalogOutputDouble64^ sp)
{
	return opendnp3::AnalogOutputDouble64(sp->value);
}

AnalogOutputDouble64^ Conversions::ConvertCommand(const opendnp3::AnalogOutputDouble64& sp)
{
	return gcnew AnalogOutputDouble64(sp.value);
}

Binary^ Conversions::ConvertMeas(opendnp3::Binary meas)
{
	if (meas.IsTimeValid())
	{
		return gcnew Binary(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}
	else
	{
		return gcnew Binary(meas.GetValue(), meas.GetQuality());
	}
}

DoubleBitBinary^ Conversions::ConvertMeas(opendnp3::DoubleBitBinary meas)
{
	if (meas.IsTimeValid())
	{
		return gcnew DoubleBitBinary(static_cast<DoubleBit>(meas.GetValue()), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}
	else
	{
		return gcnew DoubleBitBinary(static_cast<DoubleBit>(meas.GetValue()), meas.GetQuality());
	}
}

Analog^ Conversions::ConvertMeas(opendnp3::Analog meas)
{
	if (meas.IsTimeValid())
	{
		return gcnew Analog(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}
	else
	{
		return gcnew Analog(meas.GetValue(), meas.GetQuality());
	}
}

Counter^ Conversions::ConvertMeas(opendnp3::Counter meas)
{
	if (meas.IsTimeValid())
	{
		return gcnew Counter(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}
	else
	{
		return gcnew Counter(meas.GetValue(), meas.GetQuality());
	}
}

FrozenCounter^ Conversions::ConvertMeas(opendnp3::FrozenCounter meas)
{
	if (meas.IsTimeValid())
	{
		return gcnew FrozenCounter(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}
	else
	{
		return gcnew FrozenCounter(meas.GetValue(), meas.GetQuality());
	}
}

AnalogOutputStatus^ Conversions::ConvertMeas(opendnp3::AnalogOutputStatus meas)
{
	if (meas.IsTimeValid())
	{
		return gcnew AnalogOutputStatus(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}
	else
	{
		return gcnew AnalogOutputStatus(meas.GetValue(), meas.GetQuality());
	}
}

BinaryOutputStatus^ Conversions::ConvertMeas(opendnp3::BinaryOutputStatus meas)
{
	if (meas.IsTimeValid())
	{
		return gcnew BinaryOutputStatus(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
	}
	else
	{
		return gcnew BinaryOutputStatus(meas.GetValue(), meas.GetQuality());
	}
}

OctetString^ Conversions::ConvertMeas(const opendnp3::OctetString& arMeas)
{
	auto buffer = arMeas.ToReadOnly();
	array<System::Byte>^ bytes = gcnew array<System::Byte>(buffer.Size());
	for (uint32_t i = 0; i < buffer.Size(); ++i) bytes[i] = buffer[i];
	return gcnew OctetString(bytes);
}

opendnp3::Binary Conversions::ConvertMeas(Binary^ meas)
{
	return opendnp3::Binary(meas->Value, meas->Quality, TimeStamp::Convert(meas->Timestamp));
}

opendnp3::Analog Conversions::ConvertMeas(Analog^ meas)
{
	return opendnp3::Analog(meas->Value, meas->Quality, TimeStamp::Convert(meas->Timestamp));
}

opendnp3::Counter Conversions::ConvertMeas(Counter^ meas)
{
	return opendnp3::Counter(meas->Value, meas->Quality, TimeStamp::Convert(meas->Timestamp));
}

opendnp3::FrozenCounter Conversions::ConvertMeas(FrozenCounter^ meas)
{
	return opendnp3::FrozenCounter(meas->Value, meas->Quality, TimeStamp::Convert(meas->Timestamp));
}

opendnp3::AnalogOutputStatus Conversions::ConvertMeas(AnalogOutputStatus^ meas)
{
	return opendnp3::AnalogOutputStatus(meas->Value, meas->Quality, TimeStamp::Convert(meas->Timestamp));
}

opendnp3::BinaryOutputStatus Conversions::ConvertMeas(BinaryOutputStatus^ meas)
{
	return opendnp3::BinaryOutputStatus(meas->Value, meas->Quality, TimeStamp::Convert(meas->Timestamp));
}

asiopal::SerialSettings Conversions::ConvertSerialSettings(SerialSettings^ settings)
{
	asiopal::SerialSettings s;
	s.deviceName = ConvertString(settings->port);
	s.baud = settings->baud;
	s.dataBits = settings->dataBits;
	s.stopBits = (asiopal::StopBits) settings->stopBits;
	s.parity = (asiopal::ParityType) settings->parity;
	s.flowType = (asiopal::FlowType) settings->flowControl;
	return s;
}

openpal::TimeDuration Conversions::ConvertMilliseconds(System::UInt64 ms)
{
	return openpal::TimeDuration::Milliseconds(ms);
}

openpal::TimeDuration Conversions::ConvertTimespan(System::TimeSpan ts)
{
	return ConvertMilliseconds(ts.Ticks / System::TimeSpan::TicksPerMillisecond);
}

opendnp3::LinkConfig Conversions::ConvertConfig(LinkConfig^ config)
{
	return opendnp3::LinkConfig(config->isMaster, config->useConfirms, config->numRetry, config->localAddr, config->remoteAddr, ConvertMilliseconds(config->timeoutMs));
}

opendnp3::ClassMask Conversions::ConvertClassMask(ClassMask^ cm)
{
	return opendnp3::ClassMask(cm->class1, cm->class2, cm->class3);
}

opendnp3::EventBufferConfig Conversions::ConvertConfig(EventBufferConfig^ cm)
{
	opendnp3::EventBufferConfig config;
	config.maxBinaryEvents = cm->maxBinaryEvents;
	config.maxDoubleBinaryEvents = cm->maxDoubleBinaryEvents;
	config.maxCounterEvents = cm->maxCounterEvents;
	config.maxFrozenCounterEvents = cm->maxFrozenCounterEvents;
	config.maxAnalogEvents = cm->maxAnalogEvents;
	config.maxBinaryOutputStatusEvents = cm->maxBinaryOutputStatusEvents;
	config.maxAnalogOutputStatusEvents = cm->maxAnalogOutputStatusEvents;
	return config;
}

opendnp3::StaticBinaryResponse Conversions::Convert(StaticBinaryResponse rsp)
{
	return (opendnp3::StaticBinaryResponse) rsp;
}

opendnp3::StaticAnalogResponse Conversions::Convert(StaticAnalogResponse rsp)
{
	return (opendnp3::StaticAnalogResponse) rsp;
}

opendnp3::StaticCounterResponse Conversions::Convert(StaticCounterResponse rsp)
{
	return (opendnp3::StaticCounterResponse) rsp;
}

opendnp3::StaticFrozenCounterResponse Conversions::Convert(StaticFrozenCounterResponse rsp)
{
	return (opendnp3::StaticFrozenCounterResponse) rsp;
}

opendnp3::StaticAnalogOutputStatusResponse Conversions::Convert(StaticAnalogOutputStatusResponse rsp)
{
	return (opendnp3::StaticAnalogOutputStatusResponse) rsp;
}

opendnp3::EventBinaryResponse Conversions::Convert(EventBinaryResponse rsp)
{
	return (opendnp3::EventBinaryResponse) rsp;
}

opendnp3::EventAnalogResponse Conversions::Convert(EventAnalogResponse rsp)
{
	return (opendnp3::EventAnalogResponse) rsp;
}

opendnp3::EventCounterResponse Conversions::Convert(EventCounterResponse rsp)
{
	return (opendnp3::EventCounterResponse) rsp;
}

opendnp3::OutstationConfig Conversions::ConvertConfig(OutstationConfig^ config)
{
	opendnp3::OutstationConfig op;

	/* TODO

	sc.disableUnsol = config->disableUnsol;
	sc.maxControls = config->maxControls;
	sc.unsolMask = ConvertClassMask(config->unsolMask);
	sc.allowTimeSync = config->allowTimeSync;
	sc.timeSyncPeriod = ConvertMilliseconds(config->timeSyncPeriodMs);
	sc.unsolPackDelay = ConvertMilliseconds(config->unsolPackDelayMs);
	sc.unsolRetryDelay = ConvertMilliseconds(config->unsolRetryDelayMs);
	sc.selectTimeout = ConvertMilliseconds(config->selectTimeoutMs);
	sc.maxFragSize = config->maxFragSize;
	sc.staticDefaults.binary = Convert(config->staticBinary);
	sc.staticDefaults.analog = Convert(config->staticAnalog);
	sc.staticDefaults.counter = Convert(config->staticCounter);
	sc.staticDefaults.frozenCounter = Convert(config->staticFrozenCounter);
	sc.staticDefaults.analogOutputStatus = Convert(config->staticAnalogOutputStatus);
	sc.eventDefaults.binary = Convert(config->eventBinary);
	sc.eventDefaults.analog = Convert(config->eventAnalog);
	sc.eventDefaults.counter = Convert(config->eventCounter);
	*/

	return op;
}

/*
opendnp3::EventPointRecord Conversions::ConvertRecord(EventPointRecord^ epr)
{
	return opendnp3::EventPointRecord(static_cast<opendnp3::PointClass>(epr->pointClass));
}

opendnp3::DeadbandPointRecord<double> Conversions::ConvertRecord(DeadbandEventPointRecord<double>^ epr)
{
	return opendnp3::DeadbandPointRecord<double>(static_cast<opendnp3::PointClass>(epr->pointClass), epr->deadband);
}

opendnp3::DeadbandPointRecord<uint32_t> Conversions::ConvertRecord(DeadbandEventPointRecord<System::UInt32>^ epr)
{
	return opendnp3::DeadbandPointRecord<uint32_t>(static_cast<opendnp3::PointClass>(epr->pointClass), epr->deadband);
}


opendnp3::DatabaseConfiguration Conversions::ConvertConfig(DatabaseTemplate^ config)
{
	opendnp3::DatabaseTemplate tmp(config->binaries->Count,
	                               config->doubleBinaries->Count,
	                               config->analogs->Count,
	                               config->counters->Count,
	                               config->frozenCounters->Count,
	                               config->binaryOutputStatii->Count,
	                               config->analogOutputStatii->Count);

	opendnp3::DatabaseConfiguration dev(tmp);

	// TODO - finish Converting data.

	for(int i = 0; i < config->binaries->Count; ++i) dev.binaryMetadata[i] = ConvertRecord(config->binaries[i]);
	for(int i = 0; i < config->analogs->Count; ++i) dev.analogMetadata[i] = ConvertRecord(config->analogs[i]);
	for(int i = 0; i < config->counters->Count; ++i) dev.counterMetadata[i] = ConvertRecord(config->counters[i]);
	for(int i = 0; i < config->counters->Count; ++i) dev.frozenCounterMetadata[i] = ConvertRecord(config->frozenCounters[i]);

	return dev;
}
*/

opendnp3::MasterParams Conversions::ConvertConfig(MasterConfig^ config)
{
	opendnp3::MasterParams mp;

	/* TODO
	mc.FragSize = config->fragSize;
	mc.AllowTimeSync = config->allowTimeSync;
	mc.DoUnsolOnStartup = config->doUnsolOnStartup;
	mc.EnableUnsol = config->enableUnsol;
	mc.UnsolClassMask = config->unsolClassMask;
	mc.IntegrityRate = ConvertMilliseconds(config->integrityPeriodMs);
	mc.TaskRetryRate = ConvertMilliseconds(config->taskRetryPeriodMs);
	*/

	return mp;
}

opendnp3::MasterStackConfig Conversions::ConvertConfig(MasterStackConfig^ config)
{
	opendnp3::MasterStackConfig cfg;
	cfg.master = ConvertConfig(config->master);	
	cfg.link = ConvertConfig(config->link);
	return cfg;
}

opendnp3::OutstationStackConfig Conversions::ConvertConfig(OutstationStackConfig^ config)
{
	//auto temp = ConvertConfig(config->device->);
	opendnp3::OutstationStackConfig cfg(opendnp3::DatabaseTemplate::AllTypes(10)); // todo make .NET outstation db configurable
	cfg.eventBuffer = ConvertConfig(config->buffer);
	cfg.outstation = ConvertConfig(config->outstation);	
	cfg.link = ConvertConfig(config->link);
	return cfg;
}
}
}
