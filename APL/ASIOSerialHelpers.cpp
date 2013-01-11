
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// 
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "ASIOSerialHelpers.h"

#include <sstream>

#include "Exception.h"
#include "Location.h"

#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::system;

namespace apl
{
namespace asio_serial
{

template<class T>
void SetOption(boost::asio::serial_port& arPort, const T& arOption)
{
	error_code ec;
	arPort.set_option(arOption, ec);
	if(ec) throw Exception(LOCATION, ec.message());
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
		throw apl::Exception(LOCATION, "Unsupported Stop Bits");
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
		throw apl::Exception(LOCATION, "Unsupported Flow Control");
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
		throw apl::Exception(LOCATION, "Unsupported Parity");
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
}
