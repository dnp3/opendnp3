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

#include "opendnp3/StaticSizeConfiguration.h"
#include "opendnp3/Settable.h"

namespace opendnp3
{


/// Represent all of the "state" and configuration for an outstation
class OutstationContext
{
	public:

	enum TransmitState
	{
		IDLE,
		SOLICITED,
		UNSOLICITED
	};

	static uint8_t NextSeq(uint8_t seq) { return (seq + 1) % 16; }

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
	TransmitState transmitState;	
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
	ResponseContext rspContext;

	// ------ Helper methods for dealing with state ------

	IINField GetDynamicIIN();

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

	void ExamineAPDU(const openpal::ReadOnlyBuffer& fragment);

	void OnReceiveSolRequest(const APDURecord& request, const openpal::ReadOnlyBuffer& fragment);

	void RespondToRequest(const APDURecord& request, APDUEquality equality);

	void BeginResponseTx(const openpal::ReadOnlyBuffer& response);

	void BeginUnsolTx(const openpal::ReadOnlyBuffer& response);

	IINField BuildResponse(const APDURecord& request, APDUResponse& response, APDUEquality equality);

	void ContinueMultiFragResponse(uint8_t seq);

	void OnEnterIdleState();

	bool IsTransmitting() const;
	bool IsNotTransmitting() const;
	

	private:

	// ------ Helpers ---------

	static bool CancelTimer(openpal::ITimer*& pTimer);	

	// ------ Internal Events -------

	void CheckForIdleState();

	void CheckForUnsolicited();

	void OnSolConfirmTimeout();

	void OnUnsolRetryTimeout();

	void OnNewEvents();

	// ------ Function Handlers ------

	IINField HandleWrite(const APDURecord& request);
	IINField HandleRead(const APDURecord& request, APDUResponse& response);	
	IINField HandleSelect(const APDURecord& request, APDUResponse& response);
	IINField HandleOperate(const APDURecord& request, APDUResponse& response, APDUEquality equality);
	IINField HandleDirectOperate(const APDURecord& request, APDUResponse& response);
	IINField HandleDelayMeasure(const APDURecord& request, APDUResponse& response);
	IINField HandleDisableUnsolicited(const APDURecord& request, APDUResponse& response);
	IINField HandleEnableUnsolicited(const APDURecord& request, APDUResponse& response);

	IINField HandleCommandWithConstant(const APDURecord& request, APDUResponse& response, CommandStatus status);

	// ------ Static bufers -------

	openpal::StaticBuffer<sizes::MAX_RX_APDU_SIZE> rxBuffer;
	openpal::StaticBuffer<sizes::MAX_TX_APDU_SIZE> txBuffer;
};


}



#endif

