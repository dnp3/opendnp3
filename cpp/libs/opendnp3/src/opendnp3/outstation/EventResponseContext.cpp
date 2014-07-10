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

#include "opendnp3/outstation/EventWriter.h"

using namespace openpal;

namespace opendnp3
{

EventResponseContext::EventResponseContext(OutstationEventBuffer* pBuffer_) : 
	isComplete(false),
	pBuffer(pBuffer_)	
{

}


bool EventResponseContext::IsComplete() const
{
	return isComplete;
}

IINField EventResponseContext::ReadAll(const GroupVariationRecord& record)
{
	switch(record.enumeration)
	{
		// ----------- class polls --------------

	case(GroupVariation::Group60Var2) :
		criteria.AddToClass1(events::ALL_TYPES);
		return IINField::Empty;
	case(GroupVariation::Group60Var3) :
		criteria.AddToClass2(events::ALL_TYPES);
		return IINField::Empty;
	case(GroupVariation::Group60Var4) :
		criteria.AddToClass3(events::ALL_TYPES);
		return IINField::Empty;

		// ----------- variation 0 --------------

	case(GroupVariation::Group2Var0) :
		criteria.AddToAllClasses(events::BINARY);
		return IINField::Empty;

	case(GroupVariation::Group4Var0) :
		criteria.AddToAllClasses(events::DOUBLE_BIT_BINARY);
		return IINField::Empty;

	case(GroupVariation::Group11Var0) :
		criteria.AddToAllClasses(events::BINARY_OUTPUT_STATUS);
		return IINField::Empty;

	case(GroupVariation::Group22Var0) :
		criteria.AddToAllClasses(events::COUNTER);
		return IINField::Empty;

	case(GroupVariation::Group23Var0) :
		criteria.AddToAllClasses(events::FROZEN_COUNTER);
		return IINField::Empty;

	case(GroupVariation::Group32Var0) :
		criteria.AddToAllClasses(events::ANALOG);
		return IINField::Empty;

	case(GroupVariation::Group42Var0) :
		criteria.AddToAllClasses(events::ANALOG_OUTPUT_STATUS);
		return IINField::Empty;

	default:
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

IINField EventResponseContext::ReadCount(const GroupVariationRecord& record, uint32_t count)
{
	return IINField(IINBit::FUNC_NOT_SUPPORTED);
}

void EventResponseContext::Reset()
{
	isComplete = false;
	criteria.Clear();
}

bool EventResponseContext::Load(ObjectWriter& writer, const EventResponseConfig& config)
{
	if (isComplete)
	{
		return true;
	}
	else
	{
		if (criteria.HasSelection())
		{			
			auto iterator = pBuffer->Iterate();
			isComplete = EventWriter::WriteEventHeaders(writer, criteria, iterator, config);
			return isComplete;
		}
		else
		{
			return true;
		}
	}	
}

}
