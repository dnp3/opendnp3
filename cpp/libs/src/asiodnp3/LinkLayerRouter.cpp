/*
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
#include <opendnp3/link/ILinkSession.h>
#include <opendnp3/link/LinkFrame.h>

#include <algorithm>

using namespace std;
using namespace openpal;
using namespace opendnp3;

namespace asiodnp3
{

LinkLayerRouter::LinkLayerRouter(	openpal::Logger logger,
                                    openpal::IExecutor& executor,
                                    IPhysicalLayer* pPhys,
                                    const ChannelRetry& retry,
                                    IChannelStateListener* pStateHandler_,
                                    LinkChannelStatistics* pStatistics_) :

	PhysicalLayerMonitor(logger, executor, pPhys, retry),
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

bool LinkLayerRouter::AddContext(ILinkSession& context, const Route& route)
{
	if (IsRouteInUse(route))
	{
		return false;
	}
	else
	{
		auto matches = [&](const Record & record)
		{
			return record.pContext == &context;
		};

		auto iter = std::find_if(records.begin(), records.end(), matches);

		if (iter == records.end())
		{
			// record is always disabled by default
			records.push_back(Record(&context, route));
			return true;
		}
		else
		{
			SIMPLE_LOG_BLOCK(logger, flags::ERR, "Context cannot be bound 2x");
			return false;
		}
	}
}

bool LinkLayerRouter::Enable(ILinkSession* pContext)
{
	auto matches = [pContext](const Record & rec)
	{
		return rec.pContext == pContext;
	};
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

bool LinkLayerRouter::Disable(ILinkSession* pContext)
{
	auto matches = [pContext](const Record & rec)
	{
		return rec.pContext == pContext;
	};

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

bool LinkLayerRouter::Remove(ILinkSession* pContext)
{
	auto matches = [pContext](const Record & rec)
	{
		return rec.pContext == pContext;
	};
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

ILinkSession* LinkLayerRouter::GetEnabledContext(const Route& route)
{
	auto matches = [route](const Record & rec)
	{
		return rec.enabled && rec.route.Equals(route);
	};
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


ILinkSession* LinkLayerRouter::GetDestination(uint16_t dest, uint16_t src)
{
	Route route(src, dest);

	ILinkSession* pDest = GetEnabledContext(route);

	if(pDest == nullptr)
	{
		FORMAT_LOG_BLOCK_WITH_CODE(logger, flags::WARN, DLERR_UNKNOWN_ROUTE, "Frame w/ unknown route, source: %i, dest %i", route.source, route.destination);
	}

	return pDest;
}

//////////////////////////////////////////////////////
// IFrameSink Implementation
//////////////////////////////////////////////////////

bool LinkLayerRouter::OnFrame(const LinkHeaderFields& header, const openpal::RSlice& userdata)
{
	ILinkSession* pDest = GetDestination(header.dest, header.src);
	if (pDest)
	{
		return pDest->OnFrame(header, userdata);
	}
	else
	{
		return false;
	}
}

void LinkLayerRouter::OnReceive(const openpal::RSlice& input)
{
	// The order is important here. You must let the receiver process the byte or another read could write
	// over the buffer before it is processed
	parser.OnRead(input.Size(), *this); //this may trigger callbacks to the local ILinkSession interface
	if(pPhys->CanRead())   // this is required because the call above could trigger the layer to be closed
	{
		auto buff = parser.WriteBuff();
		pPhys->BeginRead(buff); //start another read
	}
}

void LinkLayerRouter::BeginTransmit(const openpal::RSlice& buffer, ILinkSession& context)
{
	if (this->IsOnline())
	{
		Transmission tx(buffer, &context);

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
	auto matches = [](const Record & rec)
	{
		return rec.enabled;
	};
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

	taskLock.SetOnline();

	for (auto & rec : records)
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

	taskLock.SetOffline();

	for (auto & rec : records)
	{
		if (rec.enabled)
		{
			rec.pContext->OnLowerLayerDown();
		}
	}
}

}
