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

#include "OutstationActions.h"

#include "opendnp3/LogLevels.h"

#include "opendnp3/app/APDULogging.h"
#include "opendnp3/app/APDUBuilders.h"
#include "opendnp3/app/APDUHeaderParser.h"

#include "opendnp3/outstation/IINHelpers.h"
#include "opendnp3/outstation/CommandActionAdapter.h"
#include "opendnp3/outstation/CommandResponseHandler.h"
#include "opendnp3/outstation/ConstantCommandAction.h"
#include "opendnp3/outstation/EventWriter.h"
#include "opendnp3/outstation/OutstationFunctions.h"

#include <openpal/logging/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

void OActions::OnNewEventData(OState& ostate)
{
	if (!ostate.pendingTaskCheckFlag)
	{
		ostate.pendingTaskCheckFlag = true;
		// post these calls so the stack can unwind
		auto lambda = [&]() 
		{ 
			ostate.pendingTaskCheckFlag = false;
			OActions::CheckForTaskStart(ostate);
		};
		ostate.pExecutor->PostLambda(lambda);
	}
}

IINField OActions::GetDynamicIIN(OState& ostate)
{	
	auto classField = ostate.eventBuffer.UnwrittenClassField();

	IINField ret;
	ret.SetBitToValue(IINBit::CLASS1_EVENTS, classField.HasClass1());
	ret.SetBitToValue(IINBit::CLASS2_EVENTS, classField.HasClass2());
	ret.SetBitToValue(IINBit::CLASS3_EVENTS, classField.HasClass3());
	ret.SetBitToValue(IINBit::EVENT_BUFFER_OVERFLOW, ostate.eventBuffer.IsOverflown());
	
	return ret;
}

IINField OActions::GetResponseIIN(OState& ostate)
{
	return ostate.staticIIN | GetDynamicIIN(ostate) | ostate.pApplication->GetApplicationIIN().ToIIN();
}

void OActions::ConfigureUnsolHeader(OState& ostate, APDUResponse& unsol)
{	
	build::NullUnsolicited(unsol, ostate.unsol.seq.num, GetResponseIIN(ostate));
}

void OActions::OnReceiveAPDU(OState& ostate, const openpal::ReadBufferView& apdu)
{	
	APDUHeader header;	
	if (APDUHeaderParser::ParseRequest(apdu, header, &ostate.logger))
	{
		FORMAT_LOG_BLOCK(ostate.logger, flags::APP_HEADER_RX,
			"FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s",
			header.control.FIR,
			header.control.FIN,
			header.control.CON,
			header.control.UNS,
			header.control.SEQ,
			FunctionCodeToString(header.function));

		// outstations should only process single fragment messages that don't request confirmation
		if (header.control.IsFirAndFin() && !header.control.CON)
		{
			auto objects = apdu.Skip(APDU_REQUEST_HEADER_SIZE);
			OActions::ExamineASDU(ostate, header, objects);			
			OActions::CheckForTaskStart(ostate);
		}
		else
		{
			SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring fragment. Requst must be FIR/FIN/!CON");
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(ostate.logger, flags::ERR, "ignoring malformed request header");
	}	
}

void OActions::OnSendResult(OState& ostate, bool isSuccess)
{
	if (ostate.isOnline && ostate.isTransmitting)
	{
		ostate.isTransmitting = false;
		OActions::CheckForTaskStart(ostate);
	}	
}

void OActions::ExamineASDU(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	if (header.control.UNS)
	{
		if (header.function == FunctionCode::CONFIRM)
		{
			ostate.unsol.pState = ostate.unsol.pState->OnConfirm(ostate, header);
		}
		else
		{
			FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring unsol with invalid function code: %s", FunctionCodeToString(header.function));
		}
	}
	else
	{
		if (header.function == FunctionCode::CONFIRM)
		{
			ostate.sol.pState = ostate.sol.pState->OnConfirm(ostate, header);
		}
		else
		{
			ostate.sol.pState = OActions::OnReceiveSolRequest(ostate, header, objects);
			ostate.history.RecordLastRequest(header, objects);
		}
	}
}

OutstationSolicitedStateBase* OActions::OnReceiveSolRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{			
	// analyze this request to see how it compares to the last request
	if (ostate.history.HasLastRequest())
	{		
		if (ostate.sol.seq.num == header.control.SEQ)
		{			
			if (ostate.history.FullyEqualsLastRequest(header, objects))
			{
				if (header.function == FunctionCode::READ)
				{
					SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring repeat read request");
					return ostate.sol.pState;
				}
				else
				{
					return ostate.sol.pState->OnRepeatNonReadRequest(ostate, header, objects);
				}
			}
			else // new operation with same SEQ
			{
				return ProcessNewRequest(ostate, header, objects);
			}
		}
		else  // completely new sequence #
		{
			return ProcessNewRequest(ostate, header, objects);
		}		
	}
	else
	{		
		return ProcessNewRequest(ostate, header, objects);
	}	
}

OutstationSolicitedStateBase* OActions::ProcessNewRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	ostate.sol.seq.num = header.control.SEQ;

	if (header.function == FunctionCode::READ)
	{
		return ostate.sol.pState->OnNewReadRequest(ostate, header, objects);
	}
	else
	{
		return ostate.sol.pState->OnNewNonReadRequest(ostate, header, objects);
	}
}

