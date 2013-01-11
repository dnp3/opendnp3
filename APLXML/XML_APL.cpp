//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include "XML_APL.h"

namespace apl
{
namespace xml
{

APLXML_Base::TCPServer_t* XML_APL::AddTCPServer(APLXML_Base::PhysicalLayerList_t& arList, const std::string& arDevice, const std::string& arEndpoint, int aPort)
{
	APLXML_Base::TCPServer_t* pLayer = new APLXML_Base::TCPServer_t();
	pLayer->Name = arDevice;
	pLayer->Endpoint = arEndpoint;
	pLayer->Port = aPort;
	pLayer->OpenRetryMS = 5000;
	arList.TCPServerVector.push_back(pLayer);
	return pLayer;
}

APLXML_Base::TCPClient_t* XML_APL::AddTCPClient(APLXML_Base::PhysicalLayerList_t& arList, const std::string& arDevice, const std::string& arAddress, int aPort)
{
	APLXML_Base::TCPClient_t* pLayer = new APLXML_Base::TCPClient_t();
	pLayer->Name = arDevice;
	pLayer->Address = arAddress;
	pLayer->Port = aPort;
	pLayer->OpenRetryMS = 5000;
	arList.TCPClientVector.push_back(pLayer);
	return pLayer;
}

APLXML_Base::Serial_t* XML_APL::AddSerial(APLXML_Base::PhysicalLayerList_t& arList, const std::string& arName, const std::string& arDevice)
{
	APLXML_Base::Serial_t* pLayer = new APLXML_Base::Serial_t();
	pLayer->Name = arName;
	pLayer->Device = arDevice;
	pLayer->BaudRate = APLXML_Base::BAUDRATE_9600;
	pLayer->DBits = APLXML_Base::DATABITS_8;
	pLayer->StopBits = APLXML_Base::STOPBITS_1;
	pLayer->Parity = APLXML_Base::PARITY_NONE;
	pLayer->FlowControl = APLXML_Base::FLOW_NONE;
	pLayer->OpenRetryMS = 5000;

	arList.SerialVector.push_back(pLayer);
	return pLayer;
}

}
}
