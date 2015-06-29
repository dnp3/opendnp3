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
				
				build::NullUnsolicited(response, ocontext.unsol.seq.num, ocontext.GetResponseIIN());				
				OActions::StartUnsolicitedConfirmTimer(ocontext);
				ocontext.unsol.pState = &OutstationUnsolicitedStateConfirmWait::Inst();
				OActions::BeginUnsolTx(ocontext, response.ToReadOnly());
			}			
		}
		else
		{
			// send a NULL unsolcited message									
			auto response = ocontext.unsol.tx.Start();
			build::NullUnsolicited(response, ocontext.unsol.seq.num, ocontext.GetResponseIIN());
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
		ocontext.CheckForTaskStart();
	};
	return ocontext.confirmTimer.Start(ocontext.params.unsolConfirmTimeout, timeout);
}

bool OActions::StartUnsolicitedConfirmTimer(OContext& ocontext)
{
	auto timeout = [&]()
	{ 
		ocontext.unsol.pState = ocontext.unsol.pState->OnConfirmTimeout(ocontext);
		ocontext.CheckForTaskStart();
	};
	return ocontext.confirmTimer.Start(ocontext.params.unsolConfirmTimeout, timeout);
}

}

