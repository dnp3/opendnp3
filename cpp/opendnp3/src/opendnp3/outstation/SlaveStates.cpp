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
#include "SlaveStates.h"

#include <openpal/LoggableMacros.h>

#include "opendnp3/DNPErrorCodes.h"
#include "opendnp3/outstation/Outstation.h"
#include "opendnp3/LogLevels.h"

#include <sstream>

using namespace openpal;

namespace opendnp3
{

void SlaveStateBase::OnLowerLayerUp(Outstation* slave)
{
	ERROR_LOGGER_BLOCK(slave->logger, flags::ERR, "invalid action for state: " << Name(), SERR_INVALID_STATE);
}

void SlaveStateBase::OnLowerLayerDown(Outstation* slave)
{
	ERROR_LOGGER_BLOCK(slave->logger, flags::ERR, "invalid action for state: " << Name(), SERR_INVALID_STATE);
}

void SlaveStateBase::OnSolSendSuccess(Outstation* slave)
{
	ERROR_LOGGER_BLOCK(slave->logger, flags::ERR, "invalid action for state: " << Name(), SERR_INVALID_STATE);
}

void SlaveStateBase::OnSolFailure(Outstation* slave)
{
	ERROR_LOGGER_BLOCK(slave->logger, flags::ERR, "invalid action for state: " << Name(), SERR_INVALID_STATE);
}

void SlaveStateBase::OnUnsolSendSuccess(Outstation* slave)
{
	ERROR_LOGGER_BLOCK(slave->logger, flags::ERR, "invalid action for state: " << Name(), SERR_INVALID_STATE);
}

void SlaveStateBase::OnUnsolFailure(Outstation* slave)
{
	ERROR_LOGGER_BLOCK(slave->logger, flags::ERR, "invalid action for state: " << Name(), SERR_INVALID_STATE);
}

void SlaveStateBase::OnRequest(Outstation* slave, const APDURecord&, SequenceInfo)
{
	ERROR_LOGGER_BLOCK(slave->logger, flags::ERR, "invalid action for state: " << Name(), SERR_INVALID_STATE);
}

// by default, the unsol timer expiration is deferd until it can be handled
void SlaveStateBase::OnUnsolExpiration(Outstation* slave)
{
	slave->mDeferredUnsol = true;
}

void SlaveStateBase::DoUnsolSuccess(Outstation* slave)
{
	if (!slave->mStartupNullUnsol) slave->mStartupNullUnsol = true; //it was a null unsol packet
	//slave->mRspContext.ClearAndReset(); TODO

	// this will cause us to immediately re-evaluate if we need to send another unsol rsp
	// we use the Deferred mechanism to give the slave an opportunity to respond to any Deferred request instead
	slave->mDeferredUnsol = true;
}

/* AS_Closed */

AS_Closed AS_Closed::mInstance;

void AS_Closed::OnLowerLayerUp(Outstation* slave)
{
	// this is implemented as a simple timer because it can run if the slave is connected/disconnected etc
	if (slave->mConfig.mAllowTimeSync) slave->ResetTimeIIN();
	slave->ChangeState(AS_Idle::Inst());
}

void AS_Closed::Enter(Outstation* slave)
{
	if(slave->mpTimeTimer)
	{
		slave->mpTimeTimer->Cancel();
		slave->mpTimeTimer = nullptr;
	}
}

void AS_Closed::OnDataUpdate(Outstation* slave)
{
	if(!slave->mConfig.mDisableUnsol) slave->mDeferredUnsol = true;
}

/* AS_OpenBase */

void AS_OpenBase::OnLowerLayerDown(Outstation* slave)
{
	slave->ChangeState(AS_Closed::Inst());
}

/* AS_Idle */

AS_Idle AS_Idle::mInstance;

void AS_Idle::OnRequest(Outstation* slave, const APDURecord& record, SequenceInfo sequence)
{
	slave->ChangeState(AS_WaitForRspSuccess::Inst());
	slave->RespondToRequest(record, sequence);
}

void AS_Idle::Enter(Outstation* slave)
{
	if(slave->mCachedRequest.IsSet())
	{
		slave->mCachedRequest.Apply(
		    [this, slave](const APDURecord & record, SequenceInfo info)
		{
			this->OnRequest(slave, record, info);
		}
		);
	}
	else
	{
		// check for unsolicited data to send
		if (slave->mDeferredUnsol)
		{
			slave->mDeferredUnsol = false;
			this->OnUnsolExpiration(slave);
		}
	}
}

void AS_Idle::OnDataUpdate(Outstation* slave)
{
	/*
	// start the unsol timer or act immediately if there's no pack timer
	if (!slave->mConfig.mDisableUnsol && slave->mStartupNullUnsol) // TODO && slave->mRspContext.HasEvents(slave->mConfig.mUnsolMask))
	{
		if (slave->mConfig.mUnsolPackDelay.GetMilliseconds() <= 0)
		{
			// TODO
			//slave->mRspContext.LoadUnsol(slave->mUnsol, slave->mIIN, slave->mConfig.mUnsolMask);
			//slave->SendUnsolicited(slave->mUnsol);
			//slave->ChangeState(AS_WaitForUnsolSuccess::Inst());
		}
		else if (slave->mpUnsolTimer == nullptr)
		{
			slave->StartUnsolTimer(slave->mConfig.mUnsolPackDelay);
		}
	}
	*/
}

void AS_Idle::OnUnsolExpiration(Outstation* slave)
{
	if (slave->mStartupNullUnsol)
	{
		/* TODO
		if (slave->mRspContext.HasEvents(slave->mConfig.mUnsolMask))
		{

			slave->ChangeState(AS_WaitForUnsolSuccess::Inst());
			slave->mRspContext.LoadUnsol(slave->mUnsol,  slave->mIIN,  slave->mConfig.mUnsolMask);
			slave->SendUnsolicited(slave->mUnsol);
		}
		*/
	}
	else
	{
		/* TODO
		// do the startup null unsol task
		slave->ChangeState(AS_WaitForUnsolSuccess::Inst());
		slave->mRspContext.LoadUnsol(slave->mUnsol, slave->mIIN, ClassMask(false, false, false));
		slave->SendUnsolicited(slave->mUnsol);
		*/
	}
}

/* AS_WaitForRspSuccess */

AS_WaitForRspSuccess AS_WaitForRspSuccess::mInstance;

void AS_WaitForRspSuccess::OnSolFailure(Outstation* slave)
{
	slave->ChangeState(AS_Idle::Inst());
}

void AS_WaitForRspSuccess::OnSolSendSuccess(Outstation* slave)
{
	slave->eventBuffer.Clear();

	if (slave->rspContext.IsComplete())
	{
		slave->ChangeState(AS_Idle::Inst());
	}
	else
	{
		slave->ContinueResponse();
	}
}

// When we get a request we should no longer wait for confirmation, but we should
// immediately handle the new request. We implement this behavior asynchronously, by
// canceling the response transaction, and waiting for an OnFailure callback.
// The callback may still succeed if
void AS_WaitForRspSuccess::OnRequest(Outstation* slave, const APDURecord& record, SequenceInfo seq)
{
	slave->mpAppLayer->CancelResponse();
	slave->mCachedRequest.Set(record, seq);
}

/* AS_WaitForUnsolSuccess */

AS_WaitForUnsolSuccess AS_WaitForUnsolSuccess::mInstance;

void AS_WaitForUnsolSuccess::OnUnsolFailure(Outstation* slave)
{
	// if any unsol transaction fails, we re-enable the timer with the unsol retry delay
	// slave->mRspContext.Reset(); TODO
	slave->StartUnsolTimer(slave->mConfig.mUnsolRetryDelay);
	slave->ChangeState(AS_Idle::Inst());
}

void AS_WaitForUnsolSuccess::OnUnsolSendSuccess(Outstation* slave)
{
	this->DoUnsolSuccess(slave);
	slave->ChangeState(AS_Idle::Inst());
}

void AS_WaitForUnsolSuccess::OnRequest(Outstation* slave, const APDURecord& record, SequenceInfo sequence)
{
	if (record.function == FunctionCode::READ)
	{
		//read requests should be defered until after the unsol
		slave->mCachedRequest.Set(record, sequence);
	}
	else
	{
		/* TODO
		slave->mCachedRequest.Clear(); // idempotent
		slave->ChangeState(AS_WaitForSolUnsolSuccess::Inst());
		slave->RespondToRequest(record, sequence);
		*/
	}
}

/* AS_WaitForSolUnsolSuccess */

AS_WaitForSolUnsolSuccess AS_WaitForSolUnsolSuccess::mInstance;

void AS_WaitForSolUnsolSuccess::OnRequest(Outstation* slave, const APDURecord& record, SequenceInfo sequence)
{
	// Both channels are busy... buffer the request
	slave->mCachedRequest.Set(record, sequence);
}

void AS_WaitForSolUnsolSuccess::OnSolFailure(Outstation* slave)
{
	slave->ChangeState(AS_WaitForUnsolSuccess::Inst());
}

void AS_WaitForSolUnsolSuccess::OnSolSendSuccess(Outstation* slave)
{
	slave->ChangeState(AS_WaitForUnsolSuccess::Inst());
}

void AS_WaitForSolUnsolSuccess::OnUnsolFailure(Outstation* slave)
{
	//slave->mRspContext.Reset(); TODO
	slave->ChangeState(AS_WaitForRspSuccess::Inst());
	if (slave->mConfig.mUnsolRetryDelay.GetMilliseconds() > 0)
	{
		slave->StartUnsolTimer(slave->mConfig.mUnsolRetryDelay);
	}
	else
	{
		slave->OnUnsolTimerExpiration();
	}
}

void AS_WaitForSolUnsolSuccess::OnUnsolSendSuccess(Outstation* slave)
{
	slave->ChangeState(AS_WaitForRspSuccess::Inst());
	this->DoUnsolSuccess(slave);
}

} //ens ns


