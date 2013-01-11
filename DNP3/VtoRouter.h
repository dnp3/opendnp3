/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership.  Green Enery
 * Corp licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#ifndef __VTO_ROUTER_H_
#define __VTO_ROUTER_H_

#include <queue>
#include <deque>

#include <APL/IHandlerAsync.h>
#include <APL/PhysicalLayerMonitor.h>
#include <APL/CopyableBuffer.h>

#include "VtoDataInterface.h"

namespace apl
{

class IPhysicalLayerAsync;
class IExecutor;

namespace dnp
{

class VtoReader;
class VtoWriter;
struct VtoRouterSettings;

/**
 * helper object that allows us to serialize data and up/down communication events into
 * the same data stream
 */
class VtoMessage
{
public:

	VtoMessage(VtoDataType aType) :
		type(aType), data() {}


	VtoMessage(VtoDataType aType, const uint8_t* apBuffer, size_t aBufferSize) :
		type(aType), data(apBuffer, aBufferSize) {}

	VtoDataType    type;
	CopyableBuffer data;
};


/**
 * Base Class used to route data between a VTO channel (made up of both a
 * VtoWriter and VtoReader instance) and an IPhysicalLayerAsync
 * instance.
 *
 * @code
 *                                           +--> [VtoReader]
 *                                           |
 * [IPhysicalLayerAsync] <--> [VtoRouter] <--+
 *                                           |
 *                                           +--> [VtoWriter]
 * @endcode
 *
 * The VtoRouter instance provides the necessary IVtoCallbacks hooks
 * that the VtoReader will use. The base vtorouter provides the machinery
 * to route data to and from the dnp connection and the lets the concrete
 * implementations set policy on when to start and stop the reconnecting
 * behavior.
 */
class VtoRouter : public PhysicalLayerMonitor, public IVtoCallbacks
{
public:

	/**
	 * Create a new VtoRouter instance.  Prior to using this
	 * instance, make sure to register both a IPhysicalLayerAsync
	 * and a VtoWriter using SetPhysicalLayer() and
	 * SetVtoWriter(), respectively.
	 *
	 * @param arSettings			Settings for the router including the DNP3 Virtual Terminal port (channel id)
	     * @param apLogger			the Logger that the instance should use for log messages
	 * @param apWriter                      Interface that the router will use to write vto objects
	     * @param apPhysLayer                   PhysicalLayer interface to which the router is bound
	 * @param apExecutor                    Asynchronous IExecutor interface router uses retry connections, etc
	     *
	 * @return				a new VtoRouter instance
	 */
	VtoRouter(const VtoRouterSettings& arSettings, Logger* apLogger, IVtoWriter* apWriter, IPhysicalLayerAsync* apPhysLayer, IExecutor* apExecutor);

	/**
	 * Receives data from the VTO channel and forwards it to the
	 * IPhysicalLayerAsync instance associated with this VtoRouter.
	 * Implements IVtoCallbacks::OnVtoDataReceived().
	 *
	 * @param apData		The data received from the VTO stream
	 * @param aLength		The length of the data received (in
	 *						bytes)
	 */
	void OnVtoDataReceived(const VtoData& arData);

	/**
	 * Called when the VTO data buffer size changes (startup and
	 * successuly transmission).
	 */
	void OnBufferAvailable();

protected:

	void CheckForPhysRead();
	void CheckForPhysWrite();
	void CheckForVtoWrite();

	// Implement AsyncPhysLayerMonitor

	void OnPhysicalLayerOpenSuccessCallback();

	void OnPhysicalLayerOpenFailureCallback();

	void OnPhysicalLayerCloseCallback();

	/**
	 * Receives data from the physical layer and forwards it to the
	 * VtoWriter.
	 *
	 * @param apData		The data received from the physical
	 * 						layer
	 * @param aLength		The length of the data received (in
	 *						bytes)
	 */
	void _OnReceive(const uint8_t* apData, size_t aLength);

	/**
	 * Implements IUpperLayer::_OnSendSuccess(), which was
	 * inherited via IHandlerAsync.  Called when an asynchronous
	 * transmission to the physical layer was successful.
	 */
	void _OnSendSuccess();

	/**
	 * Implements IUpperLayer::_OnSendFailure(), which was
	 * inherited via IHandlerAsync.  Called when an asynchronous
	 * transmission to the physical layer was not successful.
	 */
	void _OnSendFailure();

	virtual void DoVtoRemoteConnectedChanged(bool aOpened) = 0;
	virtual void SetLocalConnected(bool aConnected) = 0;

	//return true if should be processed, false otherwise
	virtual bool CheckIncomingVtoData(const VtoData& arData) = 0;

	void FlushBuffers();
	void NotifyRemoteSideOfState(bool aConnected);

private:

	/**
	 * The VtoWriter instance that will be used to send the data
	 * that is received by the IPhysicalLayerAsync instance to the
	 * VTO endpoint.
	 */
	IVtoWriter* mpVtoWriter;

	/**
	 * The transmit buffer for Vto -> physical layer.  The data that
	 * is put into this buffer was originally received via VTO.
	 */
	std::queue<VtoData> mPhysLayerTxBuffer;

	/**
	 * The transmit message buffer for vto actions (OPEN/CLOSE/DATA) from physical layer -> Vto.
	 * The data that is put into this buffer was originally received from the physical layer.
	 */
	std::deque<VtoMessage> mVtoTxBuffer;

	/**
	 * Buffer used to read from the physical layer
	 */
	CopyableBuffer mReadBuffer;

	/**
	 * Buffer used to write to the physical layer
	 */
	VtoData mWriteData;

};

}
}

/* vim: set ts=4 sw=4: */

#endif

