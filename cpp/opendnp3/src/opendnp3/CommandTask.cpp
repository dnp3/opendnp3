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

#include <openpal/Exception.h>
#include <openpal/LoggableMacros.h>

#include "APDUParser.h"

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
	mpFunctionSequence(nullptr)
	//crobSeq(aLogger),
	//analogInt32Seq(aLogger),
	//analogInt16Seq(aLogger),
	//analogFloat32Seq(aLogger),
	//analogDouble64Seq(aLogger)
{

}

void CommandTask::ConfigureSBO(const ControlRelayOutputBlock& command, uint32_t index, std::function<void (CommandResponse)> aCallback)
{
	//this->Configure(crobSeq, command, index, &SelectAndOperate, aCallback);
}

void CommandTask::ConfigureSBO(const AnalogOutputInt16& command, uint32_t index,  std::function<void (CommandResponse)> aCallback)
{
	//this->Configure(analogInt16Seq, command, index, &SelectAndOperate, aCallback);
}

void CommandTask::ConfigureSBO(const AnalogOutputInt32& command, uint32_t index,  std::function<void (CommandResponse)> aCallback)
{
	//this->Configure(analogInt32Seq, command, index, &SelectAndOperate, aCallback);
}

void CommandTask::ConfigureSBO(const AnalogOutputFloat32& command, uint32_t index,  std::function<void (CommandResponse)> aCallback)
{
	//this->Configure(analogFloat32Seq, command, index, &SelectAndOperate, aCallback);
}

void CommandTask::ConfigureSBO(const AnalogOutputDouble64& command, uint32_t index,  std::function<void (CommandResponse)> aCallback)
{
	//this->Configure(analogDouble64Seq, command, index, &SelectAndOperate, aCallback);
}

void CommandTask::ConfigureDO(const ControlRelayOutputBlock& command, uint32_t index,  std::function<void (CommandResponse)> aCallback)
{
	//this->Configure(crobSeq, command, index, &DirectOperate, aCallback);
}

void CommandTask::ConfigureDO(const AnalogOutputInt16& command, uint32_t index,  std::function<void (CommandResponse)> aCallback)
{
	//this->Configure(analogInt16Seq, command, index, &DirectOperate, aCallback);
}

void CommandTask::ConfigureDO(const AnalogOutputInt32& command, uint32_t index,  std::function<void (CommandResponse)> aCallback)
{
	//this->Configure(analogInt32Seq, command, index, &DirectOperate, aCallback);
}

void CommandTask::ConfigureDO(const AnalogOutputFloat32& command, uint32_t index,  std::function<void (CommandResponse)> aCallback)
{
	//this->Configure(analogFloat32Seq, command, index, &DirectOperate, aCallback);
}

void CommandTask::ConfigureDO(const AnalogOutputDouble64& command, uint32_t index,  std::function<void (CommandResponse)> aCallback)
{
	//this->Configure(analogDouble64Seq, command, index, &DirectOperate, aCallback);
}

/*
void CommandTask::ConfigureRequest(APDU& aAPDU)
{
	assert(mpFunctionSequence != nullptr);
	assert(mpActiveSequence != nullptr);
	auto code = mpFunctionSequence->Value();
	mpFunctionSequence = mpFunctionSequence->Next();
	mpActiveSequence->FormatAPDU(aAPDU, code); 	
}
*/

void CommandTask::OnFailure()
{
	callback(CommandResponse(CommandResult::TIMEOUT));
}

TaskResult CommandTask::_OnPartialResponse(const APDUResponseRecord&)
{
	LOG_BLOCK(LogLevel::Error, "Non fin responses not allowed for control tasks");
	return TR_CONTINUE;
}

TaskResult CommandTask::_OnFinalResponse(const APDUResponseRecord& record)
{
	auto result = APDUParser::ParseTwoPass(record.objects, mpActiveSequence);
	if(result == APDUParser::Result::OK)
	{				
		if(mpFunctionSequence == nullptr) // we're done
		{
			auto commandResponse = mpActiveSequence->Validate();		
			callback(commandResponse);
			return TaskResult::TR_SUCCESS;
		}
		else // we may have more depending on response
		{
			auto commandResponse = mpActiveSequence->Validate();
			if(commandResponse == CommandResponse::Success) return TaskResult::TR_CONTINUE; // more function codes		
			else
			{
				callback(commandResponse);  // something failed, end the task early
				return TaskResult::TR_SUCCESS;
			}			
		}
	}
	else
	{
		LOG_BLOCK(LogLevel::Error, "Error parsing apdu: " << static_cast<int>(result)); // TODO - improve logging
		callback(CommandResponse(CommandResult::TIMEOUT));  // maybe another code for malformed response?
		return TaskResult::TR_FAIL;
	}	
}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string CommandTask::Name() const
{
	return "CommandTask";
}
#endif


} //ens ns


