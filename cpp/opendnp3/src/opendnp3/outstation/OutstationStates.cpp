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

#include "OutstationStates.h"

#include "opendnp3/LogLevels.h"
#include "opendnp3/outstation/OutstationContext.h"


#include <openpal/LogMacros.h>

namespace opendnp3
{

// --------------------- OutstationStateBase ----------------------

bool OutstationStateBase::IsIdle()
{
	return false;
}

void OutstationStateBase::OnSolConfirm(OutstationContext* pContext, const APDURecord& frag)
{
	FORMAT_LOG_BLOCK(pContext->logger, flags::WARN, "Unexpected sol confirm with sequence: %u", frag.control.SEQ);
}

void OutstationStateBase::OnUnsolConfirm(OutstationContext* pContext, const APDURecord& frag)
{
	FORMAT_LOG_BLOCK(pContext->logger, flags::WARN, "Unexpected unsol confirm with sequence: %u", frag.control.SEQ);
}

void OutstationStateBase::OnSendResult(OutstationContext* pContext, bool isSucccess)
{
	SIMPLE_LOG_BLOCK(pContext->logger, flags::WARN, "Unexpected send result callback");
}

void OutstationStateBase::OnConfirmTimeout(OutstationContext* pContext)
{
	SIMPLE_LOG_BLOCK(pContext->logger, flags::WARN, "Unexpected confirm timeout");
}

// --------------------- OutstationStateIdle ----------------------


OutstationStateIdle OutstationStateIdle::instance;

OutstationStateBase& OutstationStateIdle::Inst()
{
	return instance;
}

bool OutstationStateIdle::IsIdle()
{
	return true;
}

void OutstationStateIdle::OnNewRequest(OutstationContext* pContext, const APDURecord& request, APDUEquality equality)
{
	if (pContext->isTransmitting)
	{
		SIMPLE_LOG_BLOCK(pContext->logger, flags::WARN, "IdleTransmitting - Received new request while transmitting response, remote is flooding");
	}
	else
	{
		pContext->solSeqN = request.control.SEQ;
		pContext->RespondToRequest(request, equality);
	}
}

void OutstationStateIdle::OnRepeatRequest(OutstationContext* pContext, const APDURecord& frag)
{
	if (pContext->isTransmitting)
	{
		SIMPLE_LOG_BLOCK(pContext->logger, flags::WARN, "IdleTransmitting - Received repeat request while transmitting response, remote is flooding");
	}
	else
	{
		pContext->isTransmitting = true;
		pContext->pLower->BeginTransmit(pContext->lastResponse);
	}
}

void OutstationStateIdle::OnSendResult(OutstationContext* pContext, bool isSucccess)
{	
	pContext->OnEnterIdleState();
}

// --------------------- OutstationStateSolConfirmWait ----------------------

OutstationStateSolConfirmWait OutstationStateSolConfirmWait::instance;

OutstationStateBase& OutstationStateSolConfirmWait::Inst()
{
	return instance;
}

void OutstationStateSolConfirmWait::OnNewRequest(OutstationContext* pContext, const APDURecord& request, APDUEquality equality)
{
	if (pContext->isTransmitting)
	{
		SIMPLE_LOG_BLOCK(pContext->logger, flags::WARN, "SolConfirmWait - Received new request while transmitting response, remote is flooding");
	}
	else
	{
		pContext->solSeqN = request.control.SEQ;
		pContext->CancelConfirmTimer();
		pContext->pState = &OutstationStateIdle::Inst();
		pContext->rspContext.Reset();
		pContext->eventBuffer.Reset();
		pContext->RespondToRequest(request, equality);
	}	
}

void OutstationStateSolConfirmWait::OnRepeatRequest(OutstationContext* pContext, const APDURecord& frag)
{
	if (pContext->isTransmitting)
	{
		SIMPLE_LOG_BLOCK(pContext->logger, flags::WARN, "SolConfirmWait - Received repeat request while transmitting response, remote is flooding");
	}
	else
	{
		pContext->CancelConfirmTimer();
		pContext->isTransmitting = true;
		pContext->pLower->BeginTransmit(pContext->lastResponse);
	}
}

void OutstationStateSolConfirmWait::OnSendResult(OutstationContext* pContext, bool isSucccess)
{	
	if (isSucccess)
	{
		pContext->StartConfirmTimer();
	}
	else
	{		
		pContext->pState = &OutstationStateIdle::Inst();
	}	
}

void OutstationStateSolConfirmWait::OnSolConfirm(OutstationContext* pContext, const APDURecord& confirm)
{
	if (pContext->pConfirmTimer)
	{
		if (confirm.control.SEQ == pContext->expectedSolConfirmSeq)
		{
			pContext->pState = &OutstationStateIdle::Inst();
			pContext->CancelConfirmTimer();			
			pContext->eventBuffer.Clear();
			if (pContext->rspContext.IsComplete())
			{
				pContext->OnEnterIdleState();
			}
			else // Continue response
			{
				pContext->ContinueMultiFragResponse(AppControlField::NextSeq(confirm.control.SEQ));
			}
		}
		else
		{
			FORMAT_LOG_BLOCK(pContext->logger, flags::WARN, "Confirm with wrong seq: %u", confirm.control.SEQ);
		}		
	}
	else
	{
		// we're still sending so this can't be our confirm
		FORMAT_LOG_BLOCK(pContext->logger, flags::WARN, "Unexpected confirm with seq: %u", confirm.control.SEQ);
	}	
}

void OutstationStateSolConfirmWait::OnConfirmTimeout(OutstationContext* pContext)
{
	if (pContext->pConfirmTimer)
	{
		pContext->pConfirmTimer = nullptr;		
		pContext->eventBuffer.Reset();
		pContext->rspContext.Reset();
		pContext->pState = &OutstationStateIdle::Inst();
		pContext->OnEnterIdleState();
	}
}

// --------------------- OutstationStateUnsolConfirmWait ----------------------

OutstationStateUnsolConfirmWait OutstationStateUnsolConfirmWait::instance;

OutstationStateBase& OutstationStateUnsolConfirmWait::Inst()
{
	return instance;
}

void OutstationStateUnsolConfirmWait::OnNewRequest(OutstationContext* pContext, const APDURecord& request, APDUEquality equality)
{
	// TODO
}

void OutstationStateUnsolConfirmWait::OnRepeatRequest(OutstationContext* pContext, const APDURecord& frag)
{
	// TODO
}

void OutstationStateUnsolConfirmWait::OnSendResult(OutstationContext* pContext, bool isSucccess)
{
	if (isSucccess)
	{
		pContext->StartConfirmTimer();
	}
	else
	{		
		pContext->StartUnsolRetryTimer();
		pContext->pState = &OutstationStateIdle::Inst();
	}	
}

void OutstationStateUnsolConfirmWait::OnUnsolConfirm(OutstationContext* pContext, const APDURecord& confirm)
{
	if (pContext->pConfirmTimer)
	{
		if (confirm.control.SEQ == pContext->expectedUnsolConfirmSeq)
		{
			pContext->CancelConfirmTimer();
			pContext->pState = &OutstationStateIdle::Inst();
					
			if (pContext->completedNullUnsol)
			{
				pContext->eventBuffer.Clear();
			}
			else
			{
				pContext->completedNullUnsol = true;
			}
			
			pContext->OnEnterIdleState();
		}
		else
		{
			FORMAT_LOG_BLOCK(pContext->logger, flags::WARN, "Unsolicited confirm with wrong seq: %u", confirm.control.SEQ);
		}
	}
	else
	{
		// we're still sending so this can't be our confirm
		FORMAT_LOG_BLOCK(pContext->logger, flags::WARN, "Unexpected unsol confirm with seq: %u", confirm.control.SEQ);
	}
}

void OutstationStateUnsolConfirmWait::OnConfirmTimeout(OutstationContext* pContext)
{
	if (pContext->pConfirmTimer)
	{
		pContext->pConfirmTimer = nullptr;

		if (pContext->completedNullUnsol)
		{
			// the unsol message contained measurements, so reset the event buffer
			pContext->eventBuffer.Reset();			
		}

		pContext->StartUnsolRetryTimer();
		pContext->pState = &OutstationStateIdle::Inst();		
	}
}

}
