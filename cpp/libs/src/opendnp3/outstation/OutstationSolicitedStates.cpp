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

#include "OutstationSolicitedStates.h"

#include "opendnp3/LogLevels.h"
#include "opendnp3/outstation/OutstationContext.h"

#include <openpal/logging/LogMacros.h>

namespace opendnp3
{

// --------------------- OutstationSolicitedStateBase ----------------------

OutstationSolicitedStateBase* OutstationSolicitedStateBase::OnConfirm(OContext& ocontext, const APDUHeader& header)
{
	FORMAT_LOG_BLOCK(ocontext.logger, flags::WARN, "Unexpected solicted confirm with sequence: %u", header.control.SEQ);
	return this;
}

OutstationSolicitedStateBase* OutstationSolicitedStateBase::OnSendResult(OContext& ocontext, bool isSucccess)
{
	SIMPLE_LOG_BLOCK(ocontext.logger, flags::WARN, "Unexpected send result callback");
	return this;
}

OutstationSolicitedStateBase* OutstationSolicitedStateBase::OnConfirmTimeout(OContext& ocontext)
{
	SIMPLE_LOG_BLOCK(ocontext.logger, flags::WARN, "Unexpected confirm timeout");
	return this;
}

OutstationSolicitedStateBase* OutstationSolicitedStateBase::OnNewReadRequest(OContext& ocontext, const APDUHeader& header, const openpal::RSlice& objects)
{
	ocontext.deferred.Set(header, objects);	
	return this;
}

OutstationSolicitedStateBase* OutstationSolicitedStateBase::OnNewNonReadRequest(OContext& ocontext, const APDUHeader& header, const openpal::RSlice& objects)
{
	ocontext.deferred.Set(header, objects);
	return this;
}

OutstationSolicitedStateBase* OutstationSolicitedStateBase::OnRepeatNonReadRequest(OContext& ocontext, const APDUHeader& header, const openpal::RSlice& objects)
{
	ocontext.deferred.Set(header, objects);
	return this;
}

// --------------------- OutstationStateIdle ----------------------


OutstationSolicitedStateIdle OutstationSolicitedStateIdle::instance;

OutstationSolicitedStateBase& OutstationSolicitedStateIdle::Inst()
{
	return instance;
}

OutstationSolicitedStateBase* OutstationSolicitedStateIdle::OnNewReadRequest(OContext& ocontext, const APDUHeader& header, const openpal::RSlice& objects)
{
	if (ocontext.unsol.IsIdle())
	{
		return ocontext.RespondToReadRequest(header, objects);
	}
	else
	{
		ocontext.deferred.Set(header, objects);
		return this;
	}
}

OutstationSolicitedStateBase* OutstationSolicitedStateIdle::OnNewNonReadRequest(OContext& ocontext, const APDUHeader& header, const openpal::RSlice& objects)
{
	return ocontext.RespondToNonReadRequest(header, objects);
}

OutstationSolicitedStateBase* OutstationSolicitedStateIdle::OnRepeatNonReadRequest(OContext& ocontext, const APDUHeader& header, const openpal::RSlice& objects)
{					
	ocontext.BeginResponseTx(ocontext.sol.tx.GetLastResponse());
	return this;			
}

// --------------------- OutstationStateSolConfirmWait ----------------------

OutstationStateSolicitedConfirmWait OutstationStateSolicitedConfirmWait::instance;

OutstationSolicitedStateBase& OutstationStateSolicitedConfirmWait::Inst()
{
	return instance;
}

OutstationSolicitedStateBase* OutstationStateSolicitedConfirmWait::OnNewReadRequest(OContext& ocontext, const APDUHeader& header, const openpal::RSlice& objects)
{	
	ocontext.deferred.Set(header, objects);
	ocontext.confirmTimer.Cancel();
	return &OutstationSolicitedStateIdle::Inst();
			
}

OutstationSolicitedStateBase* OutstationStateSolicitedConfirmWait::OnNewNonReadRequest(OContext& ocontext, const APDUHeader& header, const openpal::RSlice& objects)
{
	ocontext.deferred.Set(header, objects);
	ocontext.confirmTimer.Cancel();
	return &OutstationSolicitedStateIdle::Inst();
}

OutstationSolicitedStateBase* OutstationStateSolicitedConfirmWait::OnConfirm(OContext& ocontext, const APDUHeader& header)
{	
	if (ocontext.sol.seq.confirmNum.Equals(header.control.SEQ))
	{
		ocontext.confirmTimer.Cancel();					
		ocontext.eventBuffer.ClearWritten();

		if (ocontext.rspContext.HasSelection())
		{						
			return ocontext.ContinueMultiFragResponse(AppSeqNum(header.control.SEQ).Next());
		}
		else 
		{
			return &OutstationSolicitedStateIdle::Inst();
		}
	}
	else
	{
		FORMAT_LOG_BLOCK(ocontext.logger, flags::WARN, "Confirm with wrong seq: %u", header.control.SEQ);
		return this;
	}			
}

OutstationSolicitedStateBase* OutstationStateSolicitedConfirmWait::OnConfirmTimeout(OContext& ocontext)
{	
	SIMPLE_LOG_BLOCK(ocontext.logger, flags::WARN, "Solicited confirm timeout");	
	return &OutstationSolicitedStateIdle::Inst();	
}

}

