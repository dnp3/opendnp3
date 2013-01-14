
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
#include "PhysicalLayerMap.h"

#include "PhysLayerSettings.h"
#include "Logger.h"
#include "Exception.h"
#include "LoggableMacros.h"

namespace apl
{

PhysicalLayerMap::PhysicalLayerMap(Logger* apBaseLogger, boost::asio::io_service* apService) :
	Loggable(apBaseLogger),
	mpService(apService),
	mpBaseLogger(apBaseLogger)

{

}

PhysicalLayerMap::~PhysicalLayerMap()
{

}

PhysLayerSettings PhysicalLayerMap ::GetSettings(const std::string& arName)
{
	std::unique_lock<std::mutex> lock(mMutex);
	return _GetSettings(arName);
}

IPhysicalLayerAsync* PhysicalLayerMap::AcquireLayer(const std::string& arName)
{
	std::unique_lock<std::mutex> lock(mMutex);
	PhysLayerSettings s = this->_GetSettings(arName);
	PhysLayerInstance* pInstance = this->_GetInstance(arName);
	AcquiredMap::iterator i = mAcquiredMap.find(arName);
	if(i != mAcquiredMap.end()) throw ArgumentException("Layer with name has already been acquired: " + arName);	
	else {
		mAcquiredMap[arName] = true;
		IPhysicalLayerAsync* pLayer = pInstance->GetLayer(this->MakeLogger(arName, s.LogLevel), mpService);
		LOG_BLOCK(LEV_DEBUG, "Physical layer acquired: " << arName);
		return pLayer;
	}
}

void PhysicalLayerMap::ReleaseLayer(const std::string& arName)
{
	std::unique_lock<std::mutex> lock(mMutex);
	NameToInstanceMap::iterator i = mNameToInstanceMap.find(arName);
	if(i == mNameToInstanceMap.end()) {
		throw ArgumentException(LOCATION, "Physical layer not managed by this map: " + arName);
	}
	else {
		mAcquiredMap.erase(arName);
		PhysLayerInstance* pInst = &i->second;
		pInst->Release();
		LOG_BLOCK(LEV_DEBUG, "Physical layer released: " << arName);
	}
}

PhysLayerSettings PhysicalLayerMap ::_GetSettings(const std::string& arName)
{
	NameToSettingsMap::iterator i = mNameToSettingsMap.find(arName);
	if(i == mNameToSettingsMap.end())
		throw ArgumentException(LOCATION, "Settings with name doesn't exist: " + arName);

	return i->second;
}

PhysLayerInstance* PhysicalLayerMap ::_GetInstance(const std::string& arName)
{
	NameToInstanceMap::iterator i = mNameToInstanceMap.find(arName);
	if(i == mNameToInstanceMap.end())
		throw ArgumentException(LOCATION, "Instance with name doesn't exist: " + arName);

	return &(i->second);
}

void PhysicalLayerMap ::AddLayer(const std::string& arName, PhysLayerSettings aSettings, PhysLayerInstance aInstance)
{
	std::unique_lock<std::mutex> lock(mMutex);

	if(mNameToSettingsMap.find(arName) != mNameToSettingsMap.end())
		throw Exception(LOCATION, "Layer with that name already exists");

	mNameToSettingsMap.insert(NameToSettingsMap::value_type(arName, aSettings));
	mNameToInstanceMap.insert(NameToInstanceMap::value_type(arName, aInstance));
}

Logger* PhysicalLayerMap ::MakeLogger(const std::string& arName, FilterLevel aLevel)
{
	Logger* ret = mpBaseLogger->GetSubLogger(arName);
	ret->SetFilterLevel(aLevel);
	return ret;
}
}
