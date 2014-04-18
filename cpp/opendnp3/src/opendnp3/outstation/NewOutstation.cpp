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

#include "NewOutstation.h"

#include "opendnp3/app/APDUResponse.h"
#include "opendnp3/LogLevels.h"

#include <openpal/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

NewOutstation::NewOutstation(
		const NewOutstationConfig& config,
		IExecutor& executor,
		openpal::LogRoot& root,
		openpal::ILowerLayer& lower,
		ICommandHandler& commandHandler,
		ITimeWriteHandler& timeWriteHandler,
		Database& database,
		const EventBufferFacade& buffers) :
		context(config, executor, root, lower, commandHandler, timeWriteHandler, database, buffers)
{
	
}
	
void NewOutstation::OnLowerLayerUp()
{
	if (context.isOnline)
	{
		SIMPLE_LOG_BLOCK(context.logger, flags::ERR, "already online");		
	}
	else
	{
		context.SetOnline();
	}
}
	
void NewOutstation::OnLowerLayerDown()
{
	if (context.isOnline)
	{
		context.SetOffline();
	}
	else
	{
		SIMPLE_LOG_BLOCK(context.logger, flags::ERR, "not online");
	}
}

void NewOutstation::OnReceive(const openpal::ReadOnlyBuffer& fragment)
{
	if (context.isOnline)
	{
		++context.rxFragCount;
		context.ExamineAPDU(fragment);		
	}
	else
	{
		SIMPLE_LOG_BLOCK(context.logger, flags::ERR, "ignoring received data while offline");
	}
}

void NewOutstation::OnSendResult(bool isSuccess)
{	
	if (context.isOnline && context.isSending)
	{
		context.isSending = false;
		context.pState->OnSendResult(&context, isSuccess);		
	}
	else
	{
		SIMPLE_LOG_BLOCK(context.logger, flags::ERR, "Unexpected send callback");
	}	
}

void NewOutstation::SetRequestTimeIIN()
{
	context.staticIIN.Set(IINBit::NEED_TIME);
}

void NewOutstation::OnReceiveSolRequest(const APDURecord& request, const openpal::ReadOnlyBuffer& fragment)
{	
	if (context.firstValidRequestAccepted)
	{
		if (context.solSeqN == request.control.SEQ)
		{
			if (context.lastValidRequest.Equals(fragment))
			{
				context.pState->OnRepeatRequest(&context, request);
			}
			else // new operation with same SEQ
			{
				if (request.function != FunctionCode::SELECT) // TODO - Ask why select is special?
				{
					context.pState->OnNewRequest(&context, request, fragment);
				}
			}
		}
		else  // completely new sequence #
		{				
			context.solSeqN = request.control.SEQ;
			context.pState->OnNewRequest(&context, request, fragment);
		}
	}
	else
	{
		context.solSeqN = request.control.SEQ;
		context.firstValidRequestAccepted = true;	
		context.pState->OnNewRequest(&context, request, fragment);
	}	
	
}
	
}


