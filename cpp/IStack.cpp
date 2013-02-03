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

#include "IStack.h"
#include "IVtoEndpoint.h"
#include "VtoEndpointImpl.h"
#include "VtoDataInterface.h"
#include "PhysicalLayerAsyncTCPClient.h"
#include "PhysicalLayerAsyncTCPServer.h"
#include "Logger.h"

namespace opendnp3
{

IStack::IStack(Logger* apLogger, boost::asio::io_service* apService) :
	mpLogger(apLogger),
	mpService(apService)
{


}

IStack::~IStack()
{

}
	
void IStack::CleanupVto()
{
	std::set<IVtoEndpoint*> copy(mVtoEndpoints);
	for(auto pEndpoint: copy) pEndpoint->Shutdown();
}

IVtoEndpoint* IStack::StartVtoRouterTCPClient(const std::string& arName, FilterLevel aLevel, const std::string& arAddr, uint16_t aPort, const VtoRouterSettings& arSettings)
{
	auto pPhys = new PhysicalLayerAsyncTCPClient(mpLogger->GetSubLogger("arName", aLevel), mpService, arAddr, aPort);
	return CreateVtoEndpoint(pPhys, arSettings);
}

IVtoEndpoint* IStack::StartVtoRouterTCPServer(const std::string& arName, FilterLevel aLevel, const std::string& arEndpoint, uint16_t aPort, const VtoRouterSettings& arSettings)
{
	auto pPhys = new PhysicalLayerAsyncTCPServer(mpLogger->GetSubLogger("arName", aLevel), mpService, arEndpoint, aPort);
	return CreateVtoEndpoint(pPhys, arSettings);
}

IVtoEndpoint* IStack::CreateVtoEndpoint(IPhysicalLayerAsync* apPhys, const VtoRouterSettings& arSettings)
{
	auto pEndpoint = new VtoEndpointImpl(mpLogger->GetSubLogger("vto"), this->GetVtoWriter(), apPhys, arSettings, [this](VtoEndpointImpl* apEndpoint){
		OnVtoEndpointShutdown(apEndpoint);
	});
	this->GetVtoReader()->AddVtoChannel(pEndpoint->GetVtoCallbacks());
	this->GetVtoWriter()->AddVtoCallback(pEndpoint->GetVtoCallbacks());
	mVtoEndpoints.insert(pEndpoint);
	return pEndpoint;
}

void IStack::OnVtoEndpointShutdown(VtoEndpointImpl* apEndpoint)
{
	mVtoEndpoints.erase(apEndpoint);
	this->GetVtoReader()->RemoveVtoChannel(apEndpoint->GetVtoCallbacks());
	this->GetVtoWriter()->RemoveVtoCallback(apEndpoint->GetVtoCallbacks());
	delete apEndpoint;
}

}

