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
#ifndef __LINK_LAYER_ROUTER_H_
#define __LINK_LAYER_ROUTER_H_

#include <map>
#include <queue>
#include <vector>

#include <openpal/IShutdownHandler.h>
#include <openpal/IEventHandler.h>
#include <openpal/StaticLinkedList.h>
#include <openpal/StaticQueue.h>

#include "opendnp3/link/PhysicalLayerMonitor.h"
#include "opendnp3/link/LinkLayerReceiver.h"
#include "opendnp3/link/LinkRoute.h"
#include "opendnp3/link/IFrameSink.h"
#include "opendnp3/link/ILinkRouter.h"
#include "opendnp3/link/IOpenDelayStrategy.h"

#include "opendnp3/StaticSizeConfiguration.h"

namespace openpal
{
class IPhysicalLayerAsync;
}

namespace opendnp3
{

class ILinkContext;
class LinkFrame;

// Implements the parsing and de-multiplexing portion of
// of DNP 3 Data Link Layer. PhysicalLayerMonitor inherits
// from IHandlerAsync, which inherits from IUpperLayer
class LinkLayerRouter : public PhysicalLayerMonitor, public ILinkRouter, private IFrameSink
{
public:

	LinkLayerRouter(const openpal::Logger&,
	                openpal::IPhysicalLayerAsync*,
	                openpal::TimeDuration minOpenRetry,
	                openpal::TimeDuration maxOpenRetry,
					openpal::IEventHandler<ChannelState>* pStateHandler = nullptr,
					openpal::IShutdownHandler* pShutdownHandler = nullptr,
	                IOpenDelayStrategy* pStrategy = ExponentialBackoffStrategy::Inst());

	// Query to see if a route is in use
	bool IsRouteInUse(const LinkRoute& route);

	// Ties the lower part of the link layer to the upper part
	bool AddContext(ILinkContext* pContext, const LinkRoute& route);

	/**
	*  Tells the router to begin sending messages to the context
	*/
	bool Enable(ILinkContext* pContext);

	/**
	*  Tells the router to stop sending messages to the context associated with this route
	*  Does not remove the context entirely
	*/
	bool Disable(ILinkContext* pContext);

	/**
	* This is safe to do at runtime, so long as the request happens from the executor
	*/
	bool Remove(ILinkContext* pContext);

	// ------------ IFrameSink -----------------

	virtual void Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) override final;
	virtual void Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) override final;
	virtual void LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) override final;
	virtual void NotSupported(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc) override final;
	virtual void TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc) override final;
	virtual void ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc) override final;
	virtual void RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc) override final;
	virtual void ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const openpal::ReadOnlyBuffer& arBuffer) override final;
	virtual void UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const openpal::ReadOnlyBuffer& arBuffer) override final;

	// ------------ ILinkRouter -----------------

	virtual void QueueTransmit(const openpal::ReadOnlyBuffer& buffer, ILinkContext* pContext, bool primary) override final;

	// ------------ IUpperLayer -----------------

	virtual void OnReceive(const openpal::ReadOnlyBuffer&) override final;
	virtual void OnSendResult(bool success) override final;

protected:
	
	void OnShutdown() override final;

private:

	void OnStateChange(ChannelState aState);

	bool HasEnabledContext();

	struct Record
	{
		Record(ILinkContext* context, const LinkRoute& route_) : 
			pContext(context), 
			route(route_),
			enabled(false)
		{}

		Record() : pContext(nullptr), enabled(false)
		{}

		ILinkContext* pContext;
		LinkRoute route;
		bool enabled;
	};	

	struct Transmission
	{
		Transmission(const openpal::ReadOnlyBuffer& buffer_, ILinkContext* pContext_, bool primary_) :
			buffer(buffer_),
			pContext(pContext_),
			primary(primary_)
		{}

		Transmission() : buffer(), pContext(nullptr), primary(false)
		{}

		openpal::ReadOnlyBuffer buffer;
		ILinkContext* pContext;
		bool primary;
	};

	ILinkContext* GetDestination(uint16_t aDest, uint16_t aSrc);
	ILinkContext* GetEnabledContext(const LinkRoute&);

	void CheckForSend();	
	
	openpal::IEventHandler<ChannelState>* pStateHandler;
	openpal::IShutdownHandler* pShutdownHandler;
	
	openpal::StaticLinkedList<Record, uint16_t, sizes::MAX_STACKS_PER_CHANNEL> records;
	openpal::StaticQueue<Transmission, uint16_t, sizes::MAX_STACKS_PER_CHANNEL> transmitQueue;	

	// Handles the parsing of incoming frames
	LinkLayerReceiver mReceiver;
	bool mTransmitting;

	// Implement virtual AsyncPhysLayerMonitor
	void OnPhysicalLayerOpenSuccessCallback();
	void OnPhysicalLayerOpenFailureCallback() {}
	void OnPhysicalLayerCloseCallback();

	std::string RecvString()
	{
		return "<~";
	}


};

}



#endif

