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
#ifndef OPENDNP3_COMMANDSEQUENCE_H
#define OPENDNP3_COMMANDSEQUENCE_H

#include "opendnp3/app/APDUHandlerBase.h"

#include <openpal/serialization/Serialization.h>

namespace opendnp3
{

/**
*  Abstracts information for
*/
class ICommandSequence : public APDUHandlerBase
{
public:

	ICommandSequence(openpal::Logger logger) : APDUHandlerBase(logger) {}

	// Given an APDU and function code, configure the request
	virtual void FormatRequestHeader(APDURequest& request) = 0;

	// Given the response, what's the result of the command?
	virtual CommandResponse Validate() = 0;
};

template <class CommandType>
class CommandSequence : public ICommandSequence
{
public:
	CommandSequence(openpal::Logger logger, const DNP3Serializer<CommandType>& serializer_) :
		ICommandSequence(logger),
		serializer(serializer_)
	{}

	void Configure(const CommandType& value, uint16_t index)
	{
		this->Reset(); // resets all state inside the base class
		response = CommandResponse(CommandResult::BAD_RESPONSE);
		command = IndexedValue<CommandType, uint16_t>(value, index);
	}

	virtual void _OnIndexPrefix(const HeaderRecord&, const IterableBuffer<IndexedValue<CommandType, uint16_t>>& meas)
	{
		if(this->IsFirstHeader())
		{
			IndexedValue<CommandType, uint16_t> received;
			if(meas.ReadOnlyValue(received))
			{
				if(received.index == command.index && received.value.ValuesEqual(command.value))
				{
					response = CommandResponse(CommandResult::RESPONSE_OK, received.value.status);
				}
			}
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
			return CommandResponse(CommandResult::BAD_RESPONSE);
		}
	}

private:
	CommandResponse response;
	IndexedValue<CommandType, uint16_t> command;
	DNP3Serializer<CommandType> serializer;
};


} //ens ns

#endif
