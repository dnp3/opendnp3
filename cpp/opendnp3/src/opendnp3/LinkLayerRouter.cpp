
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
// you under the terms of the License.
//

#include "LinkLayerRouter.h"

#include <sstream>
#include <assert.h>

#include <openpal/Exception.h>
#include <openpal/LoggableMacros.h>
#include <openpal/IPhysicalLayerAsync.h>

#include "ILinkContext.h"
#include "LinkFrame.h"

using namespace std;
using namespace openpal;

namespace opendnp3
{

LinkLayerRouter::LinkLayerRouter(Logger aLogger, IPhysicalLayerAsync* apPhys, openpal::TimeDuration aOpenRetry) :
	Loggable(aLogger),
	PhysicalLayerMonitor(aLogger, apPhys, aOpenRetry, aOpenRetry),
	mReceiver(aLogger, this),
	mTransmitting(false)
{}

bool LinkLayerRouter::IsRouteInUse(const LinkRoute& arRoute)
{
	return mAddressMap.find(arRoute) != mAddressMap.end();
}

bool LinkLayerRouter::AddContext(ILinkContext* apContext, const LinkRoute& arRoute)
{
	assert(apContext != nullptr);

	if(IsRouteInUse(arRoute)) return false;
	
	for(AddressMap::value_type v: mAddressMap) {
		if(apContext == v.second.pContext) return false;		
	}

	mAddressMap[arRoute] = ContextRecord(apContext); //context is always disabled by default
	
	return true;
}

bool LinkLayerRouter::EnableRoute(const LinkRoute& arRoute)
{
	auto iter = mAddressMap.find(arRoute);
	if(iter == mAddressMap.end()) return false;
	else {
		if(!iter->second.enabled)
		{
			iter->second.enabled = true;
			if(this->IsLowerLayerUp()) iter->second.pContext->OnLowerLayerUp();
			this->Start(); // idempotent call to start router
		}
		return true;
	}
}

bool LinkLayerRouter::DisableRoute(const LinkRoute& arRoute)
{
	auto iter = mAddressMap.find(arRoute);
	if(iter == mAddressMap.end()) return false;
	else {
		if(iter->second.enabled)
		{
			iter->second.enabled = false;
			if(this->IsLowerLayerUp()) iter->second.pContext->OnLowerLayerDown();
			if(!this->HasEnabledContext()) this->Suspend();
		}
		return true;
	}
}

void LinkLayerRouter::RemoveContext(const LinkRoute& arRoute)
{
	AddressMap::iterator i = mAddressMap.find(arRoute);
	if(i == mAddressMap.end()) {
		MACRO_THROW_EXCEPTION_COMPLEX(ArgumentException, "LinkRoute not bound: " << arRoute.ToString());
	}
	else {

		auto record = i->second;
		mAddressMap.erase(i);

		if(this->GetState() == CS_OPEN && record.enabled) record.pContext->OnLowerLayerDown();

		// if no contexts are enabled, suspend the router
		if(!HasEnabledContext()) this->Suspend();		
	}
}

ILinkContext* LinkLayerRouter::GetEnabledContext(const LinkRoute& arRoute)
{
	AddressMap::iterator i = mAddressMap.find(arRoute);
	if(i == mAddressMap.end()) return nullptr;
	else return (i->second.enabled) ? i->second.pContext : nullptr;	
}


ILinkContext* LinkLayerRouter::GetDestination(uint16_t aDest, uint16_t aSrc)
{
	LinkRoute route(aSrc, aDest);

	ILinkContext* pDest = GetEnabledContext(route);

	if(pDest == nullptr) {

		ERROR_BLOCK(LEV_WARNING, "Frame w/ unknown route: " << route.ToString(), DLERR_UNKNOWN_ROUTE);
	}
	
	return pDest;
}

//////////////////////////////////////////////////////
// IFrameSink Implementation
//////////////////////////////////////////////////////

void LinkLayerRouter::Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->Ack(aIsMaster, aIsRcvBuffFull, aDest, aSrc);
}
void LinkLayerRouter::Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->Nack(aIsMaster, aIsRcvBuffFull, aDest, aSrc);
}
void LinkLayerRouter::LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->LinkStatus(aIsMaster, aIsRcvBuffFull, aDest, aSrc);
}
void LinkLayerRouter::NotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->NotSupported(aIsMaster, aIsRcvBuffFull, aDest, aSrc);
}
void LinkLayerRouter::TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->TestLinkStatus(aIsMaster, aFcb, aDest, aSrc);
}
void LinkLayerRouter::ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->ResetLinkStates(aIsMaster, aDest, aSrc);
}
void LinkLayerRouter::RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->RequestLinkStatus(aIsMaster, aDest, aSrc);
}
void LinkLayerRouter::ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const ReadOnlyBuffer& arBuffer)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->ConfirmedUserData(aIsMaster, aFcb, aDest, aSrc, arBuffer);
}
void LinkLayerRouter::UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const ReadOnlyBuffer& arBuffer)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->UnconfirmedUserData(aIsMaster, aDest, aSrc, arBuffer);
}

