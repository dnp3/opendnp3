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
#ifndef __MASTER_CONTEXT_H_
#define __MASTER_CONTEXT_H_

#include <openpal/AsyncLayerInterfaces.h>

#include <openpal/IExecutor.h>
#include <openpal/LogRoot.h>
#include <openpal/StaticQueue.h>
#include <openpal/StaticBuffer.h>

#include "opendnp3/master/MasterScheduler.h"
#include "opendnp3/master/MasterTaskList.h"


namespace opendnp3
{

class MasterContext
{
	public:	

	MasterContext(	openpal::IExecutor& executor,
					openpal::LogRoot& root, 
					openpal::ILowerLayer& lower,
					ISOEHandler* pSOEHandler,
					const MasterParams& params
				);
	
	openpal::Logger logger;
	openpal::IExecutor* pExecutor;
	openpal::ILowerLayer* pLower;

	// ------- configuration --------
	MasterParams params;

	// ------- dynamic state ---------
	bool isOnline;
	bool isSending;
	uint8_t solSeq;
	uint8_t unsolSeq;
	IMasterTask* pActiveTask;
	openpal::ITimer* pResponseTimer;
	MasterScheduler scheduler;
	MasterTaskList taskList;
	openpal::StaticQueue<APDUHeader, uint8_t, 4> confirmQueue;

	openpal::StaticBuffer<sizes::MAX_TX_APDU_SIZE> txBuffer;
	
	void PostCheckForTask();

	// ------- events ----------
	bool OnLayerUp();
	bool OnLayerDown();
	void OnSendResult(bool isSucccess);
	void OnResponse(const APDUResponseRecord& response);	
	void OnUnsolicitedResponse(const APDUResponseRecord& response);	

	private:

	void OnResponseTimeout();

	void CheckForTask();

	static uint8_t NextSeq(uint8_t seq) { return (seq + 1) % 16; }

	// -------- helpers --------

	void StartResponseTimer();

	bool CancelResponseTimer();

	void QueueConfirm(const APDUHeader& header);

	void CheckConfirmTransmit();

	void Transmit(const openpal::ReadOnlyBuffer& output);

	static bool CanConfirmResponse(TaskStatus status);	
};

}

#endif
