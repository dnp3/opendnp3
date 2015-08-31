/*
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
#ifndef OPENDNP3_COMMANDSEQUENCE_H
#define OPENDNP3_COMMANDSEQUENCE_H

#include "opendnp3/app/parsing/IAPDUHandler.h"

#include <openpal/serialization/Serialization.h>

namespace opendnp3
{

/**
*  Abstracts information for
*/
class ICommandSequence : public IAPDUHandler
{
public:

	// Given an APDU and function code, configure the request
	virtual void FormatRequestHeader(APDURequest& request) = 0;

	// Given the response, what's the result of the command?
	virtual CommandResponse Validate() = 0;
};

template <class CommandType>
class CommandSequence : public ICommandSequence
{
public:
	CommandSequence(const DNP3Serializer<CommandType>& serializer_, const CommandType& value, uint16_t index) :
		ICommandSequence(),
		serializer(serializer_),
		command(value, index),
		response(CommandResponse(TaskCompletion::FAILURE_BAD_RESPONSE))
	{}

	// TODO - make this restrictive to the type in use?
	virtual bool IsAllowed(uint32_t headerCount, GroupVariation gv, QualifierCode qc) override final
	{
		return true;
	}

	virtual IINField ProcessHeader(const PrefixHeader& header, const ICollection<Indexed<CommandType>>& meas) override final
	{
		Indexed<CommandType> received;
		if (this->IsFirstHeader() && meas.ReadOnlyValue(received))
		{
			if(received.index == command.index && received.value.ValuesEqual(command.value))
			{
				response = CommandResponse(TaskCompletion::SUCCESS, received.value.status);
				return IINField();
			}
			else
			{
				return IINBit::PARAM_ERROR;
			}
		}
		else
		{
			return IINBit::PARAM_ERROR;
		}
	}


	virtual void FormatRequestHeader(APDURequest& request) final
	{
		auto writer = request.GetWriter();
		writer.WriteSingleIndexedValue<openpal::UInt16, CommandType>(QualifierCode::UINT16_CNT_UINT16_INDEX, serializer, command.value, command.index);
	}


	virtual CommandResponse Validate() final
	{
		auto numHeaders = this->GetCurrentHeader();
		auto singleHeader = (numHeaders == 1);
		this->Reset();
		if (singleHeader)
		{
			return response;
		}
		else
		{
			return CommandResponse(TaskCompletion::FAILURE_BAD_RESPONSE);
		}
	}

private:
	DNP3Serializer<CommandType> serializer;
	Indexed<CommandType> command;
	CommandResponse response;
};


} //ens ns

#endif
