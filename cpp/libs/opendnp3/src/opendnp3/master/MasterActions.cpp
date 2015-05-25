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

#include "MasterActions.h"

#include "opendnp3/app/APDULogging.h"

namespace opendnp3
{

void MasterActions::StartTask(MasterState& mstate, IMasterTask& task)
{
	APDURequest request(mstate.txBuffer.GetWriteBufferView());
	task.BuildRequest(request, mstate.solSeq);
	MasterActions::StartResponseTimer(mstate);
	MasterActions::Transmit(mstate, request.ToReadOnly());
}

void MasterActions::QueueConfirm(MasterState& mstate, const APDUHeader& header)
{
	mstate.confirmQueue.push_back(header);
	MasterActions::CheckConfirmTransmit(mstate);
}

bool MasterActions::CheckConfirmTransmit(MasterState& mstate)
{
	if (mstate.isSending || mstate.confirmQueue.empty())
	{
		return false;
	}

	auto confirm = mstate.confirmQueue.front();
	APDUWrapper wrapper(mstate.txBuffer.GetWriteBufferView());
	wrapper.SetFunction(confirm.function);
	wrapper.SetControl(confirm.control);
	MasterActions::Transmit(mstate, wrapper.ToReadOnly());
	mstate.confirmQueue.pop_front();
	return true;	
}

void MasterActions::PostCheckForTask(MasterState& mstate)
{	
	auto callback = [&mstate]() { MasterActions::CheckForTask(mstate); };
	mstate.pExecutor->PostLambda(callback);
}

void MasterActions::CheckForTask(MasterState& mstate)
{
	mstate.OnStart();
}

void MasterActions::StartResponseTimer(MasterState& mstate)
{
	auto timeout = [&mstate](){ MasterActions::ProcessResponseTimeout(mstate); };
	mstate.responseTimer.Start(mstate.params.responseTimeout, timeout);
}

void MasterActions::ProcessResponseTimeout(MasterState& mstate)
{
	mstate.OnResponseTimeout();
}

void MasterActions::ReleaseActiveTask(MasterState& mstate)
{
	if (mstate.pActiveTask.IsDefined())
	{
		if (mstate.pActiveTask->IsRecurring())
		{
			mstate.scheduler.Schedule(std::move(mstate.pActiveTask));
		}
		else
		{
			mstate.pActiveTask.Release();
		}
	}
}

void MasterActions::Transmit(MasterState& state, const openpal::ReadBufferView& output)
{
	logging::ParseAndLogRequestTx(state.logger, output);
	assert(!state.isSending);
	state.isSending = true;
	state.pLower->BeginTransmit(output);
}
	
}