OutstationSolicitedStateBase* OActions::RespondToNonReadRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	auto response = ostate.sol.tx.Start();
	auto writer = response.GetWriter();
	response.SetFunction(FunctionCode::RESPONSE);
	response.SetControl(AppControlField(true, true, false, false, header.control.SEQ));
	auto iin = OFunctions::HandleNonReadResponse(ostate, header, objects, writer);
	response.SetIIN(iin | GetResponseIIN(ostate));
	OActions::BeginResponseTx(ostate, response.ToReadOnly());
	return &OutstationSolicitedStateIdle::Inst();
}

OutstationSolicitedStateBase* OActions::RespondToReadRequest(OState& ostate, uint8_t seq, const openpal::ReadBufferView& objects)
{
	auto response = ostate.sol.tx.Start();
	auto writer = response.GetWriter();
	response.SetFunction(FunctionCode::RESPONSE);
	auto result = OFunctions::HandleRead(ostate, objects, writer);
	result.second.SEQ = seq;
	ostate.sol.seq.expectedConSeqN = seq;
	response.SetControl(result.second);
	response.SetIIN(result.first | OActions::GetResponseIIN(ostate));		
	OActions::BeginResponseTx(ostate, response.ToReadOnly());

	if (response.GetControl().CON)
	{
		OActions::StartSolicitedConfirmTimer(ostate);
		return &OutstationStateSolicitedConfirmWait::Inst();
	}
	else
	{
		return  &OutstationSolicitedStateIdle::Inst();
	}	
}

void OActions::BeginResponseTx(OState& ostate, const ReadBufferView& response)
{		
	logging::ParseAndLogResponseTx(&ostate.logger, response);
	ostate.sol.tx.Record(response);
	ostate.isTransmitting = true;	
	ostate.pLower->BeginTransmit(response);
}

void OActions::BeginUnsolTx(OState& ostate, const ReadBufferView& response)
{	
	logging::ParseAndLogResponseTx(&ostate.logger, response);
	ostate.unsol.tx.Record(response);
	ostate.isTransmitting = true;
	ostate.unsol.seq.expectedConSeqN = ostate.unsol.seq.num;
	ostate.unsol.seq.num = AppControlField::NextSeq(ostate.unsol.seq.num);
	ostate.pLower->BeginTransmit(response);
}

