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
#ifndef OPENDNP3_STATICLOADER_H
#define OPENDNP3_STATICLOADER_H

#include <openpal/util/Uncopyable.h>
#include "opendnp3/app/StaticRange.h"
#include "opendnp3/app/HeaderWriter.h"
#include "opendnp3/outstation/Database.h"

#include <openpal/serialization/Serialization.h>


namespace opendnp3
{

enum class StaticLoadResult
{
    EMPTY,		// nothing was loaded because the response context is empty
    COMPLETED,	// at least 1 event was loaded and the response context is empty
    DISCONT,    // at least 1 event was loaded and the next range of contiguous points was reached
    FULL		// events were loaded and the APDU is full, context is not empty
};

// A function that takes a writer, range, and a database and writes some objects
typedef StaticLoadResult (*StaticLoadFun)(HeaderWriter& writer, StaticRange& range, Database& db);

class StaticLoader : private openpal::PureStatic
{
public:

	template <class Serializer>
	static StaticLoadResult LoadFixedSizeStartStop(HeaderWriter& writer, StaticRange& range, Database& db);	

private:

	template <class Target, class IndexType>
	static StaticLoadResult LoadFixedSizeStartStopWithIterator(const openpal::Indexable<DualValue<Target>, uint16_t>& values, RangeWriteIterator<IndexType, Target>& iterator, StaticRange& range);
};

template <class Serializer>
StaticLoadResult StaticLoader::LoadFixedSizeStartStop(HeaderWriter& writer, StaticRange& range, Database& db)
{
	auto values = db.Values<typename Serializer::Target>();
	if(range.IsContainedByUInt8())
	{
		auto iter = writer.IterateOverRange<openpal::UInt8, typename Serializer::Target>(QualifierCode::UINT8_START_STOP, Serializer::Inst(), static_cast<uint8_t>(range.start));
		return LoadFixedSizeStartStopWithIterator<typename Serializer::Target, openpal::UInt8>(values, iter, range);
	}
	else
	{
		auto iter = writer.IterateOverRange<openpal::UInt16, typename Serializer::Target>(QualifierCode::UINT16_START_STOP, Serializer::Inst(), range.start);
		return LoadFixedSizeStartStopWithIterator<typename Serializer::Target, openpal::UInt16>(values, iter, range);
	}
}

template <class Target, class IndexType>
StaticLoadResult StaticLoader::LoadFixedSizeStartStopWithIterator(const openpal::Indexable<DualValue<Target>, uint16_t>& values, RangeWriteIterator<IndexType, Target>& iterator, StaticRange& range)
{
	if(range.IsDefined())
	{
		while(range.IsDefined())
		{
			if(iterator.Write(values[range.position].frozen))
			{
                if(!range.Advance())
                {
                    return StaticLoadResult::DISCONT;
                }
			}
			else
			{				
				return StaticLoadResult::FULL;
			}
		}
		
		return StaticLoadResult::COMPLETED;
	}
	else
	{
		return StaticLoadResult::FULL;
	}
}

}

#endif