void LinkLayerRouter::_OnReceive(const openpal::ReadOnlyBuffer& arBuffer)
{
	// The order is important here. You must let the receiver process the byte or another read could write
	// over the buffer before it is processed
	mReceiver.OnRead(arBuffer.Size()); //this may trigger callbacks to the local ILinkContext interface
	if(mpPhys->CanRead()) { // this is required because the call above could trigger the layer to be closed
		auto buff = mReceiver.WriteBuff();
		mpPhys->AsyncRead(buff); //start another read
	}
}

bool LinkLayerRouter::Transmit(const LinkFrame& arFrame)
{
	LinkRoute lr(arFrame.GetDest(), arFrame.GetSrc());

	if (this->GetEnabledContext(lr)) {				
		if(this->IsLowerLayerUp())
		{
			this->mTransmitQueue.push_back(arFrame);
			this->CheckForSend();
			return true;
		}
		else {
			ERROR_BLOCK(LEV_ERROR, "Cannot queue a frame while router if offline", DLERR_ROUTER_OFFLINE);
			return false;
		}
	}
	else {
		ERROR_BLOCK(LEV_ERROR, "Ignoring unassociated transmit w/ route: " << lr.ToString(), DLERR_UNKNOWN_ROUTE);		
		return false;
	}
}


void LinkLayerRouter::AddStateListener(std::function<void (ChannelState)> aListener)
{
	//this call comes from an unknown thread so marshall it the router's executor
	this->mpPhys->GetExecutor()->Post([this, aListener]() {
		mListeners.push_back(aListener);
		this->NotifyListener(aListener, this->GetState()); // event the current state now
	});
}

void LinkLayerRouter::OnStateChange(ChannelState aState)
{
for(auto listener: mListeners) NotifyListener(listener, aState);
}

bool LinkLayerRouter::HasEnabledContext()
{
	for(auto record: mAddressMap) {
		if(record.second.enabled) return true;
	}
	
	return false;
}

void LinkLayerRouter::NotifyListener(std::function<void (ChannelState)> aListener, ChannelState state)
{
	this->mpPhys->GetExecutor()->Post([ = ]() {
		aListener(state);
	});
}

void LinkLayerRouter::_OnSendSuccess()
{
	assert(mTransmitQueue.size() > 0);
	assert(mTransmitting);
	/* TODO - remove dead code
	const LinkFrame& f = mTransmitQueue.front();
	LinkRoute lr(f.GetDest(), f.GetSrc());
	ILinkContext* pContext = this->GetContext(lr);
	assert(pContext != nullptr);
	*/
	mTransmitting = false;
	mTransmitQueue.pop_front();
	this->CheckForSend();
}

void LinkLayerRouter::_OnSendFailure()
{
	LOG_BLOCK(LEV_ERROR, "Unexpected _OnSendFailure");
	mTransmitting = false;
	this->CheckForSend();
}

void LinkLayerRouter::CheckForSend()
{
	if(mTransmitQueue.size() > 0 && !mTransmitting && mpPhys->CanWrite()) {
		mTransmitting = true;
		const LinkFrame& f = mTransmitQueue.front();
		LOG_BLOCK(LEV_INTERPRET, "~> " << f.ToString());
		ReadOnlyBuffer buff(f.GetBuffer(), f.GetSize());
		mpPhys->AsyncWrite(buff);
	}
}

void LinkLayerRouter::OnPhysicalLayerOpenSuccessCallback()
{
	if(mpPhys->CanRead()) {		
		auto buff = mReceiver.WriteBuff();
		mpPhys->AsyncRead(buff);
	}

	for(AddressMap::value_type p: mAddressMap) {
		if(p.second.enabled) p.second.pContext->OnLowerLayerUp();		
	}
}

void LinkLayerRouter::OnPhysicalLayerCloseCallback()
{
	// reset the state of receiver
	mReceiver.Reset();

	// Drop frames queued for transmit and tell the contexts that the router has closed
	mTransmitting = false;
	mTransmitQueue.erase(mTransmitQueue.begin(), mTransmitQueue.end());
	for(auto pair: mAddressMap) {
		if(pair.second.enabled) pair.second.pContext->OnLowerLayerDown();		
	}
}

}

/* vim: set ts=4 sw=4: */
