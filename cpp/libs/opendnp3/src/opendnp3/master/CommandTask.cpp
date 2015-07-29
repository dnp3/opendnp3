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

#include "opendnp3/app/parsing/APDUParser.h"
#include "opendnp3/LogLevels.h"

using namespace openpal;

namespace opendnp3
{

CommandTask::CommandTask(IMasterApplication& app, ICommandSequence* pSequence_, ICommandCallback& callback, const TaskConfig& config, openpal::Logger logger) :
	IMasterTask(app, MonotonicTimestamp::Min(), logger, config),
	statusResult(CommandStatus::UNDEFINED),
	pCommandCallback(&callback),
	pSequence(pSequence_)
{

}

void CommandTask::Callback(const CommandResponse& cr)
{
	if (pCommandCallback)
	{
		pCommandCallback->OnComplete(cr);
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

IMasterTask::ResponseResult CommandTask::ProcessResponse(const APDUResponseHeader& header, const openpal::ReadBufferView& objects)
{
	return ValidateSingleResponse(header) ? ProcessResponse(objects) : ResponseResult::ERROR_BAD_RESPONSE;
}

IMasterTask::TaskState CommandTask::OnTaskComplete(TaskCompletion result, openpal::MonotonicTimestamp now)
{
	switch (result)
	{
		case(TaskCompletion::SUCCESS):
			this->Callback(CommandResponse::OK(this->statusResult));
			return TaskState::Infinite();
		default:
			this->Callback(CommandResponse(result));
			return TaskState::Infinite();
	}
	
}

void CommandTask::Initialize()
{
	statusResult = CommandStatus::UNDEFINED;
}

IMasterTask::ResponseResult CommandTask::ProcessResponse(const openpal::ReadBufferView& objects)
{
	auto result = APDUParser::Parse(objects, *pSequence.get(), &logger);
	if(result == ParseResult::OK)
	{
		auto response = pSequence->Validate();

		this->statusResult = response.GetStatus();

		if(functionCodes.empty())
		{						
			return ResponseResult::OK_FINAL;
		}
		else // we may have more depending on response
		{			
			return (response == CommandResponse::Success) ? ResponseResult::OK_REPEAT : ResponseResult::OK_FINAL;			
		}
	}
	else
	{				
		return ResponseResult::ERROR_BAD_RESPONSE;
	}
}

} //ens ns
