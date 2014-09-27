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
#include "MasterContext.h"

namespace opendnp3
{

// --------- Default Actions -------------

IMasterState* IMasterState::OnStart(MasterContext*pContext)
{
	return this;
}

IMasterState* IMasterState::OnResponse(MasterContext*pContext, const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects)
{
	FORMAT_LOG_BLOCK(pContext->logger, flags::WARN, "Not expecting a response, sequence: %u", response.control.SEQ);
	return this;
}

IMasterState* IMasterState::OnResponseTimeout(MasterContext* pContext)
{
	SIMPLE_LOG_BLOCK(pContext->logger, flags::ERR, "Unexpected response timeout");
	return this;
}

// --------- IDLE -------------

MasterStateIdle MasterStateIdle::instance;

IMasterState* MasterStateIdle::OnStart(MasterContext* pContext)
{
	if (pContext->isSending)
	{
		return this;
	}
	else
	{
		auto pTask = pContext->scheduler.Start(pContext->params);
		
		if (pTask)
		{		
			if (pContext->pTaskLock->Acquire(*pContext))
			{
				FORMAT_LOG_BLOCK(pContext->logger, flags::INFO, "Begining task: %s", pTask->Name());
				pContext->pActiveTask = pTask;
				pContext->StartTask(pTask);
				return &MasterStateWaitForResponse::Instance();
			}
			else
			{
				pContext->pActiveTask = pTask;
				return &MasterStateTaskReady::Instance();
			}
		}
		else
		{
			return this;
		}
	}
}

// --------- READY -------------

MasterStateTaskReady MasterStateTaskReady::instance;

IMasterState* MasterStateTaskReady::OnStart(MasterContext*pContext)
{
	if (pContext->isSending)
	{
		return this;
	}
	else
	{
		if (pContext->pTaskLock->Acquire(*pContext))
		{
			pContext->StartTask(pContext->pActiveTask);
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

IMasterState* MasterStateWaitForResponse::OnResponse(MasterContext* pContext, const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects)
{
	if (response.control.SEQ == pContext->solSeq)
	{
		pContext->CancelResponseTimer();

		pContext->solSeq = AppControlField::NextSeq(pContext->solSeq);		
		
		auto result = pContext->pActiveTask->OnResponse(response, objects);

		if (response.control.CON) // TODO evaluate if reponse was procesed before confirming && pContext->CanConfirmResponse(result))
		{
			pContext->QueueConfirm(APDUHeader::SolicitedConfirm(response.control.SEQ));
		}

		switch (result)
		{
			case(TaskState::CONTINUE) :
				pContext->StartResponseTimer();
				return this;
			case(TaskState::REPEAT) :				
				return MasterStateTaskReady::Instance().OnStart(pContext);
			default:
				pContext->pActiveTask = nullptr;
				pContext->pTaskLock->Release(*pContext);
				pContext->PostCheckForTask();
				return &MasterStateIdle::Instance();
		}
	}
	else
	{
		FORMAT_LOG_BLOCK(pContext->logger, flags::WARN, "Response with bad sequence: %u", response.control.SEQ);
		return this;
	}
}

IMasterState* MasterStateWaitForResponse::OnResponseTimeout(MasterContext* pContext)
{	
	pContext->pResponseTimer = nullptr;
	if (!pContext->pActiveTask->OnResponseTimeout())
	{
		// TODO delete this task
	}
	pContext->pActiveTask = nullptr;
	pContext->pTaskLock->Release(*pContext);
	pContext->solSeq = AppControlField::NextSeq(pContext->solSeq);
	pContext->PostCheckForTask();
	return &MasterStateIdle::Instance();
}

}

