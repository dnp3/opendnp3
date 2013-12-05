#include <asiopal/ASIOSerialHelpers.h>

#include <sstream>

#include <openpal/Exception.h>
#include <openpal/Location.h>

#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::system;
using namespace openpal;

namespace asiopal
{

template<class T>
void SetOption(boost::asio::serial_port& arPort, const T& arOption)
{
	error_code ec;
	arPort.set_option(arOption, ec);
	if(ec) MACRO_THROW_EXCEPTION(Exception, ec.message());
}

//////////////////////////////////////////////
//	Private Helpers
//////////////////////////////////////////////
serial_port_base::stop_bits ConvertStopBits(int aStopBits)
{
	serial_port_base::stop_bits::type t;

	switch(aStopBits) {
	case(1): t = serial_port_base::stop_bits::one; break;
	case(2): t = serial_port_base::stop_bits::two; break;
	default:
		MACRO_THROW_EXCEPTION(openpal::Exception, "Unsupported Stop Bits");
	}

	return serial_port_base::stop_bits(t);
}

serial_port_base::flow_control ConvertFlow(FlowType aFlowType)
{
	serial_port_base::flow_control::type t;

	switch(aFlowType) {
	case(FLOW_NONE): t = serial_port_base::flow_control::none; break;
	case(FLOW_XONXOFF): t = serial_port_base::flow_control::software; break;
	case(FLOW_HARDWARE): t = serial_port_base::flow_control::hardware; break;
	default:
		MACRO_THROW_EXCEPTION(openpal::Exception, "Unsupported Flow Control");
	}

	return serial_port_base::flow_control(t);
}

serial_port_base::character_size ConvertDataBits(int aDataBits)
{
	return serial_port_base::character_size(static_cast<unsigned int>(aDataBits));
}

serial_port_base::baud_rate ConvertBaud(int aBaud)
{
	return serial_port_base::baud_rate(static_cast<unsigned int>(aBaud));
}

serial_port_base::parity ConvertParity(ParityType aParity)
{
	serial_port_base::parity::type t;

	switch(aParity) {
	case(PAR_NONE): t = serial_port_base::parity::none; break;
	case(PAR_EVEN): t = serial_port_base::parity::even; break;
	case(PAR_ODD): t = serial_port_base::parity::odd; break;

	default:
		MACRO_THROW_EXCEPTION(openpal::Exception, "Unsupported Parity");
	}

	return serial_port_base::parity(t);
}

void Configure(SerialSettings& arSettings, boost::asio::serial_port& arPort, error_code& ec)
{
	//Set all the various options
	arPort.set_option(ConvertBaud(arSettings.mBaud), ec); if(ec) return;
	arPort.set_option(ConvertDataBits(arSettings.mDataBits), ec); if(ec) return;
	arPort.set_option(ConvertParity(arSettings.mParity), ec); if(ec) return;
	arPort.set_option(ConvertStopBits(arSettings.mStopBits), ec); if(ec) return;
	arPort.set_option(ConvertFlow(arSettings.mFlowType), ec); if(ec) return;
}

void Configure(SerialSettings& arSettings, boost::asio::serial_port& arPort)
{
	//Set all the various options
	SetOption(arPort, ConvertBaud(arSettings.mBaud));
	SetOption(arPort, ConvertDataBits(arSettings.mDataBits));
	SetOption(arPort, ConvertParity(arSettings.mParity));
	SetOption(arPort, ConvertStopBits(arSettings.mStopBits));
	SetOption(arPort, ConvertFlow(arSettings.mFlowType)); // Hardwired to NONE currently
}

}

