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
#ifndef _XML_DNP3_H_
#define _XML_DNP3_H_

#include <string>

namespace APLXML_DNP
{
class Master_t;
class Slave_t;
class Stack_t;
class DeviceTemplate_t;
class VtoPorts_t;
}

namespace apl
{
namespace dnp
{

class XML_DNP3
{
public:
	static void Configure(APLXML_DNP::Master_t& arMaster, bool aUnsol = false);
	static void Configure(APLXML_DNP::Slave_t& arSlave);
	static void Configure(APLXML_DNP::Stack_t& arStack, bool aSlave = true);
	static void Configure(APLXML_DNP::DeviceTemplate_t& arTemplate, size_t aBinaries, size_t aAnalogs, size_t aCounters, size_t aControls, size_t aSetpoints, size_t aControlStatuses, size_t aSetpointStatuses);

	static void AddVtoPort(APLXML_DNP::VtoPorts_t& arPorts, std::string aPhysicalLayer, int aChannel, bool aStartLocal, int aOpenRetry = 5000);

	static void AddDeviceTemplateBinary(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aIndex, std::string aName);
	static void AddDeviceTemplateAnalog(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aIndex, std::string aName);
	static void AddDeviceTemplateCounter(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aIndex, std::string aName);
	static void AddDeviceTemplateControl(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aIndex, std::string aName);
	static void AddDeviceTemplateSetpoint(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aIndex, std::string aName);
	static void AddDeviceTemplateControlStatus(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aIndex, std::string aName);
	static void AddDeviceTemplateSetpointStatus(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aIndex, std::string aName);
};

}
}

#endif
