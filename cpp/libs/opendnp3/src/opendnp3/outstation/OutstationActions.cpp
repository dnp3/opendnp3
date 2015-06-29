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

IINField OActions::GetResponseIIN(OContext& ocontext)
{
	return ocontext.staticIIN | GetDynamicIIN(ocontext) | ocontext.pApplication->GetApplicationIIN().ToIIN();
}

void OActions::OnReceiveAPDU(OContext& ocontext, const openpal::ReadBufferView& apdu)
{	
	FORMAT_HEX_BLOCK(ocontext.logger, flags::APP_HEX_RX, apdu, 18, 18);

	APDUHeader header;	
	if (APDUHeaderParser::ParseRequest(apdu, header, &ocontext.logger))
	{
		FORMAT_LOG_BLOCK(ocontext.logger, flags::APP_HEADER_RX,
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
			ocontext.auth.OnReceive(ocontext, apdu, header, objects);			
		}
		else
		{
			SIMPLE_LOG_BLOCK(ocontext.logger, flags::WARN, "Ignoring fragment. Requst must be FIR/FIN/!CON");
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(ocontext.logger, flags::ERR, "ignoring malformed request header");
	}	
}

void OActions::ProcessHeaderAndObjects(OContext& ocontext, const APDUHeader& header, const openpal::ReadBufferView& objects)
{	
	if (Functions::IsNoAckFuncCode(header.function))
	{
		// this is the only request we process while we are transmitting
		// because it doesn't require a response of any kind
		OFunctions::ProcessRequestNoAck(ocontext, header, objects);
	}
	else
	{
		if (ocontext.isTransmitting)
		{			
			ocontext.deferred.Set(header, objects);
		}
		else
		{
			if (header.function == FunctionCode::CONFIRM)
			{
				OActions::ProcessConfirm(ocontext, header);				
			}
			else
			{
				OActions::ProcessRequest(ocontext, header, objects);				
			}
		}
	}
}

void OActions::OnSendResult(OContext& ocontext, bool isSuccess)
{
	if (ocontext.isTransmitting)
	{
		ocontext.isTransmitting = false;
		OActions::CheckForTaskStart(ocontext);
	}	
}

void OActions::ProcessRequest(OContext& ocontext, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	if (header.control.UNS)
	{		
		FORMAT_LOG_BLOCK(ocontext.logger, flags::WARN, "Ignoring unsol with invalid function code: %s", FunctionCodeToString(header.function));		
	}
	else
	{		
		ocontext.sol.pState = ocontext.OnReceiveSolRequest(header, objects);		
	}
}

void OActions::ProcessConfirm(OContext& ocontext, const APDUHeader& header)
{
	if (header.control.UNS)
	{
		OActions::ProcessUnsolicitedConfirm(ocontext, header);
	}
	else
	{
		OActions::ProcessSolicitedConfirm(ocontext, header);
	}
}

void OActions::BeginResponseTx(OContext& ocontext, const ReadBufferView& response)
{	
	ocontext.sol.tx.Record(response);
	BeginTx(ocontext, response);	
}

void OActions::BeginUnsolTx(OContext& ocontext, const ReadBufferView& response)
{				
	ocontext.unsol.tx.Record(response);
	ocontext.unsol.seq.confirmNum = ocontext.unsol.seq.num;
	ocontext.unsol.seq.num.Increment();
	BeginTx(ocontext, response);	
}

void OActions::BeginTx(OContext& ocontext, const openpal::ReadBufferView& response)
{
	logging::ParseAndLogResponseTx(ocontext.logger, response);
	ocontext.isTransmitting = true;
	ocontext.pLower->BeginTransmit(response);
}

void OActions::ProcessSolicitedConfirm(OContext& ocontext, const APDUHeader& header)
{
	ocontext.sol.pState = ocontext.sol.pState->OnConfirm(ocontext, header);
}

void OActions::ProcessUnsolicitedConfirm(OContext& ocontext, const APDUHeader& header)
{
	ocontext.unsol.pState = ocontext.unsol.pState->OnConfirm(ocontext, header);
}

void OActions::CheckForTaskStart(OContext& ocontext)
{		
	// do these checks in order of priority
	ocontext.auth.CheckState(ocontext);
	OActions::CheckForDeferredRequest(ocontext);
	OActions::CheckForUnsolicited(ocontext);
}

void OActions::CheckForDeferredRequest(OContext& ocontext)
{
	if (ocontext.CanTransmit() && ocontext.deferred.IsSet())
	{		
		auto handler = [&ocontext](const APDUHeader& header, const ReadBufferView& objects)
		{
			return OActions::ProcessDeferredRequest(ocontext, header, objects);
		};
		ocontext.deferred.Process(handler);
	}
}

bool OActions::ProcessDeferredRequest(OContext& ocontext, APDUHeader header, openpal::ReadBufferView objects)
{
	if (header.function == FunctionCode::CONFIRM)
	{
		OActions::ProcessConfirm(ocontext, header);
		return true;
	}
	else
	{
		if (header.function == FunctionCode::READ)
		{
			if (ocontext.unsol.IsIdle())
			{
				OActions::ProcessRequest(ocontext, header, objects);
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			OActions::ProcessRequest(ocontext, header, objects);
			return true;
		}
	}
}

void OActions::CheckForUnsolicited(OContext& ocontext)
{
	if (ocontext.CanTransmit() && ocontext.unsol.IsIdle() && ocontext.params.allowUnsolicited)
	{
		if (ocontext.unsol.completedNull)
		{				
			// are there events to be reported?
			if (ocontext.params.unsolClassMask.Intersects(ocontext.eventBuffer.UnwrittenClassField()))
			{			
				
				auto response = ocontext.unsol.tx.Start();
				auto writer = response.GetWriter();
										
				ocontext.eventBuffer.Unselect();
				ocontext.eventBuffer.SelectAllByClass(ocontext.params.unsolClassMask);
				ocontext.eventBuffer.Load(writer);					
				
				build::NullUnsolicited(response, ocontext.unsol.seq.num, GetResponseIIN(ocontext));				
				OActions::StartUnsolicitedConfirmTimer(ocontext);
				ocontext.unsol.pState = &OutstationUnsolicitedStateConfirmWait::Inst();
				OActions::BeginUnsolTx(ocontext, response.ToReadOnly());
			}			
		}
		else
		{
			// send a NULL unsolcited message									
			auto response = ocontext.unsol.tx.Start();
			build::NullUnsolicited(response, ocontext.unsol.seq.num, GetResponseIIN(ocontext));
			OActions::StartUnsolicitedConfirmTimer(ocontext);
			ocontext.unsol.pState = &OutstationUnsolicitedStateConfirmWait::Inst();
			OActions::BeginUnsolTx(ocontext, response.ToReadOnly());
		}
	}	
}

bool OActions::StartSolicitedConfirmTimer(OContext& ocontext)
{
	auto timeout = [&]()
	{ 
		ocontext.sol.pState = ocontext.sol.pState->OnConfirmTimeout(ocontext);
		OActions::CheckForTaskStart(ocontext);
	};
	return ocontext.confirmTimer.Start(ocontext.params.unsolConfirmTimeout, timeout);
}

bool OActions::StartUnsolicitedConfirmTimer(OContext& ocontext)
{
	auto timeout = [&]()
	{ 
		ocontext.unsol.pState = ocontext.unsol.pState->OnConfirmTimeout(ocontext);
		OActions::CheckForTaskStart(ocontext);
	};
	return ocontext.confirmTimer.Start(ocontext.params.unsolConfirmTimeout, timeout);
}

IINField OActions::GetDynamicIIN(OContext& ocontext)
{
	auto classField = ocontext.eventBuffer.UnwrittenClassField();

	IINField ret;
	ret.SetBitToValue(IINBit::CLASS1_EVENTS, classField.HasClass1());
	ret.SetBitToValue(IINBit::CLASS2_EVENTS, classField.HasClass2());
	ret.SetBitToValue(IINBit::CLASS3_EVENTS, classField.HasClass3());
	ret.SetBitToValue(IINBit::EVENT_BUFFER_OVERFLOW, ocontext.eventBuffer.IsOverflown());

	return ret;
}

}

