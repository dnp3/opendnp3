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
#ifndef OPENDNP3_COMMANDRESPONSEHANDLER_H
#define OPENDNP3_COMMANDRESPONSEHANDLER_H

#include "opendnp3/app/parsing/APDUHandlerBase.h"
#include "opendnp3/app/APDUResponse.h"
#include "opendnp3/outstation/ICommandAction.h"
#include "opendnp3/app/parsing/IterableTransforms.h"

#include <openpal/logging/LogMacros.h>

namespace opendnp3
{

class CommandResponseHandler : public APDUHandlerBase
{
public:

	CommandResponseHandler(openpal::Logger logger, uint8_t maxCommands_, ICommandAction* pCommandAction_, HeaderWriter* pWriter_);	

	bool AllCommandsSuccessful() const
	{
		return numRequests == numSuccess;
	}

private:

	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas) override final;
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas) override final;
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas) override final;
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas) override final;
	virtual IINField ProcessIndexPrefix(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas) override final;
	
	IINField ProcessIndexPrefixTwoByte(const HeaderRecord& record, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas);
	IINField ProcessIndexPrefixTwoByte(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas);
	IINField ProcessIndexPrefixTwoByte(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas);
	IINField ProcessIndexPrefixTwoByte(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas);
	IINField ProcessIndexPrefixTwoByte(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas);

	IINField ProcessIndexPrefixOneByte(const HeaderRecord& record, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint8_t>>& meas);
	IINField ProcessIndexPrefixOneByte(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint8_t>>& meas);
	IINField ProcessIndexPrefixOneByte(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint8_t>>& meas);
	IINField ProcessIndexPrefixOneByte(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint8_t>>& meas);
	IINField ProcessIndexPrefixOneByte(const HeaderRecord& record, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint8_t>>& meas);

	ICommandAction* pCommandAction;
	uint32_t numRequests;
	uint32_t numSuccess;
	const uint8_t maxCommands;
	HeaderWriter* pWriter;

	template <class T>
	IINField ProcessAny(const HeaderRecord& record, const IterableBuffer<IndexedValue<T, uint16_t>>& meas);

	template <class Target, class IndexType>
	IINField RespondToHeader(QualifierCode qualifier, const DNP3Serializer<Target>& serializer, const IterableBuffer<IndexedValue<Target, typename IndexType::Type>>& values);

	template <class Target, class IndexType>
	IINField RespondToHeaderWithIterator(QualifierCode qualifier, const DNP3Serializer<Target>& serializer, const IterableBuffer<IndexedValue<Target, typename IndexType::Type>>& values, PrefixedWriteIterator<IndexType, Target>* pIterator = nullptr);
};

template <class T>
IINField CommandResponseHandler::ProcessAny(const HeaderRecord& record, const IterableBuffer<IndexedValue<T, uint16_t>>& meas)
{
	if (record.GetQualifierCode() == QualifierCode::UINT8_CNT_UINT8_INDEX)
	{
		auto narrow = [](const IndexedValue<T, uint16_t>& x) { return IndexedValue<T, uint8_t>(x.value, static_cast<uint8_t>(x.index)); };
		auto transformed = MapIterableBuffer<IndexedValue<T, uint16_t>, IndexedValue<T, uint8_t>>(&meas, narrow);
		return ProcessIndexPrefixOneByte(record, transformed);		
	}
	else
	{
		return ProcessIndexPrefixTwoByte(record, meas);
	}
}


template <class Target, class IndexType>
IINField CommandResponseHandler::RespondToHeaderWithIterator(QualifierCode qualifier, const DNP3Serializer<Target>& serializer, const IterableBuffer<IndexedValue<Target, typename IndexType::Type>>& values, PrefixedWriteIterator<IndexType, Target>* pIterator)
{
	IINField ret;

	auto commands = values.Iterate();
	do
	{
		auto command = commands.Current();
		auto result = CommandStatus::TOO_MANY_OPS;

		if (numRequests < maxCommands)
		{
			result = pCommandAction->Action(command.value, command.index);
		}

		switch (result)
		{
			case(CommandStatus::SUCCESS) :
				++numSuccess;
				break;
			case(CommandStatus::NOT_SUPPORTED) :
				ret.SetBit(IINBit::PARAM_ERROR);
				break;
			default:
				break;
		}

		Target response(command.value);
		response.status = result;
		
		if (pIterator)
		{
			pIterator->Write(response, command.index);
		}		

		++numRequests;
	} 
	while (commands.MoveNext());
	
	return ret;
}


template <class Target, class IndexType>
IINField CommandResponseHandler::RespondToHeader(QualifierCode qualifier, const DNP3Serializer<Target>& serializer, const IterableBuffer<IndexedValue<Target, typename IndexType::Type>>& values)
{
	if (pWriter)
	{
		auto iter = pWriter->IterateOverCountWithPrefix<IndexType, Target>(qualifier, serializer);		
		return this->RespondToHeaderWithIterator<Target, IndexType>(qualifier, serializer, values, &iter);
	}
	else
	{		
		return this->RespondToHeaderWithIterator<Target, IndexType>(qualifier, serializer, values);
	}
}

}


#endif
