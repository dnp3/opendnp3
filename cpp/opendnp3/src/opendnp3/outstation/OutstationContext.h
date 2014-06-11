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
#ifndef __OUTSTATION_CONTEXT_H_
#define __OUTSTATION_CONTEXT_H_

#include <openpal/StaticBuffer.h>
#include <openpal/IExecutor.h>
#include <openpal/LogRoot.h>
#include <openpal/AsyncLayerInterfaces.h>

#include "opendnp3/outstation/Database.h"
#include "opendnp3/outstation/ResponseContext.h"
#include "opendnp3/outstation/OutstationConfig.h"
#include "opendnp3/app/IINField.h"
#include "opendnp3/app/ObjectWriter.h"
#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/APDUResponse.h"
#include "opendnp3/outstation/ICommandHandler.h"
#include "opendnp3/outstation/ITimeWriteHandler.h"
#include "opendnp3/outstation/OutstationStates.h"
#include "opendnp3/outstation/DeferredRequest.h"

#include "opendnp3/StaticSizeConfiguration.h"
#include "opendnp3/Settable.h"


namespace opendnp3
{

/// Represent all of the "state" and configuration for an outstation
class OutstationContext
{
	public:		

	OutstationContext(	const OutstationConfig& config,
						openpal::IExecutor& executor,
						openpal::LogRoot& root, 
						openpal::ILowerLayer& lower,
						ICommandHandler& commandHandler,
						ITimeWriteHandler& timeWriteHandler,
						Database& database, 
						const EventBufferFacade& buffers);

	// ------ Unchanging variables and self managing variables -------

	OutstationParams params;
	EventResponseConfig eventConfig;
	openpal::Logger logger;
	openpal::IExecutor* pExecutor;
	openpal::ILowerLayer* pLower;
	ICommandHandler* pCommandHandler;
	ITimeWriteHandler* pTimeWriteHandler;
	Database* pDatabase;
	OutstationEventBuffer eventBuffer;

	// ------ Dynamic "state", i.e. things that must be managed or cleanup on close ------
	
	bool isOnline;
	bool isTransmitting;
	OutstationStateBase* pState;

	IINField staticIIN;

	openpal::ITimer* pConfirmTimer;
	openpal::ITimer* pUnsolTimer;						// gets used for both retries and "pack" timer
	bool unsolPackTimerExpired;
	
	uint32_t rxFragCount;
	openpal::MonotonicTimestamp selectTime;
	uint8_t operateExpectedSeq;
	uint32_t operateExpectedFragCount;

	uint8_t solSeqN;
	uint8_t unsolSeqN;
	uint8_t expectedSolConfirmSeq;	
	uint8_t expectedUnsolConfirmSeq;
	bool completedNullUnsol;

	openpal::ReadOnlyBuffer lastValidRequest;			// points to bytes in rxBuffer
	openpal::ReadOnlyBuffer lastResponse;				// points to bytes in txBuffer
	Settable<DeferredRequest> deferredRequest;			// 

	ResponseContext rspContext;

	// ------ Helper methods for dealing with state ------	

	APDUResponse StartNewResponse();

	void ConfigureUnsolHeader(APDUResponse& unsol);	

	void SetOnline();
	void SetOffline();
	
	bool IsOperateSequenceValid();
	bool IsIdle();

	bool CancelConfirmTimer();

	bool CancelUnsolTimer();
	
	bool StartConfirmTimer();

	bool StartUnsolRetryTimer();

	void OnReceiveAPDU(const openpal::ReadOnlyBuffer& apdu);

	void OnReceiveSolRequest(const APDUHeader& header, const openpal::ReadOnlyBuffer& apdu);

	void RespondToRequest(const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, APDUEquality equality);

	void BeginResponseTx(const openpal::ReadOnlyBuffer& response);

	void BeginUnsolTx(const openpal::ReadOnlyBuffer& response);

	APDUResponseHeader BuildResponse(const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer, APDUEquality equality);

	void ContinueMultiFragResponse(uint8_t seq);

	void OnEnterIdleState();
	
	private:

	// ------ Helpers ---------

	static bool CancelTimer(openpal::ITimer*& pTimer);	

	IINField GetDynamicIIN();

	IINField GetResponseIIN();

	// ------ Internal Events -------

	void PerformTaskFromIdleState();

	bool CheckDeferredRequest();

	void CheckForUnsolicited();

	void OnSolConfirmTimeout();

	void OnUnsolRetryTimeout();

	void OnNewEvents();

	// ------ Function Handlers ------

	APDUResponseHeader HandleWrite(const openpal::ReadOnlyBuffer& objects);
	APDUResponseHeader HandleRead(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer);
	APDUResponseHeader HandleSelect(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer);
	APDUResponseHeader HandleOperate(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer, APDUEquality equality);
	APDUResponseHeader HandleDirectOperate(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer);
	APDUResponseHeader HandleDelayMeasure(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer);
	APDUResponseHeader HandleDisableUnsolicited(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer);
	APDUResponseHeader HandleEnableUnsolicited(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer);

	APDUResponseHeader HandleCommandWithConstant(const openpal::ReadOnlyBuffer& objects, ObjectWriter& writer, CommandStatus status);

	// ------ Static bufers -------

	openpal::StaticBuffer<sizes::MAX_RX_APDU_SIZE> rxBuffer;
	openpal::StaticBuffer<sizes::MAX_TX_APDU_SIZE> txBuffer;
};


}



#endif

