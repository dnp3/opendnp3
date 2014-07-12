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

#include "EventResponseContext.h"

#include <openpal/serialization/Serialization.h>

using namespace openpal;

namespace opendnp3
{

EventResponseContext::EventResponseContext(const EventResponseConfig& config, OutstationEventBuffer& buffer) :
	isComplete(false),
	criteria(config),
	pBuffer(&buffer)
{

}


bool EventResponseContext::IsComplete() const
{
	return isComplete;
}

IINField EventResponseContext::ReadAll(const GroupVariationRecord& record)
{
	return criteria.RecordAllObjects(record.enumeration);
}

IINField EventResponseContext::ReadCount(const GroupVariationRecord& record, uint16_t count)
{
	return criteria.RecordCountOfObjects(record.enumeration, count);
}

void EventResponseContext::Reset()
{
	isComplete = false;
	criteria.Clear();
}

bool EventResponseContext::Load(ObjectWriter& objectWriter)
{
	if (isComplete)
	{
		return true;
	}
	else
	{
		auto writer = pBuffer->Iterate();
		auto complete = writer.WriteAllEvents(criteria, objectWriter);
		isComplete = complete;
		return complete;
	}	
}

}
