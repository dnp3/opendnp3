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

System::Exception ^ Conversions::convertException(const openpal::Exception& ex)
{
	return gcnew System::Exception(convertString(ex.GetErrorString()));
}

std::string Conversions::convertString(System::String ^ s)
{
	return msclr::interop::marshal_as<std::string>(s);
}

System::String ^ Conversions::convertString(const std::string& s)
{
	return gcnew System::String(s.c_str());
}

openpal::LogLevel Conversions::convertLogLevel(DNP3::Interface::LogLevel level)
{
	return (openpal::LogLevel)  level;
}

DNP3::Interface::LogLevel Conversions::convertLogLevel(openpal::LogLevel level)
{
	return (DNP3::Interface::LogLevel) level;
}

ChannelState Conversions::convertChannelState(opendnp3::ChannelState aState)
{
	return (ChannelState) aState;
}


StackState  Conversions::convertStackState(opendnp3::StackState aState)
{
	return (StackState) aState;
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

ControlRelayOutputBlock ^ Conversions::convertCommand(const opendnp3::ControlRelayOutputBlock& bo)
{
	return gcnew ControlRelayOutputBlock(convertControlCode(bo.functionCode), bo.count, bo.onTimeMS, bo.offTimeMS);
}

opendnp3::ControlRelayOutputBlock Conversions::convertCommand(ControlRelayOutputBlock ^ bo)
{
	return opendnp3::ControlRelayOutputBlock(convertControlCode(bo->code), bo->count, bo->onTime, bo->offTime);
}

opendnp3::AnalogOutputInt32 Conversions::convertCommand(AnalogOutputInt32 ^ sp)
{
	return opendnp3::AnalogOutputInt32(sp->value);
}

AnalogOutputInt32 ^ Conversions::convertCommand(const opendnp3::AnalogOutputInt32& sp)
{
	return gcnew AnalogOutputInt32(sp.value);
}

opendnp3::AnalogOutputInt16 Conversions::convertCommand(AnalogOutputInt16 ^ sp)
{
	return opendnp3::AnalogOutputInt16(sp->value);
}

AnalogOutputInt16 ^ Conversions::convertCommand(const opendnp3::AnalogOutputInt16& sp)
{
	return gcnew AnalogOutputInt16(sp.value);
}

opendnp3::AnalogOutputFloat32 Conversions::convertCommand(AnalogOutputFloat32 ^ sp)
{
	return opendnp3::AnalogOutputFloat32(sp->value);
}

AnalogOutputFloat32 ^ Conversions::convertCommand(const opendnp3::AnalogOutputFloat32& sp)
{
	return gcnew AnalogOutputFloat32(sp.value);
}

opendnp3::AnalogOutputDouble64 Conversions::convertCommand(AnalogOutputDouble64 ^ sp)
{
	return opendnp3::AnalogOutputDouble64(sp->value);
}

AnalogOutputDouble64 ^ Conversions::convertCommand(const opendnp3::AnalogOutputDouble64& sp)
{
	return gcnew AnalogOutputDouble64(sp.value);
}

Binary ^ Conversions::convertMeas(opendnp3::Binary meas)
{
	return gcnew Binary(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
}

Analog ^ Conversions::convertMeas(opendnp3::Analog meas)
{
	return gcnew Analog(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
}

Counter ^ Conversions::convertMeas(opendnp3::Counter meas)
{
	return gcnew Counter(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
}

SetpointStatus ^ Conversions::convertMeas(opendnp3::SetpointStatus meas)
{
	return gcnew SetpointStatus(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
}

ControlStatus ^ Conversions::convertMeas(opendnp3::ControlStatus meas)
{
	return gcnew ControlStatus(meas.GetValue(), meas.GetQuality(), TimeStamp::Convert(meas.GetTime()));
}

OctetString^ Conversions::convertMeas(const opendnp3::OctetString& arMeas)
{
	auto buffer = arMeas.ToReadOnly();
	array<System::Byte>^ bytes = gcnew array<System::Byte>(buffer.Size());
	for(size_t i = 0; i< buffer.Size(); ++i) bytes[i] = buffer[i];
	return gcnew OctetString(bytes);	
}

opendnp3::Binary Conversions::convertMeas(Binary ^ meas)
{
	opendnp3::Binary m(meas->value, meas->quality);
	m.SetTime(TimeStamp::Convert(meas->time));
	return m;
}

opendnp3::Analog Conversions::convertMeas(Analog ^ meas)
{
	opendnp3::Analog m(meas->value, meas->quality);
	m.SetTime(TimeStamp::Convert(meas->time));
	return m;
}

opendnp3::Counter Conversions::convertMeas(Counter ^ meas)
{
	opendnp3::Counter m(meas->value, meas->quality);
	m.SetTime(TimeStamp::Convert(meas->time));
	return m;
}

opendnp3::SetpointStatus Conversions::convertMeas(SetpointStatus ^ meas)
{
	opendnp3::SetpointStatus m(meas->value, meas->quality);
	m.SetTime(TimeStamp::Convert(meas->time));
	return m;
}

opendnp3::ControlStatus Conversions::convertMeas(ControlStatus ^ meas)
{
	opendnp3::ControlStatus m(meas->value, meas->quality);
	m.SetTime(TimeStamp::Convert(meas->time));
	return m;
}

asiopal::SerialSettings Conversions::convertSerialSettings(SerialSettings ^ settings)
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

openpal::TimeDuration Conversions::convertTimeSpan(System::TimeSpan ts)
{
	return TimeDuration::Milliseconds(ts.Ticks/System::TimeSpan::TicksPerMillisecond);
}

opendnp3::LinkConfig Conversions::convertConfig(LinkConfig ^ config)
{
	return opendnp3::LinkConfig(config->isMaster, config->useConfirms, config->numRetry, config->localAddr, config->remoteAddr, convertTimeSpan(config->timeout));
}

opendnp3::AppConfig Conversions::convertConfig(AppConfig ^ config)
{
	return opendnp3::AppConfig(config->isMaster, convertTimeSpan(config->rspTimeout), config->numRetry, config->fragSize);
}

opendnp3::ClassMask Conversions::convertClassMask(ClassMask ^ cm)
{
	return opendnp3::ClassMask(cm->class1, cm->class2, cm->class3);
}

opendnp3::EventMaxConfig Conversions::convertEventMaxConfig(EventMaxConfig ^ cm)
{
	return opendnp3::EventMaxConfig(cm->maxBinaryEvents, cm->maxAnalogEvents, cm->maxCounterEvents, 0);
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

opendnp3::StaticSetpointStatusResponse Conversions::convert(StaticSetpointStatusResponse rsp)
{
	return (opendnp3::StaticSetpointStatusResponse) rsp;
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

opendnp3::SlaveConfig Conversions::convertConfig(SlaveConfig ^ config)
{
	opendnp3::SlaveConfig sc;

	sc.mDisableUnsol = config->disableUnsol;
	sc.mMaxControls = config->maxControls;
	sc.mUnsolMask = convertClassMask(config->unsolMask);
	sc.mAllowTimeSync = config->allowTimeSync;
	sc.mTimeSyncPeriod = convertTimeSpan(config->timeSyncPeriod);
	sc.mUnsolPackDelay = convertTimeSpan(config->unsolPackDelay);
	sc.mUnsolRetryDelay = convertTimeSpan(config->unsolRetryDelay);
	sc.mSelectTimeout = convertTimeSpan(config->selectTimeout);
	sc.mMaxFragSize = config->maxFragSize;
	sc.mEventMaxConfig = convertEventMaxConfig(config->eventMaxConfig);
	sc.mStaticBinary = convert(config->staticBinary);
	sc.mStaticAnalog = convert(config->staticAnalog);
	sc.mStaticCounter = convert(config->staticCounter);
	sc.mStaticSetpointStatus = convert(config->staticSetpointStatus);
	sc.mEventBinary = convert(config->eventBinary);
	sc.mEventAnalog = convert(config->eventAnalog);
	sc.mEventCounter = convert(config->eventCounter);

	return sc;
}

opendnp3::EventPointRecord Conversions::convertRecord(EventPointRecord ^ epr)
{
	return opendnp3::EventPointRecord(static_cast<opendnp3::PointClass>(epr->pointClass));
}

opendnp3::DeadbandPointRecord<double> Conversions::convertRecord(DeadbandEventPointRecord<double> ^ epr)
{
	return opendnp3::DeadbandPointRecord<double>(static_cast<opendnp3::PointClass>(epr->pointClass), epr->deadband);
}

opendnp3::DeadbandPointRecord<uint32_t> Conversions::convertRecord(DeadbandEventPointRecord<System::UInt32> ^ epr)
{
	return opendnp3::DeadbandPointRecord<uint32_t>(static_cast<opendnp3::PointClass>(epr->pointClass), epr->deadband);
}

opendnp3::DatabaseConfiguration Conversions::convertConfig(DeviceTemplate ^ config)
{
	opendnp3::DatabaseTemplate tmp(config->binaries->Count,
											config->analogs->Count,
											config->counters->Count,
											config->numControlStatii,
											config->numSetpointStatii);										

	opendnp3::DatabaseConfiguration dev(tmp);

	for(int i = 0; i < config->binaries->Count; ++i) dev.binaryMetadata[i] = convertRecord(config->binaries[i]);
	for(int i = 0; i < config->analogs->Count; ++i) dev.analogMetadata[i] = convertRecord(config->analogs[i]);
	for(int i = 0; i < config->counters->Count; ++i) dev.counterMetadata[i] = convertRecord(config->counters[i]);

	return dev;
}

opendnp3::MasterConfig Conversions::convertConfig(MasterConfig ^ config)
{
	opendnp3::MasterConfig mc;
	mc.FragSize = config->fragSize;
	mc.AllowTimeSync = config->allowTimeSync;
	mc.DoUnsolOnStartup = config->doUnsolOnStartup;
	mc.EnableUnsol = config->enableUnsol;
	mc.UnsolClassMask = config->unsolClassMask;
	mc.IntegrityRate = convertTimeSpan(config->integrityPeriod);
	mc.TaskRetryRate = convertTimeSpan(config->taskRetryPeriod);
	return mc;
}

opendnp3::MasterStackConfig Conversions::convertConfig(MasterStackConfig ^ config)
{
	opendnp3::MasterStackConfig cfg;
	cfg.master = convertConfig(config->master);
	cfg.app = convertConfig(config->app);
	cfg.link = convertConfig(config->link);
	return cfg;
}

opendnp3::SlaveStackConfig Conversions::convertConfig(SlaveStackConfig ^ config)
{
	auto temp = convertConfig(config->device);
	opendnp3::SlaveStackConfig cfg(temp);
	cfg.slave = convertConfig(config->slave);	
	cfg.app = convertConfig(config->app);
	cfg.link = convertConfig(config->link);
	return cfg;
}
}
}
