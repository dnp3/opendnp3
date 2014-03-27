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

#include <openpal/Serialization.h>

#include "opendnp3/app/WriteConversions.h"

using namespace openpal;

namespace opendnp3
{

StaticResponseContext::StaticResponseContext(Database* pDatabase_, const StaticResponseTypes& rspTypes_) :
	fragmentCount(0),
	pDatabase(pDatabase_),
	rspTypes(rspTypes_)
{}

bool StaticResponseContext::IsComplete() const
{
	return staticResponseQueue.IsEmpty();
}

void StaticResponseContext::Reset()
{
	fragmentCount = 0;
	staticResponseQueue.Clear();
}

IINField StaticResponseContext::QueueReadAllObjects(const GroupVariationRecord& record)
{	
	switch(record.enumeration)
	{
		case(GroupVariation::Group60Var1):
			return QueueStaticIntegrity();			

			// Group 1
		case(GroupVariation::Group1Var0):
			return QueueReadRange(GetFullRangeWithDefaultLoader<Binary>());
		case(GroupVariation::Group1Var2):
			return QueueReadRange(GetFullRange<Group1Var2Serializer>());

			// Group 10
		case(GroupVariation::Group10Var0):
			return QueueReadRange(GetFullRangeWithDefaultLoader<BinaryOutputStatus>());
		case(GroupVariation::Group10Var2):
			return QueueReadRange(GetFullRange<Group10Var2Serializer>());

			// Group 20
		case(GroupVariation::Group20Var0):
			return QueueReadRange(GetFullRangeWithDefaultLoader<Counter>());
		case(GroupVariation::Group20Var1):
			return QueueReadRange(GetFullRange<Group20Var1Serializer>());
		case(GroupVariation::Group20Var2):
			return QueueReadRange(GetFullRange<Group20Var2Serializer>());
		case(GroupVariation::Group20Var5):
			return QueueReadRange(GetFullRange<Group20Var5Serializer>());
		case(GroupVariation::Group20Var6):
			return QueueReadRange(GetFullRange<Group20Var6Serializer>());

			// Group 21
		case(GroupVariation::Group21Var0):
			return QueueReadRange(GetFullRangeWithDefaultLoader<FrozenCounter>());
		case(GroupVariation::Group21Var1):
			return QueueReadRange(GetFullRange<Group21Var1Serializer>());
		case(GroupVariation::Group21Var2):
			return QueueReadRange(GetFullRange<Group21Var2Serializer>());
		case(GroupVariation::Group21Var5):
			return QueueReadRange(GetFullRange<Group21Var5Serializer>());
		case(GroupVariation::Group21Var6):
			return QueueReadRange(GetFullRange<Group21Var6Serializer>());
		case(GroupVariation::Group21Var9):
			return QueueReadRange(GetFullRange<Group21Var9Serializer>());
		case(GroupVariation::Group21Var10):
			return QueueReadRange(GetFullRange<Group21Var10Serializer>());

			// Group 30
		case(GroupVariation::Group30Var0):
			return QueueReadRange(GetFullRangeWithDefaultLoader<Analog>());
		case(GroupVariation::Group30Var1):
			return QueueReadRange(GetFullRange<Group30Var1Serializer>());
		case(GroupVariation::Group30Var2):
			return QueueReadRange(GetFullRange<Group30Var2Serializer>());
		case(GroupVariation::Group30Var3):
			return QueueReadRange(GetFullRange<Group30Var3Serializer>());
		case(GroupVariation::Group30Var4):
			return QueueReadRange(GetFullRange<Group30Var4Serializer>());
		case(GroupVariation::Group30Var5):
			return QueueReadRange(GetFullRange<Group30Var5Serializer>());
		case(GroupVariation::Group30Var6):
			return QueueReadRange(GetFullRange<Group30Var6Serializer>());

			// Group 40
		case(GroupVariation::Group40Var0):
			return QueueReadRange(GetFullRangeWithDefaultLoader<AnalogOutputStatus>());
		case(GroupVariation::Group40Var1):
			return QueueReadRange(GetFullRange<Group40Var1Serializer>());
		case(GroupVariation::Group40Var2):
			return QueueReadRange(GetFullRange<Group40Var2Serializer>());
		case(GroupVariation::Group40Var3):
			return QueueReadRange(GetFullRange<Group40Var3Serializer>());
		case(GroupVariation::Group40Var4):
			return QueueReadRange(GetFullRange<Group40Var4Serializer>());

		default:
			return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

IINField StaticResponseContext::QueueReadRange(const GroupVariationRecord& record, const StaticRange& range)
{
	switch(record.enumeration)
	{
		// Group 1
		case(GroupVariation::Group1Var0):
			return QueueReadRange(GetClippedRangeWithDefaultLoader<Binary>(range));
		case(GroupVariation::Group1Var2):
			return QueueReadRange(GetClippedRange<Group1Var2Serializer>(range));

			// Group 10
		case(GroupVariation::Group10Var0):
			return QueueReadRange(GetClippedRangeWithDefaultLoader<BinaryOutputStatus>(range));
		case(GroupVariation::Group10Var2):
			return QueueReadRange(GetClippedRange<Group10Var2Serializer>(range));

			// Group 20
		case(GroupVariation::Group20Var0):
			return QueueReadRange(GetClippedRangeWithDefaultLoader<Counter>(range));
		case(GroupVariation::Group20Var1):
			return QueueReadRange(GetClippedRange<Group20Var1Serializer>(range));
		case(GroupVariation::Group20Var2):
			return QueueReadRange(GetClippedRange<Group20Var2Serializer>(range));
		case(GroupVariation::Group20Var5):
			return QueueReadRange(GetClippedRange<Group20Var5Serializer>(range));
		case(GroupVariation::Group20Var6):
			return QueueReadRange(GetClippedRange<Group20Var6Serializer>(range));


			// Group 21
		case(GroupVariation::Group21Var0):
			return QueueReadRange(GetClippedRangeWithDefaultLoader<FrozenCounter>(range));
		case(GroupVariation::Group21Var1):
			return QueueReadRange(GetClippedRange<Group21Var1Serializer>(range));
		case(GroupVariation::Group21Var2):
			return QueueReadRange(GetClippedRange<Group21Var2Serializer>(range));
		case(GroupVariation::Group21Var5):
			return QueueReadRange(GetClippedRange<Group21Var5Serializer>(range));
		case(GroupVariation::Group21Var6):
			return QueueReadRange(GetClippedRange<Group21Var6Serializer>(range));
		case(GroupVariation::Group21Var9):
			return QueueReadRange(GetClippedRange<Group21Var9Serializer>(range));
		case(GroupVariation::Group21Var10):
			return QueueReadRange(GetClippedRange<Group21Var10Serializer>(range));


			// Group 30
		case(GroupVariation::Group30Var0):
			return QueueReadRange(GetClippedRangeWithDefaultLoader<Analog>(range));
		case(GroupVariation::Group30Var1):
			return QueueReadRange(GetClippedRange<Group30Var1Serializer>(range));
		case(GroupVariation::Group30Var2):
			return QueueReadRange(GetClippedRange<Group30Var2Serializer>(range));
		case(GroupVariation::Group30Var3):
			return QueueReadRange(GetClippedRange<Group30Var3Serializer>(range));
		case(GroupVariation::Group30Var4):
			return QueueReadRange(GetClippedRange<Group30Var4Serializer>(range));
		case(GroupVariation::Group30Var5):
			return QueueReadRange(GetClippedRange<Group30Var5Serializer>(range));
		case(GroupVariation::Group30Var6):
			return QueueReadRange(GetClippedRange<Group30Var6Serializer>(range));

			// Group 40
		case(GroupVariation::Group40Var0):
			return QueueReadRange(GetClippedRangeWithDefaultLoader<AnalogOutputStatus>(range));
		case(GroupVariation::Group40Var1):
			return QueueReadRange(GetClippedRange<Group40Var1Serializer>(range));
		case(GroupVariation::Group40Var2):
			return QueueReadRange(GetClippedRange<Group40Var2Serializer>(range));
		case(GroupVariation::Group40Var3):
			return QueueReadRange(GetClippedRange<Group40Var3Serializer>(range));
		case(GroupVariation::Group40Var4):
			return QueueReadRange(GetClippedRange<Group40Var4Serializer>(range));


		default:
			return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

IINField StaticResponseContext::QueueStaticIntegrity()
{
	StaticQueue<StaticRangeLoader, uint32_t, 6> values;
	values.Enqueue(GetFullRangeWithDefaultLoader<Binary>());
	values.Enqueue(GetFullRangeWithDefaultLoader<Counter>());
	values.Enqueue(GetFullRangeWithDefaultLoader<FrozenCounter>());
	values.Enqueue(GetFullRangeWithDefaultLoader<Analog>());
	values.Enqueue(GetFullRangeWithDefaultLoader<BinaryOutputStatus>());
	values.Enqueue(GetFullRangeWithDefaultLoader<AnalogOutputStatus>());

	while(values.IsNotEmpty())
	{
		auto loader = values.Pop();
		if(loader.IsDefined())
		{
			auto iin = QueueReadRange(loader);
			if (iin.Any())
			{
				return iin;
			}
		}
	}

	return IINField::Empty;
}

IINField StaticResponseContext::QueueReadRange(const StaticRangeLoader& loader)
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
	else return IINField(IINBit::PARAM_ERROR);
}

StaticLoadResult StaticResponseContext::Load(APDUResponse& response)
{
	auto writer = response.GetWriter();
	auto result = LoadStaticData(writer);
	auto control = GetAppControl(fragmentCount, result);
	response.SetControl(control);
	++fragmentCount;
	return result;
}

AppControlField StaticResponseContext::GetAppControl(uint32_t headerCount, StaticLoadResult result)
{
	bool fir = (headerCount == 0);
	bool fin = (result != StaticLoadResult::FULL);
	return AppControlField(fir, fin, !fin, false, 0);
}

StaticLoadResult StaticResponseContext::LoadStaticData(ObjectWriter& writer)
{
	while(!staticResponseQueue.IsEmpty())
	{
		auto& front = staticResponseQueue.Peek();
		auto result = (*front.pLoadFun)(writer, front, *pDatabase);
		if(result == StaticLoadResult::COMPLETED)
		{
			staticResponseQueue.Pop();
			if(staticResponseQueue.IsEmpty()) return StaticLoadResult::COMPLETED;
			else continue;
		}
		else return result;
	}

	return StaticLoadResult::EMPTY;
}


}


