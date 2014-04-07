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

#include <openpal/LogMacros.h>

#include "opendnp3/DNPErrorCodes.h"
#include "opendnp3/outstation/Outstation.h"
#include "opendnp3/LogLevels.h"

using namespace openpal;

namespace opendnp3
{

void OutstationStateBase::OnLowerLayerUp(Outstation* outstation)
{
	FORMAT_LOG_BLOCK_WITH_CODE(outstation->logger, flags::ERR, SERR_INVALID_STATE, "invalid action for state: %s", Name());
}

void OutstationStateBase::OnLowerLayerDown(Outstation* outstation)
{
	FORMAT_LOG_BLOCK_WITH_CODE(outstation->logger, flags::ERR, SERR_INVALID_STATE, "invalid action for state: %s", Name());
}

void OutstationStateBase::OnSolSendSuccess(Outstation* outstation)
{
	FORMAT_LOG_BLOCK_WITH_CODE(outstation->logger, flags::ERR, SERR_INVALID_STATE, "invalid action for state: %s", Name());
}

void OutstationStateBase::OnSolFailure(Outstation* outstation)
{
	FORMAT_LOG_BLOCK_WITH_CODE(outstation->logger, flags::ERR, SERR_INVALID_STATE, "invalid action for state: %s", Name());
}

void OutstationStateBase::OnUnsolSendSuccess(Outstation* outstation)
{
	FORMAT_LOG_BLOCK_WITH_CODE(outstation->logger, flags::ERR, SERR_INVALID_STATE, "invalid action for state: %s", Name());
}

void OutstationStateBase::OnUnsolFailure(Outstation* outstation)
{
	FORMAT_LOG_BLOCK_WITH_CODE(outstation->logger, flags::ERR, SERR_INVALID_STATE, "invalid action for state: %s", Name());
}

void OutstationStateBase::OnRequest(Outstation* outstation, const APDURecord&, SequenceInfo)
{
	FORMAT_LOG_BLOCK_WITH_CODE(outstation->logger, flags::ERR, SERR_INVALID_STATE, "invalid action for state: %s", Name());
}

// by default, the unsol timer expiration is deferd until it can be handled
void OutstationStateBase::OnUnsolExpiration(Outstation* outstation)
{
	outstation->mDeferredUnsol = true;
}

void OutstationStateBase::DoUnsolSuccess(Outstation* outstation)
{
	if (!outstation->mStartupNullUnsol) outstation->mStartupNullUnsol = true; //it was a null unsol packet
	//outstation->mRspContext.ClearAndReset(); TODO

	// this will cause us to immediately re-evaluate if we need to send another unsol rsp
	// we use the Deferred mechanism to give the outstation an opportunity to respond to any Deferred request instead
	outstation->mDeferredUnsol = true;
}

/* AS_Closed */

AS_Closed AS_Closed::mInstance;

void AS_Closed::OnLowerLayerUp(Outstation* outstation)
{
	// this is implemented as a simple timer because it can run if the outstation is connected/disconnected etc
	if (outstation->mConfig.mAllowTimeSync) outstation->ResetTimeIIN();
	outstation->ChangeState(AS_Idle::Inst());
}

void AS_Closed::Enter(Outstation* outstation)
{
	if(outstation->mpTimeTimer)
	{
		outstation->mpTimeTimer->Cancel();
		outstation->mpTimeTimer = nullptr;
	}
}

void AS_Closed::OnDataUpdate(Outstation* outstation)
{
	if(!outstation->mConfig.mDisableUnsol) outstation->mDeferredUnsol = true;
}

/* AS_OpenBase */

void AS_OpenBase::OnLowerLayerDown(Outstation* outstation)
{
	outstation->ChangeState(AS_Closed::Inst());
}

/* AS_Idle */

AS_Idle AS_Idle::mInstance;

void AS_Idle::OnRequest(Outstation* outstation, const APDURecord& record, SequenceInfo sequence)
{
	outstation->ChangeState(AS_WaitForRspSuccess::Inst());
	outstation->RespondToRequest(record, sequence);
}

void AS_Idle::Enter(Outstation* outstation)
{
	if(outstation->mCachedRequest.IsSet())
	{
		outstation->mCachedRequest.Apply(
		    [this, outstation](const APDURecord & record, SequenceInfo info)
		{
			this->OnRequest(outstation, record, info);
		}
		);
	}
	else
	{
		// check for unsolicited data to send
		if (outstation->mDeferredUnsol)
		{
			outstation->mDeferredUnsol = false;
			this->OnUnsolExpiration(outstation);
		}
	}
}

void AS_Idle::OnDataUpdate(Outstation* outstation)
{
	/*
	// start the unsol timer or act immediately if there's no pack timer
	if (!outstation->mConfig.mDisableUnsol && outstation->mStartupNullUnsol) // TODO && outstation->mRspContext.HasEvents(outstation->mConfig.mUnsolMask))
	{
		if (outstation->mConfig.mUnsolPackDelay.GetMilliseconds() <= 0)
		{
			// TODO
			//outstation->mRspContext.LoadUnsol(outstation->mUnsol, outstation->mIIN, outstation->mConfig.mUnsolMask);
			//outstation->SendUnsolicited(outstation->mUnsol);
			//outstation->ChangeState(AS_WaitForUnsolSuccess::Inst());
		}
		else if (outstation->mpUnsolTimer == nullptr)
		{
			outstation->StartUnsolTimer(outstation->mConfig.mUnsolPackDelay);
		}
	}
	*/
}

void AS_Idle::OnUnsolExpiration(Outstation* outstation)
{
	if (outstation->mStartupNullUnsol)
	{
		/* TODO
		if (outstation->mRspContext.HasEvents(outstation->mConfig.mUnsolMask))
		{

			outstation->ChangeState(AS_WaitForUnsolSuccess::Inst());
			outstation->mRspContext.LoadUnsol(outstation->mUnsol,  outstation->mIIN,  outstation->mConfig.mUnsolMask);
			outstation->SendUnsolicited(outstation->mUnsol);
		}
		*/
	}
	else
	{
		/* TODO
		// do the startup null unsol task
		outstation->ChangeState(AS_WaitForUnsolSuccess::Inst());
		outstation->mRspContext.LoadUnsol(outstation->mUnsol, outstation->mIIN, ClassMask(false, false, false));
		outstation->SendUnsolicited(outstation->mUnsol);
		*/
	}
}

/* AS_WaitForRspSuccess */

AS_WaitForRspSuccess AS_WaitForRspSuccess::mInstance;

void AS_WaitForRspSuccess::OnSolFailure(Outstation* outstation)
{
	outstation->ChangeState(AS_Idle::Inst());
}

void AS_WaitForRspSuccess::OnSolSendSuccess(Outstation* outstation)
{
	outstation->eventBuffer.Clear();

	if (outstation->rspContext.IsComplete())
	{
		outstation->ChangeState(AS_Idle::Inst());
	}
	else
	{
		outstation->ContinueResponse();
	}
}

// When we get a request we should no longer wait for confirmation, but we should
// immediately handle the new request. We implement this behavior asynchronously, by
// canceling the response transaction, and waiting for an OnFailure callback.
// The callback may still succeed if
void AS_WaitForRspSuccess::OnRequest(Outstation* outstation, const APDURecord& record, SequenceInfo seq)
{
	outstation->mpAppLayer->CancelResponse();
	outstation->mCachedRequest.Set(record, seq);
}

/* AS_WaitForUnsolSuccess */

AS_WaitForUnsolSuccess AS_WaitForUnsolSuccess::mInstance;

void AS_WaitForUnsolSuccess::OnUnsolFailure(Outstation* outstation)
{
	// if any unsol transaction fails, we re-enable the timer with the unsol retry delay
	// outstation->mRspContext.Reset(); TODO
	outstation->StartUnsolTimer(outstation->mConfig.mUnsolRetryDelay);
	outstation->ChangeState(AS_Idle::Inst());
}

void AS_WaitForUnsolSuccess::OnUnsolSendSuccess(Outstation* outstation)
{
	this->DoUnsolSuccess(outstation);
	outstation->ChangeState(AS_Idle::Inst());
}

void AS_WaitForUnsolSuccess::OnRequest(Outstation* outstation, const APDURecord& record, SequenceInfo sequence)
{
	if (record.function == FunctionCode::READ)
	{
		//read requests should be defered until after the unsol
		outstation->mCachedRequest.Set(record, sequence);
	}
	else
	{
		/* TODO
		outstation->mCachedRequest.Clear(); // idempotent
		outstation->ChangeState(AS_WaitForSolUnsolSuccess::Inst());
		outstation->RespondToRequest(record, sequence);
		*/
	}
}

/* AS_WaitForSolUnsolSuccess */

AS_WaitForSolUnsolSuccess AS_WaitForSolUnsolSuccess::mInstance;

void AS_WaitForSolUnsolSuccess::OnRequest(Outstation* outstation, const APDURecord& record, SequenceInfo sequence)
{
	// Both channels are busy... buffer the request
	outstation->mCachedRequest.Set(record, sequence);
}

void AS_WaitForSolUnsolSuccess::OnSolFailure(Outstation* outstation)
{
	outstation->ChangeState(AS_WaitForUnsolSuccess::Inst());
}

void AS_WaitForSolUnsolSuccess::OnSolSendSuccess(Outstation* outstation)
{
	outstation->ChangeState(AS_WaitForUnsolSuccess::Inst());
}

void AS_WaitForSolUnsolSuccess::OnUnsolFailure(Outstation* outstation)
{
	//outstation->mRspContext.Reset(); TODO
	outstation->ChangeState(AS_WaitForRspSuccess::Inst());
	if (outstation->mConfig.mUnsolRetryDelay.GetMilliseconds() > 0)
	{
		outstation->StartUnsolTimer(outstation->mConfig.mUnsolRetryDelay);
	}
	else
	{
		outstation->OnUnsolTimerExpiration();
	}
}

void AS_WaitForSolUnsolSuccess::OnUnsolSendSuccess(Outstation* outstation)
{
	outstation->ChangeState(AS_WaitForRspSuccess::Inst());
	this->DoUnsolSuccess(outstation);
}

} //ens ns


