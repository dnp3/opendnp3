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
#ifndef OPENDNP3_OUTSTATIONCONTEXT_H
#define OPENDNP3_OUTSTATIONCONTEXT_H

#include <openpal/logging/LogRoot.h>
#include <openpal/executor/IExecutor.h>

#include <openpal/container/DynamicBuffer.h>
#include <openpal/container/Pair.h>
#include <openpal/container/Settable.h>

#include "opendnp3/LayerInterfaces.h"
#include "opendnp3/outstation/Database.h"
#include "opendnp3/outstation/ResponseContext.h"
#include "opendnp3/outstation/OutstationConfig.h"
#include "opendnp3/app/IINField.h"
#include "opendnp3/app/HeaderWriter.h"
#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/APDUResponse.h"
#include "opendnp3/outstation/ICommandHandler.h"
#include "opendnp3/outstation/IOutstationApplication.h"
#include "opendnp3/outstation/OutstationSolicitedStates.h"
#include "opendnp3/outstation/OutstationUnsolicitedStates.h"
#include "opendnp3/outstation/DeferredRequest.h"

namespace opendnp3
{

/// Represent all of the "state" and configuration for an outstation
class OutstationContext
{
	
	public:		

	OutstationContext(	const OutstationConfig& config,
						openpal::IExecutor& executor,
						openpal::LogRoot& root, 
						ILowerLayer& lower,
						ICommandHandler& commandHandler,
						IOutstationApplication& application,
						Database& database, 
						const EventBufferFacade& buffers);

	// ------ Unchanging variables and self managing variables -------

	OutstationParams params;
	EventResponseConfig eventConfig;
	openpal::Logger logger;
	openpal::IExecutor* pExecutor;	
	ICommandHandler* pCommandHandler;
	IOutstationApplication* pApplication;
	Database* pDatabase;
	OutstationEventBuffer eventBuffer;

	// ------ Dynamic "state", i.e. things that must be managed or cleanup on close ------
	
	bool isOnline;	
	OutstationSolicitedStateBase*	pSolicitedState;
	OutstationUnsolicitedStateBase*	pUnsolicitedState;

	IINField staticIIN;

	openpal::ITimer* pConfirmTimer;
	//openpal::ITimer* pUnsolTimer;						// gets used for both retries and "pack" timer
	bool unsolPackTimerExpired;
	
	uint32_t rxFragCount;
	openpal::MonotonicTimestamp selectTime;
	uint8_t operateExpectedSeq;
	uint32_t operateExpectedFragCount;

	bool isTransmitting;
	uint8_t solSeqN;
	uint8_t unsolSeqN;
	uint8_t expectedSolConfirmSeq;	
	uint8_t expectedUnsolConfirmSeq;
	bool completedNullUnsol;

	openpal::ReadOnlyBuffer lastValidRequest;			// points to bytes in rxBuffer
	openpal::ReadOnlyBuffer lastResponse;				// points to bytes in txBuffer

	openpal::Settable<DeferredRequest> deferredRequest;

	ResponseContext rspContext;

	// ------ Helper methods for dealing with state ------	

	APDUResponse StartNewSolicitedResponse();
	APDUResponse StartNewUnsolicitedResponse();

	void ConfigureUnsolHeader(APDUResponse& unsol);	

	void SetOnline();
	void SetOffline();
	
	bool IsOperateSequenceValid();
	bool IsIdle();

	bool CancelConfirmTimer();	
	
	bool StartSolicitedConfirmTimer();
	bool StartUnsolicitedConfirmTimer();	

	void OnReceiveAPDU(const openpal::ReadOnlyBuffer& apdu);

	void OnSendResult(bool isSuccess);

	OutstationSolicitedStateBase* OnReceiveSolRequest(const APDUHeader& header, const openpal::ReadOnlyBuffer& apdu);	

	void BeginResponseTx(const openpal::ReadOnlyBuffer& response);

	void BeginUnsolTx(const openpal::ReadOnlyBuffer& response);

	IINField BuildNonReadResponse(const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, HeaderWriter& writer, bool objectsEqualToLastRequest);

	OutstationSolicitedStateBase* ContinueMultiFragResponse(uint8_t seq);
	
	OutstationSolicitedStateBase* RespondToNonReadRequest(const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, bool objectsEqualToLastRequest);

	OutstationSolicitedStateBase* RespondToReadRequest(uint8_t seq, const openpal::ReadOnlyBuffer& objects);

	void ProcessNoResponseFunction(const APDUHeader& header, const openpal::ReadOnlyBuffer& objects);

	private:

	void PostCheckForActions();

	OutstationSolicitedStateBase* ProcessNewRequest(const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, bool objectsEqualToLastRequest);

	// private variables not available from the states

	ILowerLayer* pLower;

	// ------ Helpers ---------

	static bool CancelTimer(openpal::ITimer*& pTimer);	

	IINField GetDynamicIIN();

	IINField GetResponseIIN();

	// ------ Internal Events -------

	void CheckForTaskStart();

	void CheckDeferredRequest();

	void CheckForUnsolicited();

	void OnSolConfirmTimeout();

	void OnUnsolConfirmTimeout();	

	// ------ Function Handlers ------

	// reads are special due to multi-frag
	// returns an IIN field and a partial AppControlField (missing sequence info)
	openpal::Pair<IINField, AppControlField> HandleRead(const openpal::ReadOnlyBuffer& objects, HeaderWriter& writer);

	IINField HandleWrite(const openpal::ReadOnlyBuffer& objects);	
	IINField HandleSelect(const openpal::ReadOnlyBuffer& objects, HeaderWriter& writer);
	IINField HandleOperate(const openpal::ReadOnlyBuffer& objects, HeaderWriter& writer, bool objectsEqualToLastRequest);
	IINField HandleDirectOperate(const openpal::ReadOnlyBuffer& objects, HeaderWriter* pWriter);
	IINField HandleDelayMeasure(const openpal::ReadOnlyBuffer& objects, HeaderWriter& writer);
	IINField HandleRestart(const openpal::ReadOnlyBuffer& objects, bool isWarmRestart, HeaderWriter* pWriter);	
	IINField HandleDisableUnsolicited(const openpal::ReadOnlyBuffer& objects, HeaderWriter& writer);
	IINField HandleEnableUnsolicited(const openpal::ReadOnlyBuffer& objects, HeaderWriter& writer);

	IINField HandleCommandWithConstant(const openpal::ReadOnlyBuffer& objects, HeaderWriter& writer, CommandStatus status);

	// ------ buffers -------

	openpal::DynamicBuffer rxBuffer;
	openpal::DynamicBuffer solTxBuffer;
	openpal::DynamicBuffer unsolTxBuffer;
};


}



#endif

