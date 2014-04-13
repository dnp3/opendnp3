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

std::string Conversions::convertString(System::String^ s)
{
	return msclr::interop::marshal_as<std::string>(s);
}

System::String^ Conversions::convertString(const std::string& s)
{
	return gcnew System::String(s.c_str());
}

ChannelState Conversions::convertChannelState(opendnp3::ChannelState aState)
{
	return (ChannelState) aState;
}

CommandResponse Conversions::convertCommandResponse(opendnp3::CommandResponse response)
{
	auto result = convertCommandResult(response.GetResult());
	auto status = convertCommandStatus(response.GetStatus());
	return CommandResponse(result, status);
}

CommandResult Conversions::convertCommandResult(opendnp3::CommandResult result)
{
	return (CommandResult) result;
}

CommandStatus Conversions::convertCommandStatus(opendnp3::CommandStatus status)
{
	return (CommandStatus) status;
}

opendnp3::CommandStatus Conversions::convertCommandStatus(CommandStatus status)
{
	return (opendnp3::CommandStatus) status;
}

ControlCode Conversions::convertControlCode(opendnp3::ControlCode code)
{
	return (ControlCode) ControlCodeToType(code);
}

opendnp3::ControlCode Conversions::convertControlCode(ControlCode code)
{
	return (opendnp3::ControlCode) code;
}

ControlRelayOutputBlock^ Conversions::convertCommand(const opendnp3::ControlRelayOutputBlock& bo)
{
	return gcnew ControlRelayOutputBlock(convertControlCode(bo.functionCode), bo.count, bo.onTimeMS, bo.offTimeMS);
}

opendnp3::ControlRelayOutputBlock Conversions::convertCommand(ControlRelayOutputBlock^ bo)
{
	return opendnp3::ControlRelayOutputBlock(convertControlCode(bo->code), bo->count, bo->onTime, bo->offTime);
}

opendnp3::AnalogOutputInt32 Conversions::convertCommand(AnalogOutputInt32^ sp)
{
	return opendnp3::AnalogOutputInt32(sp->value);
}

AnalogOutputInt32^ Conversions::convertCommand(const opendnp3::AnalogOutputInt32& sp)
{
	return gcnew AnalogOutputInt32(sp.value);
}

opendnp3::AnalogOutputInt16 Conversions::convertCommand(AnalogOutputInt16^ sp)
{
	return opendnp3::AnalogOutputInt16(sp->value);
}

AnalogOutputInt16^ Conversions::convertCommand(const opendnp3::AnalogOutputInt16& sp)
{
	return gcnew AnalogOutputInt16(sp.value);
}

opendnp3::AnalogOutputFloat32 Conversions::convertCommand(AnalogOutputFloat32^ sp)
{
	return opendnp3::AnalogOutputFloat32(sp->value);
}

AnalogOutputFloat32^ Conversions::convertCommand(const opendnp3::AnalogOutputFloat32& sp)
{
	return gcnew AnalogOutputFloat32(sp.value);
}

opendnp3::AnalogOutputDouble64 Conversions::convertCommand(AnalogOutputDouble64^ sp)
{
	return opendnp3::AnalogOutputDouble64(sp->value);
}

AnalogOutputDouble64^ Conversions::convertCommand(const opendnp3::AnalogOutputDouble64& sp)
{
	return gcnew AnalogOutputDouble64(sp.value);
}

Binary^ Conversions::convertMeas(opendnp3::Binary meas)
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

DoubleBitBinary^ Conversions::convertMeas(opendnp3::DoubleBitBinary meas)
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

Analog^ Conversions::convertMeas(opendnp3::Analog meas)
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

Counter^ Conversions::convertMeas(opendnp3::Counter meas)
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

FrozenCounter^ Conversions::convertMeas(opendnp3::FrozenCounter meas)
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

AnalogOutputStatus^ Conversions::convertMeas(opendnp3::AnalogOutputStatus meas)
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

BinaryOutputStatus^ Conversions::convertMeas(opendnp3::BinaryOutputStatus meas)
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

OctetString^ Conversions::convertMeas(const opendnp3::OctetString& arMeas)
{
	auto buffer = arMeas.ToReadOnly();
	array<System::Byte>^ bytes = gcnew array<System::Byte>(buffer.Size());
	for (uint32_t i = 0; i < buffer.Size(); ++i) bytes[i] = buffer[i];
	return gcnew OctetString(bytes);
}

