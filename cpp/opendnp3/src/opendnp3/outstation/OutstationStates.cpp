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

// --------------------- OutstationSolicitedStateBase ----------------------

void OutstationSolicitedStateBase::OnConfirm(OutstationContext* pContext, const APDUHeader& header)
{
	FORMAT_LOG_BLOCK(pContext->logger, flags::WARN, "Unexpected solicted confirm with sequence: %u", header.control.SEQ);
}

void OutstationSolicitedStateBase::OnSendResult(OutstationContext* pContext, bool isSucccess)
{
	SIMPLE_LOG_BLOCK(pContext->logger, flags::WARN, "Unexpected send result callback");
}

void OutstationSolicitedStateBase::OnConfirmTimeout(OutstationContext* pContext)
{
	SIMPLE_LOG_BLOCK(pContext->logger, flags::WARN, "Unexpected confirm timeout");
}

// --------------------- OutstationStateIdle ----------------------


OutstationSolicitedStateIdle OutstationSolicitedStateIdle::instance;

OutstationSolicitedStateBase& OutstationSolicitedStateIdle::Inst()
{
	return instance;
}

void OutstationSolicitedStateIdle::OnNewRequest(OutstationContext* pContext, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, APDUEquality equality)
{
	if (pContext->txState == TxState::IDLE)
	{				
		pContext->RespondToNonReadRequest(header, objects, equality == APDUEquality::OBJECT_HEADERS_EQUAL);
	}
	else
	{	
		pContext->deferredRequest.Set(DeferredRequest(header, equality));		
	}
}

void OutstationSolicitedStateIdle::OnRepeatRequest(OutstationContext* pContext, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects)
{
	if (pContext->txState == TxState::IDLE)
	{
		pContext->BeginResponseTx(pContext->lastResponse);		
	}
}

void OutstationSolicitedStateIdle::OnSendResult(OutstationContext* pContext, bool isSucccess)
{	
	pContext->OnEnterIdleState();
}

// --------------------- OutstationStateSolConfirmWait ----------------------

OutstationStateSolicitedConfirmWait OutstationStateSolicitedConfirmWait::instance;

OutstationSolicitedStateBase& OutstationStateSolicitedConfirmWait::Inst()
{
	return instance;
}

void OutstationStateSolicitedConfirmWait::OnNewRequest(OutstationContext* pContext, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, APDUEquality equality)
{
	if (pContext->txState == TxState::IDLE)
	{
		pContext->CancelConfirmTimer();		
		pContext->rspContext.Reset();
		pContext->eventBuffer.Reset();
		pContext->deferredRequest.Set(DeferredRequest(header, equality));
		pContext->pSolicitedState = &OutstationSolicitedStateIdle::Inst();
		pContext->OnEnterIdleState();
	}
	else
	{	
		pContext->deferredRequest.Set(DeferredRequest(header, equality));		
	}	
}

void OutstationStateSolicitedConfirmWait::OnRepeatRequest(OutstationContext* pContext, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects)
{
	if (pContext->txState == TxState::IDLE)
	{
		pContext->CancelConfirmTimer();
		pContext->BeginResponseTx(pContext->lastResponse);
	}
}

void OutstationStateSolicitedConfirmWait::OnSendResult(OutstationContext* pContext, bool isSucccess)
{	
	if (isSucccess)
	{
		pContext->StartConfirmTimer();
	}
	else
	{
		pContext->pSolicitedState = &OutstationSolicitedStateIdle::Inst();
	}		
}

void OutstationStateSolicitedConfirmWait::OnConfirm(OutstationContext* pContext, const APDUHeader& header)
{
	if (pContext->pConfirmTimer)
	{
		if (header.control.SEQ == pContext->expectedSolConfirmSeq)
		{
			pContext->pSolicitedState = &OutstationSolicitedStateIdle::Inst();
			pContext->CancelConfirmTimer();			
			pContext->eventBuffer.Clear();
			if (pContext->rspContext.IsComplete())
			{
				pContext->OnEnterIdleState();
			}
			else // Continue response
			{
				pContext->ContinueMultiFragResponse(AppControlField::NextSeq(header.control.SEQ));
			}
		}
		else
		{
			FORMAT_LOG_BLOCK(pContext->logger, flags::WARN, "Confirm with wrong seq: %u", header.control.SEQ);
		}		
	}
	else
	{
		// we're still sending so this can't be our confirm
		FORMAT_LOG_BLOCK(pContext->logger, flags::WARN, "Unexpected confirm with seq: %u", header.control.SEQ);
	}	
}

void OutstationStateSolicitedConfirmWait::OnConfirmTimeout(OutstationContext* pContext)
{
	if (pContext->pConfirmTimer)
	{
		pContext->pConfirmTimer = nullptr;		
		pContext->eventBuffer.Reset();
		pContext->rspContext.Reset();
		pContext->pSolicitedState = &OutstationSolicitedStateIdle::Inst();
		pContext->OnEnterIdleState();
	}
}

/*
// --------------------- OutstationStateUnsolConfirmWait ----------------------

OutstationStateUnsolConfirmWait OutstationStateUnsolConfirmWait::instance;

OutstationStateBase& OutstationStateUnsolConfirmWait::Inst()
{
	return instance;
}

void OutstationStateUnsolConfirmWait::OnNewRequest(OutstationContext* pContext, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects, APDUEquality equality)
{
	if (pContext->isTransmitting)
	{
		pContext->deferredRequest.Set(DeferredRequest(header, equality));
	}
	else
	{
		if (header.function == FunctionCode::READ)
		{
			// read requests are deferred until we re-enter the idle state
			pContext->deferredRequest.Set(DeferredRequest(header, equality));
		}
		else
		{
			// non-read requests are responded to immediately
			pContext->RespondToRequest(header, objects, equality);
		}
	}
}

void OutstationStateUnsolConfirmWait::OnRepeatRequest(OutstationContext* pContext, const APDUHeader& header, const openpal::ReadOnlyBuffer& objects)
{
	if (!pContext->isTransmitting && header.function != FunctionCode::READ)	
	{
		pContext->isTransmitting = true;
		pContext->pLower->BeginTransmit(pContext->lastResponse);
	}
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

void OutstationStateUnsolConfirmWait::OnUnsolConfirm(OutstationContext* pContext, const APDUHeader& header)
{
	if (pContext->pConfirmTimer)
	{
		if (header.control.SEQ == pContext->expectedUnsolConfirmSeq)
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
			FORMAT_LOG_BLOCK(pContext->logger, flags::WARN, "Unsolicited confirm with wrong seq: %u", header.control.SEQ);
		}
	}
	else
	{
		// we're still sending so this can't be our confirm
		FORMAT_LOG_BLOCK(pContext->logger, flags::WARN, "Unexpected unsol confirm with seq: %u", header.control.SEQ);
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
*/

}
