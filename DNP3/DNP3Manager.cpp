
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

#include "DNP3Manager.h"

#include <APL/PhysicalLayerAsyncTCPClient.h>
#include <APL/PhysicalLayerAsyncTCPServer.h>
#include <APL/PhysicalLayerAsyncSerial.h>
#include <APL/TimeSource.h>
#include <APL/IOServiceThreadPool.h>
#include <APL/Log.h>

#include "DNP3Channel.h"

namespace apl
{
namespace dnp
{

DNP3Manager::DNP3Manager(uint32_t aConcurrency, std::function<void()> aOnThreadStart, std::function<void()> aOnThreadExit) :
	mpLog(new EventLog()),
	mpThreadPool(new IOServiceThreadPool(mpLog->GetLogger(LEV_INFO, "ThreadPool"),  aConcurrency, aOnThreadExit, aOnThreadExit))
{

}

DNP3Manager::~DNP3Manager()
{
	this->Shutdown();
}

void DNP3Manager::AddLogSubscriber(ILogBase* apLog)
{
	mpLog->AddLogSubscriber(apLog);
}

void DNP3Manager::Shutdown()
{
	std::set<DNP3Channel*> copy(mChannels);	
	for(auto pChannel: copy) pChannel->Shutdown();	
}

IChannel* DNP3Manager::AddTCPClient(const std::string& arName, FilterLevel aLevel, millis_t aOpenRetry, const std::string& arAddr, uint16_t aPort)
{	
	auto pLogger = mpLog->GetLogger(aLevel, arName);
	auto pPhys = new PhysicalLayerAsyncTCPClient(pLogger, mpThreadPool->GetIOService(), arAddr, aPort);
	return CreateChannel(pLogger, aOpenRetry, pPhys);
}

IChannel* DNP3Manager::AddTCPServer(const std::string& arName, FilterLevel aLevel, millis_t aOpenRetry, const std::string& arEndpoint, uint16_t aPort)
{	
	auto pLogger = mpLog->GetLogger(aLevel, arName);
	auto pPhys = new PhysicalLayerAsyncTCPServer(pLogger, mpThreadPool->GetIOService(), arEndpoint, aPort);
	return CreateChannel(pLogger, aOpenRetry, pPhys);
}

IChannel* DNP3Manager::AddSerial(const std::string& arName, FilterLevel aLevel, millis_t aOpenRetry, SerialSettings aSettings)
{
	auto pLogger = mpLog->GetLogger(aLevel, arName);
	auto pPhys = new PhysicalLayerAsyncSerial(pLogger, mpThreadPool->GetIOService(), aSettings);
	return CreateChannel(pLogger, aOpenRetry, pPhys);
}


IChannel* DNP3Manager::CreateChannel(Logger* apLogger, millis_t aOpenRetry, IPhysicalLayerAsync* apPhys)
{
	auto pChannel = new DNP3Channel(apLogger, aOpenRetry, mpThreadPool->GetIOService(), apPhys, TimeSource::Inst(), [this](DNP3Channel* apChannel){ 
		this->OnChannelShutdownCallback(apChannel);
	});
	mChannels.insert(pChannel);
	return pChannel;
}

void DNP3Manager::OnChannelShutdownCallback(DNP3Channel* apChannel)
{	
	mChannels.erase(apChannel);	
	delete apChannel;
}

}
}