opendnp3::Binary Conversions::convertMeas(Binary^ meas)
{
	return opendnp3::Binary(meas->Value, meas->Quality, TimeStamp::Convert(meas->Timestamp));
}

opendnp3::Analog Conversions::convertMeas(Analog^ meas)
{
	return opendnp3::Analog(meas->Value, meas->Quality, TimeStamp::Convert(meas->Timestamp));
}

opendnp3::Counter Conversions::convertMeas(Counter^ meas)
{
	return opendnp3::Counter(meas->Value, meas->Quality, TimeStamp::Convert(meas->Timestamp));
}

opendnp3::FrozenCounter Conversions::convertMeas(FrozenCounter^ meas)
{
	return opendnp3::FrozenCounter(meas->Value, meas->Quality, TimeStamp::Convert(meas->Timestamp));
}

opendnp3::AnalogOutputStatus Conversions::convertMeas(AnalogOutputStatus^ meas)
{
	return opendnp3::AnalogOutputStatus(meas->Value, meas->Quality, TimeStamp::Convert(meas->Timestamp));
}

opendnp3::BinaryOutputStatus Conversions::convertMeas(BinaryOutputStatus^ meas)
{
	return opendnp3::BinaryOutputStatus(meas->Value, meas->Quality, TimeStamp::Convert(meas->Timestamp));
}

asiopal::SerialSettings Conversions::convertSerialSettings(SerialSettings^ settings)
{
	asiopal::SerialSettings s;
	s.mDevice = convertString(settings->port);
	s.mBaud = settings->baud;
	s.mDataBits = settings->dataBits;
	s.mStopBits = settings->stopBits;
	s.mParity = (asiopal::ParityType) settings->parity;
	s.mFlowType = (asiopal::FlowType) settings->flowControl;
	return s;
}

openpal::TimeDuration Conversions::convertMilliseconds(System::UInt64 ms)
{
	return TimeDuration::Milliseconds(ms);
}

openpal::TimeDuration Conversions::convertTimespan(System::TimeSpan ts)
{
	return convertMilliseconds(ts.Ticks / System::TimeSpan::TicksPerMillisecond);
}

opendnp3::LinkConfig Conversions::convertConfig(LinkConfig^ config)
{
	return opendnp3::LinkConfig(config->isMaster, config->useConfirms, config->numRetry, config->localAddr, config->remoteAddr, convertMilliseconds(config->timeoutMs));
}

opendnp3::AppConfig Conversions::convertConfig(AppConfig^ config)
{
	return opendnp3::AppConfig(config->isMaster, convertMilliseconds(config->rspTimeoutMs), config->numRetry, config->fragSize);
}

opendnp3::ClassMask Conversions::convertClassMask(ClassMask^ cm)
{
	return opendnp3::ClassMask(cm->class1, cm->class2, cm->class3);
}

opendnp3::EventBufferConfig Conversions::convertConfig(EventBufferConfig^ cm)
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

opendnp3::StaticBinaryResponse Conversions::convert(StaticBinaryResponse rsp)
{
	return (opendnp3::StaticBinaryResponse) rsp;
}

opendnp3::StaticAnalogResponse Conversions::convert(StaticAnalogResponse rsp)
{
	return (opendnp3::StaticAnalogResponse) rsp;
}

opendnp3::StaticCounterResponse Conversions::convert(StaticCounterResponse rsp)
{
	return (opendnp3::StaticCounterResponse) rsp;
}

opendnp3::StaticFrozenCounterResponse Conversions::convert(StaticFrozenCounterResponse rsp)
{
	return (opendnp3::StaticFrozenCounterResponse) rsp;
}

opendnp3::StaticAnalogOutputStatusResponse Conversions::convert(StaticAnalogOutputStatusResponse rsp)
{
	return (opendnp3::StaticAnalogOutputStatusResponse) rsp;
}

opendnp3::EventBinaryResponse Conversions::convert(EventBinaryResponse rsp)
{
	return (opendnp3::EventBinaryResponse) rsp;
}

opendnp3::EventAnalogResponse Conversions::convert(EventAnalogResponse rsp)
{
	return (opendnp3::EventAnalogResponse) rsp;
}

opendnp3::EventCounterResponse Conversions::convert(EventCounterResponse rsp)
{
	return (opendnp3::EventCounterResponse) rsp;
}

