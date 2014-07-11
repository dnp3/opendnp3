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

#include "SelectionCriteria.h"

#include <openpal/util/Limits.h>

using namespace openpal;

namespace opendnp3
{

SelectionCriteria::SelectionCriteria() : numClass1(0), numClass2(0), numClass3(0)
{}

SelectionCriteria::SelectionCriteria(const ClassField& field) :
	numClass1(field.HasClass1() ? MaxValue<uint32_t>() : 0),
	numClass2(field.HasClass2() ? MaxValue<uint32_t>() : 0),
	numClass3(field.HasClass3() ? MaxValue<uint32_t>() : 0)
{

}

void SelectionCriteria::Clear()
{
	numClass1 = numClass2 = numClass3 = 0;
}

bool SelectionCriteria::HasSelection() const
{
	return (numClass1 | numClass2 | numClass3) > 0;
}

EventWriteOperation SelectionCriteria::GetWriteOperationFor(const EventResponseConfig& config, EventClass clazz, EventType type) const
{
	switch(clazz)
	{
		case(EventClass::EC1) :
			return EventWriteOperation();
		case(EventClass::EC2):
			return EventWriteOperation();
		case(EventClass::EC3):
			return EventWriteOperation();
		default:
			return EventWriteOperation();
	}
}

}
