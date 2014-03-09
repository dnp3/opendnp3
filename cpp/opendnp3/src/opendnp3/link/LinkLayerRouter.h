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

#include "opendnp3/link/PhysicalLayerMonitor.h"
#include "opendnp3/link/LinkLayerReceiver.h"
#include "opendnp3/link/LinkRoute.h"
#include "opendnp3/link/IFrameSink.h"
#include "opendnp3/link/ILinkRouter.h"
#include "opendnp3/link/IOpenDelayStrategy.h"


namespace openpal
{
class IPhysicalLayerAsync;
}

namespace opendnp3
{

class ILinkContext;
class LinkFrame;

//	Implements the parsing and de-multiplexing portion of
//	of DNP 3 Data Link Layer. PhysicalLayerMonitor inherits
// from IHandlerAsync, which inherits from IUpperLayer
class LinkLayerRouter : public PhysicalLayerMonitor, public IFrameSink, public ILinkRouter
{
public:

	LinkLayerRouter(openpal::Logger,
	                openpal::IPhysicalLayerAsync*,
	                openpal::TimeDuration minOpenRetry,
	                openpal::TimeDuration maxOpenRetry,
	                IOpenDelayStrategy* pStrategy = ExponentialBackoffStrategy::Inst());

	bool IsRouteInUse(const LinkRoute& arRoute);

	// Ties the lower part of the link layer to the upper part
	bool AddContext(ILinkContext*, const LinkRoute& arRoute);

	/**
	*  Tells the router to begin sending messages to the context associated with this route
	*/
	bool EnableRoute(const LinkRoute& arRoute);

	/**
	*  Tells the router to stop sending messages to the context associated with this route
	*  Does not remove the context entirely
	*/
	bool DisableRoute(const LinkRoute& arRoute);

	// This is safe to do at runtime, so long as the request happens from the io_service thread.
	void RemoveContext(const LinkRoute& arRoute);

	// Implement the IFrameSink interface - This is how the receiver pushes data
	void Ack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void Nack(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void LinkStatus(bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void NotSupported (bool aIsMaster, bool aIsRcvBuffFull, uint16_t aDest, uint16_t aSrc);
	void TestLinkStatus(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc);
	void ResetLinkStates(bool aIsMaster, uint16_t aDest, uint16_t aSrc);
	void RequestLinkStatus(bool aIsMaster, uint16_t aDest, uint16_t aSrc);
	void ConfirmedUserData(bool aIsMaster, bool aFcb, uint16_t aDest, uint16_t aSrc, const openpal::ReadOnlyBuffer& arBuffer);
	void UnconfirmedUserData(bool aIsMaster, uint16_t aDest, uint16_t aSrc, const openpal::ReadOnlyBuffer& arBuffer);

	// ILinkRouter interface
	bool Transmit(const LinkFrame&);

	// Notify the listener when the state changes
	void AddStateListener(std::function<void (ChannelState)> aListener);

protected:

	// override this function so that we can notify listeners
	void OnStateChange(ChannelState aState);

private:

	bool HasEnabledContext();

	class ContextRecord
	{
	public:
		ContextRecord(ILinkContext* context) : pContext(context), enabled(false)
		{}

		ContextRecord() : pContext(nullptr), enabled(false)
		{}

		ILinkContext* pContext;
		bool enabled;
	};

	void NotifyListener(std::function<void (ChannelState)> aListener, ChannelState state);

	std::vector<std::function<void (ChannelState)>> mListeners;

	ILinkContext* GetDestination(uint16_t aDest, uint16_t aSrc);
	ILinkContext* GetEnabledContext(const LinkRoute&);

	void CheckForSend();


	typedef std::map<LinkRoute, ContextRecord, LinkRoute::LessThan> AddressMap;
	typedef std::deque<LinkFrame> TransmitQueue;

	AddressMap mAddressMap;
	TransmitQueue mTransmitQueue;

	// Handles the parsing of incoming frames
	LinkLayerReceiver mReceiver;
	bool mTransmitting;

	/* Events - NVII delegates from IUpperLayer */

	// Called when the physical layer has read data into to the requested buffer
	void _OnReceive(const openpal::ReadOnlyBuffer&);
	void _OnSendSuccess();
	void _OnSendFailure();

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

