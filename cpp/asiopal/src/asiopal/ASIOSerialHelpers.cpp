/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#include "ASIOSerialHelpers.h"

#include <sstream>

#include <openpal/Location.h>
#include <openpal/Logger.h>
#include <openpal/LogMacros.h>

#include <asio.hpp>

using namespace asio;

namespace asiopal
{

//////////////////////////////////////////////
//	Private Helpers
//////////////////////////////////////////////
serial_port_base::stop_bits ConvertStopBits(int aStopBits)
{
	serial_port_base::stop_bits::type t = serial_port_base::stop_bits::one;

	switch(aStopBits)
	{
	case(1):
		t = serial_port_base::stop_bits::one;
		break;
	case(2):
		t = serial_port_base::stop_bits::two;
		break;
	default:
		break;
	}

	return serial_port_base::stop_bits(t);
}

serial_port_base::flow_control ConvertFlow(FlowType aFlowType)
{
	serial_port_base::flow_control::type t = serial_port_base::flow_control::none;

	switch(aFlowType)
	{
	case(FLOW_NONE):
		t = serial_port_base::flow_control::none;
		break;
	case(FLOW_XONXOFF):
		t = serial_port_base::flow_control::software;
		break;
	case(FLOW_HARDWARE):
		t = serial_port_base::flow_control::hardware;
		break;
	default:
		break;
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
	serial_port_base::parity::type t = serial_port_base::parity::none;

	switch(aParity)
	{
	case(PAR_EVEN):
		t = serial_port_base::parity::even;
		break;
	case(PAR_ODD):
		t = serial_port_base::parity::odd;
		break;
	default:
		break;
	}

	return serial_port_base::parity(t);
}

void Configure(SerialSettings& arSettings, asio::serial_port& arPort, error_code& ec)
{
	//Set all the various options
	arPort.set_option(ConvertBaud(arSettings.mBaud), ec);
	if (!ec)
	{
		if (!ec)
		{
			arPort.set_option(ConvertDataBits(arSettings.mDataBits), ec);
			if (!ec)
			{
				arPort.set_option(ConvertParity(arSettings.mParity), ec);
				if (!ec)
				{
					arPort.set_option(ConvertStopBits(arSettings.mStopBits), ec);
					if (!ec)
					{
						arPort.set_option(ConvertFlow(arSettings.mFlowType), ec);
					}
				}
			}
		}
	}
}

}

