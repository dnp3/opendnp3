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

#include "OutstationContext.h"

#include "opendnp3/LogLevels.h"
#include "opendnp3/outstation/OutstationActions.h"
#include "opendnp3/outstation/OutstationFunctions.h"

#include <openpal/logging/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

OContext::OContext(
	const OutstationConfig& config,
	const DatabaseTemplate& dbTemplate,
	openpal::Logger logger_,				
	openpal::IExecutor& executor,		
	ILowerLayer& lower,
	ICommandHandler& commandHandler,
	IOutstationApplication& application) :
	
		logger(logger_),
		pExecutor(&executor),
		pLower(&lower),	
		pCommandHandler(&commandHandler),
		pApplication(&application),
		eventBuffer(config.eventBufferConfig),
		database(dbTemplate, eventBuffer, config.params.indexMode, config.params.typesAllowedInClass0),
		rspContext(database.GetStaticLoader(), eventBuffer),
		params(config.params),	
		isOnline(false),
		isTransmitting(false),	
		staticIIN(IINBit::DEVICE_RESTART),	
		confirmTimer(executor),
		deferred(config.params.maxRxFragSize),
		sol(config.params.maxTxFragSize),
		unsol(config.params.maxTxFragSize)
{	
	
}

OutstationSolicitedStateBase* OContext::OnReceiveSolRequest(const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	// analyze this request to see how it compares to the last request
	if (this->history.HasLastRequest())
	{
		if (this->sol.seq.num.Equals(header.control.SEQ))
		{
			if (this->history.FullyEqualsLastRequest(header, objects))
			{
				if (header.function == FunctionCode::READ)
				{
					SIMPLE_LOG_BLOCK(this->logger, flags::WARN, "Ignoring repeat read request");
					return this->sol.pState;
				}
				else
				{
					return this->sol.pState->OnRepeatNonReadRequest(*this, header, objects);
				}
			}
			else // new operation with same SEQ
			{
				return this->ProcessNewRequest(header, objects);
			}
		}
		else  // completely new sequence #
		{
			return this->ProcessNewRequest(header, objects);
		}
	}
	else
	{
		return this->ProcessNewRequest(header, objects);
	}
}

OutstationSolicitedStateBase* OContext::ProcessNewRequest(const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	this->sol.seq.num = header.control.SEQ;

	if (header.function == FunctionCode::READ)
	{
		return this->sol.pState->OnNewReadRequest(*this, header, objects);
	}
	else
	{
		return this->sol.pState->OnNewNonReadRequest(*this, header, objects);
	}
}

OutstationSolicitedStateBase* OContext::RespondToNonReadRequest(const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	this->history.RecordLastProcessedRequest(header, objects);

	auto response = this->sol.tx.Start();
	auto writer = response.GetWriter();
	response.SetFunction(FunctionCode::RESPONSE);
	response.SetControl(AppControlField(true, true, false, false, header.control.SEQ));
	auto iin = OFunctions::HandleNonReadResponse(*this, header, objects, writer);
	response.SetIIN(iin | OActions::GetResponseIIN(*this));
	OActions::BeginResponseTx(*this, response.ToReadOnly());
	return &OutstationSolicitedStateIdle::Inst();
}

OutstationSolicitedStateBase* OContext::RespondToReadRequest(const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	this->history.RecordLastProcessedRequest(header, objects);

	auto response = this->sol.tx.Start();
	auto writer = response.GetWriter();
	response.SetFunction(FunctionCode::RESPONSE);
	auto result = OFunctions::HandleRead(*this, objects, writer);
	result.second.SEQ = header.control.SEQ;
	this->sol.seq.confirmNum = header.control.SEQ;
	response.SetControl(result.second);
	response.SetIIN(result.first | OActions::GetResponseIIN(*this));
	OActions::BeginResponseTx(*this, response.ToReadOnly());

	if (result.second.CON)
	{
		OActions::StartSolicitedConfirmTimer(*this);
		return &OutstationStateSolicitedConfirmWait::Inst();
	}
	else
	{
		return  &OutstationSolicitedStateIdle::Inst();
	}
}

OutstationSolicitedStateBase* OContext::ContinueMultiFragResponse(const AppSeqNum& seq)
{
	auto response = this->sol.tx.Start();
	auto writer = response.GetWriter();
	response.SetFunction(FunctionCode::RESPONSE);
	auto control = this->rspContext.LoadResponse(writer);
	control.SEQ = seq;
	this->sol.seq.confirmNum = seq;
	response.SetControl(control);
	response.SetIIN(OActions::GetResponseIIN(*this));
	OActions::BeginResponseTx(*this, response.ToReadOnly());

	if (control.CON)
	{
		OActions::StartSolicitedConfirmTimer(*this);
		return &OutstationStateSolicitedConfirmWait::Inst();
	}
	else
	{
		return &OutstationSolicitedStateIdle::Inst();
	}
}

bool OContext::GoOnline()
{
	if (isOnline)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "already online");
		return false;
	}

	isOnline = true;
	OActions::CheckForTaskStart(*this);
	return true;
}

bool OContext::GoOffline()
{
	if (!isOnline)
	{
		SIMPLE_LOG_BLOCK(logger, flags::ERR, "already offline");
		return false;
	}

	isOnline = false;
	isTransmitting = false;

	sol.Reset();
	unsol.Reset();
	history.Reset();
	deferred.Reset();
	eventBuffer.Unselect();
	rspContext.Reset();
	confirmTimer.Cancel();
	auth.Reset();

	return true;
}

bool OContext::CanTransmit() const
{
	return isOnline && !isTransmitting;
}


}

