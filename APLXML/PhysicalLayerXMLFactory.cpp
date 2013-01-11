
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
#include "PhysicalLayerXMLFactory.h"

#include <APL/PhysicalLayerFactory.h>
#include <boost/numeric/conversion/converter.hpp>

namespace apl
{
namespace xml
{

IPhysicalLayerAsyncFactory PhysicalLayerXMLFactory :: GetFactoryAsync(const APLXML_Base::PhysicalLayerDescriptor_t* apCfg)
{
	const APLXML_Base::Serial_t* pSerial = dynamic_cast<const APLXML_Base::Serial_t*>(apCfg);
	if(pSerial != NULL) return GetAsync(pSerial);

	const APLXML_Base::TCPServer_t* pServer = dynamic_cast<const APLXML_Base::TCPServer_t*>(apCfg);
	if(pServer != NULL) return GetAsync(pServer);

	const APLXML_Base::TCPClient_t* pClient = dynamic_cast<const APLXML_Base::TCPClient_t*>(apCfg);
	if(pClient != NULL) return GetAsync(pClient);

	throw Exception(LOCATION, "Unknown PhysicalLayerDescriptor_t");
}

IPhysicalLayerAsyncFactory PhysicalLayerXMLFactory :: GetAsync(const APLXML_Base::Serial_t* apCfg)
{
	SerialSettings s = GetSerialSettings(apCfg);
	return PhysicalLayerFactory::GetSerialAsync(s);
}

IPhysicalLayerAsyncFactory PhysicalLayerXMLFactory :: GetAsync(const APLXML_Base::TCPClient_t* apCfg)
{
	uint16_t port = boost::numeric::converter<uint16_t, int>::convert(apCfg->Port);
	return PhysicalLayerFactory::GetTCPClientAsync(apCfg->Address, port);
}

IPhysicalLayerAsyncFactory PhysicalLayerXMLFactory :: GetAsync(const APLXML_Base::TCPServer_t* apCfg)
{
	uint16_t port = boost::numeric::converter<uint16_t, int>::convert(apCfg->Port);
	return PhysicalLayerFactory::GetTCPServerAsync(apCfg->Endpoint, port);
}

SerialSettings GetSerialSettings(const APLXML_Base::Serial_t* apCfg)
{
	SerialSettings s;
	s.mBaud = BaudToInt(apCfg->BaudRate);
	s.mDataBits = ((apCfg->DBits == APLXML_Base::DATABITS_7) ? 7 : 8);
	s.mDevice = apCfg->Device;
	s.mFlowType = EnumToFlow(apCfg->FlowControl);
	s.mParity = EnumToParity(apCfg->Parity);
	s.mStopBits = ((apCfg->StopBits == APLXML_Base::STOPBITS_0) ? 0 : 1);
	return s;
}

int BaudToInt(APLXML_Base::BaudRateEnum aBaud)
{
	switch(aBaud) {
	case(APLXML_Base::BAUDRATE_1200): return 1200;
	case(APLXML_Base::BAUDRATE_1800): return 1800;
	case(APLXML_Base::BAUDRATE_2400): return 2400;
	case(APLXML_Base::BAUDRATE_4800): return 4800;
	case(APLXML_Base::BAUDRATE_9600): return 9600;
	case(APLXML_Base::BAUDRATE_19200): return 19200;
	case(APLXML_Base::BAUDRATE_38400): return 38400;
	case(APLXML_Base::BAUDRATE_57600): return 57600;
	case(APLXML_Base::BAUDRATE_115200): return 115200;
	case(APLXML_Base::BAUDRATE_230400): return 230400;
	}
	assert(false);
	return -1;
}

ParityType EnumToParity(APLXML_Base::ParityEnum aParity)
{
	switch(aParity) {
	case (APLXML_Base::PARITY_NONE): return PAR_NONE;
	case (APLXML_Base::PARITY_ODD): return PAR_ODD;
	case (APLXML_Base::PARITY_EVEN): return PAR_EVEN;
	}
	assert(false);
	return PAR_NONE;
}

FlowType EnumToFlow(APLXML_Base::FlowControlEnum aFlow)
{
	switch(aFlow) {
	case (APLXML_Base::FLOW_NONE): return FLOW_NONE;
	case (APLXML_Base::FLOW_HARDWARE): return FLOW_HARDWARE;
	case (APLXML_Base::FLOW_XONXOFF): return FLOW_XONXOFF;
	}
	assert(false);
	return FLOW_NONE;
}

}
}
