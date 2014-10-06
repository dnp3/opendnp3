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
#include "CommandTask.h"

#include <openpal/logging/LogMacros.h>

#include "opendnp3/app/APDUParser.h"
#include "opendnp3/LogLevels.h"

using namespace openpal;

namespace opendnp3
{

CommandTask::CommandTask(ICommandSequence* pSequence_, ICommandCallback& callback, openpal::Logger logger_) :
	logger(logger_),
	pCallback(&callback),
	pSequence(pSequence_)
{

}

void CommandTask::Callback(const CommandResponse& cr)
{
	if (pCallback)
	{
		pCallback->OnComplete(cr);
	}
}

void CommandTask::LoadSelectAndOperate()
{
	functionCodes.clear();
	functionCodes.push_back(FunctionCode::SELECT);
	functionCodes.push_back(FunctionCode::OPERATE);
}

void CommandTask::LoadDirectOperate()
{
	functionCodes.clear();
	functionCodes.push_back(FunctionCode::DIRECT_OPERATE);
}

void CommandTask::BuildRequest(APDURequest& request, uint8_t seq)
{
	if (!functionCodes.empty())
	{		
		request.SetFunction(functionCodes.front());
		functionCodes.pop_front();
		request.SetControl(AppControlField::Request(seq));
		pSequence->FormatRequestHeader(request);		
	}
}

TaskResult CommandTask::OnResponse(const APDUResponseHeader& header, const openpal::ReadOnlyBuffer& objects, const openpal::MonotonicTimestamp& now)
{
	if (header.control.FIR && header.control.FIN)
	{
		return this->OnSingleResponse(header, objects, now);
	}
	else
	{
		SIMPLE_LOG_BLOCK(logger, flags::WARN, "Ignoring unexpected response with FIR/FIN not set");
		this->Callback(CommandResponse(UserTaskResult::BAD_RESPONSE));
		return TaskResult::FAILURE;
	}
}

void CommandTask::OnResponseTimeout(const openpal::MonotonicTimestamp&)
{
	this->Callback(CommandResponse(UserTaskResult::TIMEOUT));
}

void CommandTask::OnLowerLayerClose(const openpal::MonotonicTimestamp& now)
{	
	this->Callback(CommandResponse::NoResponse(UserTaskResult::NO_COMMS));
}

TaskResult CommandTask::OnSingleResponse(const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects, const openpal::MonotonicTimestamp& now)
{
	auto result = APDUParser::ParseTwoPass(objects, pSequence.get(), &logger);
	if(result == APDUParser::Result::OK)
	{
		if(functionCodes.empty()) // we're done
		{
			auto commandResponse = pSequence->Validate();
			this->Callback(commandResponse);			
			return TaskResult::SUCCESS;
		}
		else // we may have more depending on response
		{
			auto commandResponse = pSequence->Validate();
			if (commandResponse == CommandResponse::Success)
			{				
				return TaskResult::REPEAT;
			}
			else
			{
				this->Callback(commandResponse);  // something failed, end the task early				
				return TaskResult::FAILURE;
			}
		}
	}
	else
	{		
		this->Callback(CommandResponse(UserTaskResult::BAD_RESPONSE));
		return TaskResult::FAILURE;
	}
}

} //ens ns
