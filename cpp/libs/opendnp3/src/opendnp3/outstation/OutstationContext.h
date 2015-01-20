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

#include <openpal/container/DynamicBuffer.h>
#include <openpal/container/Pair.h>
#include <openpal/container/Settable.h>

#include "opendnp3/app/IINField.h"
#include "opendnp3/app/HeaderWriter.h"
#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/APDUResponse.h"

#include "opendnp3/outstation/OutstationState.h"
#include "opendnp3/outstation/Database.h"
#include "opendnp3/outstation/ResponseContext.h"
#include "opendnp3/outstation/OutstationConfig.h"
#include "opendnp3/outstation/ICommandHandler.h"
#include "opendnp3/outstation/IOutstationApplication.h"
#include "opendnp3/outstation/EventBuffer.h"
#include "opendnp3/outstation/IOutstationAuthProvider.h"

namespace opendnp3
{

/// Represent all of the "state" and configuration for an outstation
class OutstationContext : private INewEventDataHandler
{
	
	public:		

	OutstationContext(	const OutstationConfig& config,
						const DatabaseTemplate& dbTemplate,
						openpal::IMutex* pMutex,
						openpal::IExecutor& executor,
						openpal::LogRoot& root, 
						ILowerLayer& lower,
						ICommandHandler& commandHandler,
						IOutstationApplication& application,
						IOutstationAuthProvider& authProvider);
	
	OutstationState ostate;

	// ------ Unchanging variables and self managing variables -------	
	ICommandHandler* pCommandHandler;
	IOutstationApplication* pApplication;
	IOutstationAuthProvider* pAuthProvider;
	EventBuffer eventBuffer;
	Database database;
	openpal::ReadBufferView lastResponse; // points to bytes in txBuffer	
	
	ResponseContext rspContext;

	// ------ Helper methods for dealing with state ------	

	APDUResponse StartNewSolicitedResponse();
	APDUResponse StartNewUnsolicitedResponse();

	void ConfigureUnsolHeader(APDUResponse& unsol);	

	void SetOnline();
	void SetOffline();		
	
	bool StartSolicitedConfirmTimer();
	bool StartUnsolicitedConfirmTimer();	

	void OnReceiveAPDU(const openpal::ReadBufferView& apdu);

	void ExamineASDU(const APDUHeader& header, const openpal::ReadBufferView& objects);

	void OnSendResult(bool isSuccess);

	OutstationSolicitedStateBase* OnReceiveSolRequest(const APDUHeader& header, const openpal::ReadBufferView& objects);	

	void BeginResponseTx(const openpal::ReadBufferView& response);

	void BeginUnsolTx(const openpal::ReadBufferView& response);

	IINField BuildNonReadResponse(const APDUHeader& header, const openpal::ReadBufferView& objects, HeaderWriter& writer);

	OutstationSolicitedStateBase* ContinueMultiFragResponse(uint8_t seq);
	
	OutstationSolicitedStateBase* RespondToNonReadRequest(const APDUHeader& header, const openpal::ReadBufferView& objects);

	OutstationSolicitedStateBase* RespondToReadRequest(uint8_t seq, const openpal::ReadBufferView& objects);

	void ProcessNoResponseFunction(const APDUHeader& header, const openpal::ReadBufferView& objects);	

	private:

	virtual void OnNewEventData() override final;

	void PostCheckForActions();

	OutstationSolicitedStateBase* ProcessNewRequest(const APDUHeader& header, const openpal::ReadBufferView& objects);
		
	// ------ Helpers ---------

	IINField GetDynamicIIN();

	IINField GetResponseIIN();

	// ------ Internal Events -------

	void CheckForTaskStart();

	void CheckDeferredRequest();

	void CheckForUnsolicited();

	// ------ Function Handlers ------

	// reads are special due to multi-frag
	// returns an IIN field and a partial AppControlField (missing sequence info)
	openpal::Pair<IINField, AppControlField> HandleRead(const openpal::ReadBufferView& objects, HeaderWriter& writer);

	IINField HandleWrite(const openpal::ReadBufferView& objects);	
	IINField HandleSelect(const openpal::ReadBufferView& objects, HeaderWriter& writer);
	IINField HandleOperate(const openpal::ReadBufferView& objects, HeaderWriter& writer);
	IINField HandleDirectOperate(const openpal::ReadBufferView& objects, HeaderWriter* pWriter);
	IINField HandleDelayMeasure(const openpal::ReadBufferView& objects, HeaderWriter& writer);
	IINField HandleRestart(const openpal::ReadBufferView& objects, bool isWarmRestart, HeaderWriter* pWriter);
	IINField HandleAssignClass(const openpal::ReadBufferView& objects);
	IINField HandleDisableUnsolicited(const openpal::ReadBufferView& objects, HeaderWriter& writer);
	IINField HandleEnableUnsolicited(const openpal::ReadBufferView& objects, HeaderWriter& writer);
	IINField HandleCommandWithConstant(const openpal::ReadBufferView& objects, HeaderWriter& writer, CommandStatus status);	

	// ------ tx buffers -------		
	openpal::DynamicBuffer solTxBuffer;
	openpal::DynamicBuffer unsolTxBuffer;
};


}



#endif

