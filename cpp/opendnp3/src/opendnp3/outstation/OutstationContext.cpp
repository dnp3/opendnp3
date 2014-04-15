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

#include "OutstationContext.h"

#include "opendnp3/StaticSizeConfiguration.h"
#include "opendnp3/LogLevels.h"

#include <openpal/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

OutstationContext::OutstationContext(
		const NewOutstationConfig& config,
		IExecutor& executor,
		LogRoot& root,
		ILowerLayer& lower,
		ICommandHandler& commandHandler,
		ITimeWriteHandler& timeWriteHandler,
		Database& database,
		const EventBufferFacade& buffers) :
	
	params(config.params),
	logger(root.GetLogger()),	
	pExecutor(&executor),
	pLower(&lower),
	pCommandHandler(&commandHandler),
	pTimeWriteHandler(&timeWriteHandler),
	pDatabase(&database),
	eventBuffer(buffers),
	isOnline(false),
	isSending(false),
	solConfirmWait(false),
	firstValidRequestAccepted(false),
	pConfirmTimer(nullptr),
	rxFragCount(0),		
	operateExpectedSeq(0),
	operateExpectedFragCount(0),
	solSeqN(0),
	expectedConfirmSeq(0),
	unsolSeq(0),
	rspContext(&database, &eventBuffer, StaticResponseTypes(config.defaultStaticResponses))	
{
	pDatabase->SetEventBuffer(eventBuffer);
	staticIIN.Set(IINBit::DEVICE_RESTART);

	if (params.maxTxFragSize < sizes::MIN_APDU_SIZE)
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, 
			"setting maxTxFragSize of %u to minimum of %u", 
			static_cast<unsigned int>(params.maxTxFragSize), 
			static_cast<unsigned int>(sizes::MIN_APDU_SIZE));

		params.maxTxFragSize = sizes::MIN_APDU_SIZE;
	}	
}

IINField OutstationContext::GetDynamicIIN()
{
	IINField ret;
	auto tracker = eventBuffer.UnselectedEvents();
	if (tracker.class1.HasEvents())
	{
		ret.Set(IINBit::CLASS1_EVENTS);
	}
	if (tracker.class2.HasEvents())
	{
		ret.Set(IINBit::CLASS2_EVENTS);
	}
	if (tracker.class3.HasEvents())
	{
		ret.Set(IINBit::CLASS3_EVENTS);
	}
	if (eventBuffer.IsOverflown())
	{
		ret.Set(IINBit::EVENT_BUFFER_OVERFLOW);
	}
	return ret;
}

APDUResponse OutstationContext::StartNewResponse()
{	
	return APDUResponse(txBuffer.GetWriteBuffer(params.maxTxFragSize));
}

ReadOnlyBuffer OutstationContext::RecordLastRequest(const openpal::ReadOnlyBuffer& fragment)
{
	if (fragment.Size() <= rxBuffer.Size())
	{
		lastValidRequest = fragment.CopyTo(rxBuffer.Buffer());
		return lastValidRequest;
	}
	else
	{
		return ReadOnlyBuffer::Empty();
	}
}

void OutstationContext::SetOnline()
{
	isOnline = true;
}

void OutstationContext::SetOffline()
{
	isOnline = false;
	isSending = false;
	solConfirmWait = false;
	firstValidRequestAccepted = false;
	deferredRequest.Clear();
	rspContext.Reset();
	CancelConfirmTimer();
}

void OutstationContext::Select()
{
	operateExpectedFragCount = rxFragCount + 1;
	operateExpectedSeq = NextSeq(solSeqN);
	selectTime = pExecutor->GetTime();
}

bool OutstationContext::IsOperateSequenceValid()
{	
	return (rxFragCount == operateExpectedFragCount) && (solSeqN == operateExpectedSeq);	
}

bool OutstationContext::IsIdle()
{
	return isOnline && (!isSending) && (!solConfirmWait);
}

bool OutstationContext::CancelConfirmTimer()
{
	if (pConfirmTimer)
	{
		pConfirmTimer->Cancel();
		pConfirmTimer = nullptr;
		return true;
	}
	else
	{
		return false;
	}
}

bool OutstationContext::IsExpectingSolConfirm()
{
	return (!isSending && solConfirmWait && pConfirmTimer);	
}

bool OutstationContext::StartConfirmTimerWithRunnable(const Runnable& runnable)
{
	if (pConfirmTimer)
	{
		return false;
	}
	else
	{
		pConfirmTimer = pExecutor->Start(TimeDuration::Milliseconds(5000), runnable); // todo make this configurable
		return true;
	}
}


}

