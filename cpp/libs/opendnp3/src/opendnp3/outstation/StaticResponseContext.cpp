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
#include "StaticResponseContext.h"

#include <openpal/serialization/Serialization.h>

#include "opendnp3/app/WriteConversions.h"

using namespace openpal;

namespace opendnp3
{

StaticResponseContext::StaticResponseContext(Database& database, const StaticResponseConfig& config) :
	pDatabase(&database),
	defaults(config),
	staticResponseQueue(8) // TODO - make configurable
{}

bool StaticResponseContext::IsComplete() const
{
	return staticResponseQueue.IsEmpty();
}

bool StaticResponseContext::HasRequests() const
{
	return staticResponseQueue.IsNotEmpty();
}

void StaticResponseContext::Reset()
{
	staticResponseQueue.Clear();
}

IINField StaticResponseContext::ReadAll(const GroupVariationRecord& record)
{
	if (record.enumeration == GroupVariation::Group60Var1)
	{
		IINField result;
		result = result | QueueRange<Binary>(defaults.binary);
		result = result | QueueRange<DoubleBitBinary>(defaults.doubleBinary);
		result = result | QueueRange<BinaryOutputStatus>(defaults.binaryOutputStatus);
		result = result | QueueRange<Counter>(defaults.counter);
		result = result | QueueRange<FrozenCounter>(defaults.frozenCounter);
		result = result | QueueRange<Analog>(defaults.analog);
		result = result | QueueRange<AnalogOutputStatus>(defaults.analogOutputStatus);
		return result;		
	}
	else
	{
		switch (record.group)
		{
			case(1) :
				return ReadRange(record, pDatabase->FullRange<Binary>().ToRange());
			case(3) :
				return ReadRange(record, pDatabase->FullRange<DoubleBitBinary>().ToRange());
			case(10) :
				return ReadRange(record, pDatabase->FullRange<BinaryOutputStatus>().ToRange());
			case(20) :
				return ReadRange(record, pDatabase->FullRange<Counter>().ToRange());
			case(21) :
				return ReadRange(record, pDatabase->FullRange<FrozenCounter>().ToRange());
			case(30) :
				return ReadRange(record, pDatabase->FullRange<Analog>().ToRange());
			case(40) :
				return ReadRange(record, pDatabase->FullRange<AnalogOutputStatus>().ToRange());
			default:
				return IINField(IINBit::FUNC_NOT_SUPPORTED);
		}
	}	
}

IINField StaticResponseContext::ReadRange(const GroupVariationRecord& record, const Range& range)
{
	switch (record.enumeration)
	{
		// Group 1
	case(GroupVariation::Group1Var0) :
		return QueueRange<Binary>(range, defaults.binary);
	case(GroupVariation::Group1Var2) :
		return QueueRange<Binary>(range, StaticBinaryResponse::Group1Var2);

	case(GroupVariation::Group3Var0) :
		return QueueRange<DoubleBitBinary>(range, defaults.doubleBinary);
	case(GroupVariation::Group3Var2) :
		return QueueRange<DoubleBitBinary>(range, StaticDoubleBinaryResponse::Group3Var2);

		// Group 10
	case(GroupVariation::Group10Var0) :
		return QueueRange<BinaryOutputStatus>(range, defaults.binaryOutputStatus);
	case(GroupVariation::Group10Var2) :
		return QueueRange<BinaryOutputStatus>(range, StaticBinaryOutputStatusResponse::Group10Var2);

		// Group 20
	case(GroupVariation::Group20Var0) :
		return QueueRange<Counter>(range, defaults.counter);
	case(GroupVariation::Group20Var1) :
		return QueueRange<Counter>(range, StaticCounterResponse::Group20Var1);
	case(GroupVariation::Group20Var2) :
		return QueueRange<Counter>(range, StaticCounterResponse::Group20Var2);
	case(GroupVariation::Group20Var5) :
		return QueueRange<Counter>(range, StaticCounterResponse::Group20Var5);
	case(GroupVariation::Group20Var6) :
		return QueueRange<Counter>(range, StaticCounterResponse::Group20Var6);

		// Group 21
	case(GroupVariation::Group21Var0) :
		return QueueRange<FrozenCounter>(range, defaults.frozenCounter);
	case(GroupVariation::Group21Var1) :
		return QueueRange<FrozenCounter>(range, StaticFrozenCounterResponse::Group21Var1);
	case(GroupVariation::Group21Var2) :
		return QueueRange<FrozenCounter>(range, StaticFrozenCounterResponse::Group21Var2);
	case(GroupVariation::Group21Var5) :
		return QueueRange<FrozenCounter>(range, StaticFrozenCounterResponse::Group21Var5);
	case(GroupVariation::Group21Var6) :
		return QueueRange<FrozenCounter>(range, StaticFrozenCounterResponse::Group21Var6);
	case(GroupVariation::Group21Var9) :
		return QueueRange<FrozenCounter>(range, StaticFrozenCounterResponse::Group21Var9);
	case(GroupVariation::Group21Var10) :
		return QueueRange<FrozenCounter>(range, StaticFrozenCounterResponse::Group21Var10);

		// Group 30
	case(GroupVariation::Group30Var0) :
		return QueueRange<Analog>(range, defaults.analog);
	case(GroupVariation::Group30Var1) :
		return QueueRange<Analog>(range, StaticAnalogResponse::Group30Var1);
	case(GroupVariation::Group30Var2) :
		return QueueRange<Analog>(range, StaticAnalogResponse::Group30Var2);
	case(GroupVariation::Group30Var3) :
		return QueueRange<Analog>(range, StaticAnalogResponse::Group30Var3);
	case(GroupVariation::Group30Var4) :
		return QueueRange<Analog>(range, StaticAnalogResponse::Group30Var4);
	case(GroupVariation::Group30Var5) :
		return QueueRange<Analog>(range, StaticAnalogResponse::Group30Var5);
	case(GroupVariation::Group30Var6) :
		return QueueRange<Analog>(range, StaticAnalogResponse::Group30Var6);

		// Group 40
	case(GroupVariation::Group40Var0) :
		return QueueRange<AnalogOutputStatus>(range, defaults.analogOutputStatus);
	case(GroupVariation::Group40Var1) :
		return QueueRange<AnalogOutputStatus>(range, StaticAnalogOutputStatusResponse::Group40Var1);
	case(GroupVariation::Group40Var2) :
		return QueueRange<AnalogOutputStatus>(range, StaticAnalogOutputStatusResponse::Group40Var2);
	case(GroupVariation::Group40Var3) :
		return QueueRange<AnalogOutputStatus>(range, StaticAnalogOutputStatusResponse::Group40Var3);
	case(GroupVariation::Group40Var4) :
		return QueueRange<AnalogOutputStatus>(range, StaticAnalogOutputStatusResponse::Group40Var4);

	default:
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

IINField StaticResponseContext::QueueLoader(const StaticRangeLoader& loader)
{
	if(loader.IsDefined())
	{
		if(staticResponseQueue.Enqueue(loader))
		{
			return loader.IsClipped() ? IINField(IINBit::PARAM_ERROR) : IINField::Empty;
		}
		else
		{
			return IINField(IINBit::PARAM_ERROR);
		}
	}
	else
	{
		return IINField::Empty;
	}
}

bool StaticResponseContext::Load(HeaderWriter& writer)
{
	switch (LoadStaticData(writer))
	{
	case(StaticLoadResult::EMPTY):
	case(StaticLoadResult::COMPLETED) :
		return true;
	case(StaticLoadResult::FULL) :
	default:
		return false;
	}
}

StaticLoadResult StaticResponseContext::LoadStaticData(HeaderWriter& writer)
{
	while(!staticResponseQueue.IsEmpty())
	{
		auto pFront = staticResponseQueue.Peek();
		auto result = StaticLoadResult::DISCONT;
        
        // Repeat for each contiguous range of points
        while(result == StaticLoadResult::DISCONT)
        {
            result = (*pFront->pLoadFun)(writer, *pFront, *pDatabase);
        }
		if(result == StaticLoadResult::COMPLETED)
		{
			staticResponseQueue.Pop();
			if (staticResponseQueue.IsEmpty())
			{
				return StaticLoadResult::COMPLETED;
			}			
		}
		else
		{
			return result;
		}
	}

	return StaticLoadResult::EMPTY;
}


}


