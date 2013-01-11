
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
#include "XmlToConfig.h"

#include <XMLBindings/APLXML_DNP.h>
#include <XMLBindings/APLXML_Base.h>

#include <APLXML/XMLConversion.h>
#include <APLXML/PhysicalLayerXMLFactory.h>

#include <boost/numeric/conversion/converter.hpp>

#include <DNP3/DNPConstants.h>
#include <DNP3/LinkConfig.h>
#include <DNP3/AppConfig.h>
#include <DNP3/VtoConfig.h>
#include <DNP3/MasterConfig.h>
#include <DNP3/SlaveConfig.h>
#include <DNP3/DeviceTemplate.h>
#include <DNP3/MasterStackConfig.h>
#include <DNP3/SlaveStackConfig.h>
#include <DNP3/AsyncStackManager.h>

using namespace APLXML_Base;

namespace apl
{
namespace dnp
{

bool XmlToConfig::Configure(const APLXML_Base::PhysicalLayerList_t& arList, FilterLevel aLevel, AsyncStackManager& arMgr)
{

	for (size_t i = 0; i < arList.TCPClientVector.size(); i++ ) {
		TCPClient_t* pCfg = arList.TCPClientVector[i];
		PhysLayerSettings s(aLevel, pCfg->OpenRetryMS);
		arMgr.AddTCPClient(pCfg->Name, s, pCfg->Address, pCfg->Port);
	}
	for (size_t i = 0; i < arList.TCPServerVector.size(); i++ ) {
		TCPServer_t* pCfg = arList.TCPServerVector[i];
		PhysLayerSettings s(aLevel, pCfg->OpenRetryMS);
		arMgr.AddTCPServer(pCfg->Name, s, pCfg->Endpoint, pCfg->Port);
	}
	for (size_t i = 0; i < arList.SerialVector.size(); i++ ) {
		Serial_t* pCfg = arList.SerialVector[i];
		PhysLayerSettings s(aLevel, pCfg->OpenRetryMS);
		arMgr.AddSerial(pCfg->Name, s, xml::GetSerialSettings(pCfg) );
	}

	return true;
}

MasterStackConfig XmlToConfig::GetMasterConfig(const APLXML_DNP::Master_t& arCfg)
{
	MasterStackConfig cfg;
	cfg.app = Convert(arCfg.Stack.AppLayer);
	cfg.link = Convert(arCfg.Stack.LinkLayer);
	cfg.master = Convert(arCfg);
	cfg.vto = Convert(arCfg.VtoPorts);
	return cfg;
}

SlaveStackConfig XmlToConfig::GetSlaveConfig(const APLXML_DNP::Slave_t& arCfg, const APLXML_DNP::DeviceTemplate_t& arTmp, bool aStartOnline)
{
	DeviceTemplate tmp = Convert(arTmp, aStartOnline);
	return GetSlaveConfig(arCfg, tmp);
}

SlaveStackConfig XmlToConfig::GetSlaveConfig(const APLXML_DNP::Slave_t& arCfg, const DeviceTemplate& arTmp)
{
	SlaveStackConfig cfg;
	cfg.app = Convert(arCfg.Stack.AppLayer);
	cfg.link = Convert(arCfg.Stack.LinkLayer);
	cfg.slave = Convert(arCfg.SlaveConfig, arCfg.Stack.AppLayer);
	cfg.vto = Convert(arCfg.VtoPorts);
	cfg.device = arTmp;
	return cfg;
}

LinkConfig XmlToConfig::Convert(const APLXML_DNP::LinkLayer_t& arLink)
{
	LinkConfig cfg(arLink.IsMaster, arLink.UseConfirmations);
	cfg.IsMaster = arLink.IsMaster;
	cfg.LocalAddr = boost::numeric::converter<int, uint16_t>::convert(arLink.LocalAddress);
	cfg.RemoteAddr = boost::numeric::converter<int, uint16_t>::convert(arLink.RemoteAddress);
	cfg.Timeout = arLink.AckTimeoutMS;
	cfg.UseConfirms = arLink.UseConfirmations;
	cfg.NumRetry = boost::numeric::converter<int, size_t>::convert(arLink.NumRetries);
	return cfg;
}

AppConfig XmlToConfig::Convert(const APLXML_DNP::AppLayer_t& arCfg)
{
	AppConfig cfg;
	cfg.FragSize = boost::numeric::converter<int, size_t>::convert(arCfg.MaxFragSize);
	cfg.RspTimeout = arCfg.TimeoutMS;
	cfg.NumRetry = boost::numeric::converter<int, size_t>::convert(arCfg.NumRetries);
	return cfg;
}

MasterConfig XmlToConfig::Convert(const APLXML_DNP::Master_t& arCfg)
{
	MasterConfig cfg;
	cfg.AllowTimeSync = true;
	cfg.DoUnsolOnStartup = arCfg.Unsol.DoTask;
	cfg.EnableUnsol = arCfg.Unsol.Enable;
	cfg.FragSize = arCfg.Stack.AppLayer.MaxFragSize;
	cfg.IntegrityRate = arCfg.MasterSettings.IntegrityPeriodMS;
	cfg.TaskRetryRate = arCfg.MasterSettings.TaskRetryMS;
	cfg.UnsolClassMask = ClassMask::GetMask(arCfg.Unsol.Class1, arCfg.Unsol.Class2, arCfg.Unsol.Class3);
	cfg.IntegrityRate = arCfg.MasterSettings.IntegrityPeriodMS;
	cfg.UseNonStandardVtoFunction = arCfg.MasterSettings.UseNonStandardVtoTransferCode;

	std::vector<APLXML_DNP::ExceptionScan_t*>& vec = arCfg.ScanList.ExceptionScanVector;
	for(size_t i = 0; i < vec.size(); ++i) {
		int mask = ClassMask::GetMask(vec[i]->Class1, vec[i]->Class2, vec[i]->Class3);
		cfg.AddExceptionScan(mask, vec[i]->PeriodMS);
	}
	return cfg;
}

VtoConfig XmlToConfig::Convert(const APLXML_DNP::VtoPorts_t& arCfg)
{
	VtoConfig cfg;

	std::vector<APLXML_DNP::VtoPort_t*>& vec = arCfg.VtoPortVector;
	for(size_t i = 0; i < vec.size(); ++i) {
		VtoRouterSettings vrs(vec[i]->Index, vec[i]->StartLocal, false, vec[i]->OpenRetry);
		VtoRouterConfig c;
		c.mPhysicalLayerName = vec[i]->PhysicalLayer;
		c.mSettings = vrs;
		cfg.AddVtoRouterConfig(c);
	}
	return cfg;
}

SlaveConfig XmlToConfig::Convert(const APLXML_DNP::SlaveConfig_t& arCfg, const APLXML_DNP::AppLayer_t& arApp)
{
	SlaveConfig c;

	c.mAllowTimeSync = arCfg.TimeIINTask.DoTask;
	c.mTimeSyncPeriod = arCfg.TimeIINTask.PeriodMS;

	c.mUnsolPackDelay = arCfg.UnsolDefaults.PackDelayMS;
	c.mUnsolRetryDelay = arCfg.UnsolDefaults.RetryMS;

	c.mStaticBinary = Convert(arCfg.StaticRsp.BinaryGrpVar);
	c.mStaticAnalog = Convert(arCfg.StaticRsp.AnalogGrpVar);
	c.mStaticCounter =  Convert(arCfg.StaticRsp.CounterGrpVar);
	c.mStaticSetpointStatus =  Convert(arCfg.StaticRsp.SetpointStatusGrpVar);

	c.mEventBinary =  Convert(arCfg.EventRsp.BinaryGrpVar);
	c.mEventAnalog =  Convert(arCfg.EventRsp.AnalogGrpVar);
	c.mEventCounter =  Convert(arCfg.EventRsp.CounterGrpVar);

	c.mEventMaxConfig.mMaxBinaryEvents = arCfg.MaxBinaryEvents;
	c.mEventMaxConfig.mMaxAnalogEvents = arCfg.MaxAnalogEvents;
	c.mEventMaxConfig.mMaxCounterEvents = arCfg.MaxCounterEvents;

	c.mMaxControls = 1;

	c.mMaxFragSize = boost::numeric::converter<int, size_t>::convert(arApp.MaxFragSize);;

	return c;
}

GrpVar XmlToConfig::Convert(const APLXML_DNP::GrpVar_t& arCfg)
{
	GrpVar gp(arCfg.Grp, arCfg.Var);
	return gp;
}

DeviceTemplate XmlToConfig::Convert(const APLXML_DNP::DeviceTemplate_t& arCfg, bool aStartOnline)
{
	size_t numBinary = CalcNumType(arCfg.BinaryData.BinaryVector);
	size_t numAnalog = CalcNumType(arCfg.AnalogData.AnalogVector);
	size_t numCounter = CalcNumType(arCfg.CounterData.CounterVector);
	size_t numControl = CalcNumType(arCfg.ControlData.ControlVector);
	size_t numSetpoint = CalcNumType(arCfg.SetpointData.SetpointVector);
	size_t numControlStatus = CalcNumType(arCfg.ControlStatusData.ControlStatusVector);
	size_t numSetpointStatus = CalcNumType(arCfg.SetpointStatusData.SetpointStatusVector);

	DeviceTemplate t(numBinary, numAnalog, numCounter, numControlStatus, numSetpointStatus, numControl, numSetpoint);

	AddEventPoints(arCfg.BinaryData.BinaryVector, t.mBinary);
	AddDeadbandPoints(arCfg.AnalogData.AnalogVector, t.mAnalog);
	AddEventPoints(arCfg.CounterData.CounterVector, t.mCounter);

	AddCommandData(arCfg.ControlData.ControlVector, t.mControls);
	AddCommandData(arCfg.SetpointData.SetpointVector, t.mSetpoints);

	AddPoints(arCfg.ControlStatusData.ControlStatusVector, t.mControlStatus);
	AddPoints(arCfg.SetpointStatusData.SetpointStatusVector, t.mSetpointStatus);

	t.mStartOnline = aStartOnline;

	return t;
}

CommandModes XmlToConfig::ConvertMode(const std::string& arMode)
{
	if(arMode == "SBO") return CM_SBO_ONLY;
	if(arMode == "DO_ONLY") return CM_DO_ONLY;
	if(arMode == "SBO_OR_DO") return CM_SBO_OR_DO;

	throw ArgumentException(LOCATION, "invalid command mode");
}

}
}
