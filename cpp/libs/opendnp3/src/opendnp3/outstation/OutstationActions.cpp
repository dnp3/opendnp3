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

#include "opendnp3/app/Functions.h"
#include "opendnp3/app/APDULogging.h"
#include "opendnp3/app/APDUBuilders.h"
#include "opendnp3/app/parsing/APDUHeaderParser.h"

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

IINField OActions::GetResponseIIN(OState& ostate)
{
	return ostate.staticIIN | GetDynamicIIN(ostate) | ostate.pApplication->GetApplicationIIN().ToIIN();
}

void OActions::OnReceiveAPDU(OState& ostate, const openpal::ReadBufferView& apdu)
{	
	FORMAT_HEX_BLOCK(ostate.logger, flags::APP_HEX_RX, apdu, 18, 18);

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
			ostate.pAuthProvider->OnReceive(ostate, apdu, header, objects);			
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

void OActions::ProcessHeaderAndObjects(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{	
	if (Functions::IsNoAckFuncCode(header.function))
	{
		// this is the only request we process while we are transmitting
		// because it doesn't require a response of any kind
		OFunctions::ProcessRequestNoAck(ostate, header, objects);
	}
	else
	{
		if (ostate.isTransmitting)
		{			
			ostate.deferred.Set(header, objects);
		}
		else
		{
			if (header.function == FunctionCode::CONFIRM)
			{
				OActions::ProcessConfirm(ostate, header);				
			}
			else
			{
				OActions::ProcessRequest(ostate, header, objects);				
			}
		}
	}
}

void OActions::OnSendResult(OState& ostate, bool isSuccess)
{
	if (ostate.isTransmitting)
	{
		ostate.isTransmitting = false;
		OActions::CheckForTaskStart(ostate);
	}	
}

void OActions::ProcessRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	if (header.control.UNS)
	{		
		FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Ignoring unsol with invalid function code: %s", FunctionCodeToString(header.function));		
	}
	else
	{		
		ostate.sol.pState = OActions::OnReceiveSolRequest(ostate, header, objects);		
	}
}

void OActions::ProcessConfirm(OState& ostate, const APDUHeader& header)
{
	if (header.control.UNS)
	{
		OActions::ProcessUnsolicitedConfirm(ostate, header);
	}
	else
	{
		OActions::ProcessSolicitedConfirm(ostate, header);
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
	ostate.history.RecordLastProcessedRequest(header, objects);

	auto response = ostate.sol.tx.Start();
	auto writer = response.GetWriter();
	response.SetFunction(FunctionCode::RESPONSE);
	response.SetControl(AppControlField(true, true, false, false, header.control.SEQ));
	auto iin = OFunctions::HandleNonReadResponse(ostate, header, objects, writer);
	response.SetIIN(iin | GetResponseIIN(ostate));
	OActions::BeginResponseTx(ostate, response.ToReadOnly());
	return &OutstationSolicitedStateIdle::Inst();
}

OutstationSolicitedStateBase* OActions::RespondToReadRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	ostate.history.RecordLastProcessedRequest(header, objects);

	auto response = ostate.sol.tx.Start();
	auto writer = response.GetWriter();
	response.SetFunction(FunctionCode::RESPONSE);
	auto result = OFunctions::HandleRead(ostate, objects, writer);
	result.second.SEQ = header.control.SEQ;
	ostate.sol.seq.confirmNum = header.control.SEQ;
	response.SetControl(result.second);
	response.SetIIN(result.first | OActions::GetResponseIIN(ostate));		
	OActions::BeginResponseTx(ostate, response.ToReadOnly());

	if (result.second.CON)
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
	ostate.unsol.seq.confirmNum = ostate.unsol.seq.num;
	ostate.unsol.seq.num = AppControlField::NextSeq(ostate.unsol.seq.num);
	ostate.pLower->BeginTransmit(response);
}

void OActions::ProcessSolicitedConfirm(OState& ostate, const APDUHeader& header)
{
	ostate.sol.pState = ostate.sol.pState->OnConfirm(ostate, header);
}

void OActions::ProcessUnsolicitedConfirm(OState& ostate, const APDUHeader& header)
{
	ostate.unsol.pState = ostate.unsol.pState->OnConfirm(ostate, header);
}

OutstationSolicitedStateBase* OActions::ContinueMultiFragResponse(OState& ostate, uint8_t seq)
{	
	auto response = ostate.sol.tx.Start();	
	auto writer = response.GetWriter();
	response.SetFunction(FunctionCode::RESPONSE);
	auto control = ostate.rspContext.LoadResponse(writer);
	control.SEQ = seq;
	ostate.sol.seq.confirmNum = seq;
	response.SetControl(control);
	response.SetIIN(GetResponseIIN(ostate));
	OActions::BeginResponseTx(ostate, response.ToReadOnly());
	
	if (control.CON)
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
	// do these checks in order of priority
	ostate.pAuthProvider->CheckState(ostate);
	OActions::CheckForDeferredRequest(ostate);
	OActions::CheckForUnsolicited(ostate);
}

void OActions::CheckForDeferredRequest(OState& ostate)
{
	if (ostate.CanTransmit() && ostate.deferred.IsSet())
	{		
		auto handler = [&ostate](const APDUHeader& header, const ReadBufferView& objects)
		{
			return OActions::ProcessDeferredRequest(ostate, header, objects);
		};
		ostate.deferred.Process(handler);
	}
}

bool OActions::ProcessDeferredRequest(OState& ostate, APDUHeader header, openpal::ReadBufferView objects)
{
	if (header.function == FunctionCode::CONFIRM)
	{
		OActions::ProcessConfirm(ostate, header);
		return true;
	}
	else
	{
		if (header.function == FunctionCode::READ)
		{
			if (ostate.unsol.IsIdle())
			{
				OActions::ProcessRequest(ostate, header, objects);
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			OActions::ProcessRequest(ostate, header, objects);
			return true;
		}
	}
}

void OActions::CheckForUnsolicited(OState& ostate)
{
	if (ostate.CanTransmit() && ostate.unsol.IsIdle() && ostate.params.allowUnsolicited)
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
				build::NullUnsolicited(response, ostate.unsol.seq.num, GetResponseIIN(ostate));				
				OActions::StartUnsolicitedConfirmTimer(ostate);
				ostate.unsol.pState = &OutstationUnsolicitedStateConfirmWait::Inst();
				OActions::BeginUnsolTx(ostate, response.ToReadOnly());
			}			
		}
		else
		{
			// send a NULL unsolcited message									
			auto response = ostate.unsol.tx.Start();
			build::NullUnsolicited(response, ostate.unsol.seq.num, GetResponseIIN(ostate));
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

}

