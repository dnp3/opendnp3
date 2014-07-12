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

namespace opendnp3
{

ResponseContext::ResponseContext(Database& database, OutstationEventBuffer& buffer, const StaticResponseTypes& rspTypes, const EventResponseConfig& config) :
	fragmentCount(0),
	staticContext(database, rspTypes),
	eventContext(config, buffer)
{

}

void ResponseContext::Reset()
{
	fragmentCount = 0;
	eventContext.Reset();
	staticContext.Reset();
}

bool ResponseContext::IsComplete() const
{
	return eventContext.IsComplete() && staticContext.IsComplete();
}

AppControlField ResponseContext::LoadSolicited(ObjectWriter& writer)
{
	auto fir = fragmentCount == 0;
	++fragmentCount;
	
	auto preLoadSize = writer.Remaining();

	auto allEventsLoaded = eventContext.Load(writer);
	auto wereEventsWritten = writer.Remaining() < preLoadSize;
		
	if (allEventsLoaded)
	{		
		auto complete = staticContext.Load(writer);
		return GetControl(fir, complete, wereEventsWritten);
	}
	else
	{				
		return GetControl(fir, false, wereEventsWritten);
	}
}

AppControlField ResponseContext::GetControl(bool fir, bool fin, bool hasEvents)
{	
	auto con = (!fin) || hasEvents; // request confirmation on any non-fin fragment or if it has events
	return AppControlField(fir, fin, con, false);		
}

IINField ResponseContext::ReadAllObjects(const GroupVariationRecord& record)
{
	switch (record.type)
	{
		case(GroupVariationType::STATIC) :
			return staticContext.ReadAll(record);
		case(GroupVariationType::EVENT) :
			return eventContext.ReadAll(record);
		default:
			return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

IINField ResponseContext::ReadRange(const GroupVariationRecord& record, const StaticRange& range)
{
	switch (record.type)
	{
	case(GroupVariationType::STATIC) :
		return staticContext.ReadRange(record, range);
	default:
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

IINField ResponseContext::ReadCount(const GroupVariationRecord& record, uint32_t count)
{
	switch (record.type)
	{	
		case(GroupVariationType::EVENT) :
			return eventContext.ReadCount(record, count);
		default:
			return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}

}
