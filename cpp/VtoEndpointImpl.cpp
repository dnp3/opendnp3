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

#include "VtoEndpointImpl.h"

#include "AlwaysOpeningVtoRouter.h"
#include "EnhancedVtoRouter.h"
#include "IPhysicalLayerAsync.h"
#include "ExecutorPause.h"

namespace opendnp3
{

VtoEndpointImpl::VtoEndpointImpl(	Logger* apLogger, 
									IVtoWriter* apWriter, 
									IPhysicalLayerAsync* apPhys,
									const VtoRouterSettings& arSettings,
									std::function<void (VtoEndpointImpl*)> aOnShutdown) : 
	mpPhys(apPhys),
	mpRouter(FGetVtoRouter(arSettings, apLogger, apWriter, apPhys)),
	mOnShutdown(aOnShutdown)
{
	
}

VtoEndpointImpl::~VtoEndpointImpl()
{
	Cleanup();
}

IVtoCallbacks* VtoEndpointImpl::GetVtoCallbacks()
{
	return mpRouter.get();
}

void VtoEndpointImpl::Shutdown()
{
	Cleanup();
	mOnShutdown(this);
}

void VtoEndpointImpl::Cleanup()
{
	{
		ExecutorPause p(mpPhys->GetExecutor());	
		this->mpRouter->Shutdown();
	}	
	mpRouter->WaitForShutdown();
}

VtoRouter* VtoEndpointImpl::FGetVtoRouter(const VtoRouterSettings& arSettings, Logger* apLogger, IVtoWriter* apWriter, IPhysicalLayerAsync* apPhys)
{
	if(arSettings.DISABLE_EXTENSIONS) {
		return new AlwaysOpeningVtoRouter(arSettings, apLogger, apWriter, apPhys);
	}
	else {
		if(arSettings.START_LOCAL) {
			return new  ServerSocketVtoRouter(arSettings, apLogger, apWriter, apPhys);
		}
		else {
			return new ClientSocketVtoRouter(arSettings, apLogger, apWriter, apPhys);
		}
	}
}

}

