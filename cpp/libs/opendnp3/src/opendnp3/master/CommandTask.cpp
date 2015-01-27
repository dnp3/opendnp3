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

CommandTask::CommandTask(IMasterApplication& app, ICommandSequence* pSequence_, ICommandCallback& callback, openpal::Logger logger) :
	IMasterTask(app, MonotonicTimestamp::Min(), logger, nullptr, -1),	
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

IMasterTask::ResponseResult CommandTask::_OnResponse(const APDUResponseHeader& header, const openpal::ReadBufferView& objects)
{
	return ValidateSingleResponse(header) ? ProcessResponse(objects) : ResponseResult::ERROR_BAD_RESPONSE;
}

void CommandTask::OnResponseError(openpal::MonotonicTimestamp now)
{
	this->Callback(CommandResponse(TaskCompletion::FAILURE_BAD_RESPONSE));
}

void CommandTask::OnResponseOK(openpal::MonotonicTimestamp now)
{
	this->Callback(response);
}

void CommandTask::_OnResponseTimeout(openpal::MonotonicTimestamp)
{
	this->Callback(CommandResponse(TaskCompletion::FAILURE_RESPONSE_TIMEOUT));
}

void CommandTask::_OnLowerLayerClose(openpal::MonotonicTimestamp)
{	
	this->Callback(CommandResponse::NoResponse(TaskCompletion::FAILURE_NO_COMMS));
}

void CommandTask::Initialize()
{
	response = CommandResponse::NoResponse(TaskCompletion::FAILURE_BAD_RESPONSE);
}

IMasterTask::ResponseResult CommandTask::ProcessResponse(const openpal::ReadBufferView& objects)
{
	auto result = APDUParser::ParseTwoPass(objects, pSequence.get(), &logger);
	if(result == ParseResult::OK)
	{
		response = pSequence->Validate();

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
