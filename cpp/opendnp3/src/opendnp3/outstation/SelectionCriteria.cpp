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

#include "opendnp3/app/PointClass.h"

namespace opendnp3
{

SelectionCriteria SelectionCriteria::ForomUnsolMask(uint8_t unsolClassMask)
{
	SelectionCriteria sc;
	if (unsolClassMask & CLASS_1) sc.class1 = events::ALL_TYPES;
	if (unsolClassMask & CLASS_2) sc.class2 = events::ALL_TYPES;
	if (unsolClassMask & CLASS_3) sc.class3 = events::ALL_TYPES;
	return sc;
}

SelectionCriteria::SelectionCriteria() : class1(0), class2(0), class3(0)
{}

SelectionCriteria::SelectionCriteria(uint16_t clazz1, uint16_t clazz2, uint16_t clazz3) :
	class1(clazz1),
	class2(clazz2),
	class3(clazz3)
{}


void SelectionCriteria::Clear()
{
	class1 = class2 = class3 = 0;	
}

bool SelectionCriteria::HasSelection() const
{
	return (class1 | class2 | class3) > 0;
}

bool SelectionCriteria::IsMatch(EventClass clazz, EventType type) const
{
	switch(clazz)
	{
	case(EventClass::EC1):
		return (class1 & static_cast<uint32_t>(type)) != 0;
	case(EventClass::EC2):
		return (class2 & static_cast<uint32_t>(type)) != 0;
	case(EventClass::EC3):
		return (class3 & static_cast<uint32_t>(type)) != 0;
	default:
		return false;
	}
}

}
