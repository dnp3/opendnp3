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

#include "IMasterState.h"

#include <openpal/logging/LogMacros.h>

#include "opendnp3/LogLevels.h"
#include "opendnp3/master/MasterContext.h"

namespace opendnp3
{

// --------- Default Actions -------------

IMasterState* IMasterState::OnStart(MContext& mcontext)
{
	return this;
}

IMasterState* IMasterState::OnResponse(MContext& mcontext, const APDUResponseHeader& response, const openpal::ReadBufferView& objects)
{
	FORMAT_LOG_BLOCK(mcontext.logger, flags::WARN, "Not expecting a response, sequence: %u", response.control.SEQ);
	return this;
}

IMasterState* IMasterState::OnResponseTimeout(MContext& mcontext)
{
	SIMPLE_LOG_BLOCK(mcontext.logger, flags::ERR, "Unexpected response timeout");
	return this;
}

// --------- IDLE -------------

MasterStateIdle MasterStateIdle::instance;

IMasterState* MasterStateIdle::OnStart(MContext& mcontext)
{
	if (mcontext.isSending)
	{
		return this;
	}

	auto task = mcontext.scheduler.Start();

	if (task.IsDefined())
	{
		if (mcontext.BeginNewTask(task))
		{
			return &MasterStateWaitForResponse::Instance();
		}
		else
		{
			return &MasterStateTaskReady::Instance();
		}
	}
	else
	{
		return this;
	}
}

// --------- READY -------------

MasterStateTaskReady MasterStateTaskReady::instance;

IMasterState* MasterStateTaskReady::OnStart(MContext& mcontext)
{
	if (mcontext.isSending)
	{
		return this;
	}
	else
	{
		if (mcontext.ResumeActiveTask())
		{			
			return &MasterStateWaitForResponse::Instance();
		}
		else
		{
			return this;
		}
	}	
}

// --------- WaitForResponse -------------

MasterStateWaitForResponse MasterStateWaitForResponse::instance;

IMasterState* MasterStateWaitForResponse::OnResponse(MContext& mcontext, const APDUResponseHeader& header, const openpal::ReadBufferView& objects)
{
	if (header.control.SEQ != mcontext.solSeq)
	{
		FORMAT_LOG_BLOCK(mcontext.logger, flags::WARN, "Response with bad sequence: %u", header.control.SEQ);
		return this;
	}

	if (!mcontext.pActiveTask->AcceptsFunction(header.function))
	{
		FORMAT_LOG_BLOCK(mcontext.logger, flags::WARN,
			"Task %s does not accept function code in responses: %s",
			mcontext.pActiveTask->Name(),
			FunctionCodeToString(header.function));

		return this;
	}
	
	mcontext.responseTimer.Cancel();

	mcontext.solSeq.Increment();

	auto now = mcontext.pExecutor->GetTime();
		
	auto result = mcontext.pActiveTask->OnResponse(header, objects, now);

	if (header.control.CON)
	{
		mcontext.QueueConfirm(APDUHeader::SolicitedConfirm(header.control.SEQ));
	}

	switch (result)
	{
		case(IMasterTask::ResponseResult::OK_CONTINUE) :
			mcontext.StartResponseTimer();
			return this;
		case(IMasterTask::ResponseResult::OK_REPEAT) :
			return MasterStateTaskReady::Instance().OnStart(mcontext);
		default:
			// task completed or failed, either way go back to idle			
			mcontext.CompleteActiveTask();
			return &MasterStateIdle::Instance();
	}	
}

IMasterState* MasterStateWaitForResponse::OnResponseTimeout(MContext& mcontext)
{			
	auto now = mcontext.pExecutor->GetTime();
	mcontext.pActiveTask->OnResponseTimeout(now);
	mcontext.solSeq.Increment();
	mcontext.CompleteActiveTask();
	return &MasterStateIdle::Instance();
}

}

