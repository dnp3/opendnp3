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

const Sequence<FunctionCode> CommandTask::Operate(FunctionCode::OPERATE);
const Sequence<FunctionCode> CommandTask::DirectOperate(FunctionCode::DIRECT_OPERATE);
const Sequence<FunctionCode> CommandTask::SelectAndOperate(FunctionCode::SELECT, &CommandTask::Operate);

//Sequence<FunctionCode> CommandTask::selectAndOperate;

CommandTask::CommandTask(Logger aLogger) :
	MasterTaskBase(aLogger),
	mpActiveSequence(nullptr),
	mpCallback(nullptr),
	mpFunctionSequence(nullptr),
	crobSeq(aLogger, Group12Var1Serializer::Inst()),
	analogInt32Seq(aLogger, Group41Var1Serializer::Inst()),
	analogInt16Seq(aLogger, Group41Var2Serializer::Inst()),
	analogFloat32Seq(aLogger, Group41Var3Serializer::Inst()),
	analogDouble64Seq(aLogger, Group41Var4Serializer::Inst())
{

}

void CommandTask::ConfigureSBO(const ControlRelayOutputBlock& command, uint32_t index, ICommandCallback* pCallback)
{
	this->Configure(crobSeq, command, index, &SelectAndOperate, pCallback);
}

void CommandTask::ConfigureSBO(const AnalogOutputInt16& command, uint32_t index,  ICommandCallback* pCallback)
{
	this->Configure(analogInt16Seq, command, index, &SelectAndOperate, pCallback);
}

void CommandTask::ConfigureSBO(const AnalogOutputInt32& command, uint32_t index,  ICommandCallback* pCallback)
{
	this->Configure(analogInt32Seq, command, index, &SelectAndOperate, pCallback);
}

void CommandTask::ConfigureSBO(const AnalogOutputFloat32& command, uint32_t index,  ICommandCallback* pCallback)
{
	this->Configure(analogFloat32Seq, command, index, &SelectAndOperate, pCallback);
}

void CommandTask::ConfigureSBO(const AnalogOutputDouble64& command, uint32_t index,  ICommandCallback* pCallback)
{
	this->Configure(analogDouble64Seq, command, index, &SelectAndOperate, pCallback);
}

void CommandTask::ConfigureDO(const ControlRelayOutputBlock& command, uint32_t index,  ICommandCallback* pCallback)
{
	this->Configure(crobSeq, command, index, &DirectOperate, pCallback);
}

void CommandTask::ConfigureDO(const AnalogOutputInt16& command, uint32_t index,  ICommandCallback* pCallback)
{
	this->Configure(analogInt16Seq, command, index, &DirectOperate, pCallback);
}

void CommandTask::ConfigureDO(const AnalogOutputInt32& command, uint32_t index,  ICommandCallback* pCallback)
{
	this->Configure(analogInt32Seq, command, index, &DirectOperate, pCallback);
}

void CommandTask::ConfigureDO(const AnalogOutputFloat32& command, uint32_t index,  ICommandCallback* pCallback)
{
	this->Configure(analogFloat32Seq, command, index, &DirectOperate, pCallback);
}

void CommandTask::ConfigureDO(const AnalogOutputDouble64& command, uint32_t index,  ICommandCallback* pCallback)
{
	this->Configure(analogDouble64Seq, command, index, &DirectOperate, pCallback);
}

void CommandTask::ConfigureRequest(APDURequest& request)
{

	assert(mpFunctionSequence != nullptr);
	assert(mpActiveSequence != nullptr);
	auto code = mpFunctionSequence->Value();
	mpFunctionSequence = mpFunctionSequence->Next();
	mpActiveSequence->FormatRequest(request, code);

}

void  CommandTask::Callback(const CommandResponse& cr)
{
	if (mpCallback)
	{
		mpCallback->OnComplete(cr);
	}
}

void CommandTask::OnFailure()
{
	Callback(CommandResponse(CommandResult::TIMEOUT));

}

bool CommandTask::_OnPartialResponse(const APDUResponseRecord&)
{
	SIMPLE_LOG_BLOCK(logger, flags::ERR, "Non fin responses not allowed for control tasks");
	Callback(CommandResponse(CommandResult::BAD_RESPONSE));	
	return false;
}

TaskResult CommandTask::_OnFinalResponse(const APDUResponseRecord& record)
{
	auto result = APDUParser::ParseTwoPass(record.objects, mpActiveSequence, &logger);
	if(result == APDUParser::Result::OK)
	{
		if(mpFunctionSequence == nullptr) // we're done
		{
			auto commandResponse = mpActiveSequence->Validate();
			Callback(commandResponse);			
			return TaskResult::TR_SUCCESS;
		}
		else // we may have more depending on response
		{
			auto commandResponse = mpActiveSequence->Validate();
			if(commandResponse == CommandResponse::Success) return TaskResult::TR_CONTINUE; // more function codes
			else
			{
				Callback(commandResponse);  // something failed, end the task early				
				return TaskResult::TR_SUCCESS;
			}
		}
	}
	else
	{		
		Callback(CommandResponse(CommandResult::BAD_RESPONSE));
		return TaskResult::TR_FAIL;
	}
}

char const* CommandTask::Name() const
{
	return "CommandTask";
}

} //ens ns
