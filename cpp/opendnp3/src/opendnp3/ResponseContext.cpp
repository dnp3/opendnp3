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

ResponseContext::ResponseContext() : first(true), staticResponseQueue(staticRangeArray.ToIndexable())
{}

#define MACRO_QUEUE_RANGE(GV) \
	case(GroupVariation::GV): \
	return QueueRange<GV, Convert##GV>(range);

bool ResponseContext::QueueRead(GroupVariation gv, const StaticRange& range)
{	
	switch(gv)
	{		
		MACRO_QUEUE_RANGE(Group1Var2);

		MACRO_QUEUE_RANGE(Group10Var2);

		MACRO_QUEUE_RANGE(Group20Var1);
		MACRO_QUEUE_RANGE(Group20Var2);
		MACRO_QUEUE_RANGE(Group20Var5);
		MACRO_QUEUE_RANGE(Group20Var6);

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
			return false;
	}	
}

LoadResult ResponseContext::Load(Database& db, APDUResponse& response)
{
	ObjectWriter writer(response.HeaderPosition());
	return LoadStaticData(writer, db, response);
}

LoadResult ResponseContext::LoadStaticData(ObjectWriter& writer, Database& db, APDUResponse& response)
{		
	while(!staticResponseQueue.IsEmpty())
	{				
		auto& front = staticResponseQueue.Front();
		auto result = (*front.pLoadFun)(writer, front, db, response);
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

void ResponseContext::Reset()
{
	staticResponseQueue.Clear();
}

}


