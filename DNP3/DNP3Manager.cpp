
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
#include <APL/TimeSource.h>

#include "DNP3Channel.h"

namespace apl
{
namespace dnp
{

DNP3Manager::DNP3Manager(size_t aConcurrency) :
mLog(),
mThreadPool(mLog.GetLogger(LEV_INFO, "ThreadPool"), aConcurrency)
{

}

DNP3Manager::~DNP3Manager()
{
	this->Shutdown();
}

void DNP3Manager::AddLogSubscriber(ILogBase* apLog)
{
	mLog.AddLogSubscriber(apLog);
}

void DNP3Manager::Shutdown()
{
	std::set<DNP3Channel*> copy(mChannels);
	for(auto pChannel: copy) pChannel->Shutdown();
	std::cout << "Done Manager Shutdown" << std::endl;
}

IChannel* DNP3Manager::AddTCPClient(const std::string& arName, FilterLevel aLevel, millis_t aOpenRetry, const std::string& arAddr, uint16_t aPort)
{
	std::lock_guard<std::mutex> lock(mMutex);
	auto pLogger = mLog.GetLogger(aLevel, arName);
	auto pPhys = new PhysicalLayerAsyncTCPClient(pLogger, mThreadPool.GetIOService(), arAddr, aPort);
	auto pChannel = new DNP3Channel(pLogger, aOpenRetry, pPhys, TimeSource::Inst(), [this](DNP3Channel* apChannel){ this->OnChannelShutdownCallback(apChannel); });
	mChannels.insert(pChannel);
	return pChannel;
}

IChannel* DNP3Manager::AddTCPServer(const std::string& arName, FilterLevel aLevel, millis_t aOpenRetry, const std::string& arEndpoint, uint16_t aPort)
{
	std::lock_guard<std::mutex> lock(mMutex);
	auto pLogger = mLog.GetLogger(aLevel, arName);
	auto pPhys = new PhysicalLayerAsyncTCPServer(pLogger, mThreadPool.GetIOService(), arEndpoint, aPort);
	auto pChannel = new DNP3Channel(pLogger, aOpenRetry, pPhys, TimeSource::Inst(), [this](DNP3Channel* apChannel){ this->OnChannelShutdownCallback(apChannel); });
	mChannels.insert(pChannel);
	return pChannel;
}

void DNP3Manager::OnChannelShutdownCallback(DNP3Channel* apChannel)
{
	std::lock_guard<std::mutex> lock(mMutex);
	mChannels.erase(apChannel);
	delete apChannel;
}

}
}

