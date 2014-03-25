#ifndef __CONVERSIONS_H_
#define __CONVERSIONS_H_

#include <string>

#include <opendnp3/LogLevels.h>

#include <opendnp3/gen/ChannelState.h>

#include <opendnp3/app/MeasurementTypes.h>
#include <opendnp3/app/ControlRelayOutputBlock.h>
#include <opendnp3/app/AnalogOutput.h>
#include <opendnp3/app/OctetString.h>

#include <opendnp3/master/CommandResponse.h>
#include <opendnp3/master/MasterStackConfig.h>

#include <opendnp3/outstation/SlaveStackConfig.h>



#include <asiopal/SerialTypes.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{

private class Conversions
{
public:

	static openpal::TimeDuration convertTimeSpan(System::TimeSpan ts);

	// Convert a .NET string to a C++ string
	static std::string convertString(System::String^ s);
	static System::String^ convertString(const std::string& s);

	// Convert the log filter enumeration
	//static uint32_t convertLogLevel(DNP3::Interface::LogLevel level);
	//static DNP3::Interface::LogLevel convertLogLevel(uint32_t flags);

	// Converting channel state enumeration
	static ChannelState convertChannelState(opendnp3::ChannelState aState);

	// Convert the command status enumeration
	static CommandStatus convertCommandStatus(opendnp3::CommandStatus status);
	static opendnp3::CommandStatus convertCommandStatus(CommandStatus status);

	static CommandResult convertCommandResult(opendnp3::CommandResult result);

	static CommandResponse convertCommandResponse(opendnp3::CommandResponse response);

	//functions for converting binary outputs

	static ControlCode convertControlCode(opendnp3::ControlCode code);
	static opendnp3::ControlCode Conversions::convertControlCode(ControlCode code);


	static ControlRelayOutputBlock^ convertCommand(const opendnp3::ControlRelayOutputBlock& bo);
	static opendnp3::ControlRelayOutputBlock convertCommand(ControlRelayOutputBlock^ bo);

	//functions for converting setpoints

	static opendnp3::AnalogOutputInt32 convertCommand(AnalogOutputInt32^ sp);
	static AnalogOutputInt32^ convertCommand(const opendnp3::AnalogOutputInt32& sp);

	static opendnp3::AnalogOutputInt16 convertCommand(AnalogOutputInt16^ sp);
	static AnalogOutputInt16^ convertCommand(const opendnp3::AnalogOutputInt16& sp);

	static opendnp3::AnalogOutputFloat32 convertCommand(AnalogOutputFloat32^ sp);
	static AnalogOutputFloat32^ convertCommand(const opendnp3::AnalogOutputFloat32& sp);

	static opendnp3::AnalogOutputDouble64 convertCommand(AnalogOutputDouble64^ sp);
	static AnalogOutputDouble64^ convertCommand(const opendnp3::AnalogOutputDouble64& sp);

	//functions for converting Measurement types
	static Binary^ convertMeas(opendnp3::Binary meas);
	static DoubleBitBinary^ convertMeas(opendnp3::DoubleBitBinary meas);
	static Analog^ convertMeas(opendnp3::Analog meas);
	static Counter^ convertMeas(opendnp3::Counter meas);
	static FrozenCounter^ convertMeas(opendnp3::FrozenCounter meas);
	static AnalogOutputStatus^ convertMeas(opendnp3::AnalogOutputStatus meas);
	static BinaryOutputStatus^ convertMeas(opendnp3::BinaryOutputStatus meas);
	static OctetString^ Conversions::convertMeas(const opendnp3::OctetString& arMeas);


	static opendnp3::Binary convertMeas(Binary^ meas);
	static opendnp3::Analog convertMeas(Analog^ meas);
	static opendnp3::Counter convertMeas(Counter^ meas);
	static opendnp3::FrozenCounter convertMeas(FrozenCounter^ meas);
	static opendnp3::AnalogOutputStatus convertMeas(AnalogOutputStatus^ meas);
	static opendnp3::BinaryOutputStatus convertMeas(BinaryOutputStatus^ meas);

	//Convert the configuration types
	static asiopal::SerialSettings convertSerialSettings(SerialSettings^ settings);
	static opendnp3::ClassMask convertClassMask(ClassMask^ cm);
	static opendnp3::EventMaxConfig convertEventMaxConfig(EventMaxConfig^ cm);

	static opendnp3::StaticBinaryResponse convert(StaticBinaryResponse rsp);
	static opendnp3::StaticAnalogResponse convert(StaticAnalogResponse rsp);
	static opendnp3::StaticCounterResponse convert(StaticCounterResponse rsp);
	static opendnp3::StaticFrozenCounterResponse convert(StaticFrozenCounterResponse rsp);
	static opendnp3::StaticAnalogOutputStatusResponse convert(StaticAnalogOutputStatusResponse rsp);

	static opendnp3::EventBinaryResponse convert(EventBinaryResponse rsp);
	static opendnp3::EventAnalogResponse convert(EventAnalogResponse rsp);
	static opendnp3::EventCounterResponse convert(EventCounterResponse rsp);

	static opendnp3::EventPointRecord convertRecord(EventPointRecord^ epr);
	static opendnp3::DeadbandPointRecord<double> convertRecord(DeadbandEventPointRecord<double>^ epr);
	static opendnp3::DeadbandPointRecord<uint32_t> convertRecord(DeadbandEventPointRecord<System::UInt32>^ epr);

	static opendnp3::LinkConfig convertConfig(LinkConfig^ config);
	static opendnp3::AppConfig convertConfig(AppConfig^ config);
	static opendnp3::MasterConfig convertConfig(MasterConfig^ config);
	static opendnp3::SlaveConfig convertConfig(SlaveConfig^ config);
	static opendnp3::DatabaseConfiguration convertConfig(DeviceTemplate^ config);
	static opendnp3::MasterStackConfig convertConfig(MasterStackConfig^ config);
	static opendnp3::SlaveStackConfig convertConfig(SlaveStackConfig^ config);
};

}
}

#endif
