#ifndef __CONVERSIONS_H_
#define __CONVERSIONS_H_

#include <string>

#include <APL/CommandTypes.h>
#include <APL/LogTypes.h>
#include <APL/DataTypes.h>
#include <APL/SerialTypes.h>

#include <DNP3/SlaveStackConfig.h>
#include <DNP3/MasterStackConfig.h>

using namespace DNP3::Interface;

namespace DNP3
{	
namespace Adapter
{

	class Conversions
	{
		public:

		static System::Exception^ convertException(const apl::Exception& ex);		
		
		// Convert a .NET string to a C++ string
		static std::string convertString(System::String^ s);
		static System::String^ convertString(const std::string& s);

		// Convert the log filter enumeration
		static apl::FilterLevel convertFilterLevel(FilterLevel level);
		static FilterLevel convertFilterLevel(apl::FilterLevel level);

		// Convert the command status enumeration
		static CommandStatus convertCommandStatus(apl::CommandStatus status);
		static apl::CommandStatus convertCommandStatus(CommandStatus status);

		//functions for converting binary outputs		

		static ControlCode convertControlCode(apl::ControlCode code);
		static apl::ControlCode Conversions::convertControlCode(ControlCode code);
		static BinaryOutput^ convertBO(const apl::BinaryOutput& bo);
		static apl::BinaryOutput convertBO(BinaryOutput^ bo);

		//functions for converting setpoints

		static SetpointEncodingType convertSetpointEncoding(apl::SetpointEncodingType encoding);
		static apl::SetpointEncodingType convertSetpointEncoding(SetpointEncodingType encoding);
		static apl::Setpoint convertSP(Setpoint^ sp);
		static Setpoint^ convertSP(const apl::Setpoint& sp);

		//functions for converting Measurement types
		static Binary^ convertMeas(apl::Binary meas);
		static Analog^ convertMeas(apl::Analog meas);
		static Counter^ convertMeas(apl::Counter meas);
		static SetpointStatus^ convertMeas(apl::SetpointStatus meas);
		static ControlStatus^ convertMeas(apl::ControlStatus meas);

		static apl::Binary convertMeas(Binary^ meas);
		static apl::Analog convertMeas(Analog^ meas);
		static apl::Counter convertMeas(Counter^ meas);
		static apl::SetpointStatus convertMeas(SetpointStatus^ meas);
		static apl::ControlStatus convertMeas(ControlStatus^ meas);

		//Convert the configuration types
		static apl::SerialSettings convertSerialSettings(SerialSettings^ settings);
		static apl::dnp::ClassMask convertClassMask(ClassMask^ cm);
		static apl::dnp::EventMaxConfig convertEventMaxConfig(EventMaxConfig^ cm);
		static apl::dnp::GrpVar convertGrpVar(GrpVar^ gv);

		static apl::dnp::PointRecord convertRecord(PointRecord^ epr);
		static apl::dnp::ControlRecord convertRecord(ControlRecord^ epr);
		static apl::dnp::EventPointRecord convertRecord(EventPointRecord^ epr);
		static apl::dnp::DeadbandPointRecord convertRecord(DeadbandEventPointRecord^ epr);

		static apl::dnp::LinkConfig convertConfig(LinkConfig^ config);
		static apl::dnp::AppConfig convertConfig(AppConfig^ config);
		static apl::dnp::MasterConfig convertConfig(MasterConfig^ config);
		static apl::dnp::SlaveConfig convertConfig(SlaveConfig^ config);
		static apl::dnp::DeviceTemplate convertConfig(DeviceTemplate^ config);
		static apl::dnp::MasterStackConfig convertConfig(MasterStackConfig^ config);
		static apl::dnp::SlaveStackConfig convertConfig(SlaveStackConfig^ config);
	};

}}

#endif
