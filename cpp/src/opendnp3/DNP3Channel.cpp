
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

#include "DNP3Channel.h"

#ifndef OPENDNP3_NO_MASTER
#include "MasterStackImpl.h"
#endif

#include "OutstationStackImpl.h"
#include "IPhysicalLayerAsync.h"
#include "ExecutorPause.h"

namespace opendnp3
{

DNP3Channel::DNP3Channel(Logger* apLogger, millis_t aOpenRetry, boost::asio::io_service* apService, IPhysicalLayerAsync* apPhys, ITimeSource* apTimeSource, std::function<void (DNP3Channel*)> aOnShutdown) :
	Loggable(apLogger),
	mpService(apService),
	mpPhys(apPhys),
	mOnShutdown(aOnShutdown),
	mRouter(apLogger->GetSubLogger("Router"), mpPhys.get(), aOpenRetry)
#ifndef OPENDNP3_NO_MASTER
	, mGroup(apPhys->GetExecutor(), apTimeSource)
#endif
{

}

DNP3Channel::~DNP3Channel()
{
	this->Cleanup();
}

void DNP3Channel::Shutdown()
{
	this->Cleanup();
	mOnShutdown(this);
}

void DNP3Channel::AddStateListener(std::function<void (ChannelState)> aListener)
{
	mRouter.AddStateListener(aListener);
}

void DNP3Channel::Cleanup()
{
	std::set<IStack*> copy(mStacks);
for(auto pStack: copy) pStack->Shutdown();
	{
		ExecutorPause p(mpPhys->GetExecutor());
#ifndef OPENDNP3_NO_MASTER
		this->mGroup.Shutdown();	// no more task callbacks
#endif		
		this->mRouter.Shutdown();	// start shutting down the router
	}
	mRouter.WaitForShutdown();
}

#ifndef OPENDNP3_NO_MASTER
IMaster* DNP3Channel::AddMaster(const std::string& arLoggerId, FilterLevel aLevel, IDataObserver* apPublisher, const MasterStackConfig& arCfg)
{
	auto pLogger = mpLogger->GetSubLogger(arLoggerId, aLevel);
	LinkRoute route(arCfg.link.RemoteAddr, arCfg.link.LocalAddr);
	auto pMaster = new MasterStackImpl(pLogger, mpService, mpPhys->GetExecutor(), apPublisher, &mGroup, arCfg, [this, route](IStack * apStack) {
		this->OnStackShutdown(apStack, route);
	});
	pMaster->SetLinkRouter(&mRouter);
	mStacks.insert(pMaster);
	{
		ExecutorPause p(mpPhys->GetExecutor());
		mRouter.AddContext(pMaster->GetLinkContext(), route);
	}
	return pMaster;
}
#endif

IOutstation* DNP3Channel::AddOutstation(const std::string& arLoggerId, FilterLevel aLevel, ICommandHandler* apCmdHandler, const SlaveStackConfig& arCfg)
{
	auto pLogger = mpLogger->GetSubLogger(arLoggerId, aLevel);
	LinkRoute route(arCfg.link.RemoteAddr, arCfg.link.LocalAddr);
	auto pOutstation = new OutstationStackImpl(pLogger, mpService, mpPhys->GetExecutor(), apCmdHandler, arCfg, [this, route](IStack * apStack) {
		this->OnStackShutdown(apStack, route);
	});
	pOutstation->SetLinkRouter(&mRouter);
	mStacks.insert(pOutstation);
	{
		ExecutorPause p(mpPhys->GetExecutor());
		mRouter.AddContext(pOutstation->GetLinkContext(), route);
	}
	return pOutstation;
}

void DNP3Channel::OnStackShutdown(IStack* apStack, LinkRoute route)
{
	mStacks.erase(apStack);
	{
		ExecutorPause p(mpPhys->GetExecutor());
		mRouter.RemoveContext(route);
	}
	delete apStack;
}

}

