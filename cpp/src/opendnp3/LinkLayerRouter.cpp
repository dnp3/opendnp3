
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
using namespace std::chrono;

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

void LinkLayerRouter::AddContext(ILinkContext* apContext, const LinkRoute& arRoute)
{
	assert(apContext != NULL);

	if(IsRouteInUse(arRoute)) {		
		MACRO_THROW_EXCEPTION_COMPLEX(ArgumentException, "Route already in use: " << arRoute);
	}

	for(AddressMap::value_type v: mAddressMap) {
		if(apContext == v.second) {
			MACRO_THROW_EXCEPTION_COMPLEX(ArgumentException, "Context already is bound to route:  " << v.first);
		}
	}

	mAddressMap[arRoute] = apContext;	
	if(this->GetState() == CS_OPEN) apContext->OnLowerLayerUp();
	this->Start();
}

void LinkLayerRouter::RemoveContext(const LinkRoute& arRoute)
{
	AddressMap::iterator i = mAddressMap.find(arRoute);
	if(i == mAddressMap.end()) {
		MACRO_THROW_EXCEPTION_COMPLEX(ArgumentException, "LinkRoute not bound: " << arRoute.ToString());
	}
	else {

		ILinkContext* pContext = i->second;
		mAddressMap.erase(i);

		if(this->GetState() == CS_OPEN) pContext->OnLowerLayerDown();

		// if no stacks are bound, suspend the router
		if(mAddressMap.size() == 0) {
			this->Suspend();
		}
	}


}

ILinkContext* LinkLayerRouter::GetContext(const LinkRoute& arRoute)
{
	AddressMap::iterator i = mAddressMap.find(arRoute);
	return (i == mAddressMap.end()) ? NULL : i->second;
}


ILinkContext* LinkLayerRouter::GetDestination(uint16_t aDest, uint16_t aSrc)
{
	LinkRoute route(aSrc, aDest);

	ILinkContext* pDest = GetContext(route);

	if(pDest == NULL && mLogger.IsEnabled(LEV_WARNING)) {

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
		std::ostringstream oss;
		oss << "Frame w/ unknown route: " << route;
		LogEntry le(LEV_WARNING, mLogger.GetName(), LOCATION, oss.str(), DLERR_UNKNOWN_ROUTE);
#else
		LogEntry le(LEV_WARNING, mpLogger->GetName(), LOCATION, "", DLERR_UNKNOWN_ROUTE);
#endif
		le.AddValue("SOURCE", aSrc);
		le.AddValue("DESTINATION", aDest);
		mLogger.Log(le);
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
void LinkLayerRouter::ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const uint8_t* apData, size_t aDataLength)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->ConfirmedUserData(aIsMaster, aFcb, aDest, aSrc, apData, aDataLength);
}
void LinkLayerRouter::UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const uint8_t* apData, size_t aDataLength)
{
	ILinkContext* pDest = GetDestination(aDest, aSrc);
	if(pDest) pDest->UnconfirmedUserData(aIsMaster, aDest, aSrc, apData, aDataLength);
}

void LinkLayerRouter::_OnReceive(const uint8_t*, size_t aNumBytes)
{
	// The order is important here. You must let the receiver process the byte or another read could write
	// over the buffer before it is processed
	mReceiver.OnRead(aNumBytes); //this may trigger callbacks to the local ILinkContext interface
	if(mpPhys->CanRead()) { // this is required because the call above could trigger the layer to be closed
		mpPhys->AsyncRead(mReceiver.WriteBuff(), mReceiver.NumWriteBytes()); //start another read
	}
}

void LinkLayerRouter::Transmit(const LinkFrame& arFrame)
{
	LinkRoute lr(arFrame.GetDest(), arFrame.GetSrc());

	if (this->GetContext(lr)) {
		if (!this->IsLowerLayerUp()) {
			MACRO_THROW_EXCEPTION(InvalidStateException, "LowerLayerDown");
		}
		this->mTransmitQueue.push_back(arFrame);
		this->CheckForSend();
	}
	else {
		MACRO_THROW_EXCEPTION_COMPLEX(ArgumentException, "Unassociated context w/ route: " << lr);
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
	const LinkFrame& f = mTransmitQueue.front();
	LinkRoute lr(f.GetDest(), f.GetSrc());
	ILinkContext* pContext = this->GetContext(lr);
	assert(pContext != NULL);
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
	if(mTransmitQueue.size() > 0 && !mTransmitting) {
		mTransmitting = true;
		const LinkFrame& f = mTransmitQueue.front();
		LOG_BLOCK(LEV_INTERPRET, "~> " << f.ToString());
		mpPhys->AsyncWrite(f.GetBuffer(), f.GetSize());
	}
}

void LinkLayerRouter::OnPhysicalLayerOpenSuccessCallback()
{
	if(mpPhys->CanRead())
		mpPhys->AsyncRead(mReceiver.WriteBuff(), mReceiver.NumWriteBytes());

for(AddressMap::value_type p: mAddressMap) {
		p.second->OnLowerLayerUp();
	}
}

void LinkLayerRouter::OnPhysicalLayerCloseCallback()
{
	// reset the state of receiver
	mReceiver.Reset();

	// Drop frames queued for transmit and tell the contexts that the router has closed
	mTransmitting = false;
	mTransmitQueue.erase(mTransmitQueue.begin(), mTransmitQueue.end());
	for(auto pair: mAddressMap) pair.second->OnLowerLayerDown();
}

}

/* vim: set ts=4 sw=4: */
