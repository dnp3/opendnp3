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
#ifndef __XML_TO_CONFIG_H_
#define __XML_TO_CONFIG_H_

#include <string>


#include <DNP3/DeviceTemplate.h>
#include <APL/PhysLayerSettings.h>
#include <APLXML/XMLConversion.h>

#include <vector>
#include <set>

namespace APLXML_DNP
{
class Master_t;
class Slave_t;
class LinkLayer_t;
class AppLayer_t;
class SlaveConfig_t;
class DeviceTemplate_t;
class VtoPorts_t;
class GrpVar_t;
class Control_t;
class Setpoint_t;
}

namespace APLXML_Base
{
class PhysicalLayerList_t;
}

namespace apl
{
namespace dnp
{

struct LinkConfig;
struct AppConfig;
struct MasterConfig;
struct SlaveConfig;
struct VtoConfig;
struct GrpVar;
struct MasterStackConfig;
struct SlaveStackConfig;
class  AsyncStackManager;

class FixedObject;


class XmlToConfig
{
public:

	static bool Configure(const APLXML_Base::PhysicalLayerList_t& arList, FilterLevel aLevel, AsyncStackManager& arMgr);


	static MasterStackConfig GetMasterConfig(const APLXML_DNP::Master_t& arCfg);
	static SlaveStackConfig GetSlaveConfig(const APLXML_DNP::Slave_t& arCfg, const APLXML_DNP::DeviceTemplate_t& arTmp, bool aStartOnline = false);
	static SlaveStackConfig GetSlaveConfig(const APLXML_DNP::Slave_t& arCfg, const DeviceTemplate& arTmp);

	//individual helper functions
	static LinkConfig Convert(const APLXML_DNP::LinkLayer_t& arCfg);
	static AppConfig Convert(const APLXML_DNP::AppLayer_t& arCfg);
	static MasterConfig Convert(const APLXML_DNP::Master_t& arCfg);
	static SlaveConfig Convert(const APLXML_DNP::SlaveConfig_t& arCfg, const APLXML_DNP::AppLayer_t& arApp);
	static VtoConfig Convert(const APLXML_DNP::VtoPorts_t& arCfg);
	static DeviceTemplate Convert( const APLXML_DNP::DeviceTemplate_t& arCfg, bool aStartOnline = false);

private:

	static CommandModes ConvertMode(const std::string& arMode);
	static GrpVar Convert(const APLXML_DNP::GrpVar_t& arCfg);

	template <typename T>
	static size_t CalcNumType(const std::vector<T*>& arIdxVec) {
		std::set<size_t> indexSet;

		for(size_t i = 0; i < arIdxVec.size(); ++i) {
			indexSet.insert(arIdxVec[i]->Index);
		}
		if(indexSet.size() > 0) return *indexSet.rbegin() + 1;
		else return 0;
	}

	template <class T>
	static void AddPoints(const std::vector<T*>& arXML, std::vector<PointRecord>& arVec) {
		for(size_t i = 0; i < arXML.size(); ++i)
			arVec[arXML[i]->Index] = PointRecord(arXML[i]->Name);
	}

	template <class T>
	static void AddEventPoints(const std::vector<T*>& arXML, std::vector<EventPointRecord>& arVec) {
		for(size_t i = 0; i < arXML.size(); ++i)
			arVec[arXML[i]->Index] = EventPointRecord(arXML[i]->Name, IntToPointClass(arXML[i]->ClassGroup));
	}

	template <class T>
	static void AddDeadbandPoints(const std::vector<T*>& arXML, std::vector<DeadbandPointRecord>& arVec) {
		for(size_t i = 0; i < arXML.size(); ++i)
			arVec[arXML[i]->Index] = DeadbandPointRecord(arXML[i]->Name, IntToPointClass(arXML[i]->ClassGroup), arXML[i]->Deadband);
	}

	template <class T>
	static void AddCommandData(const std::vector<T*>& arXML, std::vector<ControlRecord>& arVec) {
		for (size_t i = 0; i < arXML.size(); i++)
			arVec[arXML[i]->Index] = ControlRecord(arXML[i]->Name, ConvertMode(arXML[i]->ControlMode), arXML[i]->SelectTimeoutMS);
	}
};

}
}

#endif
