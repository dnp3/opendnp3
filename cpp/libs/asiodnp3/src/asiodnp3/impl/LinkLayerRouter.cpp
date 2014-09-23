/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#include "LinkLayerRouter.h"

#include <assert.h>

#include <openpal/logging/LogMacros.h>
#include <openpal/channel/IPhysicalLayer.h>

#include <opendnp3/LogLevels.h>
#include <opendnp3/link/ILinkContext.h>
#include <opendnp3/link/LinkFrame.h>

#include <algorithm>

using namespace std;
using namespace openpal;
using namespace opendnp3;

namespace asiodnp3
{

LinkLayerRouter::LinkLayerRouter(	openpal::LogRoot& root,
									openpal::IExecutor& executor,
                                    IPhysicalLayer* pPhys,
                                    openpal::TimeDuration minOpenRetry,
                                    openpal::TimeDuration maxOpenRetry,
									IChannelStateListener* pStateHandler_,                                    
									IOpenDelayStrategy& strategy,
									LinkChannelStatistics* pStatistics_) :

	PhysicalLayerMonitor(root, executor, pPhys, minOpenRetry, maxOpenRetry, strategy),
	pStateHandler(pStateHandler_),	
	pStatistics(pStatistics_),
	parser(logger, pStatistics_),
	isTransmitting(false)
{}

void LinkLayerRouter::SetShutdownHandler(const Action0& action)
{
	this->shutdownHandler = action;
}

bool LinkLayerRouter::IsRouteInUse(const Route& route)
{
	auto matches = [route](const Record & record)
	{
		return record.route.Equals(route);
	};

	auto iter = std::find_if(records.begin(), records.end(), matches);
	return iter != records.end();
}

bool LinkLayerRouter::AddContext(ILinkContext* pContext, const Route& route)
{
	assert(pContext != nullptr);

	if (IsRouteInUse(route))
	{
		return false;
	}
	else
	{
		auto matches = [pContext](const Record & record)
		{
			return record.pContext == pContext;
		};		

		auto iter = std::find_if(records.begin(), records.end(), matches);

		if (iter == records.end())
		{
			// record is always disabled by default
			Record(pContext, route);
			records.push_back(Record(pContext, route));	
			return true;
		}
		else
		{
			SIMPLE_LOG_BLOCK(logger, flags::ERR, "Context cannot be bound 2x");
			return false;
		}
	}
}

bool LinkLayerRouter::Enable(ILinkContext* pContext)
{
	auto matches = [pContext](const Record & rec) { return rec.pContext == pContext; };
	auto iter = std::find_if(records.begin(), records.end(), matches);

	if(iter != records.end())
	{
		if(iter->enabled)
		{
			// already enabled
			return true;			
		}
		else
		{
			iter->enabled = true;

			if (this->IsOnline())
			{
				iter->pContext->OnLowerLayerUp();
			}

			this->Start(); // idempotent call to start router

			return true;
		}
		
	}
	else
	{
		return false;
	}
}

bool LinkLayerRouter::Disable(ILinkContext* pContext)
{
	auto matches = [pContext](const Record & rec) { return rec.pContext == pContext; };

	auto iter = std::find_if(records.begin(), records.end(), matches);

	if (iter != records.end())
	{
		if (iter->enabled)
		{
			iter->enabled = false;

			if (this->IsOnline())
			{
				iter->pContext->OnLowerLayerDown();
			}

			if (!this->HasEnabledContext())
			{
				this->Suspend();
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool LinkLayerRouter::Remove(ILinkContext* pContext)
{
	auto matches = [pContext](const Record & rec) { return rec.pContext == pContext; };
	auto iter = std::find_if(records.begin(), records.end(), matches);

	if(iter != records.end())
	{
		if (this->GetState() == ChannelState::OPEN && iter->enabled)
		{
			iter->pContext->OnLowerLayerDown();
		}

		records.erase(iter);

		// if no contexts are enabled, suspend the router
		if (!HasEnabledContext())
		{
			this->Suspend();
		}
		
		return true;
	}
	else
	{
		return false;
	}
}

ILinkContext* LinkLayerRouter::GetEnabledContext(const Route& route)
{
	auto matches = [route](const Record & rec) { return rec.enabled && rec.route.Equals(route); };	
	auto iter = std::find_if(records.begin(), records.end(), matches);
	if (iter == records.end())
	{
		return nullptr;
	}
	else
	{
		return iter->pContext;
	}	
}


ILinkContext* LinkLayerRouter::GetDestination(uint16_t dest, uint16_t src)
{
	Route route(src, dest);

	ILinkContext* pDest = GetEnabledContext(route);

	if(pDest == nullptr)
	{
		FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::WARN, DLERR_UNKNOWN_ROUTE, "Frame w/ unknown route, source: %i, dest %i", route.source, route.destination);
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

void LinkLayerRouter::OnReceive(const openpal::ReadOnlyBuffer& input)
{
	// The order is important here. You must let the receiver process the byte or another read could write
	// over the buffer before it is processed	
	parser.OnRead(input.Size(), this); //this may trigger callbacks to the local ILinkContext interface
	if(pPhys->CanRead())   // this is required because the call above could trigger the layer to be closed
	{
		auto buff = parser.WriteBuff();
		pPhys->BeginRead(buff); //start another read
	}	
}

void LinkLayerRouter::BeginTransmit(const openpal::ReadOnlyBuffer& buffer, ILinkContext* pContext)
{
	if (this->IsOnline())
	{
		Transmission tx(buffer, pContext);

		transmitQueue.push_back(tx);
		this->CheckForSend();		
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "Router received transmit request while offline");
	}
}

void LinkLayerRouter::OnStateChange(ChannelState state)
{
	if (this->pStateHandler)
	{
		pStateHandler->OnStateChange(state);
	}
}

void LinkLayerRouter::OnShutdown()
{
	shutdownHandler.Apply();	
}

bool LinkLayerRouter::HasEnabledContext()
{
	auto matches = [](const Record & rec) { return rec.enabled; };
	auto iter = std::find_if(records.begin(), records.end(), matches);
	return iter != records.end();
}

void LinkLayerRouter::OnSendResult(bool result)
{
	assert(!transmitQueue.empty());
	assert(isTransmitting);
	isTransmitting = false;

	auto pTx = transmitQueue.front();
	transmitQueue.pop_front();
	pTx.pContext->OnTransmitResult(result);
	this->CheckForSend();
}

void LinkLayerRouter::CheckForSend()
{
	if (!transmitQueue.empty() && !isTransmitting && pPhys->CanWrite())
	{
		if (pStatistics) ++pStatistics->numLinkFrameTx;
		auto tx = transmitQueue.front();
		isTransmitting = true;
		pPhys->BeginWrite(tx.buffer);
	}
}

void LinkLayerRouter::OnPhysicalLayerOpenSuccessCallback()
{
	if(pPhys->CanRead())
	{		
		auto buff = parser.WriteBuff();
		pPhys->BeginRead(buff);
	}

	for (auto& rec : records)
	{
		if (rec.enabled)
		{
			rec.pContext->OnLowerLayerUp();
		}
	}
}

void LinkLayerRouter::OnPhysicalLayerCloseCallback()
{	
	// reset the state of receiver
	parser.Reset();

	// Drop frames queued for transmit and tell the contexts that the router has closed
	isTransmitting = false;
	transmitQueue.clear();

	for (auto& rec : records)
	{
		if (rec.enabled)
		{
			rec.pContext->OnLowerLayerDown();
		}
	}	
}

}


