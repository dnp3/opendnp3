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
#include "opendnp3/outstation/OutstationActions.h"
#include "opendnp3/outstation/OutstationFunctions.h"
#include "opendnp3/app/FunctionHelpers.h"


#include <openpal/logging/LogMacros.h>

namespace opendnp3
{

// --------------------- OutstationSolicitedStateBase ----------------------

OutstationSolicitedStateBase* OutstationSolicitedStateBase::OnConfirm(OState& ostate, const APDUHeader& header)
{
	FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Unexpected solicted confirm with sequence: %u", header.control.SEQ);
	return this;
}

OutstationSolicitedStateBase* OutstationSolicitedStateBase::OnSendResult(OState& ostate, bool isSucccess)
{
	SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Unexpected send result callback");
	return this;
}

OutstationSolicitedStateBase* OutstationSolicitedStateBase::OnConfirmTimeout(OState& ostate)
{
	SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Unexpected confirm timeout");
	return this;
}

OutstationSolicitedStateBase* OutstationSolicitedStateBase::OnNewReadRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	ostate.deferred.Set(header, objects);	
	return this;
}

OutstationSolicitedStateBase* OutstationSolicitedStateBase::OnNewNonReadRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	ostate.deferred.Set(header, objects);
	return this;
}

OutstationSolicitedStateBase* OutstationSolicitedStateBase::OnRepeatNonReadRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	ostate.deferred.Set(header, objects);
	return this;
}

// --------------------- OutstationStateIdle ----------------------


OutstationSolicitedStateIdle OutstationSolicitedStateIdle::instance;

OutstationSolicitedStateBase& OutstationSolicitedStateIdle::Inst()
{
	return instance;
}

OutstationSolicitedStateBase* OutstationSolicitedStateIdle::OnNewReadRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	if (ostate.unsol.IsIdle())
	{
		return OActions::RespondToReadRequest(ostate, header.control.SEQ, objects);
	}
	else
	{
		ostate.deferred.Set(header, objects);
		return this;
	}
}

OutstationSolicitedStateBase* OutstationSolicitedStateIdle::OnNewNonReadRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	return OActions::RespondToNonReadRequest(ostate, header, objects);			
}

OutstationSolicitedStateBase* OutstationSolicitedStateIdle::OnRepeatNonReadRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{					
	OActions::BeginResponseTx(ostate, ostate.sol.tx.GetLastResponse());
	return this;			
}

// --------------------- OutstationStateSolConfirmWait ----------------------

OutstationStateSolicitedConfirmWait OutstationStateSolicitedConfirmWait::instance;

OutstationSolicitedStateBase& OutstationStateSolicitedConfirmWait::Inst()
{
	return instance;
}

OutstationSolicitedStateBase* OutstationStateSolicitedConfirmWait::OnNewReadRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{	
	ostate.deferred.Set(header, objects);
	ostate.confirmTimer.Cancel();
	return &OutstationSolicitedStateIdle::Inst();
			
}

OutstationSolicitedStateBase* OutstationStateSolicitedConfirmWait::OnNewNonReadRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects)
{
	ostate.deferred.Set(header, objects);
	ostate.confirmTimer.Cancel();
	return &OutstationSolicitedStateIdle::Inst();
}

OutstationSolicitedStateBase* OutstationStateSolicitedConfirmWait::OnConfirm(OState& ostate, const APDUHeader& header)
{	
	if (header.control.SEQ == ostate.sol.seq.confirmNum)
	{
		ostate.confirmTimer.Cancel();

		// Lock the database for the remainder of this method as we will be manipulating the buffers
		Transaction tx(ostate.database);				
		ostate.eventBuffer.ClearWritten();

		if (ostate.rspContext.HasSelection())
		{						
			return OActions::ContinueMultiFragResponse(ostate, AppControlField::NextSeq(header.control.SEQ));
		}
		else 
		{
			return &OutstationSolicitedStateIdle::Inst();
		}
	}
	else
	{
		FORMAT_LOG_BLOCK(ostate.logger, flags::WARN, "Confirm with wrong seq: %u", header.control.SEQ);
		return this;
	}			
}

OutstationSolicitedStateBase* OutstationStateSolicitedConfirmWait::OnConfirmTimeout(OState& ostate)
{	
	SIMPLE_LOG_BLOCK(ostate.logger, flags::WARN, "Solicited confirm timeout");	
	return &OutstationSolicitedStateIdle::Inst();	
}

}

