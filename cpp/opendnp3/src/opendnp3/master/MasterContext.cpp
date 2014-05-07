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

#include "MasterContext.h"

#include "opendnp3/LogLevels.h"
#include "opendnp3/app/APDUWrapper.h"
#include "opendnp3/app/APDUHeaderParser.h"
#include "opendnp3/app/APDULogging.h"

#include <openpal/LogMacros.h>

namespace opendnp3
{

MasterContext::MasterContext(
	openpal::IExecutor& executor,
	openpal::LogRoot& root,
	openpal::ILowerLayer& lower,
	ISOEHandler* pSOEHandler,
	const MasterParams& params_
	) :

	logger(root.GetLogger(sources::MASTER)),
	pExecutor(&executor),
	pLower(&lower),
	params(params_),
	isOnline(false),
	isSending(false),
	solSeq(0),
	unsolSeq(0),
	pActiveTask(nullptr),
	pResponseTimer(nullptr),
	scheduler(executor),
	taskList(pSOEHandler, &logger, params)
{
	auto callback = [this](){ PostCheckForTask(); };
	scheduler.SetExpirationHandler(openpal::Bind(callback));
}

bool MasterContext::OnLayerUp()
{
	if (isOnline)
	{		
		return false;
	}
	else
	{
		isOnline = true;
		taskList.Initialize();
		taskList.ScheduleNext(scheduler);
		return true;
	}
}

bool MasterContext::OnLayerDown()
{
	if (isOnline)
	{
		pActiveTask = nullptr;
		scheduler.Reset();
		this->CancelResponseTimer();
		isOnline = false;
		solSeq = 0;
		unsolSeq = 0;
		return true;
	}
	else
	{
		return false;
	}
}

void MasterContext::PostCheckForTask()
{
	auto callback = [this]() { this->CheckForTask(); };
	pExecutor->PostLambda(callback);
}

void MasterContext::CheckForTask()
{
	if (isOnline && pActiveTask == nullptr && !isSending)
	{
		auto pTask = scheduler.Start();
		if (pTask)
		{
			FORMAT_LOG_BLOCK(logger, flags::INFO, "Begining task: %s", pTask->Name());
			pActiveTask = pTask;			
			APDURequest request(txBuffer.GetWriteBuffer());
			pTask->BuildRequest(request, params, solSeq);			
			this->Transmit(request.ToReadOnly());
		}
	}
}

void MasterContext::OnResponseTimeout()
{
	if (pResponseTimer)
	{
		pResponseTimer = nullptr;
		if (pActiveTask)
		{
			pActiveTask->OnResponseTimeout(params, scheduler);
			pActiveTask = nullptr;
			solSeq = NextSeq(solSeq);
			this->PostCheckForTask();
		}
	}
}

void MasterContext::OnSendResult(bool isSucccess)
{
	if (isSending)
	{
		isSending = false;
		if (pActiveTask && pResponseTimer == nullptr)
		{
			this->StartResponseTimer();
		}
		this->CheckConfirmTransmit();
	}
}


void MasterContext::OnResponse(const APDUResponseRecord& response)
{	
	if (response.control.UNS)
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring response with UNS bit");
	}
	else
	{
		if (pActiveTask && pResponseTimer && (response.control.SEQ == this->solSeq))
		{
			solSeq = NextSeq(solSeq);

			this->CancelResponseTimer();

			auto result = pActiveTask->OnResponse(response, params, scheduler);
			if (response.control.CON && CanConfirmResponse(result))
			{
				this->QueueConfirm(APDUHeader::SolicitedConfirm(response.control.SEQ));								
			}

			if (result == TaskStatus::CONTINUE)
			{
				solSeq = NextSeq(solSeq);
				this->StartResponseTimer();
			}
			else
			{
				pActiveTask = nullptr;
				this->PostCheckForTask();
			}
		}
		else
		{
			FORMAT_LOG_BLOCK(logger, flags::WARN, "Unexpected response with sequence: %u", response.control.SEQ);
		}
	}	
}

void MasterContext::OnUnsolicitedResponse(const APDUResponseRecord& response)
{
	if (response.control.UNS)
	{
		

		if (response.control.CON)
		{
			this->QueueConfirm(APDUHeader::SolicitedConfirm(response.control.SEQ));
		}
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring unsolicited response with UNS bit not set");
	}
}

void MasterContext::StartResponseTimer()
{
	if (pResponseTimer == nullptr)
	{
		auto timeout = [this](){ this->OnResponseTimeout(); };
		pResponseTimer = pExecutor->Start(params.responseTimeout, openpal::Bind(timeout));
	}	
}

bool MasterContext::CancelResponseTimer()
{
	if (pResponseTimer)
	{
		pResponseTimer->Cancel();
		pResponseTimer = nullptr;
		return true;
	}
	else
	{
		return false;
	}
}	

void MasterContext::QueueConfirm(const APDUHeader& header)
{
	this->confirmQueue.Enqueue(header);
	this->CheckConfirmTransmit();
}

void MasterContext::CheckConfirmTransmit()
{
	if (!isSending && confirmQueue.IsNotEmpty())
	{
		auto confirm = confirmQueue.Pop();
		APDUWrapper wrapper(txBuffer.GetWriteBuffer());
		wrapper.SetFunction(confirm.function);
		wrapper.SetControl(confirm.control);
		this->Transmit(wrapper.ToReadOnly());
	}
}

void MasterContext::Transmit(const openpal::ReadOnlyBuffer& output)
{
	logging::ParseAndLogRequestTx(&logger, output);	
	assert(!isSending);
	isSending = true;
	pLower->BeginTransmit(output);	
}

bool MasterContext::CanConfirmResponse(TaskStatus status)
{
	switch (status)
	{
		case(TaskStatus::SUCCESS) :
		case(TaskStatus::CONTINUE) :
			return true;
		default:
			return false;
	}
}

}
