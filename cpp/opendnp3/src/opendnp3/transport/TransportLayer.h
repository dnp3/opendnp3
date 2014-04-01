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
#ifndef __TRANSPORT_LAYER_H_
#define __TRANSPORT_LAYER_H_

#include "TransportRx.h"
#include "TransportTx.h"

#include <openpal/IExecutor.h>
#include <openpal/Loggable.h>
#include <openpal/AsyncLayerInterfaces.h>

#include "opendnp3/link/ILinkLayer.h"
#include "opendnp3/StaticSizeConfiguration.h"


namespace opendnp3
{

class TLS_Base;

/** Implements the DNP3 transport layer as a generic
asynchronous protocol stack layer
*/
class TransportLayer : public openpal::IUpperLayer, public openpal::ILowerLayer, private openpal::Loggable
{
	friend class TransportRx;
	friend class TransportTx;

	friend class TLS_Ready;
	friend class TLS_Sending;

public:

	TransportLayer(const openpal::Logger& logger, openpal::IExecutor* pExecutor_, uint32_t maxFragSize = sizes::DEFAULT_APDU_BUFFER_SIZE);

	static std::string ToString(uint8_t aHeader);

	openpal::Logger& GetLogger()
	{
		return logger;
	}

	/// ILowerLayer

	virtual void Send(const openpal::ReadOnlyBuffer&) override final;

	/// IUpperLayer

	virtual void OnReceive(const openpal::ReadOnlyBuffer&) override final;
	virtual void OnLowerLayerUp() override final;
	virtual void OnLowerLayerDown() override final;
	virtual void OnSendResult(bool isSuccess) override final;

	void SetAppLayer(openpal::IUpperLayer* pUpperLayer_);
	void SetLinkLayer(ILinkLayer* pLinkLayer_);

private:

	openpal::IUpperLayer* pUpperLayer;
	ILinkLayer* pLinkLayer;

	// Actions - Taken by the states/transmitter/receiver in response to events

	void ChangeState(TLS_Base* apNewState);

	void TransmitAPDU(const openpal::ReadOnlyBuffer&);

	void ReceiveAPDU(const openpal::ReadOnlyBuffer&);
	void ReceiveTPDU(const openpal::ReadOnlyBuffer&);

	void SignalSendResult(bool isSuccess);

	/* Members and Helpers */
	bool isOnline;
	TLS_Base* pState;
	openpal::IExecutor* pExecutor;

	const uint32_t M_FRAG_SIZE;

	/* Transmitter and Receiver Classes */
	TransportRx receiver;
	TransportTx transmitter;
};

}

#endif

