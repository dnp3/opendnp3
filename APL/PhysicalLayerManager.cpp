
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
#include "PhysicalLayerManager.h"

#include "PhysicalLayerFactory.h"
#include "PhysLayerSettings.h"
#include "Logger.h"
#include "Log.h"
#include "Util.h"
#include "Location.h"

using namespace std;

namespace apl
{
PhysicalLayerManager :: PhysicalLayerManager(Logger* apBaseLogger, boost::asio::io_service* apService) :
	PhysicalLayerMap(apBaseLogger, apService)
{

}

PhysicalLayerManager :: ~PhysicalLayerManager()
{
	for(auto pair: mNameToInstanceMap) {
		pair.second.Release();
	}
}

void PhysicalLayerManager::Remove(const std::string& arName)
{	
	NameToInstanceMap::iterator i = mNameToInstanceMap.find(arName);
	if(i == mNameToInstanceMap.end()) throw ArgumentException(LOCATION, "Unknown layer");
	i->second.Release();
	this->ReleaseLayer(arName);
	mNameToInstanceMap.erase(i);
	mNameToSettingsMap.erase(arName);	
}

void PhysicalLayerManager ::AddPhysicalLayer(const std::string& arName, PhysLayerSettings s, IPhysicalLayerAsync* apPhys)
{
	PhysLayerInstance pli(apPhys);
	this->AddLayer(arName, s, pli);
}

void PhysicalLayerManager ::AddTCPClient(const std::string& arName, PhysLayerSettings s, const std::string& arAddr, uint16_t aPort)
{
	IPhysicalLayerAsyncFactory fac = PhysicalLayerFactory::GetTCPClientAsync(arAddr, aPort);
	PhysLayerInstance pli(fac);
	this->AddLayer(arName, s, pli);
}

void PhysicalLayerManager ::AddTCPServer(const std::string& arName, PhysLayerSettings s, const std::string& arEndpoint, uint16_t aPort)
{
	IPhysicalLayerAsyncFactory fac = PhysicalLayerFactory::GetTCPServerAsync(arEndpoint, aPort);
	PhysLayerInstance pli(fac);
	this->AddLayer(arName, s, pli);
}

void PhysicalLayerManager ::AddSerial(const std::string& arName, PhysLayerSettings s, SerialSettings aSerial)
{
	IPhysicalLayerAsyncFactory fac = PhysicalLayerFactory::GetSerialAsync(aSerial);
	PhysLayerInstance pli(fac);
	this->AddLayer(arName, s, pli);
}

}
