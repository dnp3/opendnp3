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

#include "AsyncStackManager.h"
#include "LinkChannel.h"

#include <boost/asio.hpp>

#include <APL/ASIOExecutor.h>
#include <APL/Exception.h>
#include <APL/Logger.h>
#include <APL/IPhysicalLayerAsync.h>
#include <APL/AsyncTaskGroup.h>
#include <APL/GetKeys.h>

#include <DNP3/MasterStack.h>
#include <DNP3/SlaveStack.h>
#include <DNP3/DeviceTemplate.h>
#include <DNP3/VtoRouter.h>
#include <DNP3/VtoConfig.h>

#include <iostream>


using namespace std;

namespace apl
{
namespace dnp
{

AsyncStackManager::AsyncStackManager(Logger* apLogger) :
	Loggable(apLogger),
	mService(),
	mExecutor(mService.Get()),
	mMgr(apLogger->GetSubLogger("channels", LEV_WARNING), mService.Get()),
	mScheduler(&mExecutor),
	mVtoManager(apLogger->GetSubLogger("vto"), &mExecutor, &mMgr),
	mThread(this),
	mpInfiniteTimer(mExecutor.StartInfinite()),
	mIsShutdown(false)
{
	mThread.Start();
}

AsyncStackManager::~AsyncStackManager()
{
	/*
	 * Tell every channel to stop
	 */
	this->Shutdown();

}

std::vector<std::string> AsyncStackManager::GetStackNames()
{

	return GetKeys<StackMap, string>(mStackMap);
}

std::vector<std::string> AsyncStackManager::GetPortNames()
{
	return GetKeys<ChannelToChannelMap, string>(mChannelNameToChannel);
}

void AsyncStackManager::AddTCPClient(const std::string& arName, PhysLayerSettings aSettings, const std::string& arAddr, uint16_t aPort)
{
	this->ThrowIfAlreadyShutdown();
	mMgr.AddTCPClient(arName, aSettings, arAddr, aPort);
}

void AsyncStackManager::AddTCPServer(const std::string& arName, PhysLayerSettings aSettings, const std::string& arEndpoint, uint16_t aPort)
{
	this->ThrowIfAlreadyShutdown();
	mMgr.AddTCPServer(arName, aSettings, arEndpoint, aPort);
}

void AsyncStackManager::AddSerial(const std::string& arName, PhysLayerSettings aSettings, SerialSettings aSerial)
{
	this->ThrowIfAlreadyShutdown();
	mMgr.AddSerial(arName, aSettings, aSerial);
}

// Adds a custom physical layer to the stack
void AsyncStackManager::AddPhysicalLayer(const std::string& arName, PhysLayerSettings aSettings, IPhysicalLayerAsync* apPhys)
{
	this->ThrowIfAlreadyShutdown();
	mMgr.AddPhysicalLayer(arName, aSettings, apPhys);
}

ICommandAcceptor* AsyncStackManager::AddMaster( const std::string& arPortName, const std::string& arStackName, FilterLevel aLevel, IDataObserver* apPublisher,
        const MasterStackConfig& arCfg)
{
	this->ThrowIfAlreadyShutdown();
	LinkChannel* pChannel = this->GetOrCreateChannel(arPortName);
	Logger* pLogger = mpLogger->GetSubLogger(arStackName, aLevel);
	pLogger->SetVarName(arStackName);

	MasterStack* pMaster = new MasterStack(pLogger, &mExecutor, apPublisher, pChannel->GetGroup(), arCfg);
	LinkRoute route(arCfg.link.RemoteAddr, arCfg.link.LocalAddr);

	this->AddStackToChannel(arStackName, pMaster, pChannel, route);

	// add any vto routers we've configured
	for(VtoRouterConfig s: arCfg.vto.mRouterConfigs) {
		this->StartVtoRouter(s.mPhysicalLayerName, arStackName, s.mSettings);
	}

	return pMaster->mMaster.GetCmdAcceptor();
}

IDataObserver* AsyncStackManager::AddSlave( const std::string& arPortName, const std::string& arStackName, FilterLevel aLevel, ICommandAcceptor* apCmdAcceptor,
        const SlaveStackConfig& arCfg)
{
	this->ThrowIfAlreadyShutdown();
	LinkChannel* pChannel = this->GetOrCreateChannel(arPortName);
	Logger* pLogger = mpLogger->GetSubLogger(arStackName, aLevel);
	pLogger->SetVarName(arStackName);

	SlaveStack* pSlave = new SlaveStack(pLogger, &mExecutor, apCmdAcceptor, arCfg);

	LinkRoute route(arCfg.link.RemoteAddr, arCfg.link.LocalAddr);
	this->AddStackToChannel(arStackName, pSlave, pChannel, route);

	// add any vto routers we've configured
	for(VtoRouterConfig s: arCfg.vto.mRouterConfigs) {
		this->StartVtoRouter(s.mPhysicalLayerName, arStackName, s.mSettings);
	}

	return pSlave->mSlave.GetDataObserver();
}

void AsyncStackManager::AddVtoChannel(const std::string& arStackName,
                                      IVtoCallbacks* apCallbacks)
{
	this->ThrowIfAlreadyShutdown();
	StackRecord rec = this->GetStackRecordByName(arStackName);
	rec.stack->GetVtoWriter()->AddVtoCallback(apCallbacks);
	rec.stack->GetVtoReader()->AddVtoChannel(apCallbacks);
}

void AsyncStackManager::RemoveVtoChannel(const std::string& arStackName, IVtoCallbacks* apCallbacks)
{
	this->ThrowIfAlreadyShutdown();
	StackRecord rec = this->GetStackRecordByName(arStackName);
	rec.stack->GetVtoWriter()->RemoveVtoCallback(apCallbacks);
	rec.stack->GetVtoReader()->RemoveVtoChannel(apCallbacks);
}

void AsyncStackManager::StartVtoRouter(const std::string& arPortName,
                                       const std::string& arStackName, const VtoRouterSettings& arSettings)
{
	this->ThrowIfAlreadyShutdown();
	StackRecord rec = this->GetStackRecordByName(arStackName);
	VtoRouter* pRouter = mVtoManager.StartRouter(arPortName, arSettings, rec.stack->GetVtoWriter());
	this->AddVtoChannel(arStackName, pRouter);
}

void AsyncStackManager::StopVtoRouter(const std::string& arStackName, uint8_t aVtoChannelId)
{
	this->ThrowIfAlreadyShutdown();
	IVtoWriter* pWriter = this->GetVtoWriter(arStackName);
	RouterRecord rec = mVtoManager.GetRouterOnWriter(pWriter, aVtoChannelId);
	this->RemoveVtoChannel(arStackName, rec.mpRouter.get());
	mVtoManager.StopRouter(pWriter, aVtoChannelId);
}

void AsyncStackManager::StopAllRoutersOnStack(const std::string& arStackName)
{
	this->ThrowIfAlreadyShutdown();
	IVtoWriter* pWriter = this->GetVtoWriter(arStackName);
	this->mVtoManager.StopAllRoutersOnWriter(pWriter);
}

IVtoWriter* AsyncStackManager::GetVtoWriter(const std::string& arStackName)
{
	this->ThrowIfAlreadyShutdown();
	return this->GetStackRecordByName(arStackName).stack->GetVtoWriter();
}

// Remove a port and all associated stacks
void AsyncStackManager::RemovePort(const std::string& arPortName)
{
	this->ThrowIfAlreadyShutdown();
	LinkChannel* pChannel = this->GetChannelMaybeNull(arPortName);
	if(pChannel != NULL) { // the channel is in use
		std::auto_ptr<LinkChannel> autoDeleteChannel(pChannel); //will delete at end of function
		mChannelNameToChannel.erase(arPortName);

		mExecutor.Synchronize([pChannel](){
			// Tell the channel to shut down permanenently
			pChannel->GetGroup()->Shutdown(); // no more task callbacks
			pChannel->BeginShutdown();
		});
		pChannel->WaitUntilShutdown();

		vector<string> stacks = pChannel->StacksOnChannel();
		for(string s: stacks) {
			this->RemoveStack(s);
		}
		this->mScheduler.ReleaseGroup(pChannel->GetGroup());
	}

	// remove the physical layer from the list
	mMgr.Remove(arPortName);
}

void AsyncStackManager::RemoveStack(const std::string& arStackName)
{
	this->ThrowIfAlreadyShutdown();
	std::auto_ptr<Stack> pStack(this->SeverStackFromChannel(arStackName));
	mVtoManager.StopAllRoutersOnWriter(pStack->GetVtoWriter());
}

AsyncStackManager::StackRecord AsyncStackManager::GetStackRecordByName(const std::string& arStackName)
{
	StackMap::iterator i = mStackMap.find(arStackName);
	if (i == mStackMap.end()) throw ArgumentException(LOCATION, "Unknown stack");
	return i->second;
}

void AsyncStackManager::ThrowIfAlreadyShutdown()
{
	if(mIsShutdown) throw InvalidStateException(LOCATION, "Stack has been permanently shutdown");
}

void AsyncStackManager::Shutdown()
{
	if(!mIsShutdown) {

		vector<string> ports = this->GetPortNames();
		for(string s: ports) {
			LOG_BLOCK(LEV_DEBUG, "Removing port: " << s);
			this->RemovePort(s);
			LOG_BLOCK(LEV_DEBUG, "Done removing Port: " << s);
		}

		// if we've cleaned up correctly, canceling the infinite timer will cause the thread to stop executing
		mpInfiniteTimer->Cancel();
		LOG_BLOCK(LEV_DEBUG, "Joining on io_service thread");
		mThread.WaitForStop();
		LOG_BLOCK(LEV_DEBUG, "Join complete on io_service thread");

		mIsShutdown = true;
	}
}

LinkChannel* AsyncStackManager::GetOrCreateChannel(const std::string& arName)
{
	LinkChannel* pChannel = this->GetChannelMaybeNull(arName);
	return (pChannel == NULL) ? this->CreateChannel(arName) : pChannel;
}

LinkChannel* AsyncStackManager::GetChannelOrExcept(const std::string& arName)
{
	LinkChannel* pChannel = this->GetChannelMaybeNull(arName);
	if(pChannel == NULL) throw ArgumentException(LOCATION, "Channel doesn't exist: " + arName);
	return pChannel;
}

LinkChannel* AsyncStackManager::CreateChannel(const std::string& arName)
{
	if(GetChannelMaybeNull(arName) != NULL) throw ArgumentException(LOCATION, "Channel already exists with name: " + arName);

	PhysLayerSettings s = mMgr.GetSettings(arName);
	IPhysicalLayerAsync* pPhys = mMgr.AcquireLayer(arName);
	Logger* pChannelLogger = mpLogger->GetSubLogger(arName, s.LogLevel);
	pChannelLogger->SetVarName(arName);
	AsyncTaskGroup* pGroup = mScheduler.CreateNewGroup();

	LinkChannel* pChannel = new LinkChannel(pChannelLogger, arName, &mExecutor, pPhys, pGroup, s.RetryTimeout);
	if(s.mpObserver) pChannel->AddPhysicalLayerObserver(s.mpObserver);
	mChannelNameToChannel[arName] = pChannel;
	return pChannel;
}

LinkChannel* AsyncStackManager::GetChannelMaybeNull(const std::string& arName)
{
	ChannelToChannelMap::iterator i = mChannelNameToChannel.find(arName);
	return (i == mChannelNameToChannel.end()) ? NULL : i->second;
}

void AsyncStackManager::Run()
{
	size_t num = 0;

	do {
		try {
			num = mService.Get()->run();
		}
		catch(const std::exception& ex) {
			LOG_BLOCK(LEV_ERROR, "Unhandled exception: " << ex.what());
		}
	}
	while(num > 0);

	mService.Get()->reset();
}

Stack* AsyncStackManager::SeverStackFromChannel(const std::string& arStackName)
{
	StackMap::iterator i = mStackMap.find(arStackName);
	if(i == mStackMap.end()) throw ArgumentException(LOCATION, "Stack not found: " + arStackName);

	StackRecord rec = i->second;
	mStackMap.erase(i);

	LOG_BLOCK(LEV_DEBUG, "Begin severing stack: " << arStackName);
	mExecutor.Synchronize([&](){ //this action needs to be safely on correct executor		
		rec.channel->RemoveStackFromChannel(arStackName);
	});
	LOG_BLOCK(LEV_DEBUG, "Done severing stack: " << arStackName);

	return rec.stack;
}

void AsyncStackManager::AddStackToChannel(const std::string& arStackName, Stack* apStack, LinkChannel* apChannel, const LinkRoute& arRoute)
{	
	mExecutor.Synchronize([&](){		
		apChannel->BindStackToChannel(arStackName, apStack, arRoute);
	});

	mStackMap[arStackName] = StackRecord(apStack, apChannel);
}

}
}

/* vim: set ts=4 sw=4: */