OutstationSolicitedStateBase* OActions::ContinueMultiFragResponse(OState& ostate, uint8_t seq)
{	
	auto response = ostate.sol.tx.Start();	
	auto writer = response.GetWriter();
	response.SetFunction(FunctionCode::RESPONSE);
	auto control = ostate.rspContext.LoadResponse(writer);
	control.SEQ = seq;
	ostate.sol.seq.expectedConSeqN = seq;
	response.SetControl(control);
	response.SetIIN(ostate.staticIIN | GetDynamicIIN(ostate));		
	OActions::BeginResponseTx(ostate, response.ToReadOnly());
	
	if (response.GetControl().CON)
	{
		OActions::StartSolicitedConfirmTimer(ostate);
		return &OutstationStateSolicitedConfirmWait::Inst();
	}
	else
	{
		return &OutstationSolicitedStateIdle::Inst();
	}	
}

void OActions::CheckForTaskStart(OState& ostate)
{		
	// if we're online, the solicited state is idle, and the unsolicited state 
	// is not transmitting we may be able to do a task
	if (ostate.isOnline && !ostate.isTransmitting && ostate.sol.IsIdle())
	{		
		if (ostate.deferred.IsSet())
		{			
			ostate.deferred.Process(ostate, ProcessDeferredRequest);
		}
		else
		{					
			OActions::CheckForUnsolicited(ostate);
		}
	}	
}

bool OActions::ProcessDeferredRequest(OState& ostate, APDUHeader header, openpal::ReadBufferView objects, bool equalsLastRequest)
{
	if (header.function == FunctionCode::READ)
	{
		if (ostate.unsol.IsIdle())
		{
			ostate.sol.pState = ostate.sol.pState->OnNewReadRequest(ostate, header, objects);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (equalsLastRequest)
		{
			ostate.sol.pState = ostate.sol.pState->OnRepeatNonReadRequest(ostate, header, objects);
		}
		else
		{
			ostate.sol.pState = ostate.sol.pState->OnNewNonReadRequest(ostate, header, objects);
		}

		return true;
	}
}

void OActions::CheckForUnsolicited(OState& ostate)
{
	if (ostate.unsol.IsIdle() && ostate.params.allowUnsolicited)
	{
		if (ostate.unsol.completedNull)
		{				
			// are there events to be reported?
			if (ostate.params.unsolClassMask.Intersects(ostate.eventBuffer.UnwrittenClassField()))
			{			
				
				auto response = ostate.unsol.tx.Start();
				auto writer = response.GetWriter();

				{
					// even though we're not loading static data, we need to lock 
					// the database since it updates the event buffer
					Transaction tx(ostate.database);
					ostate.eventBuffer.Unselect();
					ostate.eventBuffer.SelectAllByClass(ostate.params.unsolClassMask);
					ostate.eventBuffer.Load(writer);					
				}
					
				OActions::ConfigureUnsolHeader(ostate, response);
				OActions::StartUnsolicitedConfirmTimer(ostate);
				ostate.unsol.pState = &OutstationUnsolicitedStateConfirmWait::Inst();
				OActions::BeginUnsolTx(ostate, response.ToReadOnly());
			}			
		}
		else
		{
			// send a NULL unsolcited message									
			auto response = ostate.unsol.tx.Start();
			OActions::ConfigureUnsolHeader(ostate, response);
			OActions::StartUnsolicitedConfirmTimer(ostate);
			ostate.unsol.pState = &OutstationUnsolicitedStateConfirmWait::Inst();
			OActions::BeginUnsolTx(ostate, response.ToReadOnly());
		}
	}	
}

bool OActions::StartSolicitedConfirmTimer(OState& ostate)
{
	auto timeout = [&]() 
	{ 
		ostate.sol.pState = ostate.sol.pState->OnConfirmTimeout(ostate);
		OActions::CheckForTaskStart(ostate);
	};
	return ostate.confirmTimer.Start(ostate.params.unsolConfirmTimeout, timeout);
}

bool OActions::StartUnsolicitedConfirmTimer(OState& ostate)
{
	auto timeout = [&]() 
	{ 
		ostate.unsol.pState = ostate.unsol.pState->OnConfirmTimeout(ostate);
		OActions::CheckForTaskStart(ostate);
	};
	return ostate.confirmTimer.Start(ostate.params.unsolConfirmTimeout, timeout);
}

}

