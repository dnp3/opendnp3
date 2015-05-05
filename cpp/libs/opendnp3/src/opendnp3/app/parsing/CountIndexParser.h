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
#ifndef OPENDNP3_COUNTINDEXPARSER_H
#define OPENDNP3_COUNTINDEXPARSER_H

#include <openpal/container/ReadBufferView.h>
#include <openpal/logging/Logger.h>

#include "opendnp3/app/parsing/ParseResult.h"
#include "opendnp3/app/parsing/IAPDUHandler.h"
#include "opendnp3/app/parsing/ParseResult.h"
#include "opendnp3/app/parsing/NumParser.h"
#include "opendnp3/app/parsing/ParserSettings.h"

#include "opendnp3/app/parsing/LazyIterable.h"

namespace opendnp3
{

class CountIndexParser
{
	typedef void(*HandleFun)(const HeaderRecord& record, uint16_t count, const NumParser& numparser, const openpal::ReadBufferView& buffer, IAPDUHandler& handler);

public:
	
	static ParseResult ParseHeader(
		openpal::ReadBufferView& buffer,
		const NumParser& numparser,
		const ParserSettings& settings, 
		const HeaderRecord& record, 
		openpal::Logger* pLogger, 
		IAPDUHandler* pHandler
		);

private:

	// Process the count handler against the buffer
	ParseResult Process(const HeaderRecord& record, openpal::ReadBufferView& buffer, IAPDUHandler* pHandler, openpal::Logger* pLogger) const;

	// Create a count handler from a fixed size descriptor
	template <class Descriptor>
	static CountIndexParser From(uint16_t count, const NumParser& numparser);

	// Create a count handler from a fixed size descriptor
	template <class Type>
	static CountIndexParser FromType(uint16_t count, const NumParser& numparser);

	// Create a count handler from a fixed size descriptor, handle it with a lazy collection
	template <class Descriptor>
	static CountIndexParser FromCollection(uint16_t count, const NumParser& numparser);

	static ParseResult ParseCountOfObjects(openpal::ReadBufferView& buffer, const HeaderRecord& record, const NumParser& numparser, uint16_t count, openpal::Logger* pLogger, IAPDUHandler* pHandler);

	static ParseResult ParseIndexPrefixedOctetData(openpal::ReadBufferView& buffer, const HeaderRecord& record, const NumParser& numParser, uint32_t count, openpal::Logger* pLogger, IAPDUHandler* pHandler);
		
	template <class Descriptor>
	static void InvokeCountOf(const HeaderRecord& record, uint16_t count, const NumParser& numparser, const openpal::ReadBufferView& buffer, IAPDUHandler& handler);

	template <class Type>
	static void InvokeCountOfType(const HeaderRecord& record, uint16_t count, const NumParser& numparser, const openpal::ReadBufferView& buffer, IAPDUHandler& handler);

	template <class Descriptor>
	static void InvokeCountOfCollection(const HeaderRecord& record, uint16_t count, const NumParser& numparser, const openpal::ReadBufferView& buffer, IAPDUHandler& handler);

	CountIndexParser(uint16_t count, uint32_t requiredSize, const NumParser& numparser, HandleFun handler);
		
	uint16_t count;
	uint32_t requiredSize;
	NumParser numparser;
	HandleFun handler;
	
	CountIndexParser() = delete;
};

template <class Descriptor>
CountIndexParser CountIndexParser::From(uint16_t count, const NumParser& numparser)
{
	const uint32_t SIZE = static_cast<uint32_t>(count) * (Descriptor::Size() + numparser.NumBytes());
	return CountIndexParser(count, SIZE, numparser, &InvokeCountOf<Descriptor>);
}

template <class Type>
CountIndexParser CountIndexParser::FromType(uint16_t count, const NumParser& numparser)
{
	const uint32_t SIZE = static_cast<uint32_t>(count) * (Type::Size() + numparser.NumBytes());
	return CountIndexParser(count, SIZE, numparser, &InvokeCountOfType<Type>);
}

template <class Descriptor>
CountIndexParser CountIndexParser::FromCollection(uint16_t count, const NumParser& numparser)
{
	const uint32_t SIZE = static_cast<uint32_t>(count)* (Descriptor::Size() + numparser.NumBytes());
	return CountIndexParser(count, SIZE, numparser, &InvokeCountOfCollection<Descriptor>);
}

template <class Descriptor>
void CountIndexParser::InvokeCountOfCollection(const HeaderRecord& record, uint16_t count, const NumParser& numparser, const openpal::ReadBufferView& buffer, IAPDUHandler& handler)
{	
	auto reader = [numparser](openpal::ReadBufferView & buffer, uint32_t) 
	{ 		
		auto index = numparser.ReadNum(buffer);
		typename Descriptor::Target target;
		Descriptor::ReadTarget(buffer, target);
		return IndexedValue<typename Descriptor::Target, uint16_t>(target, index);
	};		

	auto collection = CreateLazyIterable<IndexedValue<typename Descriptor::Target, uint16_t>>(buffer, count, reader);
	handler.OnIndexPrefix(record, collection);
}

template <class Descriptor>
void CountIndexParser::InvokeCountOf(const HeaderRecord& record, uint16_t count, const NumParser& numparser, const openpal::ReadBufferView& buffer, IAPDUHandler& handler)
{
	openpal::ReadBufferView copy(buffer);

	for (uint16_t i = 0; i < count; ++i)
	{
		auto index = numparser.ReadNum(copy);
		typename Descriptor::Target target;
		Descriptor::ReadTarget(copy, target);
		handler.OnIndexPrefix(record, count, target, index);
	}	
}

template <class Type>
void CountIndexParser::InvokeCountOfType(const HeaderRecord& record, uint16_t count, const NumParser& numparser, const openpal::ReadBufferView& buffer, IAPDUHandler& handler)
{
	openpal::ReadBufferView copy(buffer);

	for (uint16_t i = 0; i < count; ++i)
	{
		auto index = numparser.ReadNum(copy);
		Type value;
		Type::Read(copy, value);
		handler.OnIndexPrefix(record, count, value, index);
	}
}

}

#endif
