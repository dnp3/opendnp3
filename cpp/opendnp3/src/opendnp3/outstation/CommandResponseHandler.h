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
#ifndef __COMMAND_RESPONSE_HANDLER_H_
#define __COMMAND_RESPONSE_HANDLER_H_

#include "opendnp3/StaticSizeConfiguration.h"
#include "opendnp3/app/APDUHandlerBase.h"
#include "opendnp3/app/APDUResponse.h"
#include "opendnp3/outstation/ICommandAction.h"

#include <openpal/StaticBuffer.h>
#include <openpal/Loggable.h>
#include <openpal/LoggableMacros.h>

namespace opendnp3
{

class CommandResponseHandler : public APDUHandlerBase
{
public:

	CommandResponseHandler(openpal::Logger logger, uint8_t maxCommands_, ICommandAction* pCommandAction_, APDUResponse& response_);

	virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas) override final;
	virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas) override final;
	virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas) override final;
	virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas) override final;
	virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas) override final;

	virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint8_t>>& meas) override final;
	virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputInt16, uint8_t>>& meas) override final;
	virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputInt32, uint8_t>>& meas) override final;
	virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint8_t>>& meas) override final;
	virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint8_t>>& meas) override final;

	bool AllCommandsSuccessful() const { return numRequests == numSuccess; }

private:
			
	ICommandAction* pCommandAction;
	uint32_t numRequests;
	uint32_t numSuccess;
	const uint8_t maxCommands;
	ObjectWriter writer;

	template <class Target, class IndexType>
	void RespondToHeader(QualifierCode qualifier, IDNP3Serializer<Target>* pSerializer, const IterableBuffer<IndexedValue<Target, typename IndexType::Type>>& meas);

};

template <class Target, class IndexType>
void CommandResponseHandler::RespondToHeader(QualifierCode qualifier, IDNP3Serializer<Target>* pSerializer, const IterableBuffer<IndexedValue<Target, typename IndexType::Type>>& meas)
{		
	auto iter = writer.IterateOverCountWithPrefix<IndexType, Target>(qualifier, pSerializer);
	meas.foreach([this, &iter](const IndexedValue<Target, typename IndexType::Type>& command) {				
		auto result = CommandStatus::TOO_MANY_OPS;
		if (numRequests < maxCommands) 
		{
			result = pCommandAction->Action(command.value, command.index);
		}		
		if (result == CommandStatus::SUCCESS) ++numSuccess;
		Target response(command.value);
		response.status = result;
		iter.Write(response, command.index);
		++numRequests;
	});
	iter.Complete();	
}

}


#endif