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
#ifndef ASIODNP3_LINKLAYERROUTER_H
#define ASIODNP3_LINKLAYERROUTER_H

#include "asiodnp3/PhysicalLayerMonitor.h"

#include <opendnp3/Route.h>
#include <opendnp3/link/LinkLayerParser.h>
#include <opendnp3/link/IFrameSink.h>
#include <opendnp3/link/ILinkTx.h>
#include <opendnp3/link/ChannelRetry.h>
#include <opendnp3/link/IChannelStateListener.h>
#include <opendnp3/master/MultidropTaskLock.h>

#include <vector>
#include <deque>

namespace openpal
{
class IPhysicalLayer;
}

namespace opendnp3
{

class ILinkSession;
class LinkFrame;

}

namespace asiodnp3
{

// Implements the parsing and de-multiplexing portion of
// of DNP 3 Data Link Layer. PhysicalLayerMonitor inherits
// from IHandler, which inherits from IUpperLayer
class LinkLayerRouter : public asiodnp3::PhysicalLayerMonitor, public opendnp3::ILinkTx, private opendnp3::IFrameSink
{
public:

	LinkLayerRouter(openpal::Logger logger,
	                openpal::IExecutor& executor,
	                openpal::IPhysicalLayer*,
	                const opendnp3::ChannelRetry& retry,
	                opendnp3::IChannelStateListener* pStateHandler = nullptr,
	                opendnp3::LinkChannelStatistics* pStatistics = nullptr);

	opendnp3::ITaskLock& GetTaskLock()
	{
		return taskLock;
	}

	// called when the router shuts down
	void SetShutdownHandler(const openpal::Action0& action);

	// Query to see if a route is in use
	bool IsRouteInUse(const opendnp3::Route& route);

	// Ties the lower part of the link layer to the upper part
	bool AddContext(opendnp3::ILinkSession& context, const opendnp3::Route& route);

	/**
	*  Tells the router to begin sending messages to the context
	*/
	bool Enable(opendnp3::ILinkSession* pContext);

	/**
	*  Tells the router to stop sending messages to the context associated with this route
	*  Does not remove the context entirely
	*/
	bool Disable(opendnp3::ILinkSession* pContext);

	/**
	* This is safe to do at runtime, so long as the request happens from the executor
	*/
	bool Remove(opendnp3::ILinkSession* pContext);

	// ------------ IFrameSink -----------------
	virtual bool OnFrame(const opendnp3::LinkHeaderFields& header, const openpal::RSlice& userdata) override final;

	// ------------ ILinkTx -----------------

	virtual void BeginTransmit(const openpal::RSlice& buffer, opendnp3::ILinkSession& context) override final;

	// ------------ IUpperLayer -----------------

	virtual void OnReceive(const openpal::RSlice&) override final;
	virtual void OnSendResult(bool success) override final;

protected:

	void OnShutdown() override final;

private:

	void OnStateChange(opendnp3::ChannelState aState) override final;

	bool HasEnabledContext();

	struct Record
	{
		Record(opendnp3::ILinkSession* context, const opendnp3::Route& route_) :
			pContext(context),
			route(route_),
			enabled(false)
		{}

		Record() : pContext(nullptr), enabled(false)
		{}

		opendnp3::ILinkSession* pContext;
		opendnp3::Route route;
		bool enabled;
	};

	struct Transmission
	{
		Transmission(const openpal::RSlice& buffer_, opendnp3::ILinkSession* pContext_) :
			buffer(buffer_),
			pContext(pContext_)
		{}

		Transmission() : buffer(), pContext(nullptr)
		{}

		openpal::RSlice buffer;
		opendnp3::ILinkSession* pContext;
	};

	opendnp3::ILinkSession* GetDestination(uint16_t aDest, uint16_t aSrc);
	opendnp3::ILinkSession* GetEnabledContext(const opendnp3::Route&);

	void CheckForSend();

	opendnp3::MultidropTaskLock taskLock;
	opendnp3::IChannelStateListener* pStateHandler;
	openpal::Action0 shutdownHandler;

	std::vector<Record> records;
	std::deque<Transmission>  transmitQueue;

	// Handles the parsing of incoming frames

	opendnp3::LinkChannelStatistics* pStatistics;
	opendnp3::LinkLayerParser parser;
	bool isTransmitting;

	// Implement virtual PhysLayerMonitor
	void OnPhysicalLayerOpenSuccessCallback() override;
	void OnPhysicalLayerOpenFailureCallback() override {}
	void OnPhysicalLayerCloseCallback() override;

};

}



#endif