opendnp3::OutstationConfig Conversions::convertConfig(OutstationConfig^ config)
{
	opendnp3::OutstationConfig sc;

	sc.disableUnsol = config->disableUnsol;
	sc.maxControls = config->maxControls;
	sc.unsolMask = convertClassMask(config->unsolMask);
	sc.allowTimeSync = config->allowTimeSync;
	sc.timeSyncPeriod = convertMilliseconds(config->timeSyncPeriodMs);
	sc.unsolPackDelay = convertMilliseconds(config->unsolPackDelayMs);
	sc.unsolRetryDelay = convertMilliseconds(config->unsolRetryDelayMs);
	sc.selectTimeout = convertMilliseconds(config->selectTimeoutMs);
	sc.maxFragSize = config->maxFragSize;
	sc.staticDefaults.binary = convert(config->staticBinary);
	sc.staticDefaults.analog = convert(config->staticAnalog);
	sc.staticDefaults.counter = convert(config->staticCounter);
	sc.staticDefaults.frozenCounter = convert(config->staticFrozenCounter);
	sc.staticDefaults.analogOutputStatus = convert(config->staticAnalogOutputStatus);
	sc.eventBinary = convert(config->eventBinary);
	sc.eventAnalog = convert(config->eventAnalog);
	sc.eventCounter = convert(config->eventCounter);

	return sc;
}

/*
opendnp3::EventPointRecord Conversions::convertRecord(EventPointRecord^ epr)
{
	return opendnp3::EventPointRecord(static_cast<opendnp3::PointClass>(epr->pointClass));
}

opendnp3::DeadbandPointRecord<double> Conversions::convertRecord(DeadbandEventPointRecord<double>^ epr)
{
	return opendnp3::DeadbandPointRecord<double>(static_cast<opendnp3::PointClass>(epr->pointClass), epr->deadband);
}

opendnp3::DeadbandPointRecord<uint32_t> Conversions::convertRecord(DeadbandEventPointRecord<System::UInt32>^ epr)
{
	return opendnp3::DeadbandPointRecord<uint32_t>(static_cast<opendnp3::PointClass>(epr->pointClass), epr->deadband);
}


opendnp3::DatabaseConfiguration Conversions::convertConfig(DatabaseTemplate^ config)
{
	opendnp3::DatabaseTemplate tmp(config->binaries->Count,
	                               config->doubleBinaries->Count,
	                               config->analogs->Count,
	                               config->counters->Count,
	                               config->frozenCounters->Count,
	                               config->binaryOutputStatii->Count,
	                               config->analogOutputStatii->Count);

	opendnp3::DatabaseConfiguration dev(tmp);

	// TODO - finish converting data.

	for(int i = 0; i < config->binaries->Count; ++i) dev.binaryMetadata[i] = convertRecord(config->binaries[i]);
	for(int i = 0; i < config->analogs->Count; ++i) dev.analogMetadata[i] = convertRecord(config->analogs[i]);
	for(int i = 0; i < config->counters->Count; ++i) dev.counterMetadata[i] = convertRecord(config->counters[i]);
	for(int i = 0; i < config->counters->Count; ++i) dev.frozenCounterMetadata[i] = convertRecord(config->frozenCounters[i]);

	return dev;
}
*/

opendnp3::MasterConfig Conversions::convertConfig(MasterConfig^ config)
{
	opendnp3::MasterConfig mc;
	mc.FragSize = config->fragSize;
	mc.AllowTimeSync = config->allowTimeSync;
	mc.DoUnsolOnStartup = config->doUnsolOnStartup;
	mc.EnableUnsol = config->enableUnsol;
	mc.UnsolClassMask = config->unsolClassMask;
	mc.IntegrityRate = convertMilliseconds(config->integrityPeriodMs);
	mc.TaskRetryRate = convertMilliseconds(config->taskRetryPeriodMs);
	return mc;
}

opendnp3::MasterStackConfig Conversions::convertConfig(MasterStackConfig^ config)
{
	opendnp3::MasterStackConfig cfg;
	cfg.master = convertConfig(config->master);
	cfg.app = convertConfig(config->app);
	cfg.link = convertConfig(config->link);
	return cfg;
}

opendnp3::OutstationStackConfig Conversions::convertConfig(OutstationStackConfig^ config)
{
	//auto temp = convertConfig(config->device->);
	opendnp3::OutstationStackConfig cfg(opendnp3::DatabaseTemplate::AllTypes(10)); // todo make .NET outstation db configurable
	cfg.eventBuffer = convertConfig(config->buffer);
	cfg.outstation = convertConfig(config->outstation);
	cfg.app = convertConfig(config->app);
	cfg.link = convertConfig(config->link);
	return cfg;
}
}
}
