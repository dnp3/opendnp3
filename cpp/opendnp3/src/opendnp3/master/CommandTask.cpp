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


#include <openpal/LogMacros.h>

#include "opendnp3/app/APDUParser.h"

#include "opendnp3/objects/Group12.h"
#include "opendnp3/objects/Group41.h"

#include "opendnp3/LogLevels.h"

using namespace openpal;

namespace opendnp3
{


CommandTask::CommandTask(openpal::Logger* pLogger_) :
	pLogger(pLogger_),
	pCallback(nullptr),
	pActiveSequence(nullptr),	
	crobSeq(*pLogger_, Group12Var1Serializer::Inst()),
	analogInt32Seq(*pLogger_, Group41Var1Serializer::Inst()),
	analogInt16Seq(*pLogger_, Group41Var2Serializer::Inst()),
	analogFloat32Seq(*pLogger_, Group41Var3Serializer::Inst()),
	analogDouble64Seq(*pLogger_, Group41Var4Serializer::Inst())
{

}

void CommandTask::LoadSelectAndOperate()
{
	functionCodes.Clear();
	functionCodes.Enqueue(FunctionCode::SELECT);
	functionCodes.Enqueue(FunctionCode::OPERATE);
}

void CommandTask::LoadDirectOperate()
{
	functionCodes.Clear();
	functionCodes.Enqueue(FunctionCode::DIRECT_OPERATE);
}

void CommandTask::SelectAndOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback* pCallback)
{
	this->LoadSelectAndOperate();
	this->Configure(crobSeq, command, index, pCallback);
}

void CommandTask::SelectAndOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback* pCallback)
{
	this->LoadSelectAndOperate();
	this->Configure(analogInt16Seq, command, index, pCallback);
}

void CommandTask::SelectAndOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback* pCallback)
{
	this->LoadSelectAndOperate();
	this->Configure(analogInt32Seq, command, index, pCallback);
}

void CommandTask::SelectAndOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback* pCallback)
{
	this->LoadSelectAndOperate();
	this->Configure(analogFloat32Seq, command, index, pCallback);
}

void CommandTask::SelectAndOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback* pCallback)
{
	this->LoadSelectAndOperate();
	this->Configure(analogDouble64Seq, command, index, pCallback);
}

void CommandTask::DirectOperate(const ControlRelayOutputBlock& command, uint16_t index,  ICommandCallback* pCallback)
{
	this->LoadDirectOperate();
	this->Configure(crobSeq, command, index, pCallback);
}

void CommandTask::DirectOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback* pCallback)
{
	this->LoadDirectOperate();
	this->Configure(analogInt16Seq, command, index, pCallback);
}

void CommandTask::DirectOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback* pCallback)
{
	this->LoadDirectOperate();
	this->Configure(analogInt32Seq, command, index, pCallback);
}

void CommandTask::DirectOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback* pCallback)
{
	this->LoadDirectOperate();
	this->Configure(analogFloat32Seq, command, index, pCallback);
}

void CommandTask::DirectOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback* pCallback)
{
	this->LoadDirectOperate();
	this->Configure(analogDouble64Seq, command, index, pCallback);
}

void CommandTask::BuildRequest(APDURequest& request, const MasterParams& params, uint8_t seq)
{
	if (functionCodes.IsNotEmpty() && pActiveSequence)
	{
		auto code = functionCodes.Pop();
		request.SetFunction(functionCodes.Pop());
		request.SetControl(AppControlField::Request(seq));
		pActiveSequence->FormatRequestHeader(request);
	}
}

TaskStatus CommandTask::OnResponse(const APDUResponseRecord& response, const MasterParams& params, IMasterScheduler& scheduler)
{
	if (response.control.FIR && response.control.FIN)
	{
		return this->OnSingleResponse(response, params, scheduler);
	}
	else
	{
		SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Ignoring unexpected response with FIR/FIN not set");
		pCallback->OnComplete(CommandResponse(CommandResult::BAD_RESPONSE));
		return TaskStatus::FAIL;
	}
}

void CommandTask::OnResponseTimeout(const MasterParams& params, IMasterScheduler& scheduler)
{
	pCallback->OnComplete(CommandResponse(CommandResult::TIMEOUT));
}

TaskStatus CommandTask::OnSingleResponse(const APDUResponseRecord& response, const MasterParams& params, IMasterScheduler& scheduler)
{
	auto result = APDUParser::ParseTwoPass(response.objects, pActiveSequence, pLogger);
	if(result == APDUParser::Result::OK)
	{
		if(functionCodes.IsEmpty()) // we're done
		{
			auto commandResponse = pActiveSequence->Validate();
			this->Callback(commandResponse);			
			return TaskStatus::SUCCESS;
		}
		else // we may have more depending on response
		{
			auto commandResponse = pActiveSequence->Validate();
			if (commandResponse == CommandResponse::Success)
			{
				scheduler.Schedule(this);
				return TaskStatus::SUCCESS;
			}
			else
			{
				pCallback->OnComplete(commandResponse);  // something failed, end the task early				
				return TaskStatus::FAIL;
			}
		}
	}
	else
	{		
		pCallback->OnComplete(CommandResponse(CommandResult::BAD_RESPONSE));
		return TaskStatus::FAIL;
	}
}

} //ens ns
