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
	crobSeq(*pLogger_, Group12Var1::Inst()),
	analogInt32Seq(*pLogger_, Group41Var1::Inst()),
	analogInt16Seq(*pLogger_, Group41Var2::Inst()),
	analogFloat32Seq(*pLogger_, Group41Var3::Inst()),
	analogDouble64Seq(*pLogger_, Group41Var4::Inst())
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

void CommandTask::SelectAndOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback& callback)
{
	this->LoadSelectAndOperate();
	this->Configure(crobSeq, command, index, callback);
}

void CommandTask::SelectAndOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback& callback)
{
	this->LoadSelectAndOperate();
	this->Configure(analogInt16Seq, command, index, callback);
}

void CommandTask::SelectAndOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback& callback)
{
	this->LoadSelectAndOperate();
	this->Configure(analogInt32Seq, command, index, callback);
}

void CommandTask::SelectAndOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback& callback)
{
	this->LoadSelectAndOperate();
	this->Configure(analogFloat32Seq, command, index, callback);
}

void CommandTask::SelectAndOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback& callback)
{
	this->LoadSelectAndOperate();
	this->Configure(analogDouble64Seq, command, index, callback);
}

void CommandTask::DirectOperate(const ControlRelayOutputBlock& command, uint16_t index,  ICommandCallback& callback)
{
	this->LoadDirectOperate();
	this->Configure(crobSeq, command, index, callback);
}

void CommandTask::DirectOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback& callback)
{
	this->LoadDirectOperate();
	this->Configure(analogInt16Seq, command, index, callback);
}

void CommandTask::DirectOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback& callback)
{
	this->LoadDirectOperate();
	this->Configure(analogInt32Seq, command, index, callback);
}

void CommandTask::DirectOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback& callback)
{
	this->LoadDirectOperate();
	this->Configure(analogFloat32Seq, command, index, callback);
}

void CommandTask::DirectOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback& callback)
{
	this->LoadDirectOperate();
	this->Configure(analogDouble64Seq, command, index, callback);
}

void CommandTask::BuildRequest(APDURequest& request, const MasterParams& params, uint8_t seq)
{
	if (!functionCodes.empty() && pActiveSequence)
	{		
		request.SetFunction(functionCodes.front());
		functionCodes.pop_front();
		request.SetControl(AppControlField::Request(seq));
		pActiveSequence->FormatRequestHeader(request);		
	}
}

TaskStatus CommandTask::OnResponse(const APDUResponseHeader& header, const openpal::ReadOnlyBuffer& objects, const MasterParams& params, IMasterScheduler& scheduler)
{
	if (header.control.FIR && header.control.FIN)
	{
		return this->OnSingleResponse(header, objects, params, scheduler);
	}
	else
	{
		SIMPLE_LOGGER_BLOCK(pLogger, flags::WARN, "Ignoring unexpected response with FIR/FIN not set");
		this->Callback(CommandResponse(CommandResult::BAD_RESPONSE));		
		return TaskStatus::FAIL;
	}
}

void CommandTask::OnResponseTimeout(const MasterParams& params, IMasterScheduler& scheduler)
{
	this->Callback(CommandResponse(CommandResult::TIMEOUT));
}

void CommandTask::OnLowerLayerClose()
{	
	this->Callback(CommandResponse::NoResponse(CommandResult::NO_COMMS));
}

TaskStatus CommandTask::OnSingleResponse(const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects, const MasterParams& params, IMasterScheduler& scheduler)
{
	auto result = APDUParser::ParseTwoPass(objects, pActiveSequence, pLogger);
	if(result == APDUParser::Result::OK)
	{
		if(functionCodes.empty()) // we're done
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
				return TaskStatus::REPEAT;
			}
			else
			{
				this->Callback(commandResponse);  // something failed, end the task early				
				return TaskStatus::FAIL;
			}
		}
	}
	else
	{		
		this->Callback(CommandResponse(CommandResult::BAD_RESPONSE));
		return TaskStatus::FAIL;
	}
}

} //ens ns
