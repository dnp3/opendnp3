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

ResponseContext::ResponseContext(Database* pDatabase, OutstationEventBuffer* pBuffer, const StaticResponseTypes& rspTypes) :
	fragmentCount(0),
	staticContext(pDatabase, rspTypes),
	eventContext(pBuffer)
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

AppControlField ResponseContext::Load(APDUResponse& response)
{
	auto writer = response.GetWriter();
	auto result = eventContext.Load(writer);
	auto count = fragmentCount;
	++fragmentCount;
	if (result.complete)
	{
		auto complete = staticContext.Load(writer);
		return GetControl(response, count, result.Any(), complete);		
	}
	else
	{		
		return GetControl(response, count, result.Any(), false);		
	}
}

AppControlField ResponseContext::GetControl(APDUResponse& response, uint16_t fragCount, bool hasEvents, bool fin)
{
	auto fir = (fragCount == 0);
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

}
