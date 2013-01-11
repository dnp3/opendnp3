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
#include "XML_DNP3.h"

#include <XMLBindings/APLXML_DNP.h>
#include <XMLBindings/APLXML_Base.h>

#include <boost/numeric/conversion/converter.hpp>

#include <DNP3/DNPConstants.h>
#include <DNP3/LinkConfig.h>
#include <DNP3/AppConfig.h>
#include <DNP3/MasterConfig.h>

namespace apl
{
namespace dnp
{

void XML_DNP3::Configure(APLXML_DNP::Master_t& arMaster, bool aUnsol)
{
	arMaster.MasterSettings.TaskRetryMS = 5000;
	arMaster.MasterSettings.UseNonStandardVtoTransferCode = false;

	arMaster.Unsol.DoTask = aUnsol;
	arMaster.Unsol.Enable = aUnsol;
	arMaster.Unsol.Class1 = true;
	arMaster.Unsol.Class2 = true;
	arMaster.Unsol.Class3 = true;

	arMaster.MasterSettings.IntegrityPeriodMS = 60000;

	if(!aUnsol) {
		APLXML_DNP::ExceptionScan_t* pScan = new APLXML_DNP::ExceptionScan_t();
		pScan->Class1 = pScan->Class2 = pScan->Class3 = true;
		pScan->PeriodMS = 2000;
		arMaster.ScanList.ExceptionScanVector.push_back(pScan);
	}

	// Configure the LinkLayer
	Configure(arMaster.Stack, false);
}

void XML_DNP3::Configure(APLXML_DNP::Slave_t& arSlave)
{
	//arSlave.SlaveConfig.SlaveConfig.MaxControls = 10;
	arSlave.SlaveConfig.MaxBinaryEvents = 100;
	arSlave.SlaveConfig.MaxAnalogEvents = 100;
	arSlave.SlaveConfig.MaxCounterEvents = 100;
	arSlave.SlaveConfig.UnsolDefaults.PackDelayMS = 200;
	arSlave.SlaveConfig.UnsolDefaults.RetryMS = 2000;

	arSlave.SlaveConfig.StaticRsp.BinaryGrpVar.Grp = 1;
	arSlave.SlaveConfig.StaticRsp.BinaryGrpVar.Var = 2;
	arSlave.SlaveConfig.StaticRsp.AnalogGrpVar.Grp = 30;
	arSlave.SlaveConfig.StaticRsp.AnalogGrpVar.Var = 1;
	arSlave.SlaveConfig.StaticRsp.CounterGrpVar.Grp = 20;
	arSlave.SlaveConfig.StaticRsp.CounterGrpVar.Var = 1;
	arSlave.SlaveConfig.StaticRsp.SetpointStatusGrpVar.Grp = 40;
	arSlave.SlaveConfig.StaticRsp.SetpointStatusGrpVar.Var = 1;

	arSlave.SlaveConfig.EventRsp.BinaryGrpVar.Grp = 2;
	arSlave.SlaveConfig.EventRsp.BinaryGrpVar.Var = 1;
	arSlave.SlaveConfig.EventRsp.AnalogGrpVar.Grp = 32;
	arSlave.SlaveConfig.EventRsp.AnalogGrpVar.Var = 1;
	arSlave.SlaveConfig.EventRsp.CounterGrpVar.Grp = 22;
	arSlave.SlaveConfig.EventRsp.CounterGrpVar.Var = 1;

	arSlave.SlaveConfig.TimeIINTask.DoTask = false;
	arSlave.SlaveConfig.TimeIINTask.PeriodMS = 30000;

	Configure(arSlave.Stack, true);
}

void XML_DNP3::Configure(APLXML_DNP::Stack_t& arStack, bool aSlave)
{
	arStack.LinkLayer.AckTimeoutMS = 1000;
	arStack.LinkLayer.IsMaster = aSlave ? false : true;
	arStack.LinkLayer.LocalAddress = aSlave ? 1 : 100;
	arStack.LinkLayer.NumRetries = 3;
	arStack.LinkLayer.RemoteAddress = aSlave ? 100 : 1;
	arStack.LinkLayer.UseConfirmations = false;

	arStack.AppLayer.TimeoutMS = 10000;
	arStack.AppLayer.MaxFragSize = apl::dnp::DEFAULT_FRAG_SIZE;
	arStack.AppLayer.NumRetries = 3;
}

void XML_DNP3::AddVtoPort(APLXML_DNP::VtoPorts_t& arPorts, std::string aPhysicalLayer, int aChannel, bool aStartLocal, int aOpenRetry)
{
	APLXML_DNP::VtoPort_t* port = new APLXML_DNP::VtoPort_t();
	port->Index = aChannel;
	port->StartLocal = aStartLocal;
	port->PhysicalLayer = aPhysicalLayer;
	port->OpenRetry = aOpenRetry;

	arPorts.VtoPortVector.push_back(port);
}

std::string DefaultName(const std::string aBase, size_t aCount)
{
	std::ostringstream oss;
	oss << aBase;
	if (aCount < 10) oss << "0";
	oss << aCount;
	return oss.str();
}

void XML_DNP3::AddDeviceTemplateBinary(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aIndex, std::string aName)
{
	APLXML_DNP::Binary_t* b = new APLXML_DNP::Binary_t();
	b->ClassGroup = 1;
	b->Index = boost::numeric::converter<int, size_t>::convert(aIndex);
	b->Name = aName;
	arCfg.BinaryData.BinaryVector.push_back(b);
}

void XML_DNP3::AddDeviceTemplateAnalog(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aIndex, std::string aName)
{
	APLXML_DNP::Analog_t* b = new APLXML_DNP::Analog_t();
	b->ClassGroup = 1;
	b->Index = boost::numeric::converter<int, size_t>::convert(aIndex);
	b->Name = aName;
	b->Deadband = 0;
	arCfg.AnalogData.AnalogVector.push_back(b);
}

void XML_DNP3::AddDeviceTemplateCounter(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aIndex, std::string aName)
{
	APLXML_DNP::Counter_t* b = new APLXML_DNP::Counter_t();
	b->ClassGroup = 1;
	b->Index = boost::numeric::converter<int, size_t>::convert(aIndex);
	b->Name = aName;
	arCfg.CounterData.CounterVector.push_back(b);
}

void XML_DNP3::AddDeviceTemplateControl(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aIndex, std::string aName)
{
	APLXML_DNP::Control_t* b = new APLXML_DNP::Control_t();
	b->Index = boost::numeric::converter<int, size_t>::convert(aIndex);
	b->Name = aName;
	b->ControlMode = "SBO";
	b->SelectTimeoutMS = 5000;
	arCfg.ControlData.ControlVector.push_back(b);
}

void XML_DNP3::AddDeviceTemplateSetpoint(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aIndex, std::string aName)
{
	APLXML_DNP::Setpoint_t* b = new APLXML_DNP::Setpoint_t();
	b->Index = boost::numeric::converter<int, size_t>::convert(aIndex);
	b->Name = aName;
	b->ControlMode = "SBO";
	b->SelectTimeoutMS = 5000;
	arCfg.SetpointData.SetpointVector.push_back(b);
}
void XML_DNP3::AddDeviceTemplateControlStatus(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aIndex, std::string aName)
{
	APLXML_DNP::ControlStatus_t* cs = new APLXML_DNP::ControlStatus_t();
	cs->Index = boost::numeric::converter<int, size_t>::convert(aIndex);
	cs->Name = aName;
	arCfg.ControlStatusData.ControlStatusVector.push_back(cs);
}
void XML_DNP3::AddDeviceTemplateSetpointStatus(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aIndex, std::string aName)
{
	APLXML_DNP::SetpointStatus_t* ss = new APLXML_DNP::SetpointStatus_t();
	ss->Index = boost::numeric::converter<int, size_t>::convert(aIndex);
	ss->Name = aName;
	arCfg.SetpointStatusData.SetpointStatusVector.push_back(ss);
}

void XML_DNP3::Configure(APLXML_DNP::DeviceTemplate_t& arCfg, size_t aBinaries, size_t aAnalogs, size_t aCounters, size_t aControls, size_t aSetpoints, size_t aControlStatuses, size_t aSetpointStatuses)
{
	arCfg.TemplateName = "default";

	for(size_t i = 0; i < aBinaries; i++) {
		AddDeviceTemplateBinary(arCfg, i, DefaultName("binary", i));
	}
	for(size_t i = 0; i < aAnalogs; i++) {
		AddDeviceTemplateAnalog(arCfg, i, DefaultName("analog", i));
	}
	for(size_t i = 0; i < aCounters; i++) {
		AddDeviceTemplateCounter(arCfg, i, DefaultName("counter", i));
	}
	for(size_t i = 0; i < aControls; i++) {
		AddDeviceTemplateControl(arCfg, i, DefaultName("control", i));
	}
	for(size_t i = 0; i < aSetpoints; i++) {
		AddDeviceTemplateSetpoint(arCfg, i, DefaultName("setpoint", i));
	}
	for(size_t i = 0; i < aControlStatuses; i++ ) {
		AddDeviceTemplateControlStatus(arCfg, i, DefaultName("controlstatus", i));
	}
	for(size_t i = 0; i < aSetpointStatuses; i++ ) {
		AddDeviceTemplateSetpointStatus(arCfg, i, DefaultName("setpointstatus", i));
	}
}
}
}
