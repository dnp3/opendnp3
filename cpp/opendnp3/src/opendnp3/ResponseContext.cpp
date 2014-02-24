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
#include "ResponseContext.h"

#include <openpal/Serialization.h>

#include "WriteConversions.h"

using namespace openpal;

namespace opendnp3
{

ResponseContext::ResponseContext(Database* pDatabase_) : 
	fragmentCount(0),
	isStaticRequest(false),
	pDatabase(pDatabase_)	
{}

bool ResponseContext::IsComplete() const
{
	return staticResponseQueue.IsEmpty();
}

bool ResponseContext::IsStaticRequest() const
{
	return isStaticRequest;
}

void ResponseContext::Reset()
{
	fragmentCount = 0;
	isStaticRequest = false;
	staticResponseQueue.Clear();
}


#define MACRO_QUEUE_FULL_RANGE(GV, TYPE) { \
	auto range = pDatabase->FullRange<TYPE>(); \
	if(range.IsDefined()) { \
		auto result = QueueReadRange(GV, range); \
		if(result != QueueResult::SUCCESS) return result; \
	} \
}

QueueResult ResponseContext::QueueReadAllObjects(GroupVariation gv)
{
	switch(gv)
	{
		case(GroupVariation::Group60Var1):		
			return QueueStaticIntegrity();
		case(GroupVariation::Group1Var0):
			return QueueReadRange(GroupVariation::Group1Var2, pDatabase->FullRange<Binary>());
		case(GroupVariation::Group1Var2):
			return QueueReadRange(GroupVariation::Group1Var2, pDatabase->FullRange<Binary>());
		case(GroupVariation::Group10Var0):
			return QueueReadRange(GroupVariation::Group10Var2, pDatabase->FullRange<ControlStatus>());
		case(GroupVariation::Group10Var2):
			return QueueReadRange(GroupVariation::Group10Var2, pDatabase->FullRange<ControlStatus>());
		case(GroupVariation::Group20Var0):
			return QueueReadRange(GroupVariation::Group20Var2, pDatabase->FullRange<Counter>());
		default:
			return QueueResult::OBJECT_UNDEFINED;
	}
}

QueueResult ResponseContext::QueueStaticIntegrity()
{
	StaticArray<RangeGroupVariation, uint32_t, 6> values;
	values[0] = RangeGroupVariation(pDatabase->FullRange<Binary>(), GroupVariation::Group1Var2);
	values[1] = RangeGroupVariation(pDatabase->FullRange<Analog>(), GroupVariation::Group30Var1);
	values[2] = RangeGroupVariation(pDatabase->FullRange<Counter>(), GroupVariation::Group20Var2);
	values[3] = RangeGroupVariation(pDatabase->FullRange<FrozenCounter>(), GroupVariation::Group21Var1);
	values[4] = RangeGroupVariation(pDatabase->FullRange<ControlStatus>(), GroupVariation::Group10Var2);
	values[5] = RangeGroupVariation(pDatabase->FullRange<SetpointStatus>(), GroupVariation::Group40Var1);

	for(uint32_t i=0; i< values.Size(); ++i)
	{		
		auto result = QueueReadRange(values[i].groupVariation, values[i].range);
		if(result != QueueResult::SUCCESS) return result;		
	}

	return QueueResult::SUCCESS;
}

#define MACRO_QUEUE_RANGE(GV) \
	case(GroupVariation::GV): \
	return QueueRange<GV::Target, GV##Serializer>(GV::ID, range);

QueueResult ResponseContext::QueueReadRange(GroupVariation gv, const StaticRange& range)
{	
	if(range.IsDefined()) 
	{
		switch(gv)
		{		
			MACRO_QUEUE_RANGE(Group1Var2);

			MACRO_QUEUE_RANGE(Group10Var2);

			MACRO_QUEUE_RANGE(Group20Var1);
			MACRO_QUEUE_RANGE(Group20Var2);
			MACRO_QUEUE_RANGE(Group20Var5);
			MACRO_QUEUE_RANGE(Group20Var6);

			MACRO_QUEUE_RANGE(Group21Var1);
			MACRO_QUEUE_RANGE(Group21Var2);
			MACRO_QUEUE_RANGE(Group21Var5);
			MACRO_QUEUE_RANGE(Group21Var6);
			MACRO_QUEUE_RANGE(Group21Var9);
			MACRO_QUEUE_RANGE(Group21Var10);

			MACRO_QUEUE_RANGE(Group30Var1);
			MACRO_QUEUE_RANGE(Group30Var2);
			MACRO_QUEUE_RANGE(Group30Var3);
			MACRO_QUEUE_RANGE(Group30Var4);
			MACRO_QUEUE_RANGE(Group30Var5);
			MACRO_QUEUE_RANGE(Group30Var6);

			MACRO_QUEUE_RANGE(Group40Var1);
			MACRO_QUEUE_RANGE(Group40Var2);
			MACRO_QUEUE_RANGE(Group40Var3);
			MACRO_QUEUE_RANGE(Group40Var4);
		
			default:
				return QueueResult::OBJECT_UNDEFINED;
		}	
	}
	else return QueueResult::SUCCESS;
}

LoadResult ResponseContext::Load(APDUResponse& response)
{	
	auto writer = response.GetWriter();
	auto result = LoadStaticData(writer);
	auto control = GetAppControl(fragmentCount, result);
	response.SetControl(control);
	++fragmentCount;
	return result;
}

AppControlField ResponseContext::GetAppControl(uint32_t headerCount, LoadResult result)
{
	bool fir = (headerCount == 0);
	bool fin = (result != LoadResult::FULL);
	return AppControlField(fir, fin, !fin, false, 0);
}

LoadResult ResponseContext::LoadStaticData(ObjectWriter& writer)
{		
	while(!staticResponseQueue.IsEmpty())
	{				
		auto& front = staticResponseQueue.Front();
		auto result = (*front.pLoadFun)(front.groupVar, writer, front, *pDatabase);
		if(result == LoadResult::COMPLETED)
		{
			staticResponseQueue.Pop();
			if(staticResponseQueue.IsEmpty()) return LoadResult::COMPLETED;
			else continue;
		}
		else return result;
	}

	return LoadResult::EMPTY;
}


}


