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
#ifndef OPENDNP3_RANGEPARSER_H
#define OPENDNP3_RANGEPARSER_H

#include <openpal/container/ReadBufferView.h>
#include <openpal/logging/Logger.h>

#include "opendnp3/app/parsing/ParseResult.h"
#include "opendnp3/app/parsing/IAPDUHandler.h"
#include "opendnp3/app/parsing/ParseResult.h"
#include "opendnp3/app/parsing/NumParser.h"
#include "opendnp3/app/parsing/ParserSettings.h"
#include "opendnp3/app/parsing/BitReader.h"
#include "opendnp3/app/parsing/LazyIterable.h"

#include "opendnp3/app/Range.h"


namespace opendnp3
{

class RangeParser
{
	typedef void (*HandleFun)(const HeaderRecord& record, const Range& range, const openpal::ReadBufferView& buffer, IAPDUHandler& handler);

	

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

	// Process the range against the buffer
	ParseResult Process(const HeaderRecord& record, openpal::ReadBufferView& buffer, IAPDUHandler* pHandler, openpal::Logger* pLogger) const;

	// Create a range parser from a fixed size descriptor
	template <class Descriptor>
	static RangeParser FromFixedSize(const Range& range);

	

	// Create a range parser from a bitfield and a function to map the bitfield to values
	template <class Conversion>
	static RangeParser FromBitfield(const Range& range);

	template <class Conversion>
	static RangeParser FromDoubleBitfield(const Range& range);

	static ParseResult ParseRangeOfObjects(openpal::ReadBufferView& buffer, const HeaderRecord& record, const Range& range, openpal::Logger* pLogger, IAPDUHandler* pHandler);

	static ParseResult ParseRangeOfOctetData(openpal::ReadBufferView& buffer, const HeaderRecord& record, const Range& range, openpal::Logger* pLogger, IAPDUHandler* pHandler);	
	
	template <class Descriptor>
	static void InvokeRangeOf(const HeaderRecord& record, const Range& range, const openpal::ReadBufferView& buffer, IAPDUHandler& handler);	

	template <class Conversion>
	static void InvokeRangeBitfield(const HeaderRecord& record, const Range& range, const openpal::ReadBufferView& buffer, IAPDUHandler& handler);

	template <class Conversion>
	static void InvokeRangeDoubleBitfield(const HeaderRecord& record, const Range& range, const openpal::ReadBufferView& buffer, IAPDUHandler& handler);

	RangeParser(const Range& range, uint32_t requiredSize, HandleFun handler);
		
	Range range;
	uint32_t requiredSize;
	HandleFun handler;
	
	RangeParser() = delete;
};

template <class Descriptor>
RangeParser RangeParser::FromFixedSize(const Range& range)
{
	uint32_t size = range.Count() * Descriptor::Size();
	return RangeParser(range, size, &InvokeRangeOf<Descriptor>);
}

template <class Descriptor>
void RangeParser::InvokeRangeOf(const HeaderRecord& record, const Range& range, const openpal::ReadBufferView& buffer, IAPDUHandler& handler)
{	
	auto reader = [range](openpal::ReadBufferView &buffer, uint32_t pos) 
	{ 
		return IndexedValue<typename Descriptor::Target, uint16_t>(Descriptor::ReadTarget(buffer), range.start + pos); 
	};		
	
	auto collection = CreateLazyIterable<IndexedValue<typename Descriptor::Target, uint16_t>>(buffer, range.Count(), reader);
	handler.OnRange(record, collection);
}

template <class Conversion>
RangeParser RangeParser::FromBitfield(const Range& range)
{
	uint32_t size = NumBytesInBits(range.Count());
	return RangeParser(range, size, &InvokeRangeBitfield<Conversion>);
}

template <class Conversion>
void RangeParser::InvokeRangeBitfield(const HeaderRecord& record, const Range& range, const openpal::ReadBufferView& buffer, IAPDUHandler& handler)
{
	auto read = [range](openpal::ReadBufferView & buffer, uint32_t pos)
	{
		return Conversion::Convert(GetBit(buffer, pos), pos + range.start);
	};

	auto collection = CreateLazyIterable<typename Conversion::Target>(buffer, range.Count(), read);
	handler.OnRange(record, collection);			
}

template <class Conversion>
RangeParser RangeParser::FromDoubleBitfield(const Range& range)
{
	uint32_t size = NumBytesInDoubleBits(range.Count());
	return RangeParser(range, size, &InvokeRangeDoubleBitfield<Conversion>);
}

template <class Conversion>
void RangeParser::InvokeRangeDoubleBitfield(const HeaderRecord& record, const Range& range, const openpal::ReadBufferView& buffer, IAPDUHandler& handler)
{
	auto read = [range](openpal::ReadBufferView & buffer, uint32_t pos)
	{
		return Conversion::Convert(GetDoubleBit(buffer, pos), pos + range.start);
	};

	auto collection = CreateLazyIterable<typename Conversion::Target>(buffer, range.Count(), read);
	handler.OnRange(record, collection);
}

template <class T>
class BitfieldConversion : openpal::PureStatic
{
public:
	typedef IndexedValue<T, uint16_t> Target;

	static IndexedValue<T, uint16_t> Convert(bool value, uint16_t index)
	{
		return IndexedValue<T, uint16_t>(T(value), index);
	}
};

template <class T>
class DoubleBitfieldConversion : openpal::PureStatic
{
public:
	typedef IndexedValue<T, uint16_t> Target;

	static IndexedValue<T, uint16_t> Convert(DoubleBit value, uint16_t index)
	{
		return IndexedValue<T, uint16_t>(T(value), index);
	}
};


}

#endif
